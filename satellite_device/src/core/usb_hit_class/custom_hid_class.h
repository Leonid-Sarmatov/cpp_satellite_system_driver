
/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __CUSTOM_HID_CLASS_H
#define __CUSTOM_HID_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_std.h"
#include "usbd_core.h"

/**
  * @brief usb custom hid use endpoint define
  */
#define USBD_CUSTOM_HID_IN_EPT                  0x81
#define USBD_CUSTOM_HID_OUT_EPT                 0x01

/**
  * @brief usb custom hid in and out max packet size define
  */
#define USBD_CUSTOM_IN_MAXPACKET_SIZE           0x40
#define USBD_CUSTOM_OUT_MAXPACKET_SIZE          0x40

typedef struct
{
  uint8_t g_rxhid_buff[USBD_CUSTOM_OUT_MAXPACKET_SIZE];
  uint8_t g_txhid_buff[USBD_CUSTOM_IN_MAXPACKET_SIZE];

  uint32_t hid_protocol;
  uint32_t hid_set_idle;
  uint32_t alt_setting;
  
  uint8_t hid_set_report[64];
  uint8_t hid_get_report[64];
  uint8_t hid_state;
  uint8_t send_state;
}custom_hid_type;

extern usbd_class_handler custom_hid_class_handler;
usb_sts_type custom_hid_class_send_report(void *udev, uint8_t *report, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
