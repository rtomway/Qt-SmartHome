#ifndef __LED_H
#define __LED_H
#include "main.h"

#define LED_GPIO_Port GPIOA
#define LED_Pin_HALL GPIO_PIN_4
#define LED_Pin_BEDROOM GPIO_PIN_2
#define LED_Pin_BATHROOM GPIO_PIN_6

typedef struct
{
    char *led_name;
    GPIO_TypeDef *GPIO_Port;
    uint16_t Pin;
} LED_Config;


static LED_Config led_configs[] =
{
    {"hall_light",LED_GPIO_Port, LED_Pin_HALL},
    {"bedroom_light",LED_GPIO_Port, LED_Pin_BEDROOM},
    {"bathroom_light",LED_GPIO_Port, LED_Pin_BATHROOM}
};


void led_control(LED_Config led, GPIO_PinState state);

void led_key_on();

void update_led_state(const char *device, const char *property, const char *value);

#endif // __LED_H
