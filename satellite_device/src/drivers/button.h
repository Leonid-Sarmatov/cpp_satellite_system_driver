
/* define to prevent recursive inclusion -----------------------------------*/
#ifndef __BUTTON_H
#define __BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ----------------------------------------------------------------*/
#include "at32f435_437_wk_config.h"

/* exported types ----------------------------------------------------------*/
typedef struct {
    gpio_type *gpio; 
    uint16_t pin;
} button_t;

/* exported functions ------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif