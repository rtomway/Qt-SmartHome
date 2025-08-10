#ifndef MYADC_H_
#define MYADC_H_

#include "adc.h"

void myAdc_Start(void);
void myAdc_Stop(void);
float myAdc_GetValue(void);

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

extern float adc_value;
extern uint32_t adc_buf[1];

#endif /* MYADC_H_ */
