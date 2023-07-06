#include "driver/dma.h"
#include "share_ware.h"

DMA_XferWidth_Type byteToXferWidth(uint8_t bytes) {
    if (bytes == 8u) return DMA_XferWidth_8b;
    if (bytes == 16u) return DMA_XferWidth_16b;
    return DMA_XferWidth_32b;
}

void DMAInit(dma_channel_enum ch, uint8_t dir, uint8_t bytes, uint32_t count, uint32_t p_add, uint32_t m_add) {
    switch(ch & 0xF0) {
        case 0x00:  RCC_EnableAHB1Periphs(RCC_AHB1_PERIPH_DMA1, ZF_ENABLE);    break;
        case 0x10:  RCC_EnableAHB1Periphs(RCC_AHB1_PERIPH_DMA2, ZF_ENABLE);    break;
    }

    DMA_Channel_Init_Type dma_channel_init;
    dma_channel_init.XferMode = (DMA_XferMode_Type)dir;
    dma_channel_init.ReloadMode = DMA_ReloadMode_AutoReload;
    dma_channel_init.PeriphAddrIncMode = DMA_AddrIncMode_StayAfterXfer;
    dma_channel_init.MemAddrIncMode = DMA_AddrIncMode_IncAfterXfer;
    dma_channel_init.XferWidth = byteToXferWidth(bytes);
    dma_channel_init.Priority = DMA_Priority_Highest;
    dma_channel_init.XferCount = count;
    dma_channel_init.MemAddr = (uint32_t)m_add;
    dma_channel_init.PeriphAddr = (uint32_t)p_add;

    switch(ch & 0xF0) {
        case 0x00: {
            DMA_InitChannel(DMA1, (ch & 0x0f), &dma_channel_init);
            DMA_EnableChannelInterrupts(DMA1, (ch & 0x0f), DMA_CHN_INT_XFER_DONE, true);
            break;
        }
        case 0x10: {
            DMA_InitChannel(DMA2, (ch & 0x0f), &dma_channel_init);
            DMA_EnableChannelInterrupts(DMA2, (ch & 0x0f), DMA_CHN_INT_XFER_DONE, true);
            break;
        }
    }
    switch(ch) {
        case DMA1_CHANNEL1: interrupt_enable(DMA1_CH1_IRQn);    break;
        case DMA1_CHANNEL2: interrupt_enable(DMA1_CH2_IRQn);    break;
        case DMA1_CHANNEL3: interrupt_enable(DMA1_CH3_IRQn);    break;
        case DMA1_CHANNEL4: interrupt_enable(DMA1_CH4_IRQn);    break;
        case DMA1_CHANNEL5: interrupt_enable(DMA1_CH5_IRQn);    break;
        case DMA1_CHANNEL6: interrupt_enable(DMA1_CH6_IRQn);    break;
        case DMA1_CHANNEL7: interrupt_enable(DMA1_CH7_IRQn);    break;
        case DMA1_CHANNEL8: interrupt_enable(DMA1_CH8_IRQn);    break;

        case DMA2_CHANNEL1: interrupt_enable(DMA2_CH1_IRQn);    break;
        case DMA2_CHANNEL2: interrupt_enable(DMA2_CH2_IRQn);    break;
        case DMA2_CHANNEL3: interrupt_enable(DMA2_CH3_IRQn);    break;
        case DMA2_CHANNEL4: interrupt_enable(DMA2_CH4_IRQn);    break;
        case DMA2_CHANNEL5: interrupt_enable(DMA2_CH5_IRQn);    break;
        case DMA2_CHANNEL6: interrupt_enable(DMA2_CH6_IRQn);    break;
        case DMA2_CHANNEL7: interrupt_enable(DMA2_CH7_IRQn);    break;
        case DMA2_CHANNEL8: interrupt_enable(DMA2_CH8_IRQn);    break;
    }
}

void DMA1_CH4_Handler() {
    com_tx_done = true;
}

void DMA1_CH5_Handler() {
    com_rx_done = true;
}