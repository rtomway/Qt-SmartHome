#ifndef __MYTIMER_H__
#define __MYTIMER_H__

#include <stdint.h>
#include "main.h"

void timer_start();
void timer_stop();
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* __MYTIMER_H__ */