
/* Includes ------------------------------------------------------------------*/
#include "led.h"

/**
  * @brief  Initialize LED GPIO and internal state.
  * @param  led: Pointer to led_t structure.
  * @param  gpio: GPIO port.
  * @param  pin: GPIO pin number.
  * @retval None
  */
void LED_Init(led_t *led, gpio_type *gpio, uint16_t pin) { 
    if (!led) return;

    gpio_init_type gpio_init_struct;
    gpio_default_para_init(&gpio_init_struct);

    gpio_bits_set(gpio, pin);

    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_pins = pin;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init(gpio, &gpio_init_struct);

    led->pin = pin;
    led->gpio = gpio;
    led->state = false;
}

/**
  * @brief  Get the current enable state of the LED.
  * @param  led: Pointer to led_t structure.
  * @retval true if LED is enabled, false otherwise.
  */
bool LED_IsEnableState(led_t *led) {
    return led ? led->state : false;
}

/**
  * @brief  Set LED to enabled state (ON).
  * @param  led: Pointer to led_t structure.
  * @retval None
  */
void LED_SetEnableState(led_t *led) {
    if (!led) return;
    gpio_bits_reset(led->gpio, led->pin);
    led->state = true;
}

/**
  * @brief  Set LED to disabled state (OFF).
  * @param  led: Pointer to led_t structure.
  * @retval None
  */
void LED_SetDisableState(led_t *led) {
    if (!led) return;
    gpio_bits_set(led->gpio, led->pin);
    led->state = false;
}

/**
  * @brief  Toggle (reverse) LED state.
  * @param  led: Pointer to led_t structure.
  * @retval None
  */
void LED_ReverseState(led_t *led) {
    if (!led) return;
    if (led->state) {
        gpio_bits_set(led->gpio, led->pin);
    } else {
        gpio_bits_reset(led->gpio, led->pin);
    }
    led->state = !led->state;
}
