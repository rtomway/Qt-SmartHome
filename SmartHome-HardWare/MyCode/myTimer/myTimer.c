#include "myTimer.h"
#include "oled/oled.h"
#include "tim.h"
#include "util/mqttPacket.h"
#include "esp8266/esp8266.h"
#include <string.h>
#include <adc.h>
#include <myAdc/myAdc.h>

uint8_t timing_flag = 0;

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

