/*
 * Copyright 2021 MindMotion Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __HAL_ENET_H__
#define __HAL_ENET_H__

#include "hal_common.h"

/*!
 * @addtogroup ENET
 * @{
 */

/*!
 * @addtogroup ENET_STATUS
 * @brief ENET_STATUS_SMI_BUSY used in ENET_GetSmiStatus() function.
 * @{
 */
#define ENET_STATUS_MAC_TX_FIFO_NOT_EMPTY         ENET_MACSR_TXFNE_MASK  /*!< ENET tx fifo not empty. */
#define ENET_STATUS_MAC_TX_FIFO_WRITE_ACTIVE      ENET_MACSR_TXFWA_MASK  /*!< ENET tx fifo write active and data is transferred to tx fifo. */
#define ENET_STATUS_MAC_RX_FIFO_WRITE_ACTIVE      ENET_MACSR_RXFWA_MASK  /*!< ENET rx fifo write active and data is receive to rx fifo. */
#define ENET_STATUS_MAC_TX_MII_NOT_EMPTY          ENET_MACSR_MTE_MASK    /*!< ENET mac internal MII sending engine is actively sending data which is not idle. */
#define ENET_STATUS_MAC_RX_MII_NOT_EMPTY          ENET_MACSR_MRE_MASK    /*!< MAC internal MII receiving engine is actively receiving data, it is not idle. */
#define ENET_STATUS_SMI_BUSY                      ENET_MACSMIAR_SMIBY_MASK  /*!< ENET smi is busy. */
/*!
 * @}
 */

/*!
 * @addtogroup ENET_INT
 * @brief ENET_INT ENET dma interrupt mask codes.
 * @{
 */
#define ENET_INT_NORMAL                         ENET_DMASR_NIS_MASK  /*!< Ordinary event interrupt is valid. */
#define ENET_INT_ABNORMAL                       ENET_DMASR_AIS_MASK  /*!< Abnormal Interrupt is valid. */
#define ENET_INT_RX_DONE                        ENET_DMASR_RCS_MASK  /*!< The frame reception has been completed, the specific frame status information has been published in the descriptor, and the reception keeps running. */
#define ENET_INT_TX_DONE                        ENET_DMASR_TCS_MASK  /*!< The frame transfer has been completed. */
/*!
 * @}
 */

/*!
 * @addtogroup ENET_FILTERS
 * @brief ENET_FILTERS ENET address filter select and address mask
 * @{
 */
#define ENET_FILTERS_SOURCE_ADDR                   ENET_MACAFR_SAF_MASK   /*!< The mac receiver discards the frame that does not match the filtering value set by the source address. */
#define ENET_FILTERS_SOURCE_ADDR_INVERSE           ENET_MACAFR_SAIF_MASK  /*!< When the source address field of the frame received by the mac receiver matches the set filtering value, it is considered as filtering failure. */
#define ENET_FILTERS_DEST_ADDR                     ENET_MACAFR_MCF_MASK   /*!< Complete destination address filtering is performed on mac multicast frames, that is, DA field is compared with the set filtering value. */
#define ENET_FILTERS_DEST_ADDR_INVERSE             ENET_MACAFR_DAIF_MASK  /*!< When the destination address of a unicast frame or a multicast frame received by the MAC receiver matches the set filtering value, it is determined that the filtering fails. */
#define ENET_FILTERS_RECEIVE_ALL                   ENET_MACAFR_RALL_MASK  /*!< The mac receiver transmits all the received frames to the application, regardless of whether they have passed address filtering or not. */
#define ENET_FILTERS_BOARDCAST_FILTER              ENET_MACAFR_BCF_MASK   /*!< The address filter filters all incoming broadcast frames. */
#define ENET_FILTERS_PASSMODE                      ENET_MACAFR_PM_MASK    /*!< The address filter transmits all incoming frames regardless of the destination or source address. */
#define ENET_FILTERS_ADDRMASK_0                    (1u << 24u)  /*!< Mask MACADDL[7:0] bit during address filtering. */
#define ENET_FILTERS_ADDRMASK_1                    (1u << 25u)  /*!< Mask MACADDL[15:8] bit during address filtering. */
#define ENET_FILTERS_ADDRMASK_2                    (1u << 26u)  /*!< Mask MACADDL[23:16] bit during address filtering. */
#define ENET_FILTERS_ADDRMASK_3                    (1u << 27u)  /*!< Mask MACADDL[31:24] bit during address filtering. */
#define ENET_FILTERS_ADDRMASK_4                    (1u << 28u)  /*!< Mask MACADDH[7:0] bit during address filtering. */
#define ENET_FILTERS_ADDRMASK_5                    (1u << 29u)  /*!< Mask MACADDH[15:8] bit during address filtering. */
/*!
 * @}
 */

