#include "mqttPacket.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "oled/oled.h"

/**
 * @brief 解析JSON数据
 *
 * @return MqttJsonConfig
 * @author xu
 * @date 2025-08-07
 */
MqttJsonConfig parseMqttJson(char *receive_data,uint16_t Size)
{
    char json_str[256];
    // 接收到来自于MQTT broker发送的消息
    int index = -1;

    // 构建json_str
    for (int i = 0; i < Size; i++)
    {
        if (index == -1 && (receive_data[i] == '{' || receive_data[i] == '['))
        {
            // 说明从这一位开始是json数据
            index = i;
        }
        if (index != -1)
        {
            json_str[i - index] = receive_data[i];
        }
    }
    // 处理json数据
    cJSON *root = cJSON_Parse(json_str);
    MqttJsonConfig mqttJsonConfig;
    cJSON *product = cJSON_GetObjectItem(root, "product");
    strcpy(mqttJsonConfig.product, product->valuestring);
    cJSON *device = cJSON_GetObjectItem(root, "device");
    strcpy(mqttJsonConfig.device, device->valuestring);
    cJSON *property = cJSON_GetObjectItem(root, "property");
    strcpy(mqttJsonConfig.property, property->valuestring);
    cJSON *value = cJSON_GetObjectItem(root, "value");
    strcpy(mqttJsonConfig.value, value->valuestring);

    cJSON_Delete(root);

    return mqttJsonConfig;
}


/**
 * @brief 打包JSON数组数据
 * 
 * @param config_array 
 * @param count 
 * @param json_str 
 * @param buf_size 
 * @author xu
 * @date 2025-08-11
 */
void packetMqttJsonArray(MqttJsonConfig *config_array, uint8_t count, char *json_str, uint16_t buf_size)
{
    memset(json_str, 0, buf_size); 
    if (count == 0 || config_array == NULL)
    {
        strcpy(json_str, "{}"); 
        return;
    }

    // 拼接JSON起始符
    uint16_t len = snprintf(json_str, buf_size, "{");

    // 遍历数组，拼接每个传感器的JSON对象
    for (uint8_t i = 0; i < count; i++)
    {
        MqttJsonConfig *config = &config_array[i];

        // 拼接单个传感器对象（格式与packetMqttJson一致）
        char temp[128]; 
        snprintf(temp, sizeof(temp),
                 "\\\"%s\\\":{\\\"device\\\":\\\"%s\\\"\\,\\\"property\\\":\\\"%s\\\"\\,\\\"value\\\":\\\"%s\\\"}",
                 config->product, config->device, config->property, config->value);

        if (i > 0)
        {
            len += snprintf(json_str + len, buf_size - len, "\\,"); 
        }
        len += snprintf(json_str + len, buf_size - len, "%s", temp);

        if (len >= buf_size - 16)
        { 
            break;
        }
    }

    snprintf(json_str + len, buf_size - len, "}");
}