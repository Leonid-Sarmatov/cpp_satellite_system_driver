#ifndef SATELLITE_H
#define SATELLITE_H

/* Linux headers */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/sched.h>

#define VENDOR_ID  0x2e3c   // VID
#define PRODUCT_ID 0x5745   // PID

#define DEVICE_NAME "satellite"  // Basic device name
#define MAX_DEVICES 8            // Maximun support device

/**
 * struct sat_device - Main structure for storing the device state
 * @udev:               Reference to the USB device
 * @interface:          USB interface
 * @interrupt_out_ep:   Endpoint address for outgoing interrupts
 * @interrupt_in_ep:    Endpoint address for incoming interrupts
 * @interrupt_out_size: Buffer size for outgoing interrupts
 * @interrupt_in_size:  Buffer size for incoming interrupts
 * 
 * Character device fields:
 * @cdev:               Character device structure
 * @dev_num:            Device number (major/minor)
 * @io_mutex:           Mutex for synchronizing I/O operations
 * @read_wait:          Wait queue for blocking reads
 * @response_buffer:    Buffer for storing the device response
 * @response_length:    Length of the received response
 * @response_ready:     Flag indicating a response is ready
 */
struct sat_device {
    struct usb_device *udev;
    struct usb_interface *interface;
    __u8 interrupt_out_ep;
    __u8 interrupt_in_ep;
    int interrupt_out_size;
    int interrupt_in_size;

    struct cdev cdev;
    dev_t dev_num;
    struct mutex io_mutex;
    wait_queue_head_t read_wait;
    unsigned char *response_buffer;
    int response_length;
    bool response_ready;
};

// Prototypes from probe_disconnect.c
int satellite_probe(struct usb_interface *interface, const struct usb_device_id *id);
void satellite_disconnect(struct usb_interface *interface);

// Prototypes from files_operations.c
int satellite_char_init(struct sat_device *dev);
void satellite_char_cleanup(struct sat_device *dev);
int satellite_open(struct inode *inode, struct file *file);
int satellite_release(struct inode *inode, struct file *file);
ssize_t satellite_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);
ssize_t satellite_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
int send_led_command(struct sat_device *dev, __u8 state, unsigned char *out_response, int *response_len);


extern struct usb_driver satellite_driver;
extern struct usb_device_id satellite_table[];
extern struct file_operations sat_fops;
extern struct class *sat_class;

#endif // SATELLITE_H