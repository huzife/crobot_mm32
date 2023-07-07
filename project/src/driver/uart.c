#include "driver/uart.h"
#include "driver/dma.h"
#include "share_ware.h"

UART_Type *uart_index[UART_NUM] = {UART1, UART2, UART3, UART4, UART5, UART6, UART7};
IRQn_Type uart_irq[UART_NUM] = {UART1_IRQn, UART2_IRQn, UART3_IRQn, UART4_IRQn, UART5_IRQn, UART6_IRQn, UART7_IRQn};

/**
 *  函数功能：通信串口初始化
 *  入口参数：None
 *  返 回 值：None
 */
void comUartInit() {
    // 初始化uart，启动串口DMA
    uart_init(COM_UART_INDEX, COM_UART_BAUDRATE, COM_UART_TX_PIN, COM_UART_RX_PIN);
    uartEnableDMA(COM_UART_INDEX);
    
    // 启用空闲中断
    uartEnableInterrupt(COM_UART_INDEX, UART_IER_RXIDLEIEN_MASK, 1);
    
    // 初始化并启用DMA通道
    DMAInit(COM_UART_TX_CH, DMA_DIR_M2P, COM_DATA_WIDTH, COM_DATA_TX_SIZE, UART_GetTxDataRegAddr(uart_index[COM_UART_INDEX]), (uint32_t)com_tx_data);
    DMAInit(COM_UART_RX_CH, DMA_DIR_P2M, COM_DATA_WIDTH, COM_DATA_RX_SIZE, UART_GetRxDataRegAddr(uart_index[COM_UART_INDEX]), (uint32_t)com_rx_data);
    enableDMA(COM_UART_RX_CH);
}

/**
 *  函数功能：使能串口DMA
 *  入口参数：uart枚举值
 *  返 回 值：None
 */
void uartEnableDMA(uart_index_enum uartn) {
    UART_EnableDMA(uart_index[uartn], true);
}

/**
 *  函数功能：使能串口中断
 *  入口参数：uart枚举值、中断使能标志、中断优先级
 *  返 回 值：None
 */
void uartEnableInterrupt(uart_index_enum uartn, uint32_t interrupts, uint8 priority) {
    UART_EnableInterrupts(uart_index[uartn], interrupts, true);
    NVIC_EnableIRQ(uart_irq[uartn]);
    NVIC_SetPriority(uart_irq[uartn], priority);
}

/**
 *  函数功能：uart1空闲中断回调函数
 *  入口参数：None
 *  返 回 值：None
 */
void uart1_irq_handler() {
    // 读取CNDTR寄存器的值，计算接收到的数据长度
    com_rx_len = COM_DATA_RX_SIZE - DMA1->CH[4].CNDTR;
    disableDMA(COM_UART_RX_CH);
    
    // 将数据发送至消息队列
    if (message_queue != NULL) {
        BaseType_t xHigherPriorityTaskWoken;
        for (int i = 0; i < com_rx_len; i++) {
            xQueueSendFromISR(message_queue, (uint8_t *)(com_rx_data + i), &xHigherPriorityTaskWoken);
        }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    
    enableDMA(COM_UART_RX_CH);
}