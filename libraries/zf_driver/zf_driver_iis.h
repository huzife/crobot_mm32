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
* 文件名称          zf_driver_iis
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

#ifndef _zf_driver_iis_h_
#define _zf_driver_iis_h_

#include "hal_i2s.h"

#include "zf_common_typedef.h"

typedef enum                                                                    // 枚举 IIS SCK 引脚 此枚举定义不允许用户修改
{
    // IIS1 引脚 CK
    IIS1_CK_A5          = 0x00550,                                              // 0x 005[A5]  5[AF5] 0[IIS1]
    IIS1_CK_B3          = 0x02350,                                              // 0x 023[B3]  5[AF5] 0[IIS1]
    IIS1_CK_E12         = 0x08C50,                                              // 0x 08C[E12] 5[AF5] 0[IIS1]
    IIS1_CK_F7          = 0x0A750,                                              // 0x 0A7[F7]  5[AF5] 0[IIS1]

    // IIS2 引脚 CK
    IIS2_CK_B10         = 0x02A51,                                              // 0x 02A[B10] 5[AF5] 1[IIS2]
    IIS2_CK_B13         = 0x02D51,                                              // 0x 02D[B13] 5[AF5] 1[IIS2]
    IIS2_CK_D3          = 0x06351,                                              // 0x 063[D3]  5[AF5] 1[IIS2]
    IIS2_CK_E2          = 0x08251,                                              // 0x 082[E2]  5[AF5] 1[IIS2]
    IIS2_CK_F1          = 0x0A151,                                              // 0x 0A1[F1]  5[AF5] 1[IIS2]
    IIS2_CK_G13         = 0x0CD51,                                              // 0x 0CD[G13] 5[AF5] 1[IIS2]

    // IIS3 引脚 CK
    IIS3_CK_B3          = 0x02362,                                              // 0x 023[B3]  6[AF6] 2[IIS3]
    IIS3_CK_C10         = 0x04A62,                                              // 0x 04A[C10] 6[AF6] 2[IIS3]
    IIS3_CK_D4          = 0x06452,                                              // 0x 064[D4]  5[AF5] 2[IIS3]
    IIS3_CK_D12         = 0x06C62,                                              // 0x 06C[D12] 6[AF6] 2[IIS3]
}iis_ck_pin_enum;

typedef enum                                                                    // 枚举 IIS SD 引脚 此枚举定义不允许用户修改
{
    // IIS1 引脚 SD
    IIS1_SD_A7          = 0x00750,                                              // 0x 007[A7]  5[AF5] 0[IIS1]
    IIS1_SD_B5          = 0x02550,                                              // 0x 025[B5]  5[AF5] 0[IIS1]
    IIS1_SD_E14         = 0x08E50,                                              // 0x 08E[E14] 5[AF5] 0[IIS1]
    IIS1_SD_F9          = 0x0A950,                                              // 0x 0A9[F9]  5[AF5] 0[IIS1]
    IIS1_SD_F11         = 0x0AB50,                                              // 0x 0AB[F11] 5[AF5] 0[IIS1]

    // IIS2 引脚 SD
    IIS2_SD_B15         = 0x02F51,                                              // 0x 02F[B15] 5[AF5] 1[IIS2]
    IIS2_SD_C3          = 0x04351,                                              // 0x 043[C3]  5[AF5] 1[IIS2]
    IIS2_SD_E6          = 0x08651,                                              // 0x 086[E6]  5[AF5] 1[IIS2]
    IIS2_SD_G14         = 0x0CE51,                                              // 0x 0CE[G14] 5[AF5] 1[IIS2]

    // IIS3 引脚 SD
    IIS3_SD_B5          = 0x02562,                                              // 0x 025[B5]  6[AF6] 2[IIS3]
    IIS3_SD_C12         = 0x04C62,                                              // 0x 04C[C12] 6[AF6] 2[IIS3]
    IIS3_SD_D6          = 0x06652,                                              // 0x 066[D6]  5[AF5] 2[IIS3]
    IIS3_SD_D14         = 0x06E62,                                              // 0x 06E[D14] 6[AF6] 2[IIS3]
}iis_sd_pin_enum;