/*!
 * @brief ENET smi clock type, select according to the system clock.
 */
typedef enum
{
    ENET_SmiClockAlt_Alt0 = 0u,  /*!< system clock freq is 60MHz ~ 100MHz, the MDC clock is system_clockfreq/42u. */
    ENET_SmiClockAlt_Alt1 = 1u,  /*!< system clock freq is 100MHz ~ 120MHz, the MDC clock is system_clockfreq/62u. */
    ENET_SmiClockAlt_Alt2 = 2u,  /*!< system clock freq is 20MHz ~ 35MHz, the MDC clock is system_clockfreq/16u. */
    ENET_SmiClockAlt_Alt3 = 3u,  /*!< system clock freq is 35MHz ~ 60MHz, the MDC clock is system_clockfreq/26u. */
} ENET_SmiClockAlt_Type;

/*!
 * @brief ENET source address replace type.
 */
typedef enum
{
    ENET_SourceAddrReplaceAlt_Alt0 = 0u,  /*!< The processing of source address in the transmitted frame is controlled by the SAIRC bit of tx descriptor TDES1. */
    ENET_SourceAddrReplaceAlt_Alt1 = 2u,  /*!< The address value in mac filter address register 0 is inserted into the transmission frame as source address. */
    ENET_SourceAddrReplaceAlt_Alt2 = 3u,  /*!< Replace the source address field in the tx frame with the address value in mac filter address register 0 as the source address. */
    ENET_SourceAddrReplaceAlt_Alt3 = 6u,  /*!< The address value in mac filter address register 1 is inserted into the transmission frame as source address. */
    ENET_SourceAddrReplaceAlt_Alt4 = 7u,  /*!< Replace the source address field in the tx frame with the address value in mac filter address register 1 as the source address. */
} ENET_SourceAddrReplaceAlt_Type;

/*!
 * @brief ENET Interframe gap type.
 */
typedef enum
{
    ENET_InterFrameGap_96 = 0u,  /*!< Minimum gap between frames during transmission, 96 bit time. */
    ENET_InterFrameGap_88 = 1u,  /*!< Minimum gap between frames during transmission, 88 bit time. */
    ENET_InterFrameGap_80 = 2u,  /*!< Minimum gap between frames during transmission, 80 bit time. */
    ENET_InterFrameGap_72 = 3u,  /*!< Minimum gap between frames during transmission, 72 bit time. */
    ENET_InterFrameGap_64 = 4u,  /*!< Minimum gap between frames during transmission, 64 bit time. */
    ENET_InterFrameGap_56 = 5u,  /*!< Minimum gap between frames during transmission, 56 bit time. */
    ENET_InterFrameGap_48 = 6u,  /*!< Minimum gap between frames during transmission, 48 bit time. */
    ENET_InterFrameGap_40 = 7u,  /*!< Minimum gap between frames during transmission, 40 bit time. */
} ENET_InterFrameGap_Type;

/*!
 * @brief ENET Duplex mode type.
 */
typedef enum
{
    ENET_DuplexMode_HalfDuplex = 0u,  /*!< ENET duplex mode selcet: halfduplex. */
    ENET_DuplexMode_FullDuplex = 1u,  /*!< ENET duplex mode selcet: fullduplex. */
} ENET_DuplexMode_Type;

/*!
 * @brief ENET Back-off limit type, only worked in half duplex mode.
 */
