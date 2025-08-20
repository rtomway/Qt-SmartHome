#include "cmdMap.h"
#include "oled/oled.h"
#include <string.h>

//初始化命令映射表
static const CommandMap cmd_map[] = {
    {"light", update_led_state},
    {"fan", update_fan_state}
};

/**
 * @brief 查询命令并执行对应函数
 *
 * @param cmd
 * @param device
 * @param property
 * @param value
 * @author xu
 * @date 2025-08-06
 */
void execute_command(const char *cmd, const char *device, const char *property, const char *value)
{
    for (int i = 0; i < sizeof(cmd_map) / sizeof(CommandMap); i++)
    {
        if (strcmp(cmd_map[i].cmd, cmd) == 0)
        {
            cmd_map[i].func(device, property, value);
            return;
        }
    }

    OLED_ShowString(2, 0, "Command Not Found");
}