#include "zf_common_headfile.h"
#include "share_ware.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"

float speed;

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
    kinematics_inverse.angular_z = 0;
    speed = 0;
    
    while (true) {
        vTaskDelay(1);
        
        if (++i == 200) {
            i = 0;
            kinematics_inverse.angular_z = stop ? 0 : speed;
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

// 输出目标转速和实际转速的波形
void debugTxTask(void *pvParameters) {
    taskENTER_CRITICAL();
    debug_tx_done = true;
    taskEXIT_CRITICAL();
    
    while (true) {
        if (debug_tx_done) {
            float x;
            // 电机1
            x = kinematics_inverse.m1_rpm;
            memcpy(debug_tx_data, &x, 4);
            x = kinematics_forward.m1_rpm;
            memcpy(debug_tx_data + 4, &x, 4);
            // 电机2
            x = kinematics_inverse.m2_rpm;
            memcpy(debug_tx_data + 8, &x, 4);
            x = kinematics_forward.m2_rpm;
            memcpy(debug_tx_data + 12, &x, 4);
            // 电机3
            x = kinematics_inverse.m3_rpm;
            memcpy(debug_tx_data + 16, &x, 4);
            x = kinematics_forward.m3_rpm;
            memcpy(debug_tx_data + 20, &x, 4);
            // 电机4
            x = kinematics_inverse.m4_rpm;
            memcpy(debug_tx_data + 24, &x, 4);
            x = kinematics_forward.m4_rpm;
            memcpy(debug_tx_data + 28, &x, 4);
            
            debug_tx_done = false;
            vTaskDelay(1);
            EnableDMA(D_UART_TX_CH);
        }
    }
}

// 调pid参数
void debugRxTask(void *pvParameters) {
    taskENTER_CRITICAL();
    debug_rx_done = false;
    gpio_init(H2, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    taskEXIT_CRITICAL();
    
    while (true) {
        if (debug_rx_done) {
            IncPID *pid = NULL;
            uint8_t idx = *(uint8_t *)(debug_rx_data);
            uint32_t type = *(uint32_t *)(debug_rx_data + 4);
            swap(debug_rx_data + 8, debug_rx_data + 11);
            swap(debug_rx_data + 9, debug_rx_data + 10);
            float value = *(float *)(debug_rx_data + 8);
            
            switch (idx) {
                case 0u: speed = value; break;
                case 1u: pid = &pid1; break;
                case 2u: pid = &pid2; break;
                case 3u: pid = &pid3; break;
                case 4u: pid = &pid4; break;
                default: break;
            }
            
            if (pid != NULL) {
                gpio_set_level(H2, GPIO_LOW);
                vTaskDelay(20);
                gpio_set_level(H2, GPIO_HIGH);
                if (type == 0u) pid->P = value;
                else if (type == 1u) pid->I = value;
                else if (type == 2u) pid->D = value;
            }
            
            debug_rx_done = false;
            EnableDMA(D_UART_RX_CH);
        }
    }
    
}


void freertos_init() {
    xTaskCreate(kinematicsTask, "kinematics_task", 128, NULL, 3, NULL);
    xTaskCreate(debugTxTask, "debug_tx_task", 128, NULL, 3, NULL);
    xTaskCreate(debugRxTask, "debug_rx_task", 128, NULL, 3, NULL);
}