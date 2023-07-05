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

#ifndef _zf_driver_can_h_
#define _zf_driver_can_h_

#include "hal_flexcan.h"

#include "zf_common_typedef.h"

typedef enum                                                                    // 枚举串口引脚 此枚举定义不允许用户修改
{
    // CAN 1 TX 引脚
    CAN1_TX_A12        = 0x00C90,                                               // 0x 00C[A12] 9[AF9] 0[CAN1]
    CAN1_TX_B9         = 0x02990,                                               // 0x 029[B9 ] 9[AF9] 0[CAN1]
    CAN1_TX_D2         = 0x06290,                                               // 0x 062[D2 ] 9[AF9] 0[CAN1]
    CAN1_TX_I1         = 0x10190,                                               // 0x 101[I1 ] 9[AF9] 0[CAN1]

    // CAN 2 TX 引脚
    CAN2_TX_B6         = 0x02691,                                               // 0x 026[B6 ] 9[AF9] 1[CAN2]
    CAN2_TX_B13        = 0x02D91,                                               // 0x 02D[B13] 9[AF9] 1[CAN2]
}can_tx_pin_enum;

typedef enum                                                                    // 枚举串口引脚 此枚举定义不允许用户修改
{
    // CAN 1 RX 引脚
    CAN1_RX_A11        = 0x00B90,                                               // 0x 00B[A11] 9[AF9] 0[CAN1]
    CAN1_RX_B8         = 0x02890,                                               // 0x 028[B8 ] 9[AF9] 0[CAN1]
    CAN1_RX_D1         = 0x06190,                                               // 0x 061[D1 ] 9[AF9] 0[CAN1]
    CAN1_RX_I0         = 0x10090,                                               // 0x 100[I0 ] 9[AF9] 0[CAN1]

    // CAN 2 RX 引脚
    CAN2_RX_B5         = 0x02591,                                               // 0x 025[B5 ] 9[AF9] 1[CAN2]
    CAN2_RX_B12        = 0x02C91,                                               // 0x 02C[B12] 9[AF9] 1[CAN2]
}can_rx_pin_enum;

typedef enum                                                                    // 枚举串口号 此枚举定义不允许用户修改
{
    CAN_1,
    CAN_2,

    CAN_NUM,
}can_index_enum;

void    can_write_buffer    (can_index_enum can_n, uint16 target_id, const uint8 *data_buffer, uint8 data_len);
uint8   can_query_buffer    (can_index_enum can_n, uint16 *remote_id, uint8 data_buffer[8], uint8 *data_len);
void    can_read_buffer     (can_index_enum can_n, uint16 *remote_id, uint8 data_buffer[8], uint8 *data_len);
void    can_init            (can_index_enum can_n, uint32 bps, can_tx_pin_enum tx_pin, can_rx_pin_enum rx_pin);

#endif
