#include "sensor/encoder.h"

/**
 *  函数功能：编码器结构体初始化
 *  入口参数：编码器结构体指针
 *  返 回 值：None
 */
void encoderInit(Encoder *encoder) {
    encoder->last_count = 0;
    encoder->d_value = 0;
}

/**
 *  函数功能：更新编码器数值
 *  入口参数：编码器结构体指针、编码器计数值、方向符号
 *  返 回 值：None
 *  说    明：左右两边轮子计数方向不同，所以需要sign处理
 */
void encoderUpdateValue(Encoder *encoder, int16_t count, int8_t sign) {
    // 处理编码器计数方向
    count *= sign;
    
    // 计算两次计数差值
    int32_t ret = (int32_t)count - (int32_t)encoder->last_count;
    
    if (ret > 32767) ret -= 65536;
    else if (ret < -32768) ret += 65536;
    
    encoder->d_value = (int16_t)ret;
    encoder->last_count = count;
}

/**
 *  函数功能：读取编码器计数值
 *  入口参数：编码器枚举值
 *  返 回 值：编码器计数值
 */
int16_t encoderGetCount(encoder_index_enum encoder_n) {
    uint32_t result = 0;
    switch(encoder_n) {
        case TIM1_ENCODER:  result = TIM_GetCounterValue((TIM_Type *) TIM1);   break;
        case TIM2_ENCODER:  result = TIM_GetCounterValue((TIM_Type *) TIM2);   break;
        case TIM3_ENCODER:  result = TIM_GetCounterValue((TIM_Type *) TIM3);   break;
        case TIM4_ENCODER:  result = TIM_GetCounterValue((TIM_Type *) TIM4);   break;
        case TIM5_ENCODER:  result = TIM_GetCounterValue((TIM_Type *) TIM5);   break;
        case TIM8_ENCODER:  result = TIM_GetCounterValue((TIM_Type *) TIM8);   break;
        default: break;
    }
    
    return (int16_t)result;
}
