#include "myAdc.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "oled/oled.h"

/**
 * @brief 开启adc转换
 * 
 * @author xu
 * @date 2025-07-21
 */
void myAdc_Start(void)
{
    HAL_ADC_Start(&hadc1);
}

/**
 * @brief 关闭adc转换
 * 
 * @author xu
 * @date 2025-07-21
 */
void myAdc_Stop(void)
{
    HAL_ADC_Stop(&hadc1);
}

/**
 * @brief 获取adc转换结果
 * 
 * @return uint16_t 
 * @author xu
 * @date 2025-07-21
 */
static float adc_value = 0;
float myAdc_GetValue()
{
    HAL_ADCEx_Calibration_Start(&hadc1);
    myAdc_Start();

    float temperature;
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        adc_value = HAL_ADC_GetValue(&hadc1);
        temperature = adc_value * 3.3 / 4095 - 3;
    }
    myAdc_Stop();
    //sprintf(str, "%.3f", adc_value);

    return adc_value;
}



/**
 * @brief 重写adc转换完成回调函数
 *      (外部触发定时器比较输出触发channel模式要设置成toggle模式)
 * @param hadc 
 * @author xu
 * @date 2025-07-21
 */
//uint32_t adc_buf[1] = {0};
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc_value = HAL_ADC_GetValue(&hadc1);
    OLED_ShowNum(1, 0, adc_value, 5);
    uint32_t adc_val = adc_buf[0];
    OLED_ShowNum(2, 0, adc_val, 5);
}

void HAL_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
{
    uint32_t adc_val = adc_buf[0];
    OLED_ShowNum(2, 8, 1, 2);
   HAL_ADC_Start_DMA(&hadc1, adc_buf, 1);
}

