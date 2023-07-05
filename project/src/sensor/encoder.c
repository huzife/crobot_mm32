#include "sensor/encoder.h"

void encoderInit(Encoder *encoder) {
    encoder->last_count = 0;
    encoder->d_value = 0;
}

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

int8_t sgn(int16_t a)//符号函数
{
	if(a>0) return 1;
	else if(a<0) return -1;
	else return 0;
}

void encoderUpdateValue2(Encoder *encoder, int16_t count, int8_t sign) {
    int16_t count_static = count * sign;
    
    if (sgn(count_static) != sgn(encoder->last_count)) {
        uint16_t i1 = abs(count_static) + abs(encoder->last_count);
        uint16_t i2 = 65536 - i1;
        if (i1 < i2) {
            if (count_static > encoder->last_count) encoder->d_value = i1;
            else encoder->d_value = -i1;
        }
        else {
            if (count_static > encoder->last_count) encoder->d_value = -i2;
            else encoder->d_value = i2;
        }
    }
    else {
        encoder->d_value = count_static - encoder->last_count;
    }
    
    encoder->last_count = count_static;
    
}

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