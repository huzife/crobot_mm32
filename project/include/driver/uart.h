#ifndef _UART_H
#define _UART_H

#include "zf_common_headfile.h"

// 上层板通信uart接口相关定义
#define COM_UART_INDEX      UART_1
#define COM_UART_BAUDRATE   115200
#define COM_UART_TX_PIN     UART1_TX_B6
#define COM_UART_RX_PIN     UART1_RX_B7
#define COM_UART_TX_CH      UART1_TX_CH
#define COM_UART_RX_CH      UART1_RX_CH

#define COM_DATA_WIDTH    8u
#define COM_DATA_TX_SIZE  20u
#define COM_DATA_RX_SIZE  12u

//extern UART_Type *uart_index[UART_NUM];
//extern IRQn_Type uart_irq[UART_NUM];

void comUartInit();
void testUartInit();
void uartEnableDMA(uart_index_enum uartn);
void uartEnableInterrupt(uart_index_enum uartn, uint32_t interrupts, uint8 priority);


#endif  // _UART_H