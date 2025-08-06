#include "esp8266.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <usart.h>
#include "oled/oled.h"


#define  ESP8266_BUFFER_SIZE  256
#define  ESP8266_RECEIVE_TIMEOUT 1000

char esp8266_receive_data[ESP8266_BUFFER_SIZE] = {0};
int esp8266_wifiState = 0;
int esp8266_mqttState = 0;

HAL_StatusTypeDef esp8266_init(void)
{
    //检查esp8266是否正常工作
    esp8266_at_query("AT");
    if (strstr(esp8266_receive_data, "OK") == NULL)
    {
        return HAL_ERROR;
    }
    //检查wifi是否正常工作
    if(esp8266_wifi_checkandReCon()== HAL_ERROR)
    {
        return HAL_ERROR;
    }
    //检查mqtt是否正常工作
    if(esp8266_mqtt_checkandReCon() == HAL_ERROR)
    {
        return HAL_ERROR;
    }

    OLED_ShowString(0, 0, "ESP8266 Init OK");
    //初始化订阅主题
    esp8266_init_subTopic();


    return HAL_OK;
}

/**
 * @brief 检查WiFi连接状态并尝试重新连接
 *
 * @return HAL_StatusTypeDef
 * @author xu
 * @date 2025-08-06
 */
HAL_StatusTypeDef esp8266_wifi_checkandReCon(void)
{
    int wifiConState = 0;
    // 查询WiFi连接状态
    esp8266_at_query("AT+CWSTATE?");

    // 处于连接状态
    if (strstr(esp8266_receive_data, "OK") != NULL)
    {
        // 解析连接状态并保存
        esp8266_wifiState = esp8266_receive_data[22] - '0';
        return HAL_OK;
    }
    // 如果wifi没有连接，尝试重新连接5次
    if (strstr(esp8266_receive_data, "OK") == NULL)
    {
        wifiConState = 0;
        OLED_ShowString(1, 0, "WiFi Reconnecting");
        for (int i = 0; i < 5; i++)
        {
            if (esp8266_wifi_connect() == HAL_OK)
            {
                OLED_ShowString(1, 0, "WiFi Connected");
                wifiConState = 1;
                break;
            }
        }
    }

    // 返回重连连接状态
    if (wifiConState == 0)
    {
        OLED_ShowString(1, 0, "WiFi Failed");
        return HAL_ERROR;
    }
    // 解析连接状态并保存
    esp8266_wifi_checkandReCon();
    return HAL_OK;
}

/**
 * @brief 连接WiFi
 * 
 * @return HAL_StatusTypeDef 
 * @author xu
 * @date 2025-08-06
 */
HAL_StatusTypeDef esp8266_wifi_connect(void)
{
    //设置sta模式
    esp8266_at_query("AT+CWMODE=1");
    if (strstr(esp8266_receive_data, "OK") == NULL)
    {
        return HAL_ERROR;
    }
    //连接wifi
    WIFI_CONNCFG wifiConnCfg;
    strcpy(wifiConnCfg.ssid, "xx");
    strcpy(wifiConnCfg.password, "200600xx");

    char wifiConnCmd[100];
    sprintf(wifiConnCmd, "AT+CWJAP=\"%s\",\"%s\"", wifiConnCfg.ssid, wifiConnCfg.password);

    esp8266_at_query(wifiConnCmd);
    if (strstr(esp8266_receive_data, "OK") == NULL)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief 检查MQTT连接状态并尝试重新连接
 * 
 * @return HAL_StatusTypeDef 
 * @author xu
 * @date 2025-08-06
 */
HAL_StatusTypeDef esp8266_mqtt_checkandReCon(void)
{
    esp8266_at_query("AT+MQTTCONN?");
    // 解析MQTT连接状态
    esp8266_mqttState = esp8266_receive_data[26] - '0';
    //已连接
    if (esp8266_mqttState >=4)
    {
        esp8266_mqttState =1;
        OLED_ShowString(3, 0, "MQTT Connected");
        return HAL_OK;
    }
    //如果mqtt没有连接，尝试重新连接
   if(esp8266_mqtt_connect() == HAL_OK)
    {
        OLED_ShowString(2, 0, "MQTT reConnected");
        esp8266_mqtt_checkandReCon();
        return HAL_OK;
    }
    //如果mqtt连接失败
    OLED_ShowString(3, 0, "MQTT Failed");
    esp8266_mqttState = 0;
    return HAL_ERROR;
}

/**
 * @brief 连接MQTT服务器
 * 
 * @return HAL_StatusTypeDef 
 * @author xu
 * @date 2025-08-06
 */
HAL_StatusTypeDef esp8266_mqtt_connect(void)
{
    //设置MQTT用户参数
    MQTT_USERCFG mqttUserCfg;
    mqttUserCfg.scheme = 2; 
    strcpy(mqttUserCfg.clientId, "esp8266_xx");
    strcpy(mqttUserCfg.username, "smartHome-LowerMachine");
    strcpy(mqttUserCfg.password, "200600xx");

    char mqttUserCmd[200];
    sprintf(mqttUserCmd, "AT+MQTTUSERCFG=0,%d,\"%s\",\"%s\",\"%s\",0,0,\"\"",
            mqttUserCfg.scheme, mqttUserCfg.clientId, mqttUserCfg.username, mqttUserCfg.password);

    esp8266_at_query(mqttUserCmd);
    if (strstr(esp8266_receive_data, "OK") == NULL)
    {
        OLED_ShowString(2, 0, "MQTT User Failed");
        return HAL_ERROR;
    }

    // 连接MQTT服务器
    MQTT_CONNCFG mqttConnCfg;
    strcpy(mqttConnCfg.host, "m1b11b11.ala.cn-hangzhou.emqxsl.cn");
    mqttConnCfg.port = 8883;
    mqttConnCfg.reconnect = 0;

    char mqttConnCmd[100];
    sprintf(mqttConnCmd, "AT+MQTTCONN=0,\"%s\",%d,%d",
         mqttConnCfg.host, mqttConnCfg.port, mqttConnCfg.reconnect);

    esp8266_at_query(mqttConnCmd);
    if (strstr(esp8266_receive_data, "OK") == NULL)
    {
        OLED_ShowString(2, 0, "MQTT CON Failed");
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef esp8266_disconnect(void)
{
    return HAL_OK;
}

HAL_StatusTypeDef esp8266_at_query(char *data)
{
    memset(esp8266_receive_data, 0, ESP8266_BUFFER_SIZE);
    printf("%s\r\n", data);
    HAL_UART_Receive(&huart1, (uint8_t *)esp8266_receive_data, ESP8266_BUFFER_SIZE, ESP8266_RECEIVE_TIMEOUT);
    return HAL_OK;
}

char *esp8266_get_data()
{
    return esp8266_receive_data;
}

/**
 * @brief 订阅MQTT主题
 * 
 * @param topic 
 * @param qos 
 * @return HAL_StatusTypeDef 
 * @author xu
 * @date 2025-08-06
 */
HAL_StatusTypeDef esp8266_sub_topic(char *topic, int qos)
{
    char mqttSubTopic[200];
    sprintf(mqttSubTopic, "AT+MQTTSUB=0,\"%s\",%d",topic,qos);

    esp8266_at_query(mqttSubTopic);
    if (strstr(esp8266_receive_data, "OK") == NULL)
    {
        OLED_ShowString(2, 0, "Sub Topic Failed");
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief 初始化订阅的主题
 * 
 * @author xu
 * @date 2025-08-06
 */
void esp8266_init_subTopic(void)
{
    esp8266_sub_topic("smartHome/cmd", 0);
}

