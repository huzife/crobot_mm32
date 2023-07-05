/*
 * Copyright 2021 MindMotion Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hal_enet.h"

/* Initialization enet smi clock. */
void ENET_InitSmi(ENET_Type * ENETx, ENET_SmiClockAlt_Type SmiClockAlt)
{
    ENETx->MACSMIAR |= ENET_MACSMIAR_SMICLK(SmiClockAlt);
}

/* Write data to set PHY register. */
bool ENET_WriteMacPhyReg(ENET_Type * ENETx, uint32_t phy_addr, uint32_t phy_reg, uint16_t data)
{
    ENETx->MACSMIAR &= ~(ENET_MACSMIAR_SMIADD_MASK | ENET_MACSMIAR_SMIREG_MASK | ENET_MACSMIAR_SMIWR_MASK);
    ENETx->MACSMIDR = data;
    ENETx->MACSMIAR |= ( ENET_MACSMIAR_SMIADD(phy_addr) | ENET_MACSMIAR_SMIREG(phy_reg) | ENET_MACSMIAR_SMIWR_MASK);
    ENETx->MACSMIAR |= ENET_MACSMIAR_SMIBY_MASK;
    
    while ( 0u != (ENET_GetSmiStatus(ENETx) & ENET_STATUS_SMI_BUSY) )  /* When read or write smi busy bit, need wait this bit to 0u. */
    {
    }
    return true;
}

/* Read data from PHY register. */
bool ENET_ReadMacPhyReg(ENET_Type * ENETx, uint32_t phy_addr, uint32_t phy_reg, uint32_t * dat)
{
    ENETx->MACSMIAR &= ~(ENET_MACSMIAR_SMIADD_MASK | ENET_MACSMIAR_SMIREG_MASK | ENET_MACSMIAR_SMIWR_MASK);
    ENETx->MACSMIAR |= ( ENET_MACSMIAR_SMIADD(phy_addr) | ENET_MACSMIAR_SMIREG(phy_reg) | ENET_MACSMIAR_SMIBY_MASK);
    while ( 0u != (ENET_GetSmiStatus(ENETx) & ENET_STATUS_SMI_BUSY) )  /* When read or write smi busy bit, need wait this bit to 0u. */
    {
    }
    *dat = ENETx->MACSMIDR;

    return true;
}

/* Enable MAC Tx (Not open to users). */
void ENET_EnableTx(ENET_Type * ENETx, bool enable)
{
    if (enable)
    {
        ENETx->MACCR |= ENET_MACCR_TE_MASK;
    }
    else
    {
        ENETx->MACCR &= ~ENET_MACCR_TE_MASK;
    }
}

/* Enable MAC Rx (Not open to users). */
void ENET_EnableRx(ENET_Type * ENETx, bool enable)
{
    if (enable)
    {
        ENETx->MACCR |= ENET_MACCR_RE_MASK;
    }
    else
    {
        ENETx->MACCR &= ~ENET_MACCR_RE_MASK;
    }
}

/* Initialize the enet module with mac mode. */
void ENET_InitMacController(ENET_Type * ENETx, ENET_Init_Type * init)
{
    uint32_t temp = ( ENET_MACCR_SAIRC(init->SourceAddrReplace) | ENET_MACCR_CST(init->StripCRC) | ENET_MACCR_WTD(init->DisableWatchdog) 
                    | ENET_MACCR_JAB(init->DisableJabberCount) | ENET_MACCR_IFG(init->InterframeGap) | ENET_MACCR_LM(init->EnableLoopback)
                    | ENET_MACCR_DM(init->DuplexMode) | ENET_MACCR_IPCO(init->EnableIPChecksumOffload) | ENET_MACCR_APCS(init->EnableAutoStripCRCPAD) );
    if (init->DuplexMode == ENET_DuplexMode_HalfDuplex)
    {
        temp |= ( ENET_MACCR_MCRS(init->DisableCarrierSense) | ENET_MACCR_RDIS(init->DisableReceive) | ENET_MACCR_RETY(init->DisableRetryTransfer) 
                | ENET_MACCR_BL(init->BackoffLimit) | ENET_MACCR_DLYC(init->EnableDelayCheck) );
    }
    ENETx->MACCR = temp;

    ENET_EnableTx(ENETx, true);
    ENET_EnableRx(ENETx, true);
}

