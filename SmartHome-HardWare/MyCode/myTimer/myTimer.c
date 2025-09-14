#include "myTimer.h"
#include "oled/oled.h"
#include "tim.h"
#include "util/mqttPacket.h"
#include "esp8266/esp8266.h"
#include <string.h>
#include "adc.h"
#include "myAdc/myAdc.h"
#include "stdlib.h"

#define SERVO_MIN_ANGLE 0    
#define SERVO_MAX_ANGLE 180  
#define SERVO_MIN_PULSE 500  
#define SERVO_MAX_PULSE 2500

uint8_t volatile timing_flag = 0;
FAN_STATE fan_state = FAN_OFF;
uint16_t current_angle = 0;

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
void fan_control(uint16_t speed)
{
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, speed);
    OLED_ShowNum(2, 0, __HAL_TIM_GetCompare(&htim2, TIM_CHANNEL_3),4);
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

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：curtain_servo_init
 *  * 功能描述：初始化窗帘舵机
 ***********************************************************************************************************************/
void curtain_servo_init()
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, angle_to_ccr(0));
}
/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：angle_to_ccr
 *  * 功能描述：将角度转换为定时器CCR值
 *  * 输入参数：angle {type}
 *  * 返 回 值：uint32_t
 ***********************************************************************************************************************/
static uint32_t angle_to_ccr(uint16_t angle)
{
    if (angle > SERVO_MAX_ANGLE)
    {
        angle = SERVO_MAX_ANGLE;
    }
       
    uint32_t pulse_width = SERVO_MIN_PULSE +(angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE)) / SERVO_MAX_ANGLE;
    return pulse_width; 
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：curtain_servo_control
 *  * 功能描述：控制窗帘舵机位置
 *  * 输入参数：target_angle {type}
 ***********************************************************************************************************************/
void curtain_servo_control(uint16_t target_angle)
{
    while (current_angle != target_angle)
    {
        if (current_angle < target_angle)
            current_angle++;
        else
            current_angle--;

        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, angle_to_ccr(current_angle));
        HAL_Delay(15); 
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：update_curtain_position
 *  * 功能描述：更新窗帘位置 接收窗帘控制命令，并更新对应窗帘位置
 *  * 输入参数：device {type}
 *  * 输入参数：property {type}
 *  * 输入参数：value {type}
 ***********************************************************************************************************************/
void update_curtain_position(const char *device, const char *property, const char *value)
{
    if (device == NULL || property == NULL || value == NULL)
    {
        return;
    }

    if (strcmp(property, "position") == 0)
    {
        int percentage = atoi(value);
        uint16_t angle = percentage * SERVO_MAX_ANGLE / 100;
        curtain_servo_control((uint16_t)angle);
    }
}