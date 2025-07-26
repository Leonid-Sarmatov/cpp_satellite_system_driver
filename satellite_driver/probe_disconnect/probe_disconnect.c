#include "probe_disconnect.h"

/**
 * satellite_probe - Device detection function
 * @interface: USB interface from the device
 * @id:        ID struct
 */
int satellite_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_device *udev = interface_to_usbdev(interface);
    struct sat_device *dev;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i, retval = -ENOMEM;

    /* Allocate memory for the device struct */
    dev = kzalloc(sizeof(struct sat_device), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;

    /* Save references to the USB device and interface */
    dev->udev = usb_get_dev(udev);
    dev->interface = interface;

    /* Getting the current alternate interface */
    iface_desc = interface->cur_altsetting;

    /* Initialization of synchronization mutex */
    mutex_init(&dev->io_mutex);          
    init_waitqueue_head(&dev->read_wait);

    /* Allocating buffer for device responses */
    dev->response_buffer = kmalloc(64, GFP_KERNEL);
    if (!dev->response_buffer)
        goto error;

    /* Searching for endpoints in the interface */
    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++) {
        endpoint = &iface_desc->endpoint[i].desc;

        /* Check endpoint type */
        if (usb_endpoint_is_int_out(endpoint)) {
            // Endpoint for outgoing interrupts
            dev->interrupt_out_ep = endpoint->bEndpointAddress;
            dev->interrupt_out_size = usb_endpoint_maxp(endpoint);
        }
        else if (usb_endpoint_is_int_in(endpoint)) {
            // Endpoint for incoming interrupts
            dev->interrupt_in_ep = endpoint->bEndpointAddress;
            dev->interrupt_in_size = usb_endpoint_maxp(endpoint);
        }
    }

    /* Check presence of both endpoints */
    if (!dev->interrupt_out_ep || !dev->interrupt_in_ep) {
        retval = -ENODEV;
        goto error;
    }

    /* Registering the char device */
    // 1. Allocating device number range
    retval = alloc_chrdev_region(&dev->dev_num, 0, 1, DEVICE_NAME);
    if (retval < 0)
        goto error;

    // 2. Initializing the cdev struct
    cdev_init(&dev->cdev, &sat_fops);
    dev->cdev.owner = THIS_MODULE;

    // 3. Adding the device to system
    retval = cdev_add(&dev->cdev, dev->dev_num, 1);
    if (retval)
        goto free_region;

    /* Creating the device class (if it doesn't exist) */
    if (!sat_class) {
        sat_class = class_create(DEVICE_NAME);
        if (IS_ERR(sat_class)) {
            retval = PTR_ERR(sat_class);
            goto del_cdev;
        }
    }

    /* Creating a device file in /dev */
    device_create(sat_class, NULL, dev->dev_num, NULL, "satellite%d", interface->minor);
    
    /* Saving the device pointer in the interface */
    usb_set_intfdata(interface, dev);
    return 0;  // Successfully finished probe

/* Error handlers (rollback of performed actions)) */
del_cdev:
    cdev_del(&dev->cdev);
free_region:
    unregister_chrdev_region(dev->dev_num, 1);
error:
    if (dev->response_buffer) kfree(dev->response_buffer);
    usb_put_dev(udev);
    kfree(dev);
    return retval;
}

/**
 * satellite_disconnect - Device disconnection handler
 * @interface: USB interface from the device
 * 
 * Called when the device is disconnected. Frees all allocated resources.
 */
void satellite_disconnect(struct usb_interface *interface)
{
    struct sat_device *dev = usb_get_intfdata(interface);

    if (dev) {
        /* Removing the device file */
        device_destroy(sat_class, dev->dev_num);
        
        /* Removing char device */
        cdev_del(&dev->cdev);
        
        /* Free device number range */
        unregister_chrdev_region(dev->dev_num, 1);
        
        /* Free allocated resources */
        kfree(dev->response_buffer);
        usb_put_dev(dev->udev);
        kfree(dev);
    }

    /* Destroying the device class upon the last disconnection */
    if (sat_class) {
        class_destroy(sat_class);
        sat_class = NULL;
    }
}