/* Initialize the enet module with dma mode. */
void ENET_InitDmaController(ENET_Type * ENETx, ENET_Init_Type * init)
{
    ENETx->DMABSR &= ~(ENET_DMABSR_DMAA_MASK | ENET_DMABSR_DSEN_MASK | ENET_DMABSR_FTPR_MASK | ENET_DMABSR_TXPR_MASK | ENET_DMABSR_RPBL_MASK | ENET_DMABSR_TPBL_MASK | ENET_DMABSR_AAL_MASK);
    /* DMA bus controll. */
    uint32_t temp = (ENET_DMABSR_DSEN(init->EnableAdvancedDesp)  | ENET_DMABSR_SPBL_MASK | ENET_DMABSR_TPBL(init->TxBurstLen) 
                   | ENET_DMABSR_RPBL(init->RxBurstLen) |ENET_DMABSR_MPBL(init->MultipleProgrammableBurstLen) | ENET_DMABSR_AAL(init->AdressAlign) );
    if (init->DmaXferPrior == ENET_DmaXferPrior_FixedRxPriorTx)    /* DMA tx and rx priority. */
    {
        temp &= ~ENET_DMABSR_TXPR_MASK;
        temp |= ENET_DMABSR_DMAA_MASK;
    }
    else if (init->DmaXferPrior == ENET_DmaXferPrior_FixedTxPriorRx)
    {
        temp |= (ENET_DMABSR_DMAA_MASK | ENET_DMABSR_TXPR_MASK);
    }
    else
    {
        temp &= ~ENET_DMABSR_DMAA_MASK;
        temp |= ENET_DMABSR_FTPR(init->DmaXferPrior);
    }

    ENETx->DMABSR |= temp;

    temp = ( ENET_DMAMDR_OSF(init->EnableOperateOnSecondFrame) | ENET_DMAMDR_RSF(init->RxFifoMode) | ENET_DMAMDR_TSF(init->TxFifoMode) );
    if (init->TxFifoMode == ENET_FifoXferMode_Threshold)
    {
        temp |= ENET_DMAMDR_TTC(init->TxThreshold);
    }
    if (init->RxFifoMode == ENET_FifoXferMode_Threshold)
    {
        temp |= ENET_DMAMDR_RTC(init->RxThreshold);
    }
    ENETx->DMAMDR = temp;
}

/* Start enet tx dma. */
void ENET_EnableTxDma(ENET_Type * ENETx, bool enable)
{
    if (enable)
    {
        ENETx->DMAMDR |= ENET_DMAMDR_STT_MASK;
    }
    else
    {
        ENETx->DMAMDR &= ~ENET_DMAMDR_STT_MASK;
    }
}

/* Start enet rx dma. */
void ENET_EnableRxDma(ENET_Type * ENETx, bool enable)
{
    if (enable)
    {
        ENETx->DMAMDR |= ENET_DMAMDR_STR_MASK;
    }
    else
    {
        ENETx->DMAMDR &= ~ENET_DMAMDR_STR_MASK;
    }
}

/* Get enet mac status. */
uint32_t ENET_GetStatus(ENET_Type * ENETx)
{
    return ENETx->MACSR;
}

/* Get the status flags of the enet dma module. */
uint32_t ENET_GetDmaStatus(ENET_Type * ENETx)
{
    return ENETx->DMASR;
}

/* Get the status flags of the enet smi module. */
uint32_t ENET_GetSmiStatus(ENET_Type * ENETx)
{
    return ENETx->MACSMIAR;
}

/* Clear the status flags of the enet dma module. */
void ENET_ClearDmaStatus(ENET_Type * ENETx, uint32_t status)
{
    ENETx->DMASR |= status;
}

/* Enable interrupts of the ENET module. */
void ENET_EnableDmaInterrupts(ENET_Type * ENETx, uint32_t interrupts, bool enable)
{
    if (enable)
    {
        ENETx->DMAIR |= interrupts;
    }
    else
    {
        ENETx->DMAIR &= ~interrupts;
    }
}

/* Set enet tx dma descriptor first address. */
void ENET_SetTxDmaDesp(ENET_Type * ENETx, uint32_t addr)
{
    ENETx->DMATXDSAR = addr;
}

/* Set enet rx dma descriptor first address. */
void ENET_SetRxDmaDesp(ENET_Type * ENETx, uint32_t addr)
{
    ENETx->DMARXDSAR = addr;
}