typedef enum
{
    ENET_BackoffLimitAlt_Alt0  = 0u,   /* Back-off limit decise random (r) time slice delays during retry after conflict, 0<=r<2^k, k=min(n,10). */
    ENET_BackoffLimitAlt_Alt1  = 1u,   /* Back-off limit decise random (r) time slice delays during retry after conflict, 0<=r<2^k, k=min(n,8). */
    ENET_BackoffLimitAlt_Alt2  = 2u,   /* Back-off limit decise random (r) time slice delays during retry after conflict, 0<=r<2^k, k=min(n,4). */
    ENET_BackoffLimitAlt_Alt3  = 3u,   /* Back-off limit decise random (r) time slice delays during retry after conflict, 0<=r<2^k, k=min(n,1). */
} ENET_BackoffLimitAlt_Type;

/*!
 * @brief ENET tx and rx priority type.
 */
typedef enum
{
    ENET_DmaXferPrior_CyclicPriorRxTx1 = 0,  /*!< Fixed transfer priority ratio, the priority ratio of rx and tx to bus is 1:1. */
    ENET_DmaXferPrior_CyclicPriorRxTx2 = 1,  /*!< The priority ratio of rx and tx to bus is 2:1. */
    ENET_DmaXferPrior_CyclicPriorRxTx3 = 2,  /*!< The priority ratio of rx and tx to bus is 3:1. */
    ENET_DmaXferPrior_CyclicPriorRxTx4 = 3,  /*!< The priority ratio of rx and tx to bus is 4:1. */
    ENET_DmaXferPrior_FixedRxPriorTx   = 4,  /*!< RxDMA transactions have higher priority than TxDMA transactions. */
    ENET_DmaXferPrior_FixedTxPriorRx   = 5,  /*!< TxDMA transactions have higher priority than RxDMA transactions. */
} ENET_DmaXferPrior_Type;

/*!
 * @brief ENET fifo xfer mode type.
 */
typedef enum
{
    ENET_FifoXferMode_Threshold     = 0u,   /*!< When the value in fifo reaches the threshold, the transmission is started. */
    ENET_FifoXferMode_StoreForward  = 1u,   /*!< When there is a complete frame in the fifo, start the transmission. */
} ENET_FifoXferMode_Type;

/*!
 * @brief ENET tx threshold type. Start DMA transmission when the frame size in tx fifo is larger than the tx threshold.
 */
typedef enum
{
    ENET_TxThreshold_64  = 0u,    /*!< Control transfer threshold is 64. */
    ENET_TxThreshold_128 = 1u,    /*!< Control transfer threshold is 128. */
    ENET_TxThreshold_192 = 2u,    /*!< Control transfer threshold is 192. */
    ENET_TxThreshold_256 = 3u,    /*!< Control transfer threshold is 256. */
    ENET_TxThreshold_40  = 4u,    /*!< Control transfer threshold is 40. */
    ENET_TxThreshold_32  = 5u,    /*!< Control transfer threshold is 32. */
    ENET_TxThreshold_24  = 6u,    /*!< Control transfer threshold is 24. */
    ENET_TxThreshold_16  = 7u,    /*!< Control transfer threshold is 16. */
} ENET_TxThreshold_Type;

/*!
 * @brief ENET rx threshold type. Start DMA receive when the frame size in rx fifo is larger than the rx threshold.
 */
typedef enum
{
    ENET_RxThreshold_64 = 0u,   /*!< Control receive threshold is 64. */
    ENET_RxThreshold_32 = 1u,   /*!< Control receive threshold is 32. */
    ENET_RxThreshold_96 = 2u,   /*!< Control receive threshold is 96. */
    ENET_RxThreshold_128 = 3u,  /*!< Control receive threshold is 128. */
} ENET_RxThreshold_Type;

/*!
 * @brief ENET dma programmable burst length type. Indicates the maximum number of beats transmitted.
 */
typedef enum
{
    ENET_DmaBurstLen_1Beat = 0x1,    /*!< The maximum number of beats transmitted in one TxDMA or RxDMA transaction is 1 beats. */
    ENET_DmaBurstLen_2Beat = 0x2,    /*!< The maximum number of beats transmitted in one TxDMA or RxDMA transaction is 2 beats. */
    ENET_DmaBurstLen_4Beat = 0x4,    /*!< The maximum number of beats transmitted in one TxDMA or RxDMA transaction is 4 beats. */
    ENET_DmaBurstLen_8Beat = 0x8,    /*!< The maximum number of beats transmitted in one TxDMA or RxDMA transaction is 8 beats. */
    ENET_DmaBurstLen_16Beat = 0x10,  /*!< The maximum number of beats transmitted in one TxDMA or RxDMA transaction is 16 beats. */
    ENET_DmaBurstLen_32Beat = 0x20,  /*!< The maximum number of beats transmitted in one TxDMA or RxDMA transaction is 32 beats. */
} ENET_DmaBurstLen_Type;

