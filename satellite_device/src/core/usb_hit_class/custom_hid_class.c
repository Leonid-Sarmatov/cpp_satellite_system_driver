
#include "usbd_core.h"
#include "custom_hid_class.h"
#include "custom_hid_desc.h"

#include "led.h"
#include "controlled_systems.h"

static usb_sts_type class_init_handler(void *udev);
static usb_sts_type class_clear_handler(void *udev);
static usb_sts_type class_setup_handler(void *udev, usb_setup_type *setup);
static usb_sts_type class_ept0_tx_handler(void *udev);
static usb_sts_type class_ept0_rx_handler(void *udev);
static usb_sts_type class_in_handler(void *udev, uint8_t ept_num);
static usb_sts_type class_out_handler(void *udev, uint8_t ept_num);
static usb_sts_type class_sof_handler(void *udev);
static usb_sts_type class_event_handler(void *udev, usbd_event_type event);

static void usb_hid_buf_process(void *udev, uint8_t *report, uint16_t len);
custom_hid_type custom_hid_struct;

/* usb device class handler */
usbd_class_handler custom_hid_class_handler =
{
  class_init_handler,
  class_clear_handler,
  class_setup_handler,
  class_ept0_tx_handler,
  class_ept0_rx_handler,
  class_in_handler,
  class_out_handler,
  class_sof_handler,
  class_event_handler,
  &custom_hid_struct
};

/**
  * @brief  initialize usb custom hid endpoint
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_init_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;
  /* open custom hid in endpoint */
  usbd_ept_open(pudev, USBD_CUSTOM_HID_IN_EPT, EPT_INT_TYPE, USBD_CUSTOM_IN_MAXPACKET_SIZE);

  /* open custom hid out endpoint */
  usbd_ept_open(pudev, USBD_CUSTOM_HID_OUT_EPT, EPT_INT_TYPE, USBD_CUSTOM_OUT_MAXPACKET_SIZE);

  /* set out endpoint to receive status */
  usbd_ept_recv(pudev, USBD_CUSTOM_HID_OUT_EPT, pcshid->g_rxhid_buff, USBD_CUSTOM_OUT_MAXPACKET_SIZE);

  pcshid->send_state = 0;
  
  return status;
}

/**
  * @brief  clear endpoint or other state
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_clear_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;

  /* close custom hid in endpoint */
  usbd_ept_close(pudev, USBD_CUSTOM_HID_IN_EPT);

  /* close custom hid out endpoint */
  usbd_ept_close(pudev, USBD_CUSTOM_HID_OUT_EPT);

  return status;
}

/**
  * @brief  usb device class setup request handler
  * @param  udev: to the structure of usbd_core_type
  * @param  setup: setup packet
  * @retval status of usb_sts_type
  */
static usb_sts_type class_setup_handler(void *udev, usb_setup_type *setup)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;
  uint16_t len;
  uint8_t *buf;

  switch(setup->bmRequestType & USB_REQ_TYPE_RESERVED)
  {
    /* class request */
    case USB_REQ_TYPE_CLASS:
      switch(setup->bRequest)
      {
        case HID_REQ_SET_PROTOCOL:
          pcshid->hid_protocol = (uint8_t)setup->wValue;
          break;
        case HID_REQ_GET_PROTOCOL:
          usbd_ctrl_send(pudev, (uint8_t *)&pcshid->hid_protocol, 1);
          break;
        case HID_REQ_SET_IDLE:
          pcshid->hid_set_idle = (uint8_t)(setup->wValue >> 8);
          break;
        case HID_REQ_GET_IDLE:
          usbd_ctrl_send(pudev, (uint8_t *)&pcshid->hid_set_idle, 1);
          break;
        case HID_REQ_SET_REPORT:
          pcshid->hid_state = HID_REQ_SET_REPORT;
          usbd_ctrl_recv(pudev, pcshid->hid_set_report, setup->wLength);
          break;
        case HID_REQ_GET_REPORT:
          usbd_ctrl_send(pudev, pcshid->hid_get_report, setup->wLength);
          break;
        default:
          usbd_ctrl_unsupport(pudev);
          break;
      }
      break;
    /* standard request */
    case USB_REQ_TYPE_STANDARD:
      switch(setup->bRequest)
      {
        case USB_STD_REQ_GET_DESCRIPTOR:
          if(setup->wValue >> 8 == HID_REPORT_DESC)
          {
            len = MIN(USBD_CUSHID_SIZ_REPORT_DESC, setup->wLength);
            buf = (uint8_t *)g_usbd_custom_hid_report;
            usbd_ctrl_send(pudev, (uint8_t *)buf, len);
          }
          else if(setup->wValue >> 8 == HID_DESCRIPTOR_TYPE)
          {
            len = MIN(9, setup->wLength);
            buf = (uint8_t *)g_custom_hid_usb_desc;
            usbd_ctrl_send(pudev, (uint8_t *)buf, len);
          }
          else
          {
            usbd_ctrl_unsupport(pudev);
          }
          break;
        case USB_STD_REQ_GET_INTERFACE:
          usbd_ctrl_send(pudev, (uint8_t *)&pcshid->alt_setting, 1);
          break;
        case USB_STD_REQ_SET_INTERFACE:
          pcshid->alt_setting = setup->wValue;
          break;
        case USB_STD_REQ_CLEAR_FEATURE:
          break;
        case USB_STD_REQ_SET_FEATURE:
          break;
        default:
          usbd_ctrl_unsupport(pudev);
          break;
      }
      break;
    default:
      usbd_ctrl_unsupport(pudev);
      break;
  }
  return status;
}

