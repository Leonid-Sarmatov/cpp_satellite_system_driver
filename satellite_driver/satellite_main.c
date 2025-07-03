#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define VENDOR_ID  0x2e3c
#define PRODUCT_ID 0x5745

// Структура для управления устройством
struct sat_device {
    struct usb_device *udev;
    struct usb_interface *interface;
    __u8 interrupt_out_ep;  // Адрес endpoint для отправки
    int interrupt_out_size; // Размер пакета
};

static int send_led_command(struct sat_device *dev, __u8 state)
{
    int retval;
    int actual_length;
    unsigned char *buffer = kmalloc(dev->interrupt_out_size, GFP_KERNEL);
    
    if (!buffer)
        return -ENOMEM;
    
    // Формируем команду:
    buffer[0] = 0x02;   // Report ID для светодиода
    buffer[1] = state;  // 0 - вкл, 1 - выкл
    memset(buffer + 2, 0, dev->interrupt_out_size - 2); // Остальные байты - 0

    retval = usb_interrupt_msg(dev->udev,
                              usb_sndintpipe(dev->udev, dev->interrupt_out_ep),
                              buffer,
                              dev->interrupt_out_size,
                              &actual_length,
                              1000); // Таймаут 1 секунда
                              
    kfree(buffer);
    
    if (retval < 0) {
        dev_err(&dev->interface->dev, "Error sending LED command: %d\n", retval);
        return retval;
    }
    
    dev_info(&dev->interface->dev, "Sent LED command: state %d, bytes: %d\n",
             state, actual_length);
    return 0;
}

static int satellite_probe(struct usb_interface *interface,
                          const struct usb_device_id *id)
{
    struct usb_device *udev = interface_to_usbdev(interface);
    struct sat_device *dev;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;
    
    // Выделяем память под структуру устройства
    dev = kzalloc(sizeof(struct sat_device), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;
    
    dev->udev = usb_get_dev(udev);
    dev->interface = interface;
    
    // Ищем interrupt OUT endpoint
    iface_desc = interface->cur_altsetting;
    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++) {
        endpoint = &iface_desc->endpoint[i].desc;
        
        if (usb_endpoint_is_int_out(endpoint)) {
            dev->interrupt_out_ep = endpoint->bEndpointAddress;
            dev->interrupt_out_size = usb_endpoint_maxp(endpoint);
            break;
        }
    }
    
    if (!dev->interrupt_out_ep) {
        dev_err(&interface->dev, "Could not find interrupt-out endpoint\n");
        usb_put_dev(udev);
        kfree(dev);
        return -ENODEV;
    }
    
    // Сохраняем ссылку на структуру
    usb_set_intfdata(interface, dev);
    
    dev_info(&interface->dev, "Device connected. Interrupt OUT endpoint: 0x%02X, size: %d\n",
             dev->interrupt_out_ep, dev->interrupt_out_size);
             
    // Пробуем включить светодиод

    int k;
    for (k = 0; k < 20; k++) {
        send_led_command(dev, 0); // 0 = включить
        msleep(100); 
        send_led_command(dev, 1); // Включить
        msleep(100);    
    }
    
    return 0;
}

static void satellite_disconnect(struct usb_interface *interface)
{
    struct sat_device *dev = usb_get_intfdata(interface);
    
    if (dev) {
        // Выключаем светодиод при отключении
        int k;
        for (k = 0; k < 20; k++) {
            send_led_command(dev, 0); // 0 = включить
            msleep(100); 
            send_led_command(dev, 1); // Включить
            msleep(100);    
        }
        usb_put_dev(dev->udev);
        kfree(dev);
        usb_set_intfdata(interface, NULL);
    }
    
    dev_info(&interface->dev, "Device disconnected\n");
}

static struct usb_device_id satellite_table[] = {
    { USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
    {}
};
MODULE_DEVICE_TABLE(usb, satellite_table);

static struct usb_driver satellite_driver = {
    .name = "satellite",
    .id_table = satellite_table,
    .probe = satellite_probe,
    .disconnect = satellite_disconnect,
};

module_usb_driver(satellite_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Driver for Satellite Device with LED control");