/*!
 * @brief This type of structure instance is used to keep the settings when calling the @ref ENET_LinkRxDmaDesp() to set enet rx dma conventional descriptor.
 */
typedef struct
{
    struct
    {
        uint32_t    DAS       : 1;  /*!< Target address filter success. */
        uint32_t    CRE       : 1;  /*!< Receive frame has CRC error. */
        uint32_t    DBE       : 1;  /*!< Dribble bit error, the received frame has bytes that are not integer multiples, only valid in MII mode. */
        uint32_t    REE       : 1;  /*!< During frame reception, when the PHY sends out RX_DV signal, it sends out RX_ER signal. */
        uint32_t    WTE       : 1;  /*!< Watchdog count error, the receiving watchdog timer times out when receiving the current frame, and the current frame is truncated after the watchdog times out. */
        uint32_t    FRAT      : 1;  /*!< MAC receive frame type. */
        uint32_t    LCOE      : 1;  /*!< Late collision error. */
        uint32_t    IPCE      : 1;  /*!< IP checksum error. */
        uint32_t    RLS       : 1;  /*!< Receive last description. */
        uint32_t    RFS       : 1;  /*!< Receive first description. */
        uint32_t    VLANT     : 1;  /*!< The frame pointed by the descriptor is a VLAN frame marked by the MAC kernel. */
        uint32_t    OVE       : 1;  /*!< Specify overflow error. */
        uint32_t    LEE       : 1;  /*!< The actual length of the received frame does not match the value of the length/type field. */
        uint32_t    SAF       : 1;  /*!< 1 is source address filtering failed. */
        uint32_t    DPE       : 1;  /*!< Description error. */
        uint32_t    ERSM      : 1;  /*!< Error receive summary. */
        uint32_t    FL        : 14; /*!< The byte length of the received frame transmitted to the host memory, length including CRC. */
        uint32_t    DAF       : 1;  /*!< 1 is target address filtering failed. */
        uint32_t    OWN       : 1;  /*!< Description owner, 0 is CPU, 1 is DMA. */
    };
    struct
    {
        uint32_t    RBS1      : 13;  /*!< Receive buffer size 1. */
        uint32_t    RESEVED0  : 1;   /*!< Reseved. */
        uint32_t    RCH       : 1;   /*!< 1 is the second address in the descriptor is the next descriptor address, 0 is the second address in the descriptor is second buffer address. */
        uint32_t    RER       : 1;   /*!< Receive description end of ring, DMA returns the first address of the descriptor list. */
        uint32_t    RBS2      : 13;  /*!< Receive buffer size 2. */
        uint32_t    RESEVED1  : 2;   /*!< Reseved. */
        uint32_t    REID      : 1;   /*!< Receive end and disable interrupt. */
    };
    uint32_t        RBA1;      /*!< Receive buffer address 1, current receive fifo address. */
    uint32_t        RBA2;      /*!< Receive buffer address 2, next descriptor address. */
} ENET_RxDmaDesp_Type;

/*!
 * @brief This type of structure instance is used to keep the settings when calling the @ref ENET_LinkTxDmaDesp() to set enet tx dma conventional descriptor.
 */
