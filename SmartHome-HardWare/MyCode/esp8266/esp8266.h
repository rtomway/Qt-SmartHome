#ifndef ESP8266_H
#define ESP8266_H

#include "main.h"

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


HAL_StatusTypeDef
esp8266_init(void);
HAL_StatusTypeDef esp8266_wifi_connect(void);
HAL_StatusTypeDef esp8266_wifi_checkandReCon(void);
HAL_StatusTypeDef esp8266_mqtt_connect(void);
HAL_StatusTypeDef esp8266_mqtt_checkandReCon(void);
HAL_StatusTypeDef esp8266_disconnect(void);
HAL_StatusTypeDef esp8266_at_query(char *data);
char* esp8266_get_data();


void esp8266_init_subTopic(void);
HAL_StatusTypeDef esp8266_sub_topic(char *topic, int qos);



#endif /* ESP8266_H */





