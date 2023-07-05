#ifndef _SHARE_WARE_H
#define _SHARE_WARE_H

// driver
#include "driver/uart.h"
#include "driver/dma.h"

// control
#include "control/kinematics.h"
#include "control/filter.h"
#include "control/velocity.h"
#include "control/pid.h"
#include "control/motor.h"

// sensor
#include "sensor/encoder.h"

extern uint8_t debug_rx_done; 
extern uint8_t debug_tx_done;

extern Kinematics kinematics_inverse; //车体运动学逆解结构体
extern Kinematics kinematics_forward; //车体运动学正解结构体

extern Filter filter1;  // 创建低通滤波结构体1
extern Filter filter2;  // 创建低通滤波结构体2
extern Filter filter3;  // 创建低通滤波结构体3
extern Filter filter4;  // 创建低通滤波结构体4

extern Velocity velocity;   // 创建速度控制结构体

extern IncPID pid1; // 创建电机PID结构体1
extern IncPID pid2; // 创建电机PID结构体2
extern IncPID pid3; // 创建电机PID结构体3
extern IncPID pid4; // 创建电机PID结构体4

extern Encoder encoder1;    // 创建编码器结构体1
extern Encoder encoder2;    // 创建编码器结构体2
extern Encoder encoder3;    // 创建编码器结构体3
extern Encoder encoder4;    // 创建编码器结构体4




float constrain(float amt, float min, float max);   // 数值约束函数

#endif // _SHARE_WARE_H