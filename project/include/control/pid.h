#ifndef _PID_H
#define _PID_H

#include "zf_common_headfile.h"

// 增量式PID结构体参数
typedef struct{
	__IO float P; // 比例常数 Proportional Consonal Const
	__IO float I; // 积分常数 Integral Const
	__IO float D; // 微分常数 Derivative Const
	
	__IO float error;       // e(k)	
	__IO float prev_error;  // e(k-1)
	__IO float last_error;  // e(k-2)
} IncPID;

// 位置式PID结构体参数
typedef struct{
	__IO float P; // 比例常数 Proportional Constional Const
	__IO float I; // 积分常数 Integral Const
	__IO float D; // 微分常数 Derivative Const
	
	__IO float error;       // e(k)	
	__IO float prev_error;  // e(k-1)
	
	__IO float integral_error;      // 累积误差值
	__IO float integral_error_min;  // 累积误差值限幅最小值
	__IO float integral_error_max;  // 累积误差值限幅最大值
} PosPID;

void setIncPID(IncPID *PID, float P, float I, float D); // 设置PID
void setPosPID(PosPID *PID, float P, float I, float D); // 设置PID
void IncPIDInit(IncPID *PID, float P, float I, float D);// 增量式PID参数初始化
void PosPIDInit(PosPID *PID, float P, float I, float D, float integral_error_min, float integral_error_max);// 位置式PID参数初始化
float incrementalPID(IncPID *PID, float next_point, float set_point);// 增量式PID计算函数
float positionPID(PosPID *PID, float next_point, float set_point);// 位置式PID计算函数

#endif  // _PID_H
