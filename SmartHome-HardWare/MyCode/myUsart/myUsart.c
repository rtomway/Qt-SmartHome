#include "myUsart.h"
#include "usart.h"
#include "oled/oled.h"

#include "led/led.h"
#define RECEIVE_BUFFER_SIZE 256
#define UART_HANDLE &huart1

char receive_data[RECEIVE_BUFFER_SIZE] = {0};
CommandMap *cmdMap = NULL; // 全局哈希表

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
    HAL_UARTEx_ReceiveToIdle_IT(UART_HANDLE, (uint8_t *)receive_data, RECEIVE_BUFFER_SIZE);

    initCmdHash(); // 初始化命令哈希表
}

/**
 * @brief 串口空闲中断处理函数
 *
 * @author xu
 * @date 2025-08-06
 */
void ReceiveData_idle_handler()
{
    // 处理接收到的数据
    if (strstr(receive_data, "on") != NULL)
    {
        // 执行对应的命令
        if (strstr(receive_data, "All_light") != NULL)
            execute_command("light", "All_light", "state", "on");

        // 执行对应的命令
        if (strstr(receive_data, "hall_light") != NULL)
            execute_command("light", "hall_light", "state", "on");

        // 执行对应的命令
        if (strstr(receive_data, "bedroom_light") != NULL)
            execute_command("light", "bedroom_light", "state", "on");

        // 执行对应的命令
        if (strstr(receive_data, "bathroom_light") != NULL)
            execute_command("light", "bathroom_light", "state", "on");
    }
    if (strstr(receive_data, "off") != NULL)
    {
        // 执行对应的命令
        if (strstr(receive_data, "All_light") != NULL)
            execute_command("light", "All_light", "state", "off");

        // 执行对应的命令
        if (strstr(receive_data, "hall_light") != NULL)
            execute_command("light", "hall_light", "state", "off");

        // 执行对应的命令
        if (strstr(receive_data, "bedroom_light") != NULL)
            execute_command("light", "bedroom_light", "state", "off");

        // 执行对应的命令
        if (strstr(receive_data, "bathroom_light") != NULL)
            execute_command("light", "bathroom_light", "state", "off");
    }

    // 清除缓冲区等待下次接收
    memset(receive_data, 0, RECEIVE_BUFFER_SIZE);
    HAL_UARTEx_ReceiveToIdle_IT(UART_HANDLE, (uint8_t *)receive_data, RECEIVE_BUFFER_SIZE);
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
        ReceiveData_idle_handler(); // 调用处理函数
    }
}

// 初始化指令哈希表
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