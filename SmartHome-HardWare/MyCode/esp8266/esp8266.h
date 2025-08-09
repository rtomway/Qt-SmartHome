#ifndef ESP8266_H
#define ESP8266_H

#include "main.h"
#include "util/mqttPacket.h"

#define ESP8266_BUFFER_SIZE 256

typedef struct WIFI_CONNCFG
{
    char ssid[32]; 
    char password[64]; 
} WIFI_CONNCFG;

typedef struct MQTT_USERCFG
{
    int scheme;
    char clientId[32];  
    char username[32];  
    char password[64];  
} MQTT_USERCFG;

typedef struct MQTT_CONNCFG
{
    char host[64];  
    int port;  
    int reconnect;  
} MQTT_CONNCFG;

extern char esp8266_receive_data[ESP8266_BUFFER_SIZE];


HAL_StatusTypeDef
esp8266_init(void);
HAL_StatusTypeDef esp8266_init_atCmd(char *data);

HAL_StatusTypeDef esp8266_wifi_connect(void);
HAL_StatusTypeDef esp8266_wifi_checkandReCon(void);
HAL_StatusTypeDef esp8266_mqtt_connect(void);
HAL_StatusTypeDef esp8266_mqtt_checkandReCon(void);
HAL_StatusTypeDef esp8266_disconnect(void);


void esp8266_init_subTopic(void);
HAL_StatusTypeDef esp8266_sub_topic(char *topic, int qos);

HAL_StatusTypeDef esp8266_public_data(char *topic,  char*data, int qos, int retain);

#endif /* ESP8266_H */





