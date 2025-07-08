
#include "usb_conf.h"
#include "usb_core.h"
#include "wk_system.h"

#include "usbd_int.h"
#include "custom_hid_class.h"
#include "custom_hid_desc.h"
#include "controlled_systems.h"

#include "FreeRTOS.h"
#include "task.h"

#include "led.h"
#include "controlled_systems.h"
#include "usart_debug.h"

/* private includes ----------------------------------------------------------*/

/* private typedef -----------------------------------------------------------*/

/* private define ------------------------------------------------------------*/

/* private macro -------------------------------------------------------------*/

/* private variables ---------------------------------------------------------*/

/**
  * @brief interface structure to USB OTG FS1 controller
  */
static otg_core_type otg_core_struct_fs1;

/**
  * @brief FreeRTOS task descriptor
  */
static TaskHandle_t usbTaskHandler;

/* private function prototypes --------------------------------------------*/

void usbAppTask(void *pvParameters);

/**
  * @brief  usb application initialization
  * @param  none
  * @retval none
  */
void usbAppInit(void) {
  /* FreeRTOS task create */
  taskENTER_CRITICAL();

  xTaskCreate((TaskFunction_t )usbAppTask,
              (const char*    )"USB_task",
              (uint16_t       )1024,
              (void*          )NULL,
              (UBaseType_t    )0,
              (TaskHandle_t*  )&usbTaskHandler);
  
  taskEXIT_CRITICAL();
}

/**
  * @brief  FreeRTOS task 
  * @param  pvParameters
  * @retval none
  */
void usbAppTask(void *pvParameters) {
  vTaskDelay(pdMS_TO_TICKS(5000));
  //usartDebugInit();

  /*fs1 device custom hid */
  usbd_init(&otg_core_struct_fs1,
            USB_FULL_SPEED_CORE_ID,
            USB_OTG1_ID,
            &custom_hid_class_handler,
            &custom_hid_desc_handler);
  
  vTaskDelay(pdMS_TO_TICKS(5000));
  //usartDebugInit();

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(200));
  }
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
