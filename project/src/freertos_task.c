#include "zf_common_headfile.h"
#include "share_ware.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"

// 底层电机控制和速度反馈
void kinematicsTask(void *pvParameters) {
    taskENTER_CRITICAL();
    // 初始化PWM
    pwm_init(M1_PWM, 21000, PWM_DUTY_MAX);
    pwm_init(M2_PWM, 21000, PWM_DUTY_MAX);
    pwm_init(M3_PWM, 21000, PWM_DUTY_MAX);
    pwm_init(M4_PWM, 21000, PWM_DUTY_MAX);
    
    // 初始化编码器模式
    encoder_quad_init(E1_TIM, E1_TIM_CH1, E1_TIM_CH2);
    encoder_quad_init(E2_TIM, E2_TIM_CH1, E2_TIM_CH2);
    encoder_quad_init(E3_TIM, E3_TIM_CH1, E3_TIM_CH2);
    encoder_quad_init(E4_TIM, E4_TIM_CH1, E4_TIM_CH2);
    
    // 初始化电机方向控制gpio
    motorDirInit(M1_DIR_PIN);
    motorDirInit(M2_DIR_PIN);
    motorDirInit(M3_DIR_PIN);
    motorDirInit(M4_DIR_PIN);
    
    // 初始化编码器结构体
    encoderInit(&encoder1);
    encoderInit(&encoder2);
    encoderInit(&encoder3);
    encoderInit(&encoder4);
    
    // 初始化运动学结构体
    kinematicsInit(&kinematics_inverse);
    kinematicsInit(&kinematics_forward);
    
    // 初始化滤波结构体
    filterInit(&filter1, 0.03);
    filterInit(&filter2, 0.03);
    filterInit(&filter3, 0.03);
    filterInit(&filter4, 0.03);
    
    taskEXIT_CRITICAL();
    
    while (true) {
        vTaskDelay(1);
        
        // 计算转速
        Vel_To_RPM(&kinematics_inverse);
        // 限制转速
        kinematics_inverse.m1_rpm = constrain(kinematics_inverse.m1_rpm, -RPM_MAX, RPM_MAX);
        kinematics_inverse.m2_rpm = constrain(kinematics_inverse.m2_rpm, -RPM_MAX, RPM_MAX);
        kinematics_inverse.m3_rpm = constrain(kinematics_inverse.m3_rpm, -RPM_MAX, RPM_MAX);
        kinematics_inverse.m4_rpm = constrain(kinematics_inverse.m4_rpm, -RPM_MAX, RPM_MAX);
        // 采样滤波
        filter1.sample_value = kinematics_inverse.m1_rpm;
        filter2.sample_value = kinematics_inverse.m2_rpm;
        filter3.sample_value = kinematics_inverse.m3_rpm;
        filter4.sample_value = kinematics_inverse.m4_rpm;
        // 设置目标转速
        velocity.m1_set_rpm = lowPassFilter(&filter1);
        velocity.m2_set_rpm = lowPassFilter(&filter2);
        velocity.m3_set_rpm = lowPassFilter(&filter3);
        velocity.m4_set_rpm = lowPassFilter(&filter4);
        // 速度闭环控制
        velocityControl(&velocity);
        
        // 获取当前速度
        kinematics_forward.m1_rpm = velocity.m1_rpm;
        kinematics_forward.m2_rpm = velocity.m2_rpm;
        kinematics_forward.m3_rpm = velocity.m3_rpm;
        kinematics_forward.m4_rpm = velocity.m4_rpm;
        RPM_To_Vel(&kinematics_forward);
    }
}

// 与上层板通信
void communicationTask(void *pvParameters) {
    taskENTER_CRITICAL();
    comUartInit();
    heartbeatTimInit();
    message_queue = xQueueCreate(100, sizeof(uint8_t));
    parserInit(&parser, 64);
    com_tx_done = true;
    taskEXIT_CRITICAL();
    
    while (true) {
        vTaskDelay(1);
        
        // 接收并解析数据帧
        if (message_queue != NULL) {
            while (!parser.flag) {
                uint8_t data;
                if (xQueueReceive(message_queue, &data, 0) == pdTRUE) {
                    parse(&parser, data);
                }
            }
        }
        
        // 处理数据帧
        if (com_tx_done && parser.flag) {
            com_tx_done = false;
            parser.flag = false;
            processData((uint8_t *)parser.buf);
        }
    }
}

// 温湿度传感器任务
void shtc3Task(void *pvParameters) {
    taskENTER_CRITICAL();
    SHTC3_Init();
    taskEXIT_CRITICAL();
    
    while (true) {
        vTaskDelay(configTICK_RATE_HZ);
        SHTC3_MEASURE(&shtc3);
    }
}

void freertosInit() {
    xTaskCreate(kinematicsTask, "kinematics_task", 128, NULL, 3, NULL);
    xTaskCreate(communicationTask, "communication_task", 128, NULL, 3, NULL);
    xTaskCreate(shtc3Task, "shtc3_task", 128, NULL, 3, NULL);
}
