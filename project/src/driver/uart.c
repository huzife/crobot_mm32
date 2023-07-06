#include "driver/uart.h"
#include "driver/dma.h"
#include "share_ware.h"

UART_Type *uart_index[UART_NUM] = {UART1, UART2, UART3, UART4, UART5, UART6, UART7};
IRQn_Type uart_irq[UART_NUM] = {UART1_IRQn, UART2_IRQn, UART3_IRQn, UART4_IRQn, UART5_IRQn, UART6_IRQn, UART7_IRQn};

void comUartInit() {
    // 初始化uart，启动串口DMA
    uart_init(COM_UART_INDEX, COM_UART_BAUDRATE, COM_UART_TX_PIN, COM_UART_RX_PIN);
    uartEnableDMA(COM_UART_INDEX);
    
    // 初始化并启用DMA通道
    DMAInit(COM_UART_TX_CH, DMA_DIR_M2P, COM_DATA_WIDTH, COM_DATA_TX_SIZE, UART_GetTxDataRegAddr(uart_index[COM_UART_INDEX]), (uint32_t)com_tx_data);
    DMAInit(COM_UART_RX_CH, DMA_DIR_P2M, COM_DATA_WIDTH, COM_DATA_RX_SIZE, UART_GetRxDataRegAddr(uart_index[COM_UART_INDEX]), (uint32_t)com_rx_data);
    enableDMA(COM_UART_TX_CH);
    enableDMA(COM_UART_RX_CH);
}

void testUartInit() {
    // 初始化uart，启动串口DMA
    uart_init(COM_UART_INDEX, COM_UART_BAUDRATE, COM_UART_TX_PIN, COM_UART_RX_PIN);
    uartEnableDMA(COM_UART_INDEX);
    
    // 启用空闲中断
//    uartEnableInterrupt(COM_UART_INDEX, UART_ISR_RXIDLEINTF_MASK, 1);
    
    // 初始化并启用DMA通道
    DMAInit(COM_UART_TX_CH, DMA_DIR_M2P, COM_DATA_WIDTH, COM_DATA_TX_SIZE, UART_GetTxDataRegAddr(uart_index[COM_UART_INDEX]), (uint32_t)com_tx_data);
    DMAInit(COM_UART_RX_CH, DMA_DIR_P2M, COM_DATA_WIDTH, COM_DATA_RX_SIZE, UART_GetRxDataRegAddr(uart_index[COM_UART_INDEX]), (uint32_t)com_rx_data);
    enableDMA(COM_UART_TX_CH);
    enableDMA(COM_UART_RX_CH);
}

void uartEnableDMA(uart_index_enum uartn) {
    UART_EnableDMA(uart_index[uartn], true);
}

void uartEnableInterrupt(uart_index_enum uartn, uint32_t interrupts, uint8 priority) {
    UART_EnableInterrupts(uart_index[uartn], interrupts, true);
    NVIC_EnableIRQ(uart_irq[uartn]);
    NVIC_SetPriority(uart_irq[uartn], priority);
}

void uart1_irq_handler() {
    com_rx_len = COM_DATA_RX_SIZE - DMA1->CH[4].CNDTR;
    disableDMA(COM_UART_TX_CH);
    disableDMA(COM_UART_RX_CH);
    com_rx_idle = true;
}