#include "myUsart.h"
#include "usart.h"
#include "oled/oled.h"
#include "led/led.h"
#include "esp8266/esp8266.h"
#include "myTimer/myTimer.h"
#include <stdio.h>
#include <string.h>

#define RECEIVE_BUFFER_SIZE 256
#define UART_HANDLE &huart1

uint8_t cmd_flag = 0;
MqttJsonConfig cmd_mqtt_config={0};

/**
 * @brief 开启串口空闲中断
 *
 * @author xu
 * @date 2025-08-06
 */
void ReceiveData_idle_init()
{
    __HAL_UART_CLEAR_IDLEFLAG(UART_HANDLE);
    __HAL_UART_ENABLE_IT(UART_HANDLE, UART_IT_IDLE);
    memset(esp8266_receive_data, 0, RECEIVE_BUFFER_SIZE);
    HAL_UARTEx_ReceiveToIdle_IT(UART_HANDLE, (uint8_t *)esp8266_receive_data, RECEIVE_BUFFER_SIZE);

}

/**
 * @brief 串口空闲中断处理函数
 *
 * @author xu
 * @date 2025-08-06
 */
void ReceiveData_idle_handler(uint16_t Size)
{
    if(cmd_flag==0)
    {
        // 处理接收到的数据
        cmd_mqtt_config = parseMqttJson(esp8266_receive_data, Size);
        cmd_flag = 1;
    }
}

/**
 * @brief 接收到数据回调函数
 *
 * @param huart
 * @param Size
 * @author xu
 * @date 2025-08-06
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        // 未连接
        if (strstr((char *)esp8266_receive_data, "MQTTDISCONNECTED") != NULL)
        {
            esp8266_mqtt_connect();
        }
        //at指令回复
        if (strstr((char *)esp8266_receive_data, "OK") != NULL)
        {
            OLED_ShowString(1, 0, "public data OK");
        }
        if (strstr((char *)esp8266_receive_data, "ERROR") != NULL)
        {
            OLED_ShowString(1, 0, "public data ERROR");
        }
        //订阅接收qt客户端指令
        if (strstr((char *)esp8266_receive_data, "MQTTSUBRECV") != NULL)
        {
            ReceiveData_idle_handler(Size); // 调用处理函数
        }
    }

    // 清除缓冲区等待下次接收
    memset(esp8266_receive_data, 0, RECEIVE_BUFFER_SIZE);
    HAL_UARTEx_ReceiveToIdle_IT(UART_HANDLE, (uint8_t *)esp8266_receive_data, RECEIVE_BUFFER_SIZE);
}