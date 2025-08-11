#include "myUsart.h"
#include "usart.h"
#include "oled/oled.h"
#include "led/led.h"
#include "esp8266/esp8266.h"

#define RECEIVE_BUFFER_SIZE 256
#define UART_HANDLE &huart1

CommandMap *cmdMap = NULL; // 全局哈希表
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

    initCmdHash(); 
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

/**
 * @brief 初始化命令哈希表
 *
 * @author xu
 * @date 2025-08-07
 */
void initCmdHash()
{
    register_command("light", update_led_state);
}

/**
 * @brief 注册命令和对应函数
 *
 * @param cmd
 * @param func
 * @author xu
 * @date 2025-08-06
 */
void register_command(const char *cmd, CommandFunc func)
{
    CommandMap *item = (CommandMap *)malloc(sizeof(CommandMap));
    strcpy(item->cmd, cmd);
    item->func = func;
    item->hh.next = NULL;
    if (cmdMap == NULL)
    {
        cmdMap = item;
    }
    else
    {
        CommandMap *current = cmdMap;
        while (current->hh.next != NULL)
        {
            current = (CommandMap *)(current->hh.next);
        }
        current->hh.next = (struct UT_hash_handle *)item;
    }
}

/**
 * @brief 查询命令并执行对应函数
 *
 * @param cmd
 * @param device
 * @param property
 * @param value
 * @author xu
 * @date 2025-08-06
 */
void execute_command(const char *cmd, const char *device, const char *property, const char *value)
{
    CommandMap *item = NULL;
    for (item = cmdMap; item != NULL; item = (CommandMap *)(item->hh.next))
    {
        if (strcmp(item->cmd, cmd) == 0)
        {
            break;
        }
    }
    if (item && item->func)
    {
        item->func(device, property, value);
    }
    else
    {
        OLED_ShowString(2, 0, "Command Not Found");
    }
}