/**
  * @brief  usb device endpoint 0 in status stage complete
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_ept0_tx_handler(void *udev)
{
  usb_sts_type status = USB_OK;

  /* ...user code... */

  return status;
}

/**
  * @brief  usb device endpoint 0 out status stage complete
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_ept0_rx_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;
  uint32_t recv_len = usbd_get_recv_len(pudev, 0);
  /* ...user code... */
  if( pcshid->hid_state == HID_REQ_SET_REPORT)
  {
    /* hid buffer process */
    usb_hid_buf_process(udev, pcshid->hid_set_report, recv_len);
    pcshid->hid_state = 0;
  }

  return status;
}

/**
  * @brief  usb device transmision complete handler
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_num: endpoint number
  * @retval status of usb_sts_type
  */
static usb_sts_type class_in_handler(void *udev, uint8_t ept_num)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;
  
  pcshid->send_state = 0;
  
  /* ...user code...
    trans next packet data
  */

  return status;
}

/**
  * @brief  usb device endpoint receive data
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_num: endpoint number
  * @retval status of usb_sts_type
  */
static usb_sts_type class_out_handler(void *udev, uint8_t ept_num)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;

  /* get endpoint receive data length  */
  uint32_t recv_len = usbd_get_recv_len(pudev, ept_num);

  /* hid buffer process */
  usb_hid_buf_process(udev, pcshid->g_rxhid_buff, recv_len);

  /* start receive next packet */
  usbd_ept_recv(pudev, USBD_CUSTOM_HID_OUT_EPT, pcshid->g_rxhid_buff, recv_len);

  return status;
}

/**
  * @brief  usb device sof handler
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_sof_handler(void *udev)
{
  usb_sts_type status = USB_OK;

  /* ...user code... */

  return status;
}

/**
  * @brief  usb device event handler
  * @param  udev: to the structure of usbd_core_type
  * @param  event: usb device event
  * @retval status of usb_sts_type
  */
static usb_sts_type class_event_handler(void *udev, usbd_event_type event)
{
  usb_sts_type status = USB_OK;
  switch(event)
  {
    case USBD_RESET_EVENT:

      /* ...user code... */

      break;
    case USBD_SUSPEND_EVENT:

      /* ...user code... */

      break;
    case USBD_WAKEUP_EVENT:
      /* ...user code... */

      break;
    default:
      break;
  }
  return status;
}

/**
  * @brief  usb device class send report
  * @param  udev: to the structure of usbd_core_type
  * @param  report: report buffer
  * @param  len: report length
  * @retval status of usb_sts_type
  */
usb_sts_type custom_hid_class_send_report(void *udev, uint8_t *report, uint16_t len)
{
  usb_sts_type status = USB_FAIL;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;

  if(usbd_connect_state_get(pudev) == USB_CONN_STATE_CONFIGURED && pcshid->send_state == 0)
  {
    pcshid->send_state = 1;
    usbd_ept_send(pudev, USBD_CUSTOM_HID_IN_EPT, report, len);
    status = USB_OK;
  }
  return status;
}

/**
  * @brief  usb device report function
  * @param  udev: to the structure of usbd_core_type
  * @param  report: report buffer
  * @param  len: report length
  * @retval none
  */
static void usb_hid_buf_process(void *udev, uint8_t *report, uint16_t len)
{
  uint32_t i_index;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  custom_hid_type *pcshid = (custom_hid_type *)pudev->class_handler->pdata;

  switch(report[0])
  {
    case HID_REPORT_ID_2:
      if(report[1] == 0)
      {
        //LED_SetDisableState(&controlLed);
        ledBlinkFlag = 0;
      }
      else
      {
        //LED_SetEnableState(&controlLed);
        ledBlinkFlag = 1;
      }

      ALIGNED_HEAD static uint8_t report_buf[64] ALIGNED_TAIL;
      memset(report_buf, 0, sizeof(report_buf));
      report_buf[0] = HID_REPORT_ID_2;
      const char *response = "bubilda";
      memcpy(&report_buf[1], response, strlen(response));
      custom_hid_class_send_report(pudev, report_buf, 64);

      break;
    case HID_REPORT_ID_3:
      if(pcshid->g_rxhid_buff[1] == 0)
      {
        
      }
      else
      {
        
      }
      break;
    case HID_REPORT_ID_4:
      if(pcshid->g_rxhid_buff[1] == 0)
      {
        
      }
      else
      {
        
      }
      break;
    case HID_REPORT_ID_6:
      for(i_index = 0; i_index < len; i_index ++)
      {
        pcshid->g_txhid_buff[i_index] = report[i_index];
      }
      custom_hid_class_send_report(pudev, pcshid->g_txhid_buff, len);
      break;
    default:
      break;
  }

}
