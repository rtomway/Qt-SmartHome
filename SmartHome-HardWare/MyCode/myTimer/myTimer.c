#include "myTimer.h"
#include "oled/oled.h"
#include "tim.h"
#include "util/mqttPacket.h"
#include "esp8266/esp8266.h"
#include <string.h>

/**
 * @brief 开启定时器2
 * 
 * @author xu
 * @date 2025-07-16
 * @version 1.0
 */
void timer_start(void)
{
   HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief 定时器2停止
 * 
 * @author xu
 * @date 2025-07-16
 * @version 1.0
 */
void timer_stop(void)
{
    HAL_TIM_Base_Stop_IT(&htim2);
}

/**
 * @brief 定时器2中断回调函数
 * 
 * @param htim 
 * @author xu
 * @date 2025-07-16
 * @version 1.0
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        /*  MqttJsonConfig mqttJsonConfig;
        strcpy(mqttJsonConfig.product, "light_xx");
        strcpy(mqttJsonConfig.device, "hall_light_xx");
        strcpy(mqttJsonConfig.property, "state_xx");
        strcpy(mqttJsonConfig.value, "on_xx");

        char mqttJsonStr[128];
        packetMqttJson(mqttJsonConfig, mqttJsonStr);

        esp8266_public_data("smartHome/data", mqttJsonStr, 0, 0); */
    }
}

