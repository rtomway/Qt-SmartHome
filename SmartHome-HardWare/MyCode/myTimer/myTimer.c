#include "myTimer.h"
#include "oled/oled.h"
#include "tim.h"
#include "util/mqttPacket.h"
#include "esp8266/esp8266.h"
#include <string.h>
#include "adc.h"
#include "myAdc/myAdc.h"

uint8_t timing_flag = 0;
FAN_STATE fan_state = FAN_OFF;

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：HAL_TIM_PeriodElapsedCallback
 *  * 功能描述：定时器中断回调函数 负责触发adc转换和设置定时标志位
 *  * 输入参数：htim {type}
 ***********************************************************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // 标志位置1主循环检测处理
    if (htim->Instance == TIM3)
    {
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buf, 5);
        //转换标志
        adc_conv_complete_flag = 0;
        timing_flag = 1;
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：fan_control
 *  * 功能描述：通过修改PWM占空比控制风扇转速
 *  * 输入参数：speed {type}
 ***********************************************************************************************************************/
void fan_control(uint8_t speed)
{
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 999);
    HAL_Delay(200);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, speed);
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：update_fan_state
 *  * 功能描述：更新风扇状态 接收风扇控制命令，并更新对应风扇状态
 *  * 输入参数：device {type}
 *  * 输入参数：property {type}
 *  * 输入参数：value {type}
 ***********************************************************************************************************************/
void update_fan_state(const char *device, const char *property, const char *value)
{
    if (strcmp(property, "speedMode") != 0)
        return;

    for (int i = 0;i<sizeof(fan_speed_table)/sizeof(FAN_SPEED);i++)
    {
        if(strcmp(value,fan_speed_table[i].speed_value) == 0)
        {
            fan_control(fan_speed_table[i].speed_pwm);
        }
    }
}