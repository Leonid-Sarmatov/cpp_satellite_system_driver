
#include "controlled_systems.h"

#include "led.h"

extern led_t controlLed = {0};

/**
  * @brief  initialization of satellite payload
  * @param  none
  * @retval none
  */
void satelliteEquipmentInit() {
  /* control led init */
  LED_Init(&controlLed, LED_GPIO_PORT, LED_PIN);
}