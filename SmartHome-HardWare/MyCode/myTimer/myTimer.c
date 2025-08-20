#include "myTimer.h"
#include "oled/oled.h"
#include "tim.h"
#include "util/mqttPacket.h"
#include "esp8266/esp8266.h"
#include <string.h>
#include <adc.h>
#include <myAdc/myAdc.h>

uint8_t timing_flag = 0;
FAN_STATE fan_state = FAN_OFF;

/**
 * @brief 定时器3中断回调函数
 * 
 * @param htim 
 * @author xu
 * @date 2025-07-16
 * @version 1.0
 */
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



/**
 * @brief 定时器2pwm风扇控制输出函数
 * 
 * @author xu
 * @date 2025-08-20
 */
void fan_control(uint8_t speed)
{
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, speed);
}

/**
 * @brief 更新风扇状态函数
 * 
 * @param device 
 * @param property 
 * @param value 
 * @author xu
 * @date 2025-08-20
 */
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