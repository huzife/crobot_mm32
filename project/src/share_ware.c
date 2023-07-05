#include "share_ware.h"

uint8_t debug_rx_done;
uint8_t debug_tx_done;

Kinematics kinematics_inverse; //车体运动学逆解结构体
Kinematics kinematics_forward; //车体运动学正解结构体

Filter filter1;    // 创建低通滤波结构体1
Filter filter2;    // 创建低通滤波结构体2
Filter filter3;    // 创建低通滤波结构体3
Filter filter4;    // 创建低通滤波结构体4

Velocity velocity;   // 创建速度控制结构体

IncPID pid1; // 创建电机PID结构体1
IncPID pid2; // 创建电机PID结构体2
IncPID pid3; // 创建电机PID结构体3
IncPID pid4; // 创建电机PID结构体4

Encoder encoder1;   // 创建编码器结构体1
Encoder encoder2;   // 创建编码器结构体2
Encoder encoder3;   // 创建编码器结构体3
Encoder encoder4;   // 创建编码器结构体4

// 数值约束函数
float constrain(float amt, float min, float max) {
    if (amt < min) return min;
    if (amt > max) return max;
    return amt;
}