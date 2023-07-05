#include "control/motor.h"

void motorDirInit(gpio_pin_enum pin) {
    gpio_init(pin, GPO, GPIO_HIGH, GPO_PUSH_PULL);
}

inline void motorSetSpeed(pwm_channel_enum pwm, const uint32 duty) {
    pwm_set_duty(pwm, MOTOR_PWM_MAX - duty);
}

// 设置电机方向
inline void motorSetDir(gpio_pin_enum pin, gpio_level_enum dir) {
    gpio_set_level(pin, dir);
}

// 电机正转
inline void motorSetDirForward(gpio_pin_enum DIR) {
    gpio_high(DIR);
}

// 电机反转
inline void motorSetDirReverse(gpio_pin_enum DIR) {
    gpio_low(DIR);
}