#ifndef myUSART_H
#define myUSART_H

#include "main.h"
#include "util/mqttPacket.h"

#define CMD_MAX_NUM 5


//标志位
extern  volatile uint8_t cmd_flag;
extern  uint8_t pushData_result_flag;

extern volatile uint8_t write_cmd_index;
extern  uint8_t read_cmd_index;

//指令存储
extern MqttJsonConfig cmd_mqtt_config[CMD_MAX_NUM];

// 串口
void ReceiveData_idle_init();
void ReceiveData_idle_handler(uint16_t Size);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);


#endif // myUsart.h