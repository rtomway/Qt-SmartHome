#ifndef __MYTIMER_H__
#define __MYTIMER_H__

#include <stdint.h>
#include "main.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

extern uint8_t timing_flag;

#endif /* __MYTIMER_H__ */