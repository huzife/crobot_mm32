/*********************************************************************************************************************
* MM32F527X-E9P Opensourec Library 即（MM32F527X-E9P 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MM32F527X-E9P 开源库的一部分
* 
* MM32F527X-E9P 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          zf_driver_can
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK 5.37
* 适用平台          MM32F527X_E9P
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-08-10        Teternal            first version
********************************************************************************************************************/

#include "hal_rcc.h"
#include "zf_common_clock.h"
#include "zf_common_debug.h"
#include "zf_driver_gpio.h"

#include "zf_driver_can.h"

#define CAN_WRITE_CHANNLE_INDEX     ( FLEXCAN_CHN_1 )
#define CAN_RECIVE_CHANNLE_INDEX    ( FLEXCAN_CHN_0 )

static  FLEXCAN_Type    *can_index[CAN_NUM]           = {FLEXCAN1, FLEXCAN2};
static  IRQn_Type       can_irq[CAN_NUM]              = {FlexCAN1_IRQn, FlexCAN2_IRQn};
static  uint8           can_interrupt_state[CAN_NUM]  = {0, 0};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 发送数据
// 参数说明     can_n           CAN 模块号 参照 zf_driver_can.h 内 can_index_enum 枚举体定义
// 参数说明     target_id       目标 ID
// 参数说明     *data_buffer    发送数据缓冲区
// 参数说明     data_len        数据长度 不超过 8
// 返回参数     void
// 使用示例     can_write_buffer(CAN_1, 0x0172, &data_buffer[0], 8);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void can_write_buffer (can_index_enum can_n, uint16 target_id, const uint8 *data_buffer, uint8 data_len)
{
    zf_assert(NULL != data_buffer);
    zf_assert(8 >= data_len);

    FLEXCAN_Mb_Type can_data_buffer;
    uint8 temp_buffer[8];

    memset(temp_buffer, 0, 8);
    memcpy(temp_buffer, data_buffer, data_len);

    can_data_buffer.ID = target_id & 0x07FF;
    can_data_buffer.LENGTH = data_len;
    can_data_buffer.BYTE0 = temp_buffer[0];
    can_data_buffer.BYTE1 = temp_buffer[1];
    can_data_buffer.BYTE2 = temp_buffer[2];
    can_data_buffer.BYTE3 = temp_buffer[3];
    can_data_buffer.BYTE4 = temp_buffer[4];
    can_data_buffer.BYTE5 = temp_buffer[5];
    can_data_buffer.BYTE6 = temp_buffer[6];
    can_data_buffer.BYTE7 = temp_buffer[7];

    FLEXCAN_WriteTxMb(can_index[can_n], CAN_WRITE_CHANNLE_INDEX, &can_data_buffer);
    FLEXCAN_SetMbCode(can_index[can_n], CAN_WRITE_CHANNLE_INDEX, FLEXCAN_MbCode_TxDataOrRemote);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 查询接收
// 参数说明     can_n           CAN 模块号 参照 zf_driver_can.h 内 can_index_enum 枚举体定义
// 参数说明     *remote_id      数据来源的远端 ID
// 参数说明     *data_buffer    接收数据缓冲区
// 参数说明     *data_len       数据长度
// 返回参数     void
// 使用示例     if(can_query_buffer(CAN_1, &remote_id, &data_buffer[0], 8))
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
uint8 can_query_buffer (can_index_enum can_n, uint16 *remote_id, uint8 data_buffer[8], uint8 *data_len)
{
    zf_assert(NULL != remote_id);
    zf_assert(NULL != data_buffer);
    zf_assert(NULL != data_len);

    FLEXCAN_Mb_Type can_data_buffer;
    uint8 temp_buffer[8];
    uint8 return_data = 0;

    memset(temp_buffer, 0, 8);

    if((FLEXCAN_GetMbStatus(can_index[can_n]) & (FLEXCAN_STATUS_MB_0 << CAN_RECIVE_CHANNLE_INDEX)))
    {
        FLEXCAN_ClearMbStatus(can_index[can_n], (FLEXCAN_STATUS_MB_0 << CAN_RECIVE_CHANNLE_INDEX));
        FLEXCAN_ReadRxMb(can_index[can_n], CAN_RECIVE_CHANNLE_INDEX, &can_data_buffer);

        *remote_id          = can_data_buffer.ID    ;
        *data_len           = can_data_buffer.LENGTH;
        temp_buffer[0]      = can_data_buffer.BYTE0 ;
        temp_buffer[1]      = can_data_buffer.BYTE1 ;
        temp_buffer[2]      = can_data_buffer.BYTE2 ;
        temp_buffer[3]      = can_data_buffer.BYTE3 ;
        temp_buffer[4]      = can_data_buffer.BYTE4 ;
        temp_buffer[5]      = can_data_buffer.BYTE5 ;
        temp_buffer[6]      = can_data_buffer.BYTE6 ;
        temp_buffer[7]      = can_data_buffer.BYTE7 ;

        memcpy(data_buffer, temp_buffer, *data_len);
        return_data = 1;
    }
    return return_data;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 阻塞接收
// 参数说明     can_n           CAN 模块号 参照 zf_driver_can.h 内 can_index_enum 枚举体定义
// 参数说明     *remote_id      数据来源的远端 ID
// 参数说明     *data_buffer    接收数据缓冲区
// 参数说明     *data_len       数据长度
// 返回参数     void
// 使用示例     can_read_buffer(CAN_1, &remote_id, &data_buffer[0], 8);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void can_read_buffer (can_index_enum can_n, uint16 *remote_id, uint8 data_buffer[8], uint8 *data_len)
{
    zf_assert(NULL != remote_id);
    zf_assert(NULL != data_buffer);
    zf_assert(NULL != data_len);

    FLEXCAN_Mb_Type can_data_buffer;
    uint8 temp_buffer[8];

    memset(temp_buffer, 0, 8);

    while(!(FLEXCAN_GetMbStatus(can_index[can_n]) & (FLEXCAN_STATUS_MB_0 << CAN_RECIVE_CHANNLE_INDEX)));
    FLEXCAN_ClearMbStatus(can_index[can_n], (FLEXCAN_STATUS_MB_0 << CAN_RECIVE_CHANNLE_INDEX));
    FLEXCAN_ReadRxMb(can_index[can_n], CAN_RECIVE_CHANNLE_INDEX, &can_data_buffer);

    *remote_id          = can_data_buffer.ID    ;
    *data_len           = can_data_buffer.LENGTH;
    temp_buffer[0]      = can_data_buffer.BYTE0 ;
    temp_buffer[1]      = can_data_buffer.BYTE1 ;
    temp_buffer[2]      = can_data_buffer.BYTE2 ;
    temp_buffer[3]      = can_data_buffer.BYTE3 ;
    temp_buffer[4]      = can_data_buffer.BYTE4 ;
    temp_buffer[5]      = can_data_buffer.BYTE5 ;
    temp_buffer[6]      = can_data_buffer.BYTE6 ;
    temp_buffer[7]      = can_data_buffer.BYTE7 ;
    memcpy(data_buffer, temp_buffer, *data_len);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CAN 初始化
// 参数说明     can_n           CAN 模块号 参照 zf_driver_can.h 内 can_index_enum 枚举体定义
// 参数说明     bps             CAN 比特率
// 参数说明     tx_pin          CAN 发送引脚 参照 zf_driver_can.h 内 can_tx_pin_enum 枚举体定义
// 参数说明     rx_pin          CAN 接收引脚 参照 zf_driver_can.h 内 can_rx_pin_enum 枚举体定义
// 返回参数     void
// 使用示例     can_init(CAN_1, 250 * 1000, CAN1_TX_D2, CAN1_RX_D1);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void can_init (can_index_enum can_n, uint32 bps, can_tx_pin_enum tx_pin, can_rx_pin_enum rx_pin)
{
    zf_assert((tx_pin & can_n) == can_n);                                       // tx_pin 与 can_n 匹配
    zf_assert((rx_pin & can_n) == can_n);                                       // rx_pin 与 can_n 匹配
    zf_assert(1000000 >= bps);                                                 // 手册上最高支持 1Mbps

    switch(can_n)
    {
        case CAN_1:
        {
            RCC_EnableAPB1Periphs(RCC_APB1_PERIPH_FLEXCAN1, true);
            RCC_ResetAPB1Periphs(RCC_APB1_PERIPH_FLEXCAN1);
        }break;
        case CAN_2:
        {
            RCC_EnableAPB2Periphs(RCC_APB2_PERIPH_FLEXCAN2, true);
            RCC_ResetAPB2Periphs(RCC_APB2_PERIPH_FLEXCAN2);
        }break;
        default:
        {
        }break;
    }

    afio_init((gpio_pin_enum)((tx_pin & 0xFFF00) >> 8), GPO, (gpio_af_enum)((tx_pin & 0xF0) >> 4), GPO_AF_PUSH_PULL);   // 提取对应IO索引 AF功能编码
    afio_init((gpio_pin_enum)((rx_pin & 0xFFF00) >> 8), GPI, (gpio_af_enum)((rx_pin & 0xF0) >> 4), GPI_FLOATING_IN);    // 提取对应IO索引 AF功能编码

    FLEXCAN_TimConf_Type        flexcan_tim_config      = {0};
    FLEXCAN_Init_Type           flexcan_init_config     = {0};
    FLEXCAN_RxMbMaskConf_Type   flexcan_mbmask_config   = {0};
    FLEXCAN_RxMbConf_Type       flexcan_mb_config       = {0};

    flexcan_tim_config.EnableExtendedTime = true;
    flexcan_tim_config.PhaSegLen1 = 2u;
    flexcan_tim_config.PhaSegLen2 = 1u;
    flexcan_tim_config.PropSegLen = 1u;
    flexcan_tim_config.JumpWidth = 1u;

    /* Setup FlexCAN. */
    flexcan_init_config.MaxXferNum = 15u;
    flexcan_init_config.ClockSource = FLEXCAN_ClockSource_Periph;
    flexcan_init_config.BitRate = bps;
    flexcan_init_config.ClockFreqHz = system_clock;
    flexcan_init_config.SelfWakeUp = FLEXCAN_SelfWakeUp_BypassFilter;
    flexcan_init_config.WorkMode = FLEXCAN_WorkMode_Normal;
    flexcan_init_config.Mask = FLEXCAN_Mask_Individual;
    flexcan_init_config.EnableSelfReception = false;
    flexcan_init_config.EnableTimerSync = true;
    flexcan_init_config.TimConf = &flexcan_tim_config;
    FLEXCAN_Init(can_index[can_n], &flexcan_init_config);

    /* Set tx mb. */
    FLEXCAN_ResetMb(can_index[can_n], CAN_WRITE_CHANNLE_INDEX);
    FLEXCAN_SetMbCode(can_index[can_n], CAN_WRITE_CHANNLE_INDEX, FLEXCAN_MbCode_TxInactive);
    
    // 开启冻结模式 才能修改全局的过滤mask
    FLEXCAN_EnableFreezeMode(can_index[can_n], true);
    flexcan_mbmask_config.MbType = FLEXCAN_MbType_Data;
    flexcan_mbmask_config.MbFormat = FLEXCAN_MbFormat_Standard;
    flexcan_mbmask_config.IdMask = 0;// 接收所有ID数据
    FLEXCAN_SetRxMbIndividualMaskConf(can_index[can_n], CAN_RECIVE_CHANNLE_INDEX, &flexcan_mbmask_config);
    FLEXCAN_EnableFreezeMode(can_index[can_n], false);

    flexcan_mb_config.Id = 0; /* Id for filtering with mask and receiving. */
    flexcan_mb_config.MbType = FLEXCAN_MbType_Data; /* Only receive standard data frame. */
    flexcan_mb_config.MbFormat = FLEXCAN_MbFormat_Standard;
    FLEXCAN_SetRxMb(can_index[can_n], CAN_RECIVE_CHANNLE_INDEX, &flexcan_mb_config);
    FLEXCAN_SetMbCode(can_index[can_n], CAN_RECIVE_CHANNLE_INDEX, FLEXCAN_MbCode_RxEmpty);/* Set for receiving. */
}
