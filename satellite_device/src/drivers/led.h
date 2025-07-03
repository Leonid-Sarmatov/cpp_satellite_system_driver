
/* define to prevent recursive inclusion -----------------------------------*/
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ----------------------------------------------------------------*/
#include "at32f435_437_wk_config.h"
#include <stdbool.h>

/* exported types ----------------------------------------------------------*/
typedef struct {
    gpio_type *gpio; 
    uint16_t pin;
    bool state;
} led_t;

/* exported functions ------------------------------------------------------*/
void LED_Init(led_t *led, gpio_type *gpio, uint16_t pin);
bool LED_IsEnableState(led_t *led);
void LED_SetEnableState(led_t *led);
void LED_SetDisableState(led_t *led);
void LED_ReverseState(led_t *led);

#ifdef __cplusplus
}
#endif

#endif