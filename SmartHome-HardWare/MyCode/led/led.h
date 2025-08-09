#ifndef __LED_H
#define __LED_H
#include "main.h"

#define LED_GPIO_Port GPIOA
#define LED_Pin_HALL GPIO_PIN_4
#define LED_Pin_BEDROOM GPIO_PIN_2
#define LED_Pin_BATHROOM GPIO_PIN_6

//灯光配置结构体
typedef struct
{
    char *LED_NAME;
    GPIO_TypeDef *GPIO_Port;
    uint16_t Pin;
} LED_Config;

//灯光数组
static LED_Config led_configs[] =
{
    {"hall_light",LED_GPIO_Port, LED_Pin_HALL},
    {"bedroom_light",LED_GPIO_Port, LED_Pin_BEDROOM},
    {"bathroom_light",LED_GPIO_Port, LED_Pin_BATHROOM}
};

//灯光和状态控制
void led_control(LED_Config led, GPIO_PinState state);
//灯光状态更新
void update_led_state(const char *device, const char *property, const char *value);
//获取灯光状态
//MqttJsonConfig get_led_state(const char *device, const char *property, const char *value);


#endif // __LED_H