typedef struct
{
    struct
    {
        uint32_t    DEE       : 1;   /*!< Delay error. */
        uint32_t    UDE       : 1;   /*!< underflow error, during DMA frame transmission, the transmission buffer is empty. */
        uint32_t    EDE       : 1;   /*!< Excessive delay error, indicates that the transmission was aborted due to an excessive delay of more than 24288 bits. */
        uint32_t    COC       : 4;   /*!< The number of collisions before sending the frame. */
        uint32_t    VLANF     : 1;   /*!< VLAN tag frame. */
        uint32_t    ECE       : 1;   /*!< Excessive collision error. When trying to send the current frame, the transmission was aborted due to 16 consecutive conflicts. */
        uint32_t    LCE       : 1;   /*!< Late collision error. */
        uint32_t    NCE       : 1;   /*!< No carrier error, carrier sense signal not triggered by PHY during transmission. */
        uint32_t    LCAE      : 1;   /*!< Lost carrier error. Lost carrier during frame transmission. */
        uint32_t    IPCE      : 1;   /*!< IP checksum error. COE engine detects an error in the payload of TCP, UDP or ICMP message, and will not update the Checksum field in the original frame. */
        uint32_t    FFF       : 1;   /*!< frame flush finish. DMA refresh the frame according to the software refresh command issued by CPU. */
        uint32_t    JTE       : 1;   /*!< Jabber timeout error. */
        uint32_t    ETSM      : 1;   /*!< Error transfer summary. */
        uint32_t    IPHE      : 1;   /*!< IP header error, MAC detect IP data header error. */
        uint32_t    TTSS      : 1;   /*!< A timestamp has been captured for the currently sent frame. */
        uint32_t    VLANC     : 2;   /*!< VLAN tag control. Indicates the tag field processing of VLAN frames. */
        uint32_t    TCH       : 1;   /*!< 1 is the second address in the descriptor is the next descriptor address, 0 is the second address in the descriptor is second buffer address. */
        uint32_t    TER       : 1;   /*!< Transfer description end of ring, DMA returns the first address of the descriptor list. */
        uint32_t    CIC       : 2;   /*!< Checksum insert control. */
        uint32_t    CRCR      : 1;   /*!< The MAC replaces  the FCS field of the transmission frame to be transmitted with the calculated CRC value. */
        uint32_t    TTSE      : 1;   /*!< Transfer timestamp enable, activate the hardware timestamp function. */
        uint32_t    DPAD      : 1;   /*!< 1 is MAC does not automatically add padding items for frames with less than 64 bytes. */
        uint32_t    DCRC      : 1;   /*!< 1 is MAC does not attach cyclic redundancy check to the end of the transmitted frame. */
        uint32_t    TFS       : 1;   /*!< Transfer first description. This bit indicates that this descriptor contains the first buffer of the frame. */
        uint32_t    TLS       : 1;   /*!< Transfer last description. This bit indicates that the buffer pointed by this descriptor is the last buffer of the frame. */
        uint32_t    TEIE      : 1;   /*!< transfer end and enable interrupt. */
        uint32_t    OWN       : 1;   /*!< Description owner, 0 is CPU, 1 is DMA. */
    };
    struct
    {
        uint32_t    TBS1      : 13;  /*!< Transfer buffer size 1. */
        uint32_t    RESEVED0  : 3;   /*!< Reseved. */
        uint32_t    TBS2      : 13;  /*!< Transfer buffer size 2. */
        uint32_t    SAIRC     : 3;   /*!< Source address insert replace control. */
    };
    uint32_t        TBA1;      /*!< Transfer buffer address 1, current tx fifo address. */
    uint32_t        TBA2;      /*!< Transfer buffer address 2, next descriptor address. */
} ENET_TxDmaDesp_Type;

/*!
 * @brief This type of structure instance is used to keep the settings when calling the @ref ENET_InitMacController() and the @ref ENET_InitDmaController() to initialize the enet mac and dma module.
 */
