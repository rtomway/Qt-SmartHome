#ifndef myUSART_H
#define myUSART_H

#include "main.h"
#include "util/mqttPacket.h"


//标志位
extern uint8_t cmd_flag;

//指令存储
extern MqttJsonConfig cmd_mqtt_config;

// 串口
void ReceiveData_idle_init();
void ReceiveData_idle_handler(uint16_t Size);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);


#endif // myUsart.h