#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define VENDOR_ID  0x2e3c
#define PRODUCT_ID 0x5745

struct sat_device {
    struct usb_device *udev;
    struct usb_interface *interface;
    __u8 interrupt_out_ep;
    __u8 interrupt_in_ep;
    int interrupt_out_size;
    int interrupt_in_size;
};

static int send_led_command(struct sat_device *dev, __u8 state)
{
    int retval;
    int actual_length;
    unsigned char *out_buf;
    unsigned char *in_buf;

    out_buf = kmalloc(dev->interrupt_out_size, GFP_KERNEL);
    if (!out_buf)
        return -ENOMEM;

    in_buf = kmalloc(dev->interrupt_in_size, GFP_KERNEL);
    if (!in_buf) {
        kfree(out_buf);
        return -ENOMEM;
    }

    memset(out_buf, 0, dev->interrupt_out_size);
    memset(in_buf, 0, dev->interrupt_in_size);

    // Формируем команду
    out_buf[0] = 0x02;  // Report ID
    out_buf[1] = state; // 0 - вкл, 1 - выкл

    dev_info(&dev->interface->dev, "Sending LED command: state=%d\n", state);

    retval = usb_interrupt_msg(dev->udev,
                              usb_sndintpipe(dev->udev, dev->interrupt_out_ep),
                              out_buf,
                              dev->interrupt_out_size,
                              &actual_length,
                              1000);
    if (retval < 0) {
        dev_err(&dev->interface->dev, "Error sending LED command: %d\n", retval);
        goto free_buffers;
    }
    dev_info(&dev->interface->dev, "Sent LED command: state=%d, bytes sent=%d\n", state, actual_length);

    // Читаем ответ с interrupt IN endpoint
    retval = usb_interrupt_msg(dev->udev,
                              usb_rcvintpipe(dev->udev, dev->interrupt_in_ep),
                              in_buf,
                              dev->interrupt_in_size,
                              &actual_length,
                              1000);
    if (retval < 0) {
        dev_err(&dev->interface->dev, "Error reading response: %d\n", retval);
        goto free_buffers;
    }

    dev_info(&dev->interface->dev, "Read %d bytes from device response:\n", actual_length);

    // Выводим в hex
    {
        int i;
        char line[80];
        int pos = 0;
        for (i = 0; i < actual_length; i++) {
            pos += snprintf(line + pos, sizeof(line) - pos, "%02x ", in_buf[i]);
            if ((i + 1) % 16 == 0 || i == actual_length - 1) {
                dev_info(&dev->interface->dev, "  %s\n", line);
                pos = 0;
            }
        }
    }

free_buffers:
    kfree(out_buf);
    kfree(in_buf);
    return retval;
}

static int satellite_probe(struct usb_interface *interface,
                           const struct usb_device_id *id)
{
    struct usb_device *udev = interface_to_usbdev(interface);
    struct sat_device *dev;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;

    dev = kzalloc(sizeof(struct sat_device), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;

    dev->udev = usb_get_dev(udev);
    dev->interface = interface;

    iface_desc = interface->cur_altsetting;

    dev->interrupt_out_ep = 0;
    dev->interrupt_in_ep = 0;
    dev->interrupt_out_size = 0;
    dev->interrupt_in_size = 0;

    // Ищем interrupt OUT и IN endpoints
    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++) {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_int_out(endpoint)) {
            dev->interrupt_out_ep = endpoint->bEndpointAddress;
            dev->interrupt_out_size = usb_endpoint_maxp(endpoint);
            dev_info(&interface->dev, "Found interrupt OUT endpoint: 0x%02X, size=%d\n",
                     dev->interrupt_out_ep, dev->interrupt_out_size);
        }
        else if (usb_endpoint_is_int_in(endpoint)) {
            dev->interrupt_in_ep = endpoint->bEndpointAddress;
            dev->interrupt_in_size = usb_endpoint_maxp(endpoint);
            dev_info(&interface->dev, "Found interrupt IN endpoint: 0x%02X, size=%d\n",
                     dev->interrupt_in_ep, dev->interrupt_in_size);
        }
    }

    if (!dev->interrupt_out_ep || !dev->interrupt_in_ep) {
        dev_err(&interface->dev, "Required interrupt IN/OUT endpoints not found\n");
        usb_put_dev(udev);
        kfree(dev);
        return -ENODEV;
    }

    usb_set_intfdata(interface, dev);

    dev_info(&interface->dev, "Satellite device connected\n");

    // Мигаем светодиодом 5 раз
    // {
    //     int k;
    //     for (k = 0; k < 5; k++) {
    //         send_led_command(dev, 0); // Вкл
    //         msleep(100);
    //         send_led_command(dev, 1); // Выкл
    //         msleep(100);
    //     }
    // }
    send_led_command(dev, 0);
    msleep(100);

    return 0;
}

static void satellite_disconnect(struct usb_interface *interface)
{
    struct sat_device *dev = usb_get_intfdata(interface);

    if (dev) {
        usb_put_dev(dev->udev);
        kfree(dev);
        usb_set_intfdata(interface, NULL);
    }

    dev_info(&interface->dev, "Satellite device disconnected\n");
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
MODULE_DESCRIPTION("Driver for Satellite Device with LED control and response reading");
