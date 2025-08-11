#include "myAdc.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "oled/oled.h"
#include <math.h>
#include "util/mqttPacket.h"
#include "esp8266/esp8266.h"



float adc_value;
uint16_t adc_buf[ADC_CHANNEL_COUNT] = {0};
float temp_value=0;
float light_value=0;
uint8_t adc_conv_complete_flag = 0;


/**
 * @brief 重写adc转换完成回调函数(定时器软件触发)
 *    
 * @param hadc 
 * @author xu
 * @date 2025-07-21
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        // 温度转换
        float Rntc = 10000.0f * adc_buf[0] / (4096.0f - adc_buf[0]);
        // 公式：T(℃) = 1/(1/298.15 + (1/3936)*ln(Rntc/10000)) - 273.15
        temp_value = 1.0f / ((1.0f / 298.15f) + (1.0f / 3936.0f) * log(Rntc / 10000.0f)) - 273.15f;

        //光照强度转换
        light_value = (4095.0f - adc_buf[1]) * 100.0f / 4095.0f;

        // 转换完成
        adc_conv_complete_flag = 1;
    }
  
}


/**
 * @brief 定时上传传感器数据
 * 
 * @author xu
 * @date 2025-08-11
 */

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