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
 * @brief 打包JSON数据
 * 
 * @param mqttJsonConfig 
 * @return char* 
 * @author xu
 * @date 2025-08-07
 */
void packetMqttJson(MqttJsonConfig mqttJsonConfig, char *json_str)
{
    // 创建 cJSON 对象
   /*  cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        return ;
    }

    cJSON *product = cJSON_CreateString(mqttJsonConfig.product);
    cJSON_AddItemToObject(root, "product", product);


    cJSON *device = cJSON_CreateString(mqttJsonConfig.device);
    cJSON_AddItemToObject(root, "device", device);

    cJSON *property = cJSON_CreateString(mqttJsonConfig.property);
    cJSON_AddItemToObject(root, "property", property);

    cJSON *value = cJSON_CreateString(mqttJsonConfig.value);
    cJSON_AddItemToObject(root, "value", value); */

  /*   // 将 JSON 对象转换为字符串
  cJSON_PrintPreallocated(root, json_str, 256, 0);
   
    // 释放 cJSON 对象
    cJSON_Delete(root); */

    snprintf(json_str, 256,
             "{\\\"product\\\":\\\"%s\\\"\\,\\\"device\\\":\\\"%s\\\"\\,\\\"property\\\":\\\"%s\\\"\\,\\\"value\\\":\\\"%s\\\"}",
             mqttJsonConfig.product, mqttJsonConfig.device, mqttJsonConfig.property, mqttJsonConfig.value);
}