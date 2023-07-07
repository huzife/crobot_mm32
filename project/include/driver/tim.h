#ifndef _TIM_H
#define _TIM_H

#include "zf_common_headfile.h"

#define HEARTBEAT_TIME 1000
#define HEARTBEAT_TIM TIM_6
#define HEARTBEAT_TIM_FREQ 1000
#define HEARTBEAT_TIM_PERIOD  (HEARTBEAT_TIME * HEARTBEAT_TIM_FREQ / 1000 * 2 - 1)

void heartbeatTimInit();
void timInit(timer_index_enum index, uint32_t step_freq, uint32_t period);
void timEnableInterrupt(timer_index_enum tim_n, uint32_t interrupts, IRQn_Type irqn, uint8 priority);
void timClear(timer_index_enum tim_n);

#endif  // _TIM_H