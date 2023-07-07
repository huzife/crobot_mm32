#ifndef _SHARE_WARE_H
#define _SHARE_WARE_H

// common
#include "FreeRTOS.h"
#include "queue.h"
#include "math.h"

// driver
#include "driver/uart.h"
#include "driver/dma.h"
#include "driver/tim.h"

// control
#include "control/kinematics.h"
#include "control/filter.h"
#include "control/velocity.h"
#include "control/pid.h"
#include "control/motor.h"
#include "control/data_com.h"

// sensor
#include "sensor/encoder.h"

// 浮点数-十六进制转换
typedef union {
    uint8_t hex[4];
    float float_value;
} FloatHexUnion;

extern FloatHexUnion float_hex;

// 共享变量
extern __IO uint8_t com_tx_data[COM_DATA_TX_SIZE];  // uart通信发送缓冲
extern __IO uint8_t com_rx_data[COM_DATA_RX_SIZE];  // uart通信接收缓冲
extern __IO uint8_t com_rx_len;                     // uart通信接收长度

extern __IO uint8_t com_rx_done;    // dma发送完成标志
extern __IO uint8_t com_tx_done;    // dma发送完成标志
extern __IO uint8_t com_rx_idle;    // 串口接收空闲标志

extern QueueHandle_t message_queue; // 消息队列句柄

// 共享结构体
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

extern Parser parser;   // 数据解析器结构体


float constrain(float amt, float min, float max);   // 数值约束函数
uint8_t fequal(float a, float b);   // 浮点数判断相等

#endif // _SHARE_WARE_H