typedef struct
{
    /* MAC config. */
    ENET_SourceAddrReplaceAlt_Type  SourceAddrReplace;             /*!< The address value in MAC filter address register is inserted or replaced into the transmission frame as source address. */
    ENET_InterFrameGap_Type         InterframeGap;                 /*!< Minimum gap between frames during transmission */
    ENET_DuplexMode_Type            DuplexMode;                    /*!< Select enet mac duplex mode. */
    ENET_BackoffLimitAlt_Type       BackoffLimit;                  /*!< Back-off limit, only worked in halfduplex.*/
    bool                            StripCRC;                      /*!< Remove and discard the last 4 bytes of all TYPE frames before forwarding the frames to the application. */
    bool                            DisableWatchdog;               /*!< Disable watchdog, can receive up to 16384 bytes of frames.. */
    bool                            DisableJabberCount;            /*!< Disable Jabber timer on MAC sender, and can send frames of up to 16384 bytes. */
    bool                            DisableCarrierSense;           /*!< Mask carrier sense, no error will be generated due to carrier loss or no carrier during this transmission, only worked in halfduplex.*/
    bool                            DisableReceive;                /*!< MAC prohibits receiving frames, only worked in halfduplex. */
    bool                            EnableLoopback;                /*!< Enable loopback mode. */
    bool                            EnableIPChecksumOffload;       /*!< Enable ip checksum offload, identify IPv4 or IPv6 frames. */
    bool                            DisableRetryTransfer;          /*!< When a collision occurs in MII mode, the MAC will ignore the current frame transmission and stop with an excessive collision error report frame in the transmission frame state, only worked in halfduplex.*/
    bool                            EnableAutoStripCRCPAD;         /*!< The PAD/FCS field on the received frame is automatically removed when the MAC length field value is less than 1536 bytes, and the PAD/FCS field is not removed when the length field is greater than or equal to 1536 bytes. */
    bool                            EnableDelayCheck;              /*!< Enable delay check function, only worked in halfduplex.*/
    /* DMA config. */
    ENET_DmaXferPrior_Type          DmaXferPrior;                  /*!< Specify enet dma xfer priority. */
    ENET_FifoXferMode_Type          TxFifoMode;                    /*!< Select tx fifo use threshold mode or store-and-forward mode. */
    ENET_FifoXferMode_Type          RxFifoMode;                    /*!< Select rx fifo use threshold mode or store-and-forward mode. */
    ENET_DmaBurstLen_Type           TxBurstLen;                    /*!< Transfer programmable burst length. */
    ENET_DmaBurstLen_Type           RxBurstLen;                    /*!< Receive programmable burst length. */
    ENET_TxThreshold_Type           TxThreshold;                   /*!< specify tx threshold. */
    ENET_RxThreshold_Type           RxThreshold;                   /*!< specify rx threshold. */
    bool                            EnableAdvancedDesp;            /*!< Specify use advanced descriptor. */
    bool                            AdressAlign;                   /*!< Align with burst address. */
    bool                            EnableOperateOnSecondFrame;    /*!< DMA processes the second transmitted data frame, even if the status of the first frame has not been obtained. */
    bool                            MultipleProgrammableBurstLen;  /*!< Tx and Rx programmable burst length * 8. */
} ENET_Init_Type;

/*!
 * @brief Initialize the enet module with smi clock mode.
 *
 * @param ENETx ENET instance.
 * @param SmiClockAlt smi clock select, related to the system frequency.
 * @return None.
 */
void ENET_InitSmi(ENET_Type * ENETx, ENET_SmiClockAlt_Type SmiClockAlt);

/*!
 * @brief Write data to set PHY register.
 *
 * @param ENETx ENET instance.
 * @param phy_addr Select PHY address.
 * @param phy_reg select which phy register will be operated.
 * @param data configuration value of register.
 * @return true.
 */
bool ENET_WriteMacPhyReg(ENET_Type * ENETx, uint32_t phy_addr, uint32_t phy_reg, uint16_t data);

/*!
 * @brief Read data from PHY register.
 *
 * @param ENETx ENET instance.
 * @param phy_addr Select PHY address.
 * @param phy_reg select which phy register will be operated.
 * @param dat Pointer to get PHY register value.
 * @return true.
 */
bool ENET_ReadMacPhyReg(ENET_Type * ENETx, uint32_t phy_addr, uint32_t phy_reg, uint32_t * dat);

/*!
 * @brief Initialize the enet module with mac mode.
 *
 * @param ENETx ENET instance.
 * @param init Pointer to the initialization structure. See to @ref ENET_Init_Type.
 * @return None.
 */
void ENET_InitMacController(ENET_Type * ENETx, ENET_Init_Type * init);

/*!
 * @brief Initialize the enet module with dma mode.
 *
 * @param ENETx ENET instance.
 * @param init Pointer to the initialization structure. See to @ref ENET_Init_Type.
 * @return None.
 */
void ENET_InitDmaController(ENET_Type * ENETx, ENET_Init_Type * init);

