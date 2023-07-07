#include "control/filter.h"

/**
 *  函数功能: 低通滤波参数初始化
 *  输入参数: 结构体参数，滤波系数
 *  返 回 值: None
 */
void filterInit(Filter *filter, float a) {
    filter->a = a;              // 滤波系数 取值范围0~1
    filter->out_value = 0;      // 滤波后的输出值
	filter->sample_value = 0;   // 采样值 
}

/**
 *  函数功能: 低通滤波计算函数
 *  输入参数: 结构体参数
 *  返 回 值: None
 */
float lowPassFilter(Filter *filter) {   
	filter->out_value = filter->a * filter->sample_value +  (1 - filter->a) * filter->out_value ;   
	return filter->sample_value;
} 
