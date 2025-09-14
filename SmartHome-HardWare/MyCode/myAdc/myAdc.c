#include "myAdc.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "oled/oled.h"
#include "main.h"
#include "util/mqttPacket.h"
#include "esp8266/esp8266.h"
#include "math.h"

#define INDOORTEMP_ABNORMAL 30

uint16_t adc_buf[ADC_CHANNEL_COUNT] = {0};
volatile uint8_t adc_conv_complete_flag = 0;
volatile uint8_t temp_abnormal_flag = 0;

float temp_value=0;
float light_value=0;

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：HAL_ADC_ConvCpltCallback
 *  * 功能描述：adc转换完成回调函数 由定时器触发，负责温度和光照强度的计算并且修改相关标志位
 *  * 输入参数：hadc {type}
 ***********************************************************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        // 温度转换
        float Rntc = 10000.0f * adc_buf[0] / (4096.0f - adc_buf[0]);
        // 公式：T(℃) = 1/(1/298.15 + (1/3936)*ln(Rntc/10000)) - 273.15
        temp_value = 1.0f / ((1.0f / 298.15f) + (1.0f / 3936.0f) * log(Rntc / 10000.0f)) - 273.15f;

        //温度异常触发警报
        temp_abnormal_flag = 0;
        if (temp_value > INDOORTEMP_ABNORMAL)
        {
            temp_abnormal_flag = 1;
        }

        //光照强度转换
        light_value = (4095.0f - adc_buf[1]) * 100.0f / 4095.0f;

        // 转换完成
        adc_conv_complete_flag = 1;
    }
  
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：myAdc_data_public
 *  * 功能描述：adc
 ***********************************************************************************************************************/
void myAdc_data_public()
{
    if(adc_conv_complete_flag==0)
        return;

    MqttJsonConfig sensors[2];

    // 温度
    strcpy(sensors[0].product, "temperatureSensor"); 
    strcpy(sensors[0].device, "esp8266");
    strcpy(sensors[0].property, "temperature");
    snprintf(sensors[0].value, sizeof(sensors[0].value), "%.1f", temp_value);

    //光照
    strcpy(sensors[1].product, "lightSensor");
    strcpy(sensors[1].device, "esp8266");
    strcpy(sensors[1].property, "light");
    snprintf(sensors[1].value, sizeof(sensors[1].value), "%.1f", light_value);

    char json_str[512];
    packetMqttJsonArray(sensors, 2, json_str, sizeof(json_str));
    esp8266_public_data("smartHome/data", json_str, 0, 0);
}