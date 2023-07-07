#include "control/velocity.h"
#include "share_ware.h"
#include "math.h"

/**
 *  函数功能：4WD速度闭环控制函数
 *  入口参数：4WD电机转速结构体参数
 *  返 回 值：None
 *  注    意：该函数需要周期性执行，控制4WD电机速度闭环
 *  调用该函数后，4WD电机的当前转速值得到更新，4WD电机转速趋向目标转速
 */
void velocityControl(Velocity *vel) {
    static float pwm_m1 = 0;
    static float pwm_m2 = 0;
    static float pwm_m3 = 0;
    static float pwm_m4 = 0;
    
    // 更新编码器数值
    encoderUpdateValue(&encoder1, encoderGetCount(E1_TIM), M1_SIGN);
    encoderUpdateValue(&encoder2, encoderGetCount(E2_TIM), M2_SIGN);
    encoderUpdateValue(&encoder3, encoderGetCount(E3_TIM), M3_SIGN);
    encoderUpdateValue(&encoder4, encoderGetCount(E4_TIM), M4_SIGN);
    
    // 计算电机当前转速
    vel->m1_rpm = encoder1.d_value / PID_H;
    vel->m2_rpm = encoder2.d_value / PID_H;
    vel->m3_rpm = encoder3.d_value / PID_H;
    vel->m4_rpm = encoder4.d_value / PID_H;
    
    // 限制目标速度
    vel->m1_set_rpm = constrain(vel->m1_set_rpm, -RPM_MAX, RPM_MAX);
    vel->m2_set_rpm = constrain(vel->m2_set_rpm, -RPM_MAX, RPM_MAX);
    vel->m3_set_rpm = constrain(vel->m3_set_rpm, -RPM_MAX, RPM_MAX);
    vel->m4_set_rpm = constrain(vel->m4_set_rpm, -RPM_MAX, RPM_MAX);
    
    // 消除静止时速度误差
    if (fequal(vel->m1_set_rpm, 0) && fequal(vel->m2_set_rpm, 0) && fequal(vel->m3_set_rpm, 0) && fequal(vel->m4_set_rpm, 0)) {
        pwm_m1 = 0;
        pwm_m2 = 0;
        pwm_m3 = 0;
        pwm_m4 = 0;
        motorSetSpeed(M1_PWM, 0);
        motorSetSpeed(M2_PWM, 0);
        motorSetSpeed(M3_PWM, 0);
        motorSetSpeed(M4_PWM, 0);
        
        return;
    }
    
    // PID计算
    // 电机1
    float v1 = fabs(vel->m1_set_rpm);
    if (v1 < 28.8f) setIncPID(&pid1, 22.568, 3.674, 3.168);
    else if (v1 < 48.0f) setIncPID(&pid1, 25.825, 3.255, 3.168);
    else if (v1 < 61.5f) setIncPID(&pid1, 24.656, 3.880, 3.168);
    else if (v1 < 80.8f) setIncPID(&pid1, 24.656, 4.055, 3.168);
    else setIncPID(&pid1, 25.156, 4.891, 3.168);
    
    // 电机2
    float v2 = fabs(vel->m2_set_rpm);
    if (v2 < 28.8f) setIncPID(&pid2, 22.568, 3.674, 3.168);
    else if (v2 < 48.0f) setIncPID(&pid2, 25.825, 3.255, 3.168);
    else if (v2 < 61.5f) setIncPID(&pid2, 24.656, 3.880, 3.168);
    else if (v2 < 80.8f) setIncPID(&pid2, 24.656, 4.055, 3.168);
    else setIncPID(&pid2, 25.156, 4.891, 3.168);
    
    // 电机3
    float v3 = fabs(vel->m3_set_rpm);
    if (v3 < 28.8f) setIncPID(&pid3, 22.568, 3.674, 3.168);
    else if (v3 < 48.0f) setIncPID(&pid3, 25.825, 3.255, 3.168);
    else if (v3 < 61.5f) setIncPID(&pid3, 24.656, 3.880, 3.168);
    else if (v3 < 80.8f) setIncPID(&pid3, 24.656, 4.055, 3.168);
    else setIncPID(&pid3, 25.156, 4.891, 3.168);
    
    // 电机4
    float v4 = fabs(vel->m4_set_rpm);
    if (v4 < 28.8f) setIncPID(&pid4, 22.568, 3.674, 3.168);
    else if (v4 < 48.0f) setIncPID(&pid4, 25.825, 3.255, 3.168);
    else if (v4 < 61.5f) setIncPID(&pid4, 24.656, 3.880, 3.168);
    else if (v4 < 80.8f) setIncPID(&pid4, 24.656, 4.055, 3.168);
    else setIncPID(&pid4, 25.156, 4.891, 3.168);
    
    // 计算PWM输出
    pwm_m1 += incrementalPID(&pid1, encoder1.d_value, vel->m1_set_rpm * PID_H);
    pwm_m2 += incrementalPID(&pid2, encoder2.d_value, vel->m2_set_rpm * PID_H);
    pwm_m3 += incrementalPID(&pid3, encoder3.d_value, vel->m3_set_rpm * PID_H);
    pwm_m4 += incrementalPID(&pid4, encoder4.d_value, vel->m4_set_rpm * PID_H);
    
    // 限制PWM
    pwm_m1 = constrain(pwm_m1, -MOTOR_PWM_MAX, MOTOR_PWM_MAX);
    pwm_m2 = constrain(pwm_m2, -MOTOR_PWM_MAX, MOTOR_PWM_MAX);
    pwm_m3 = constrain(pwm_m3, -MOTOR_PWM_MAX, MOTOR_PWM_MAX);
    pwm_m4 = constrain(pwm_m4, -MOTOR_PWM_MAX, MOTOR_PWM_MAX);
    
    // 输出PWM
    if (pwm_m1 > 0.0f) {
        motorSetSpeed(M1_PWM, pwm_m1);  // 电机1 PWM输出
        motorSetDir(M1_DIR_PIN, M1_FORWARD);    // 电机1 正转
    }
    else {
        motorSetSpeed(M1_PWM, -pwm_m1); // 电机1 PWM输出
        motorSetDir(M1_DIR_PIN, M1_REVERSE);    // 电机1 反转
    }
    
    if (pwm_m2 > 0.0f) {
        motorSetSpeed(M2_PWM, pwm_m2);  // 电机2 PWM输出
        motorSetDir(M2_DIR_PIN, M2_FORWARD);    // 电机2 正转
    }
    else {
        motorSetSpeed(M2_PWM, -pwm_m2); // 电机2 PWM输出
        motorSetDir(M2_DIR_PIN, M2_REVERSE);    // 电机2 反转
    }
    
    if (pwm_m3 > 0.0f) {
        motorSetSpeed(M3_PWM, pwm_m3);  // 电机3 PWM输出
        motorSetDir(M3_DIR_PIN, M3_FORWARD);    // 电机3 正转
    }
    else {
        motorSetSpeed(M3_PWM, -pwm_m3); // 电机3 PWM输出
        motorSetDir(M3_DIR_PIN, M3_REVERSE);    // 电机3 反转
    }
    
    if (pwm_m4 > 0.0f) {
        motorSetSpeed(M4_PWM, pwm_m4);  // 电机4 PWM输出
        motorSetDir(M4_DIR_PIN, M4_FORWARD);    // 电机4 正转
    }
    else {
        motorSetSpeed(M4_PWM, -pwm_m4); // 电机4 PWM输出
        motorSetDir(M4_DIR_PIN, M4_REVERSE);    // 电机4 反转
    }
}
