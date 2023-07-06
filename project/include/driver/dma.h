#ifndef _DMA_H
#define _DMA_H

#include "zf_common_headfile.h"

// 相关接口对应的dma通道
#define UART1_TX_CH DMA1_CHANNEL4
#define UART1_RX_CH DMA1_CHANNEL5

// DMA传输方向
#define DMA_DIR_P2M 0u
#define DMA_DIR_M2P 1u

#define enableDMA(ch) dma_enable(ch)
#define disableDMA(ch) dma_disable(ch)

void DMAInit(dma_channel_enum ch, uint8 dir, uint8 bytes, uint32 count, uint32 p_add, uint32 m_add);



#endif  // _DMA_H