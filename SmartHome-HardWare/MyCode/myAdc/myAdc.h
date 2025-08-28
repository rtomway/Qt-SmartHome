#ifndef MYADC_H_
#define MYADC_H_

#include "adc.h"

#define ADC_CHANNEL_COUNT 2

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

void myAdc_data_public();

extern uint16_t adc_buf[ADC_CHANNEL_COUNT];
extern uint8_t adc_conv_complete_flag;
extern uint8_t temp_abnormal_flag;

extern float temp_value;
extern float light_value;

#endif /* MYADC_H_ */
