#include "key.h"

// 按键读到低电平时，证明被按下
#define KEY_PRESSED GPIO_PIN_RESET
#define KEY_UNPRESSED GPIO_PIN_SET
// 按键的GPIO端口和引脚定义
#define KEY0_GPIO_Port GPIOA
#define KEY0_Pin GPIO_PIN_8

#define DELAY_TIME 150


Key_TypeDef keys[] = {{KEY0_GPIO_Port, KEY0_Pin}};

// 通过KEY_STATE的不同位来做判断
static int KEY_STATE = 0;
static uint32_t TICK = 0;

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：key_scan
 *  * 功能描述：按键扫描函数，用于检测按键是否被按下，并完成消抖功能
 *  * 输入参数：mode {type}
 *  * 返 回 值：int
 ***********************************************************************************************************************/
int key_scan(int mode)
{
    // 逐个遍历各个KEY
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
    {
        Key_TypeDef key = keys[i];
        // 第一次检测到按键被按
        if ((KEY_STATE >> i & 1) == 0 && HAL_GPIO_ReadPin(key.GPIO_Port, key.GPIO_Pin) == KEY_PRESSED)
        {
            // 第一次进入判(刚刚按下),记录KEY_STATE
            KEY_STATE |= (1 << i);

            // 记录当前时间，用于完成消抖工
            TICK = HAL_GetTick();
        }
        // 已经不是第一次按,判断其有没有等待足够的时间完成消
        else if (((KEY_STATE >> i & 1) == 1) && (HAL_GetTick() - TICK >= DELAY_TIME))
        {
            // 消抖完成，改变KEY_STATE状
            KEY_STATE &= ~(1 << i);
            // 非连续扫描模式，需要捕获上升沿
            if (mode == 1 && (HAL_GPIO_ReadPin(key.GPIO_Port, key.GPIO_Pin) == KEY_UNPRESSED))
                return i;

            else if (mode == 2 && (HAL_GPIO_ReadPin(key.GPIO_Port, key.GPIO_Pin) == KEY_PRESSED))
                return i;
        }
    }
    return -1;
}
