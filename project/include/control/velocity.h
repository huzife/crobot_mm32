#ifndef _VELOCITY_H
#define _VELOCITY_H

#include "zf_common_headfile.h"

#define VELOCITY_CONTROL_T  5       //速度闭环控制周期 ms
#define RPM_MAX             300.0f  //电机最大转速 rpm
#define COUNTS_PER_REV      89600   //电机输出轴转一圈编码器计数值89600  144000

// 参数转化: PID_H = 编码器计数(每个周期) / 转速(每分钟)
#define PID_H (float)(COUNTS_PER_REV * VELOCITY_CONTROL_T / 60000.0f)

//电机转速控制结构体参数
typedef struct{
	__IO float m1_rpm;	    //M1电机当前转速 rpm
	__IO float m2_rpm;	    //M2电机当前转速 rpm
	__IO float m3_rpm;	    //M3电机当前转速 rpm
	__IO float m4_rpm;	    //M4电机当前转速 rpm
	__IO float m1_set_rpm;  //M1电机目标转速 rpm
	__IO float m2_set_rpm;  //M2电机目标转速 rpm
	__IO float m3_set_rpm;  //M3电机目标转速 rpm
	__IO float m4_set_rpm;  //M4电机目标转速 rpm
} Velocity;

void velocityControl(Velocity *vel);    // 4WD速度闭环控制函数
bool isZero(float rpm); // 判断转速是否为零

#endif  // _VELOCITY_H
