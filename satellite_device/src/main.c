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
//#include "led.h"
#include "controlled_systems.h"

/* private includes ----------------------------------------------------------*/

/* private typedef -----------------------------------------------------------*/

/* private define ------------------------------------------------------------*/

/* private macro -------------------------------------------------------------*/

/* private variables ---------------------------------------------------------*/

led_t myLed;

/* private function prototypes --------------------------------------------*/

/* private user code ---------------------------------------------------------*/

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

  /* init usb app function. */
  wk_usb_app_init();

  /* init equipment function. */
  satelliteEquipmentInit();

  while(1)
  {
    //wk_usb_app_task();

    //gpio_bits_reset(LED_GPIO_PORT, LED_PIN);
    //LED_ReverseState(&myLed);
    //wk_delay_ms(1000);
    //gpio_bits_set(LED_GPIO_PORT, LED_PIN);
    //LED_ReverseState(&myLed);
    //wk_delay_ms(1000);
  }
}
