/*
 * Copyright 2022 MindMotion Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __HAL_CORDIC_H__
#define __HAL_CORDIC_H__

#include "hal_common.h"

/*!
 * @addtogroup CORDIC
 * @{
 */

/*!
 * @brief CORDIC driver version number.
 */
#define CORDIC_DRIVER_VERSION 0u /*!< CORDIC_0. */

/*!
 * @brief CORDIC sqrt calculate factor.
 */
#define CORDIC_SQRT_FACTOR_AN    1.646760258f /*!< Sqrt reslut equal sqrt divide sqrt factor.*/

/*!
 * @addtogroup CORDIC_STATUS
 * @{
 */
#define CORDIC_STATUS_BUSY  CORDIC_SR_BSY_MASK /*!< Status flag when CORDIC calculate is ongoing. */
/*!
 * @}
 */

/*!
 * @brief CORDIC start mode type.
 */
typedef enum
{
    CORDIC_StartMode_Direct = 0u, /*!< Direct calculate. */
    CORDIC_StartMode_WaitForStart = 1u, /*!< Wait for start command, then start calculate. */
} CORDIC_StartMode_Type;

/*!
 * @brief CORDIC calculate mode type.
 */
typedef enum
{
    CORDIC_CalcMode_Rotation = 0u, /*!< Rotation mode support sin and cos function. */
    CORDIC_CalcMode_Vectoring = 1u, /*!< Vectoring mode support arctan and sqrt function. */
} CORDIC_CalcMode_Type;

/*!
 * @brief CORDIC data format type.
 */
typedef enum
{
    CORDIC_DataFormat_None = 0u, /*!< None data format. */
    CORDIC_DataFormat_Q15 = 1u,  /*!< Q15 data format. */
    CORDIC_DataFormat_Q24 = 2u,  /*!< Q24 data format. */
} CORDIC_DataFormat_Type;

/*!
 * @brief CORDIC data type.
 */
typedef enum
{
    CORDIC_Data_AdjaSide = 0u, /*!< Adjacent side. */
    CORDIC_Data_OppoSide = 1u, /*!< Opposite side. */
    CORDIC_Data_Angle = 2u, /*!< Angle. */
} CORDIC_Data_Type;

/*!
 * @brief CORDIC result type.
 */
typedef enum
{
    CORDIC_Result_Sin = 0u, /*!< Sine result. */
    CORDIC_Result_Cos = 1u, /*!< Cosine result. */
    CORDIC_Result_Atan = 2u, /*!< Arctan result. */
    CORDIC_Result_SqrtAn = 3u, /*!< Square root multiplied by 'An' result. 'An' @ref CORDIC_SQRT_FACTOR_AN. */
} CORDIC_Result_Type;

/*!
 * @brief This type of structure instance is used to keep the settings when calling the @ref CORDIC_Init() to initialize the CORDIC module.
 */
typedef struct
{
    CORDIC_DataFormat_Type  DataFormat;    /*!< Specify the data format. */
    CORDIC_CalcMode_Type    CalcMode;      /*!< Specify the calculate mode. */
    CORDIC_StartMode_Type   StartMode;     /*!< Specify the start mode. */
} CORDIC_Init_Type;

/*!
 * @brief Initialize the CORDIC config.
 *
 * @param CORDICx CORDIC instance.
 * @param init  Pointer to the initialization structure. See to @ref CORDIC_Init_Type.
 * @return None.
 */
void CORDIC_Init(CORDIC_Type * CORDICx, CORDIC_Init_Type * init);

/*!
 * @brief Enable the CORDIC module.
 *
 * The CORDIC module should be enabled before calculate data.
 *
 * @param CORDICx CORDIC instance.
 * @param enable 'true' to enable the module, 'false' to disable the module.
 * @return None.
 */
void CORDIC_Enable(CORDIC_Type * CORDICx, bool enable);

/*!
 * @brief Get the current status flags of the CORDIC module.
 *
 * @param CORDICx CORDIC instance.
 * @return Status flags. See to @ref CORDIC_STATUS.
 */
uint32_t CORDIC_GetStatus(CORDIC_Type * CORDICx);

/*!
 * @brief Put the data into data register of the CORDIC module.
 *
 * @param CORDICx CORDIC instance.
 * @param DataType Specify set dat value type. See to @ref CORDIC_Data_Type.
 * @param value Data value to be used in the calculation.
 * @return None.
 */
void CORDIC_SetData(CORDIC_Type * CORDICx, CORDIC_Data_Type DataType, uint32_t value);

/*!
 * @brief Get the data from receiver buffer of the CORDIC module.
 *
 * @param CORDICx CORDIC instance.
 * @param ResultType Specify get calculate result type. See to @ref CORDIC_Result_Type.
 * @return The calculate result .
 */
uint32_t CORDIC_GetResult(CORDIC_Type * CORDICx, CORDIC_Result_Type ResultType);

/*!
 * @brief Start calibrate.
 *
 * The CORDIC module should be started before calculate data in WaitForStart mode.
 *
 * @param CORDICx CORDIC instance.
 * @return None.
 */
void CORDIC_Start(CORDIC_Type * CORDICx);

/*!
 *@}
 */

#endif /* __HAL_CORDIC_H__ */
