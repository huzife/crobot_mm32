#include "driver/uart.h"
#include "driver/dma.h"

UART_Type *uart_index[UART_NUM] = {UART1, UART2, UART3, UART4, UART5, UART6, UART7};

uint8 debug_tx_data[D_DATA_TBUF_SIZE];
uint8 debug_rx_data[D_DATA_RBUF_SIZE];

void uartDMAInit(uart_index_enum uartn, uint32 baud, uart_tx_pin_enum tx_pin, uart_rx_pin_enum rx_pin) {
    //memset(debug_data, 0, D_DATA_BUFFER_SIZE);
    debug_tx_data[D_DATA_TBUF_SIZE - 4] = 0x00;
    debug_tx_data[D_DATA_TBUF_SIZE - 3] = 0x00;
    debug_tx_data[D_DATA_TBUF_SIZE - 2] = 0x80;
    debug_tx_data[D_DATA_TBUF_SIZE - 1] = 0x7f;
    uart_init(uartn, baud, tx_pin, rx_pin);
    uartEnableDMA(uartn);
    DMAInit(D_UART_TX_CH, DMA_DIR_M2P, D_DATA_WIDTH, D_DATA_TBUF_SIZE, UART_GetTxDataRegAddr(uart_index[uartn]), (uint32)debug_tx_data);
    DMAInit(D_UART_RX_CH, DMA_DIR_P2M, D_DATA_WIDTH, D_DATA_RBUF_SIZE, UART_GetRxDataRegAddr(uart_index[uartn]), (uint32)debug_rx_data);
    EnableDMA(D_UART_TX_CH);
    EnableDMA(D_UART_RX_CH);
}

void uartEnableDMA(uart_index_enum uartn) {
    UART_EnableDMA(uart_index[uartn], true);
}