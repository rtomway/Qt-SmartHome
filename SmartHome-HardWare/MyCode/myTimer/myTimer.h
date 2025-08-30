#ifndef __MYTIMER_H__
#define __MYTIMER_H__

#include <stdint.h>
#include "main.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

extern uint8_t timing_flag;

// 风扇状态
typedef enum FAN_STATE
{
    FAN_OFF,
    FAN_ON_LOW,
    FAN_ON_MEDIUM,
    FAN_ON_HIGH
} FAN_STATE;

typedef struct {
    char* speed_value;
    FAN_STATE state;
    uint16_t speed_pwm;
} FAN_SPEED;

static FAN_SPEED fan_speed_table[] = 
{
    {"off", FAN_OFF, 0},
    {"low", FAN_ON_LOW, 333},
    {"medium", FAN_ON_MEDIUM, 444},
    {"high", FAN_ON_HIGH,555}
};
// 风扇控制
void fan_control(uint16_t speed);
// 风扇状态更新
void update_fan_state(const char *device, const char *property, const char *value);


//窗帘舵机
void curtain_servo_init();
static uint32_t angle_to_ccr(uint16_t angle);
void curtain_servo_control(uint16_t position);
void update_curtain_position(const char *device, const char *property, const char *value);

#endif /* __MYTIMER_H__ */