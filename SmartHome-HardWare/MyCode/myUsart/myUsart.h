#ifndef myUSART_H
#define myUSART_H

#include "main.h"
#include "util/uthash.h"
#include "util/mqttPacket.h"

// 定义函数指针类型
typedef void (*CommandFunc)(const char *device, const char *property, const char *value);

// 哈希表结构
typedef struct
{
    char cmd[10];     
    CommandFunc func;  
    UT_hash_handle hh; 
} CommandMap;


//标志位
extern uint8_t cmd_flag;

//指令存储
extern MqttJsonConfig cmd_mqtt_config;

// 串口
void ReceiveData_idle_init();
void ReceiveData_idle_handler(uint16_t Size);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

//hash映射执行函数
void initCmdHash();
void register_command(const char *cmd, CommandFunc func);
void execute_command(const char *cmd, const char *device, const char *property, const char *value);

#endif // myUsart.h