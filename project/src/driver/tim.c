#include "driver/tim.h"
#include "share_ware.h"

TIM_Type *tim_index[TIM_NUM] = {
    (TIM_Type *)TIM1, 
    (TIM_Type *)TIM2, 
    (TIM_Type *)TIM3, 
    (TIM_Type *)TIM4, 
    (TIM_Type *)TIM5, 
    (TIM_Type *)TIM6, 
    (TIM_Type *)TIM7, 
    (TIM_Type *)TIM8
};

void heartbeatTimInit() {
    timInit(HEARTBEAT_TIM, HEARTBEAT_TIM_FREQ, HEARTBEAT_TIM_PERIOD);
    timEnableInterrupt(HEARTBEAT_TIM, TIM6_DIER_UIE_MASK, TIM6_IRQn, 1);
    timer_start(HEARTBEAT_TIM);
}

void timInit(timer_index_enum index, uint32_t step_freq, uint32_t period) {
    timer_clock_enable(index);
    TIM_Init_Type tim_init;
    tim_init.ClockFreqHz = bus_clock;
    tim_init.StepFreqHz = step_freq;
    tim_init.Period = period;
    tim_init.EnablePreloadPeriod = false;
    tim_init.PeriodMode = TIM_PeriodMode_Continuous;
    tim_init.CountMode = TIM_CountMode_Increasing;
    TIM_Init(tim_index[index], &tim_init);
    timer_stop(index);
    timer_clear(index);
}

/**
 *  函数功能：使能定时器中断
 *  入口参数：timer枚举值、中断使能标志、中断优先级
 *  返 回 值：None
 */
void timEnableInterrupt(timer_index_enum tim_n, uint32_t interrupts, IRQn_Type irqn, uint8 priority) {
    TIM_EnableInterrupts(tim_index[tim_n], interrupts, true);
    NVIC_EnableIRQ(irqn);
    NVIC_SetPriority(irqn, priority);
}

/**
 *  函数功能：定时器计数清零
 *  入口参数：timer枚举值
 *  返 回 值：None
 */
void timClear(timer_index_enum tim_n) {
    tim_index[tim_n]->CNT = 0;
}

void tim6_irq_handler() {
//    gpio_toggle_level(H2);
//    system_delay_ms(50);
//    gpio_toggle_level(H2);
    kinematics_inverse.linear_x = 0;
    kinematics_inverse.linear_y = 0;
    kinematics_inverse.angular_z = 0;
}