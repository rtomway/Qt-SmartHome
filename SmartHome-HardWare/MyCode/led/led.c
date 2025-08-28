#include "led.h"
#include <string.h>
#include <stdlib.h>

#include "util/mqttPacket.h"
#include "esp8266/esp8266.h"
#include "oled/oled.h"

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：led_control
 *  * 功能描述：灯光状态的控制
 *  * 输入参数：led {type}
 *  * 输入参数：state {type}
 ***********************************************************************************************************************/
void led_control(LED_Config led, GPIO_PinState state)
{
    HAL_GPIO_WritePin(led.GPIO_Port, led.Pin, state);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：update_led_state
 *  * 功能描述：更新灯光状态 接收灯光控制命令，并更新对应灯光状态
 *  * 输入参数：device {type}
 *  * 输入参数：property {type}
 *  * 输入参数：value {type}
 ***********************************************************************************************************************/
void update_led_state(const char *device, const char *property, const char *value)
{
    GPIO_PinState led_state;
    //获取灯光状态设置
    if (strcmp(value, "on") == 0)
    {
        led_state = GPIO_PIN_SET;
    }
    else if (strcmp(value, "off") == 0)
    {
        led_state = GPIO_PIN_RESET;
    }
    //找到对应设备并且设置绑定引脚状态
    //所有灯光一键控制
    if(strcmp(device,"All_light")==0)
    {
        for (int i = 0; i < sizeof(led_configs) / sizeof(LED_Config); i++)
        {
            led_control(led_configs[i], led_state);
        }
        return;
    }
    //单一灯光控制
    for(int i = 0; i < sizeof(led_configs) / sizeof(LED_Config); i++)
    {
        if (strcmp(device, led_configs[i].LED_NAME) == 0)
        {
           
            led_control(led_configs[i], led_state);
            break;
        }
    }
  
}

