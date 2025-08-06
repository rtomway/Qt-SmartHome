/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    i2c.h
 * @brief   This file contains all the function prototypes for
 *          the i2c.c file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OLED_H__
#define __OLED_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN Includes */
#define OLED_W_SCL(x) HAL_GPIO_WritePin(I2C1_SCL_GPIO_Port, I2C1_SCL_Pin, (GPIO_PinState)x)
#define OLED_W_SDA(x) HAL_GPIO_WritePin(I2C2_SDA_GPIO_Port, I2C2_SDA_Pin, (GPIO_PinState)x)

    void OLED_Init(void);
    void OLED_Clear(void);
    void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
    void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
    void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
    void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
    void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
    void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

    void OLED_DrawPicture(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char num);
    void OLED_ShowChineseChar(uint8_t Line, uint8_t Column, unsigned char num);

    /* USER CODE END Includes */

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

    /* USER CODE BEGIN Prototypes */

    /* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */
