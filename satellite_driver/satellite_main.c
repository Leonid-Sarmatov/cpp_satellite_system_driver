#include "satellite.h"

struct class *sat_class = NULL;

struct usb_device_id satellite_table[] = {
    { USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
    {}
};
MODULE_DEVICE_TABLE(usb, satellite_table);

struct usb_driver satellite_driver = {
    .name = "satellite",
    .id_table = satellite_table,
    .probe = satellite_probe,
    .disconnect = satellite_disconnect,
};

module_usb_driver(satellite_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Best Autor");
MODULE_DESCRIPTION("Main USB driver for Satellite Device");