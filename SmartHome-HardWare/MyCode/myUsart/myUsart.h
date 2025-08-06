#ifndef myUSART_H
#define myUSART_H

#include "main.h"
#include "uthash.h"

// 串口
void ReceiveData_idle_init();
void ReceiveData_idle_handler();
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

//hash映射

// 定义函数指针类型（无参数无返回值示例）
typedef void (*CommandFunc)(const char*device,const char* property,const char* value);

// 哈希表结构
typedef struct
{
    char cmd[10];         // 字符串键（命令名）
    CommandFunc func;  // 函数指针值
    UT_hash_handle hh; // Uthash必需字段
} CommandMap;


void initCmdHash();
void register_command(const char *cmd, CommandFunc func);
void execute_command(const char *cmd, const char *device, const char *property, const char *value);

#endif // myUsart.h