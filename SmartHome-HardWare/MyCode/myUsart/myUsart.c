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
uint8_t pushData_result_flag = 0;
MqttJsonConfig cmd_mqtt_config = {0};

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：ReceiveData_idle_init
 *  * 功能描述：空闲中断初始化函数
 ***********************************************************************************************************************/
void ReceiveData_idle_init()
{
    __HAL_UART_CLEAR_IDLEFLAG(UART_HANDLE);
    __HAL_UART_ENABLE_IT(UART_HANDLE, UART_IT_IDLE);
    memset(esp8266_receive_data, 0, RECEIVE_BUFFER_SIZE);
    HAL_UARTEx_ReceiveToIdle_IT(UART_HANDLE, (uint8_t *)esp8266_receive_data, RECEIVE_BUFFER_SIZE);

}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：ReceiveData_idle_handler
 *  * 功能描述：空闲中断处理函数    负责解析接收到的数据 存储相关指令信息
 *  * 输入参数：Size {type}
 ***********************************************************************************************************************/
void ReceiveData_idle_handler(uint16_t Size)
{
    if(cmd_flag==0)
    {
        // 处理接收到的数据
        cmd_mqtt_config = parseMqttJson(esp8266_receive_data, Size);
        cmd_flag = 1;
    }
}

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：HAL_UARTEx_RxEventCallback
 *  * 功能描述：空闲中断回调函数
 *  * 输入参数：huart {type}
 *  * 输入参数：Size {type}
 ***********************************************************************************************************************/
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
            pushData_result_flag = 0;
        }
        if (strstr((char *)esp8266_receive_data, "ERROR") != NULL)
        {
            pushData_result_flag = 1;
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