/*!
 * @brief Enable enet tx dma mode.
 *
 * @param ENETx ENET instance.
 * @param enable true to start enet tx dma.
 * @return None.
 */
void ENET_EnableTxDma(ENET_Type * ENETx, bool enable);

/*!
 * @brief Enable enet rx dma mode.
 *
 * @param ENETx ENET instance.
 * @param enable 'true' to start enet rx dma.
 * @return None.
 */
void ENET_EnableRxDma(ENET_Type * ENETx, bool enable);

/*!
 * @brief Get the status flags of the enet mac module.
 *
 * @param ENETx ENET instance.
 * @return MAC status flags. See to @ref ENET_STATUS.
 */
uint32_t ENET_GetStatus(ENET_Type * ENETx);

/*!
 * @brief Get the status flags of the enet dma module.
 *
 * @param ENETx ENET instance.
 * @return ENET dma status flags. See to @ref ENET_INT.
 */
uint32_t ENET_GetDmaStatus(ENET_Type * ENETx);

/*!
 * @brief Get the status flags of the enet smi module.
 *
 * @param ENETx ENET instance.
 * @return MAC status flags. See to @ref ENET_STATUS.
 */
uint32_t ENET_GetSmiStatus(ENET_Type * ENETx);

/*!
 * @brief Clear the status flags of the enet dma module.
 *
 * @param ENETx ENET instance.
 * @param status ENET dma status flags. See to @ref ENET_INT.
 * @return None.
 */
void ENET_ClearDmaStatus(ENET_Type * ENETx, uint32_t status);

/*!
 * @brief Enable interrupts of the ENET module.
 *
 * @param ENETx ENET instance.
 * @param interrupts Interrupt code masks. See to @ref ENET_INT.
 * @param enable 'true' to enable the indicated interrupts, 'false' to disable the indicated interrupts.
 * @return None.
 */
void ENET_EnableDmaInterrupts(ENET_Type * ENETx, uint32_t interrupts, bool enable);

/*!
 * @brief Set enet tx dma descriptor first address.
 *
 * @param ENETx ENET instance.
 * @param addr ENET tx dma descriptor first address.
 * @return None.
 */
void ENET_SetTxDmaDesp(ENET_Type * ENETx, uint32_t addr);

/*!
 * @brief Set enet rx dma descriptor first address.
 *
 * @param ENETx ENET instance.
 * @param addr ENET rx dma descriptor first address.
 * @return None.
 */
void ENET_SetRxDmaDesp(ENET_Type * ENETx, uint32_t addr);

/*!
 * @brief Set enet tx dma descriptor.
 *
 * @param ENETx ENET instance.
 * @param new_desp Pointer to the current descriptor.
 * @param buffer Pointer to the tx buffer.
 * @param len The length of tx frame.
 * @param pre_desp Pointer to the previous descriptor.
 * @return None.
 */
void ENET_LinkTxDmaDesp(ENET_Type * ENETx, ENET_TxDmaDesp_Type * new_desp, uint8_t * buffer, uint32_t len, ENET_TxDmaDesp_Type * pre_desp);

/*!
 * @brief Set enet rx dma descriptor.
 *
 * @param ENETx ENET instance.
 * @param rx_desp Pointer to the receive descriptor.
 * @param buffer Pointer to the tx buffer.
 * @param len The length of rx frame.
 * @param rx_cnt Number of received descriptors.
 * @return None.
 */
void ENET_LinkRxDmaDesp(ENET_Type * ENETx, ENET_RxDmaDesp_Type * rx_desp, uint8_t * buffer, uint32_t len, uint32_t rx_cnt);

/*!
 * @brief Setup mac address filter.
 *
 * @param ENETx ENET instance.
 * @param filter The filter will be used for mac address filter. See to @ref ENET_FILTER.
 * @param addr_id Select from 0~5.
 * @param addr_mask Mask the part of mac address when filter.
 * @param addr Pointer to filter address, could be destination address or source address.
 * @return None.
 */
void ENET_SetupMacAddrFilter(ENET_Type * ENETx, uint32_t filter, uint32_t addr_id, uint32_t addr_mask, uint8_t * addr);

/*!
 *@}
 */

#endif /* __HAL_I2C_H__ */
