#include "control/motor.h"

/**
 *  函数功能：电机方向控制引脚初始化
 *  入口参数：gpio引脚枚举值
 *  返 回 值：None
 */
void motorDirInit(gpio_pin_enum pin) {
    gpio_init(pin, GPO, GPIO_HIGH, GPO_PUSH_PULL);
}

/**
 *  函数功能：设置速度输出
 *  入口参数：PWM通道枚举值、占空比
 *  返 回 值：None
 *  说    明：该小车所使用的电机驱动板输入pwm占空比 = 0时为全速，所以速度和占空比成反比
 */
inline void motorSetSpeed(pwm_channel_enum pwm, const uint32 duty) {
    pwm_set_duty(pwm, MOTOR_PWM_MAX - duty);
}

/**
 *  函数功能：设置电机方向
 *  入口参数：gpio引脚枚举值、方向枚举值
 *  返 回 值：None
 */
inline void motorSetDir(gpio_pin_enum pin, gpio_level_enum dir) {
    gpio_set_level(pin, dir);
}
