#ifndef _UART_H
#define _UART_H

#include "zf_common_headfile.h"

// debug uart接口相关定义
#define D_UART_INDEX        DEBUG_UART_INDEX        // 默认 UART_1
#define D_UART_BAUDRATE     DEBUG_UART_BAUDRATE     // 默认 115200
#define D_UART_TX_PIN       DEBUG_UART_TX_PIN       // 默认 UART1_TX_B6
#define D_UART_RX_PIN       DEBUG_UART_RX_PIN       // 默认 UART1_RX_B7

#define D_UART_TX_CH    UART1_TX_CH
#define D_UART_RX_CH    UART1_RX_CH

// debug发送数据大小
#define D_DATA_WIDTH    8u
#define D_DATA_TBUF_SIZE  36u
#define D_DATA_RBUF_SIZE  12u

extern UART_Type *uart_index[UART_NUM];

// debug数据
extern uint8 debug_tx_data[D_DATA_TBUF_SIZE];
extern uint8 debug_rx_data[D_DATA_RBUF_SIZE];

void uartDMAInit(uart_index_enum uartn, uint32 baud, uart_tx_pin_enum tx_pin, uart_rx_pin_enum rx_pin);
void uartEnableDMA(uart_index_enum uartn);



#endif  // _UART_H