/* Set enet tx dma descriptor. */
void ENET_LinkTxDmaDesp(ENET_Type * ENETx, ENET_TxDmaDesp_Type * new_desp, uint8_t * buffer, uint32_t len, ENET_TxDmaDesp_Type * pre_desp)
{
    if (new_desp == pre_desp)
    {
        ENET_SetTxDmaDesp(ENETx, (uint32_t)new_desp);
        new_desp->TFS = 1u;  /* the current desp is first desp. */
        new_desp->TLS = 1u;  /* the current desp is last desp. */
        new_desp->TCH = 1u;
        new_desp->TBS1 = len;
        new_desp->TBA1 = (uint32_t)(buffer);
        new_desp->TBA2 = (uint32_t)new_desp;
        new_desp->TEIE = 1u;
        new_desp->OWN = 1u;
    }
     else
    {
        pre_desp->TBA2 = (uint32_t)new_desp;  /* link current-desp and pre-desp. */
        pre_desp->TLS = 0u;
        pre_desp->TEIE = 0u;
        new_desp->TCH = 1u;
        new_desp->TLS = 1u;
        new_desp->TFS = 0u;
        new_desp->TEIE = 1u;
        new_desp->TBS1 = len;
        new_desp->TBA1 = (uint32_t)(buffer);
        new_desp->TBA2 = (uint32_t)new_desp;
        new_desp->OWN = 1u;
    }
}

/* Set enet rx dma descriptor. */
void ENET_LinkRxDmaDesp(ENET_Type * ENETx, ENET_RxDmaDesp_Type * rx_desp, uint8_t * buffer, uint32_t len, uint32_t rx_cnt)
{
    uint32_t i = 0u;
    ENET_RxDmaDesp_Type * temp_desp;
        for (i = 0u; i < rx_cnt; i++)
    {
        temp_desp = rx_desp + i;
        temp_desp->RCH = 1u;
        temp_desp->RBS1 = len;
        temp_desp->RBA1 = (uint32_t)(&buffer[i * len]);
        if (i < rx_cnt - 1u)
        {
            temp_desp->RBA2 = (uint32_t)(rx_desp + i + 1u);
        }
        else
        {
            temp_desp->RBA2 = (uint32_t)(rx_desp);
        }
        temp_desp->OWN = 1u;   /* DMA start send data from fifo to memory. */
    }
}

void ENET_SetupMacAddrFilter(ENET_Type * ENETx, uint32_t filter, uint32_t addr_id, uint32_t addr_mask, uint8_t * addr)
{
    ENETx->MACAFR |= filter;
    if ( (0u != (filter & ENET_FILTERS_SOURCE_ADDR) ) || (0u != (filter & ENET_FILTERS_SOURCE_ADDR_INVERSE) ) )  /* Set source address filter. */
    {
        ENETx->MACAH_L[addr_id].MACAHR = ( ENET_MACAHR_ADDE_MASK | ENET_MACAHR_SELE_MASK | (uint32_t)addr[4u] | ((uint32_t)addr[5u]<<8u) );
        ENETx->MACAH_L[addr_id].MACALR = ( (uint32_t)addr[0u] | ((uint32_t)addr[1u] << 8u) | ((uint32_t)addr[2u] << 16u) | ((uint32_t)addr[3u] << 24u) );
    }
    else if ( (0u != (filter & ENET_FILTERS_DEST_ADDR) ) || (0u != (filter & ENET_FILTERS_DEST_ADDR_INVERSE) ) ) /* Set destination address filter. */
    {
        ENETx->MACAFR &= ~(ENET_MACAFR_MCF_MASK | ENET_MACAFR_HU_MASK);
        ENETx->MACAH_L[addr_id].MACAHR = ( ENET_MACAHR_ADDE_MASK | (uint32_t)addr[4u] | ((uint32_t)addr[5u]<<8u) );
        ENETx->MACAH_L[addr_id].MACALR = ( (uint32_t)addr[0u] | ((uint32_t)addr[1u] << 8u) | ((uint32_t)addr[2u] << 16u) | ((uint32_t)addr[3u] << 24u) );
    }
    if (0u != addr_mask)
    {
        ENETx->MACAH_L[addr_id].MACAHR |= addr_mask;
    }
}

/* DMA Des. */
