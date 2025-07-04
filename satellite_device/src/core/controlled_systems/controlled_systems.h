
/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROLLED_SYSTEMS_H
#define __CONTROLLED_SYSTEMS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "led.h"

/**
  * @brief LED for monitoring and prevention
  */
extern led_t controlLed;

void satelliteEquipmentInit(void);

#ifdef __cplusplus
}
#endif

#endif
