/*
 * Copyright 2022 MindMotion Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hal_cordic.h"

#define DATA_FORMAT_CMD_Q15  0x4DBA
#define DATA_FORMAT_CMD_Q24  0x9B74EE

void CORDIC_Init(CORDIC_Type * CORDICx, CORDIC_Init_Type * init)
{
    uint32_t cr = CORDICx->CR & ~(    CORDIC_CR_STARTMODE_MASK
                                     | CORDIC_CR_WORKMODE_MASK
                                     | CORDIC_CR_WORKMODE_MASK
                                 );

    /* Initialize the result register. */
    CORDICx->RXR &=  ~CORDIC_RXR_RX_MASK;
    CORDICx->RYR &=  ~CORDIC_RYR_RY_MASK;
    CORDICx->RZR &=  ~CORDIC_RZR_RZ_MASK;

    /* Start mode. */
    if (CORDIC_StartMode_WaitForStart == init->StartMode)
    {
        cr |= CORDIC_CR_STARTMODE_MASK;
    }
    else
    {
        cr &= ~CORDIC_CR_STARTMODE_MASK;
    }

    /* Calculate mode. */
    if (CORDIC_CalcMode_Rotation == init->CalcMode)
    {
        cr |= CORDIC_CR_WORKMODE_MASK;
    }
    else
    {
        cr &= ~CORDIC_CR_WORKMODE_MASK;
    }

    CORDIC->CR = cr;

    /* Data Format. */
    switch (init->DataFormat)
    {
        case CORDIC_DataFormat_None:
            CORDICx->DXR &= ~CORDIC_DXR_DX_MASK;
            break;
        case CORDIC_DataFormat_Q15:
            CORDICx->DXR = DATA_FORMAT_CMD_Q15;
            break;
        case CORDIC_DataFormat_Q24:
            CORDICx->DXR = DATA_FORMAT_CMD_Q24;
        default:
            break;
    }
}

void CORDIC_Enable(CORDIC_Type * CORDICx, bool enable)
{
    if (enable)
    {
        CORDICx->CR |= CORDIC_CR_ENABLE_MASK;
    }
    else
    {
        CORDICx->CR &= ~CORDIC_CR_ENABLE_MASK;
    }
}

void CORDIC_Start(CORDIC_Type * CORDICx)
{
    CORDICx->CR |= CORDIC_CR_START_MASK;
}

void CORDIC_SetData(CORDIC_Type * CORDICx, CORDIC_Data_Type DataType, uint32_t value)
{
    switch (DataType)
    {
        case CORDIC_Data_AdjaSide:
            CORDIC->DXR = value;
            break;
        case CORDIC_Data_OppoSide:
            CORDIC->DYR = value;
            break;
        case CORDIC_Data_Angle:
            CORDIC->DZR = value;
            break;
        default:
            break;
    }
}

/* Return calculate result. */
uint32_t CORDIC_GetResult(CORDIC_Type * CORDICx, CORDIC_Result_Type ResultType)
{
    switch (ResultType)
    {
        case CORDIC_Result_Cos:
            return (CORDICx->RXR );
            break;
        case CORDIC_Result_Sin:
            return (CORDICx->RYR );
            break;
        case CORDIC_Result_Atan:
            return (CORDICx->RZR );
            break;
        case CORDIC_Result_SqrtAn:
            return (CORDICx->RXR );
            break;
        default:
            break;
    }

    return 1u;
}

/* Return flags */
uint32_t CORDIC_GetStatus(CORDIC_Type * CORDICx)
{
    return CORDICx->SR;
}

/* EOF. */