typedef enum                                                                    // 枚举 IIS MCK 引脚 此枚举定义不允许用户修改
{
    // IIS1 引脚 MCK
    IIS1_MCK_A3         = 0x00353,                                              // 0x 003[A3]  5[AF5] 0[IIS1]
    IIS1_MCK_A6         = 0x00650,                                              // 0x 006[A6]  5[AF5] 0[IIS1]
    IIS1_MCK_B4         = 0x02450,                                              // 0x 024[B4]  5[AF5] 0[IIS1]
    IIS1_MCK_B7         = 0x02753,                                              // 0x 027[B7]  5[AF5] 0[IIS1]
    IIS1_MCK_E10        = 0x08A53,                                              // 0x 08A[E10] 5[AF5] 0[IIS1]
    IIS1_MCK_E13        = 0x08D50,                                              // 0x 08D[E13] 5[AF5] 0[IIS1]
    IIS1_MCK_F8         = 0x0A850,                                              // 0x 0A8[F8]  5[AF5] 0[IIS1]

    // IIS2 引脚 MCK
    IIS2_MCK_B11        = 0x02B54,                                              // 0x 02B[B11] 5[AF5] 1[IIS2]
    IIS2_MCK_B14        = 0x02E51,                                              // 0x 02E[B14] 5[AF5] 1[IIS2]
    IIS2_MCK_C2         = 0x04251,                                              // 0x 042[C2]  5[AF5] 1[IIS2]
    IIS2_MCK_C6         = 0x04651,                                              // 0x 046[C6]  5[AF5] 1[IIS2]
    IIS2_MCK_E1         = 0x08154,                                              // 0x 081[E1]  5[AF5] 1[IIS2]
    IIS2_MCK_E5         = 0x08551,                                              // 0x 085[E5]  5[AF5] 1[IIS2]
    IIS2_MCK_G11        = 0x0CB54,                                              // 0x 0CB[G11] 5[AF5] 1[IIS2]
    IIS2_MCK_G12        = 0x0CC51,                                              // 0x 0CC[G12] 5[AF5] 1[IIS2]

    // IIS3 引脚 MCK
    IIS3_MCK_A14        = 0x00E65,                                              // 0x 00E[A14] 6[AF6] 2[IIS3]
    IIS3_MCK_B4         = 0x02462,                                              // 0x 024[B4]  6[AF6] 2[IIS3]
    IIS3_MCK_B7         = 0x02765,                                              // 0x 027[B7]  6[AF6] 2[IIS3]
    IIS3_MCK_C7         = 0x04752,                                              // 0x 047[C7]  5[AF5] 2[IIS3]
    IIS3_MCK_C11        = 0x04B62,                                              // 0x 04B[C11] 6[AF6] 2[IIS3]
    IIS3_MCK_D2         = 0x06255,                                              // 0x 062[D2]  5[AF5] 2[IIS3]
    IIS3_MCK_D5         = 0x06552,                                              // 0x 065[D5]  5[AF5] 2[IIS3]
    IIS3_MCK_D11        = 0x06B65,                                              // 0x 06B[D11] 6[AF6] 2[IIS3]
    IIS3_MCK_D13        = 0x06D62,                                              // 0x 06D[D13] 6[AF6] 2[IIS3]
}iis_mck_pin_enum;

