
#include "controlled_systems.h"

#include "FreeRTOS.h"
#include "task.h"

#include "led.h"

/* private includes ----------------------------------------------------------*/

/* private typedef -----------------------------------------------------------*/

/* private define ------------------------------------------------------------*/

/* private macro -------------------------------------------------------------*/

/* private variables ---------------------------------------------------------*/

static TaskHandle_t controlLedTaskHandler;

extern led_t controlLed = {0};
extern uint8_t ledBlinkFlag = 1;

/* private function prototypes --------------------------------------------*/

void vControlLedTask(void *pvParameters);
void controlLedInit(void);

/* private user code ---------------------------------------------------------*/

/**
  * @brief  initialization of satellite payload
  * @param  none
  * @retval none
  */
void satelliteEquipmentInit() {
  controlLedInit();
}

/**
  * @brief  initialization and preparation for work of control led 
  * @param  none
  * @retval none
  */
void controlLedInit() {
  /* control led init */
  LED_Init(&controlLed, LED_GPIO_PORT, LED_PIN);

  /* start control led FreeRTOS task */
  taskENTER_CRITICAL();

  xTaskCreate((TaskFunction_t )vControlLedTask,
              (const char*    )"LED_task",
              (uint16_t       )512,
              (void*          )NULL,
              (UBaseType_t    )2,
              (TaskHandle_t*  )&controlLedTaskHandler);

  taskEXIT_CRITICAL();
}

/**
  * @brief  FreeRTOS task 
  * @param  pvParameters
  * @retval none
  */
void vControlLedTask(void *pvParameters) {
  while (1) {
    if (ledBlinkFlag == 1) {
      LED_ReverseState(&controlLed);
    }

    taskENTER_CRITICAL();
    printf("led blink task, count = %lu\r\n", xTaskGetTickCount());
    taskEXIT_CRITICAL();

    vTaskDelay(pdMS_TO_TICKS(200));
  }
}