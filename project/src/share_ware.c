#include "share_ware.h"

FloatHexUnion float_hex;

// 共享变量
__IO uint8_t com_tx_data[COM_DATA_TX_SIZE];  // uart通信发送缓冲
__IO uint8_t com_rx_data[COM_DATA_RX_SIZE];  // uart通信接收缓冲
__IO uint8_t com_rx_len;                     // uart通信接收长度

__IO uint8_t com_rx_done;    // dma发送完成标志
__IO uint8_t com_tx_done;    // dma发送完成标志
__IO uint8_t com_rx_idle;    // 串口接收空闲标志

QueueHandle_t message_queue; // 消息队列句柄

// 共享结构体
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

Parser parser;   // 数据解析器结构体


// 数值约束函数
float constrain(float amt, float min, float max) {
    if (amt < min) return min;
    if (amt > max) return max;
    return amt;
}

// 浮点数判断相等
uint8_t fequal(float a, float b) {
    return fabs(a - b) < 0.005f;
}