typedef enum                                                                    // 枚举 IIS WS 引脚 此枚举定义不允许用户修改
{
    // IIS1 引脚 WS
    IIS1_WS_A4          = 0x00450,                                               // 0x 004[A4]  5[AF5] 0[IIS1]
    IIS1_WS_A15         = 0x00F50,                                               // 0x 00F[A15] 5[AF5] 0[IIS1]
    IIS1_WS_B6          = 0x02650,                                               // 0x 026[B6]  5[AF5] 0[IIS1]
    IIS1_WS_E11         = 0x08B50,                                               // 0x 08B[E11] 5[AF5] 0[IIS1]
    IIS1_WS_F6          = 0x0A650,                                               // 0x 0A6[F6]  5[AF5] 0[IIS1]

    // IIS2 引脚 WS
    IIS2_WS_B9          = 0x02951,                                               // 0x 029[B9]  5[AF5] 1[IIS2]
    IIS2_WS_B12         = 0x02C51,                                               // 0x 02C[B12] 5[AF5] 1[IIS2]
    IIS2_WS_E3          = 0x08351,                                               // 0x 083[E3]  5[AF5] 1[IIS2]
    IIS2_WS_E4          = 0x08451,                                               // 0x 084[E4]  5[AF5] 1[IIS2]
    IIS2_WS_F0          = 0x0A051,                                               // 0x 0A0[F0]  5[AF5] 1[IIS2]
    IIS2_WS_G8          = 0x0C851,                                               // 0x 0C8[G8]  5[AF5] 1[IIS2]

    // IIS3 引脚 WS
    IIS3_WS_A4          = 0x00462,                                               // 0x 004[A4]  6[AF6] 2[IIS3]
    IIS3_WS_A15         = 0x00F62,                                               // 0x 00F[A15] 6[AF6] 2[IIS3]
    IIS3_WS_B6          = 0x02662,                                               // 0x 026[B6]  6[AF6] 2[IIS2]
    IIS3_WS_D7          = 0x06752,                                               // 0x 067[D7]  5[AF5] 2[IIS3]
    IIS3_WS_D15         = 0x06F62,                                               // 0x 06F[D15] 6[AF6] 2[IIS3]
}iis_ws_pin_enum;

typedef enum                                                                    // 枚举 IIS 数据位宽 此枚举定义不允许用户修改
{
    IIS_DATA_LEN_16BIT,
    IIS_DATA_LEN_24BIT,
    IIS_DATA_LEN_32BIT,
}iis_data_len_enum;

typedef enum                                                                    // 枚举 IIS 声道位宽 此枚举定义不允许用户修改
{
    IIS_CHANNEL_LEN_16BIT,
    IIS_CHANNEL_LEN_32BIT,
}iis_channel_len_enum;

typedef enum                                                                    // 枚举 IIS 采样率 此枚举定义不允许用户修改
{
    IIS_16KHZ_SAMPL     = 16000,
    IIS_32KHZ_SAMPL     = 32000,
    IIS_44KHZ1_SAMPL    = 44100,
    IIS_48KHZ_SAMPL     = 48000,
    IIS_88KHZ2_SAMPL    = 88200,
    IIS_96KHZ_SAMPL     = 96000,
    IIS_192KHZ_SAMPL    = 192000,
}iis_sampl_freq_enum;

typedef enum                                                                    // 枚举 IIS 模式 此枚举定义不允许用户修改
{
    IIS_PHILIPS_MODE,                                                           // 飞利浦标准
    IIS_LEFT_JUSTIFIED_MODE,                                                    // 左对齐标准
    IIS_RIGHT_JUSTIFIED_MODE,                                                   // 右对齐标准 也称<日本格式 EIAJ>/<SONY 格式>
    IIS_PCM_MODE,                                                               // PCM 标准
}iis_mode_enum;

typedef enum                                                                    // 枚举 IIS 编号 此枚举定义不允许用户修改
{
    IIS_1 = 0,
    IIS_2,
    IIS_3,

    IIS_NUM,
}iis_index_enum;

void    iis_dual_channel_output     (iis_index_enum iis_n, int32 left_channel, int32 right_channel);

void    iis_init                    (   iis_index_enum          iis_n,
                                        iis_mode_enum           mode,
                                        iis_sampl_freq_enum     sampl_freq,
                                        iis_channel_len_enum    channel_len,
                                        iis_data_len_enum       data_len,
                                        iis_ck_pin_enum         ck_pin,
                                        iis_sd_pin_enum         sd_pin,
                                        iis_mck_pin_enum        mck_pin,
                                        iis_ws_pin_enum         ws_pin);

#endif
