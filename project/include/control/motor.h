#ifndef _MOTOR_H
#define _MOTOR_H

#include "zf_common_headfile.h"

#define MOTOR_PWM_MAX PWM_DUTY_MAX  //PWM最大值

#define M1_SIGN -1  // 电机1 方向取反
#define M2_SIGN 1   // 电机2 方向取反
#define M3_SIGN -1  // 电机3 方向取反
#define M4_SIGN 1   // 电机4 方向取反

#define M1_PWM TIM1_PWM_CH1_E9  // 控制电机1 PWM输出
#define	M2_PWM TIM1_PWM_CH2_E11 // 控制电机2 PWM输出
#define	M3_PWM TIM1_PWM_CH3_E13 // 控制电机3 PWM输出
#define	M4_PWM TIM1_PWM_CH4_E14 // 控制电机4 PWM输出

#define M1_DIR_PIN G5   // 控制电机1 方向输出
#define M2_DIR_PIN G6   // 控制电机2 方向输出
#define M3_DIR_PIN G7   // 控制电机3 方向输出
#define M4_DIR_PIN G8   // 控制电机4 方向输出

#define M1_DRIVER_SIGN -1   // 电机1 驱动板输出方向取反
#define M2_DRIVER_SIGN 1    // 电机2 驱动板输出方向取反
#define M3_DRIVER_SIGN -1   // 电机3 驱动板输出方向取反
#define M4_DRIVER_SIGN 1    // 电机4 驱动板输出方向取反

#define M1_FORWARD  (M1_SIGN * M1_DRIVER_SIGN)  // 电机1 正向
#define M1_REVERSE  (1 - M1_FORWARD)            // 电机1 反向
#define M2_FORWARD  (M2_SIGN * M2_DRIVER_SIGN)  // 电机2 正向
#define M2_REVERSE  (1 - M2_FORWARD)            // 电机2 反向
#define M3_FORWARD  (M3_SIGN * M3_DRIVER_SIGN)  // 电机3 正向
#define M3_REVERSE  (1 - M3_FORWARD)            // 电机3 反向
#define M4_FORWARD  (M4_SIGN * M4_DRIVER_SIGN)  // 电机4 正向
#define M4_REVERSE  (1 - M4_FORWARD)            // 电机4 反向

void motorDirInit(gpio_pin_enum pin);   // 控制电机方向输出初始化(gpio)
void motorSetSpeed(pwm_channel_enum pwm, const uint32 duty);    // 设置pwm输出
void motorSetDir(gpio_pin_enum pin, gpio_level_enum dir);   // 设置电机方向

#endif  // _MOTOR_H
