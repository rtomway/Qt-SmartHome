#ifndef myUsart_h
#define myUsart_h

#include <stdint.h>

//数据获取
typedef struct MqttJsonConfig
{
    char product[32];
    char device[32];
    char property[32];
    char value[32];
} MqttJsonConfig;

// 解析指令数据
MqttJsonConfig parseMqttJson(char*receive_data,uint16_t Size);

//打包json数据
void packetMqttJsonArray(MqttJsonConfig *config_array, uint8_t count, char *json_str, uint16_t buf_size);

#endif // myUsart.h