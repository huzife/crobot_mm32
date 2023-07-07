#ifndef _ENCODER_H
#define _ENCODER_H

#include "zf_common_headfile.h"

#define E1_TIM TIM3_ENCODER // 编码器1
#define E2_TIM TIM2_ENCODER // 编码器2
#define E3_TIM TIM4_ENCODER // 编码器3
#define E4_TIM TIM8_ENCODER // 编码器4

#define E1_TIM_CH1 TIM3_ENCODER_CH1_E2  // 编码器1 计数通道1
#define E1_TIM_CH2 TIM3_ENCODER_CH2_E3  // 编码器1 计数通道2
#define E2_TIM_CH1 TIM2_ENCODER_CH1_A15 // 编码器2 计数通道1
#define E2_TIM_CH2 TIM2_ENCODER_CH2_B3  // 编码器2 计数通道2
#define E3_TIM_CH1 TIM4_ENCODER_CH1_D12 // 编码器3 计数通道1
#define E3_TIM_CH2 TIM4_ENCODER_CH2_D13 // 编码器3 计数通道2
#define E4_TIM_CH1 TIM8_ENCODER_CH1_C6  // 编码器4 计数通道1
#define E4_TIM_CH2 TIM8_ENCODER_CH2_C7  // 编码器4 计数通道2

// 电机编码器结构体参数
typedef struct {
	__IO int16_t last_count;    // 缓存上一次计数器值
	__IO int16_t d_value;       // 编码器前后2次捕获计数的差值
} Encoder;

void encoderInit(Encoder *encoder); // 初始化编码器结构体参数
void encoderUpdateValue(Encoder *encoder, int16_t count, int8_t sign); // 编码器计数更新
int16_t encoderGetCount(encoder_index_enum encoder_n);  // 读取编码器计数

#endif  // _ENCODER_H
