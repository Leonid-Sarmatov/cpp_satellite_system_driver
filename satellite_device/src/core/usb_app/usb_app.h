
/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_APP_H
#define __USB_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* private includes ----------------------------------------------------------*/

/* private define ------------------------------------------------------------*/

/* exported types ------------------------------------------------------------*/

/* exported constants --------------------------------------------------------*/

/* exported macro ------------------------------------------------------------*/

/* exported functions --------------------------------------------------------*/

/**
  * @brief  usb application initialization
  * @param  none
  * @retval none
  */
void usbAppInit(void);

/**
  * @brief  usb application task
  * @param  none
  * @retval none
  */
void wk_usb_app_task(void);

/**
  * @brief  usb interrupt handler
  * @param  none
  * @retval none
  */
void wk_otgfs1_irq_handler(void);

#ifdef __cplusplus
}
#endif

#endif
