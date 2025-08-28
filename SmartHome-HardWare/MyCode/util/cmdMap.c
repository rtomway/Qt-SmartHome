#include "cmdMap.h"
#include "oled/oled.h"
#include <string.h>

//初始化命令映射表
static const CommandMap cmd_map[] = {
    {"light", update_led_state},
    {"fan", update_fan_state}
};

/***********************************************************************************************************************
 * @author xu
 *  * 函数名称：execute_command
 *  * 功能描述：执行命令 根据接收到的命令字符串查找对应的处理函数并执行
 *  * 输入参数：cmd {type}
 *  * 输入参数：device {type}
 *  * 输入参数：property {type}
 *  * 输入参数：value {type}
 ***********************************************************************************************************************/
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