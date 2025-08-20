#ifndef CMDMAP_H
#define CMDMAP_H

#include "led/led.h"
#include "myTimer/myTimer.h"

// 定义函数指针类型
typedef void (*CommandFunc)(const char *device, const char *property, const char *value);

// 定义命令映射表
typedef struct
{
    char cmd[10];
    CommandFunc func;
} CommandMap;

void execute_command(const char *cmd, const char *device, const char *property, const char *value);

#endif // CMDMAP_H