#include "control/pid.h"

/**
 *  函数功能：增量式PID设置
 *  入口参数：增量式PID参数结构体，比例参数，积分参数，微分参数
 *  返 回 值：None
 */
void setIncPID(IncPID *PID, float P, float I, float D) {
	PID->P = P; // 比例常数 Proportional Const
	PID->I = I; // 积分常数 Integral Const
	PID->D = D; // 微分常数 Derivative Const
}

/**
 *  函数功能：位置PID设置
 *  入口参数：位置式PID参数结构体，比例参数，积分参数，微分参数
 *  返 回 值：None
 */
void setPosPID(PosPID *PID, float P, float I, float D) {
	PID->P = P; // 比例常数 Proportional Const
	PID->I = I; // 积分常数 Integral Const
	PID->D = D; // 微分常数 Derivative Const
}

/**
 *  函数功能：增量式PID参数初始化
 *  入口参数：增量式PID参数结构体，比例参数，积分参数，微分参数
 *  返 回 值：None
 */
void IncPIDInit(IncPID *PID, float P, float I, float D) {
	setIncPID(PID, P, I, D);
	
	PID->error = 0;         // e(k)	
	PID->prev_error = 0;    // e(k-1)
	PID->last_error = 0;    // e(k-2)
}

/**
 *  函数功能：位置式PID参数初始化
 *  入口参数：位置式PID结构体参数，比例参数，积分参数，微分参数
 *  返 回 值：None
 */
void PosPIDInit(PosPID *PID, float P, float I, float D, float integral_error_min, float integral_error_max) {
	setPosPID(PID, P, I, D);
	
	PID->error = 0;         // e(k)	
	PID->prev_error = 0;    // e(k-1)
	
	PID->integral_error = 0;                        // 累积误差值
	PID->integral_error_min = integral_error_min;   // 累积误差值限幅最小值
	PID->integral_error_max = integral_error_max;   // 累积误差值限幅最大值
}

/**
 *  函数功能：增量式PID计算函数
 *  入口参数：PID结构体参数，测量值，目标值
 *  返 回 值：电机PWM
 *  根据增量式离散PID公式 
 *  pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
 *  e(k)代表本次偏差 
 *  e(k-1)代表上一次的偏差  以此类推 
 *  pwm代表增量输出
 */
float incrementalPID(IncPID *PID, float next_point, float set_point) {
    PID->error = set_point - next_point;
    
    // 增量计算
    float value = PID->P * (PID->error - PID->prev_error) + PID->I * PID->error + PID->D * (PID->error - 2 * PID->prev_error + PID->last_error);
    
    PID->last_error = PID->prev_error;
    PID->prev_error = PID->error;
    
    return value;
}

/**
 *  函数功能：位置式PID计算函数
 *  入口参数：PID结构体参数，测量值，目标值
 *  返 回 值：电机PWM
 *  根据位置式离散PID公式 
 *  pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
 *  e(k)代表本次偏差 
 *  e(k-1)代表上一次的偏差  
 *  ∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
 *  pwm代表输出
 */
float positionPID(PosPID *PID, float next_point, float set_point) {	
    PID->error = set_point - next_point;
	
    PID->integral_error +=  PID->error;
    
    // 积分限幅，防止累积值一直增大
    if(PID->integral_error > PID->integral_error_max) 
        PID->integral_error = PID->integral_error_max;
	else if(PID->integral_error < PID->integral_error_min) 
        PID->integral_error = PID->integral_error_min;
	
    float value = PID->P * PID->error + PID->I * PID->integral_error + PID->D * (PID->error - PID->prev_error);
  
    PID->prev_error = PID->error;
    
    return value; 
}
