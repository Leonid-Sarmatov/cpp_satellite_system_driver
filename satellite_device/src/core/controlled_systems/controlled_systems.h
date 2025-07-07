
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
extern uint8_t ledBlinkFlag;

void satelliteEquipmentInit(void);
void controlLedInit();

#ifdef __cplusplus
}
#endif

#endif
