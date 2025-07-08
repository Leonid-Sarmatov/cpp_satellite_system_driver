/* add user code begin Header */
/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "at32f435_437_wk_config.h"
#include "wk_acc.h"
#include "wk_usb_otgfs.h"
#include "usb_app.h"
#include "wk_system.h"
#include "usart_debug.h"
#include "controlled_systems.h"
#include "FreeRTOS.h"
#include "task.h"

/* private includes ----------------------------------------------------------*/

/* private typedef -----------------------------------------------------------*/

/* private define ------------------------------------------------------------*/

/* private macro -------------------------------------------------------------*/

/* private variables ---------------------------------------------------------*/

// TaskHandle_t controlLedTaskHandler;
// TaskHandle_t usbTaskHandler;

/* private function prototypes --------------------------------------------*/

// void test_task(void *pvParameters);
// void usb_task(void *pvParameters);

/* private user code ---------------------------------------------------------*/
/* Idle task control block and stack */
// static StackType_t idle_task_stack[configMINIMAL_STACK_SIZE];
// static StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH];

// static StaticTask_t idle_task_tcb;
// static StaticTask_t timer_task_tcb;

// /* External Idle and Timer task static memory allocation functions */
// extern void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer, StackType_t ** ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );
// extern void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer, StackType_t ** ppxTimerTaskStackBuffer, uint32_t * pulTimerTaskStackSize );

// /*
//   vApplicationGetIdleTaskMemory gets called when configSUPPORT_STATIC_ALLOCATION
//   equals to 1 and is required for static memory allocation support.
// */
// void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer, StackType_t ** ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
// {
//   *ppxIdleTaskTCBBuffer = &idle_task_tcb;
//   *ppxIdleTaskStackBuffer = &idle_task_stack[0];
//   *pulIdleTaskStackSize = (uint32_t)configMINIMAL_STACK_SIZE;
// }
// /*
//   vApplicationGetTimerTaskMemory gets called when configSUPPORT_STATIC_ALLOCATION
//   equals to 1 and is required for static memory allocation support.
// */
// void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer, StackType_t ** ppxTimerTaskStackBuffer, uint32_t * pulTimerTaskStackSize )
// {
//   *ppxTimerTaskTCBBuffer = &timer_task_tcb;
//   *ppxTimerTaskStackBuffer = &timer_task_stack[0];
//   *pulTimerTaskStackSize = (uint32_t)configTIMER_TASK_STACK_DEPTH;
// }


/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* system clock config. */
  wk_system_clock_config();

  /* config periph clock. */
  wk_periph_clock_config();

  /* nvic config. */
  wk_nvic_config();

  /* timebase config. */
  wk_timebase_init();

  /* init acc function. */
  wk_acc_init();

  /* init usb_otgfs1 function. */
  wk_usb_otgfs1_init();

  /* init usart function. */
  usartDebugInit();

  /* init equipment function. */
  satelliteEquipmentInit();

  /* init usb app function. */
  //wk_usb_app_init();
  usbAppInit();

  /* start control led FreeRTOS task */
  // xTaskCreate((TaskFunction_t )test_task,
  //             (const char*    )"LED_test_task",
  //             (uint16_t       )512,
  //             (void*          )NULL,
  //             (UBaseType_t    )2,
  //             (TaskHandle_t*  )&controlLedTaskHandler);

  // xTaskCreate((TaskFunction_t )usb_task,
  //             (const char*    )"USB_test_task",
  //             (uint16_t       )2048,
  //             (void*          )NULL,
  //             (UBaseType_t    )2,
  //             (TaskHandle_t*  )&usbTaskHandler);

  /* start scheduler */
  vTaskStartScheduler();

  while(1)
  {
    //wk_usb_app_task();

    // gpio_bits_reset(LED_GPIO_PORT, LED_PIN);
    // //LED_ReverseState(&myLed);
    // wk_delay_ms(1000);
    // gpio_bits_set(LED_GPIO_PORT, LED_PIN);
    // //LED_ReverseState(&myLed);
    // wk_delay_ms(1000);
  }
}

// void test_task(void *pvParameters) {
//   while (1) {
//     gpio_bits_reset(LED_GPIO_PORT, LED_PIN);
//     vTaskDelay(pdMS_TO_TICKS(1000));
//     gpio_bits_set(LED_GPIO_PORT, LED_PIN);
//     vTaskDelay(pdMS_TO_TICKS(1000));
//   }
// }

// void usb_task(void *pvParameters) {
//   wk_usb_app_init();
//   while (1) {}
// }
