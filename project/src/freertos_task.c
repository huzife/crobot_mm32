#include "zf_common_headfile.h"
#include "share_ware.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"

// 交换字节
void swap(uint8_t *a, uint8_t *b) {
    uint8_t temp = *a;
    *a = *b;
    *b = temp;
}

void kinematicsTask(void *pvParameters) {
    taskENTER_CRITICAL();
    // 初始化PWM
    pwm_init(M1_PWM, 21000, 0);
    pwm_init(M2_PWM, 21000, 0);
    pwm_init(M3_PWM, 21000, 0);
    pwm_init(M4_PWM, 21000, 0);
    
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
    
    // 初始化滤波结构体
    filterInit(&filter1, 0.03);
    filterInit(&filter2, 0.03);
    filterInit(&filter3, 0.03);
    filterInit(&filter4, 0.03);
    
    taskEXIT_CRITICAL();
    
    uint32 i = 0;
    uint8 stop = false;
    kinematics_inverse.linear_x = 0;
    
    while (true) {
        vTaskDelay(1);
        
        if (++i == 200) {
            i = 0;
            kinematics_inverse.linear_x = stop ? 0 : 0.1f;
            stop = !stop;
        }
        
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

void testTask(void *pvParameters) {
    taskENTER_CRITICAL();
    testUartInit();
    disableDMA(COM_UART_TX_CH);
    com_tx_done = true;
    com_rx_idle = false;
    taskEXIT_CRITICAL();
    
    while (true) {
        if (com_tx_done) {
            memcpy((uint8_t *)com_tx_data, (uint8_t *)&kinematics_inverse.linear_x, 4);
            memcpy((uint8_t *)(com_tx_data + 4), (uint8_t *)&kinematics_forward.linear_x, 4);
            memcpy((uint8_t *)(com_tx_data + 8), (uint8_t *)&kinematics_inverse.angular_z, 4);
            memcpy((uint8_t *)(com_tx_data + 12), (uint8_t *)&kinematics_forward.angular_z, 4);
            com_tx_data[16] = 0x00;
            com_tx_data[17] = 0x00;
            com_tx_data[18] = 0x80;
            com_tx_data[19] = 0x7f;
            com_tx_done = false;
            enableDMA(COM_UART_TX_CH);
            vTaskDelay(1);
        }
//        if (com_rx_idle) {
//            memcpy((uint8_t *)com_tx_data, (uint8_t *)com_rx_data, count);
//            DMA1->CH[3].CNDTR = count;
//            enableDMA(COM_UART_TX_CH);
//            enableDMA(COM_UART_RX_CH);
//            com_rx_idle = false;
//        }
    }
}

void freertos_init() {
    xTaskCreate(kinematicsTask, "kinematics_task", 128, NULL, 3, NULL);
    xTaskCreate(testTask, "test_task", 128, NULL, 3, NULL);
}