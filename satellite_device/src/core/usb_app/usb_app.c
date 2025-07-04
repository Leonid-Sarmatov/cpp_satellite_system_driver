
#include "usb_conf.h"
#include "usb_core.h"
#include "wk_system.h"

#include "usbd_int.h"
#include "custom_hid_class.h"
#include "custom_hid_desc.h"
#include "controlled_systems.h"

#include "led.h"

/* private includes ----------------------------------------------------------*/

/* private typedef -----------------------------------------------------------*/

/* private define ------------------------------------------------------------*/

/* private macro -------------------------------------------------------------*/

/* private variables ---------------------------------------------------------*/

/**
  * @brief interface structure to USB OTG FS1 controller
  */
static otg_core_type otg_core_struct_fs1;

/* private function prototypes --------------------------------------------*/

/**
  * @brief  usb application initialization
  * @param  none
  * @retval none
  */
void wk_usb_app_init(void) {
  /*fs1 device custom hid */
  usbd_init(&otg_core_struct_fs1,
            USB_FULL_SPEED_CORE_ID,
            USB_OTG1_ID,
            &custom_hid_class_handler,
            &custom_hid_desc_handler);
}

/**
  * @brief  usb application task
  * @param  none
  * @retval none
  */
void wk_usb_app_task(void) {

}

/**
  * @brief  usb interrupt handler
  * @param  none
  * @retval none
  */
void wk_otgfs1_irq_handler(void) {
  usbd_irq_handler(&otg_core_struct_fs1);
}

/**
  * @brief  usb delay function
  * @param  ms: delay number of milliseconds.
  * @retval none
  */
void usb_delay_ms(uint32_t ms) {
  wk_delay_ms(ms);
}
