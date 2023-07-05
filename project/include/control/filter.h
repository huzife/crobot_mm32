#ifndef _FILTER_H
#define _FILTER_H
	
#include "zf_common_headfile.h"

typedef struct
{
	__IO float a;// 滤波系数 取值范围0~1
	__IO float out_value;//滤波后的输出值
	__IO float sample_value;//采样值 
}Filter;

void filterInit(Filter *filter, float a);//低通滤波参数初始化
float lowPassFilter(Filter *filter);//低通滤波计算函数

#endif  // _FILTER_H
