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

#include "hal_rcc.h"
#include "zf_common_clock.h"
#include "zf_common_debug.h"

#include "zf_driver_gpio.h"
#include "zf_driver_iis.h"

static SPI_Type *spi_index[IIS_NUM] = {SPI1, SPI2, SPI3};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IIS 双声道数据输出
// 参数说明     iis_n           IIS 的模块号    参照 zf_driver_iis.h 内 iis_index_enum 枚举体定义
// 参数说明     left_channel    左声道数据
// 参数说明     right_channel   右声道数据
// 返回参数     void
// 返回参数     void
// 使用示例     iis_dual_channel_output(IIS_2, 4096, 4096);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void iis_dual_channel_output (iis_index_enum iis_n, int32 left_channel, int32 right_channel)
{
    while((spi_index[iis_n]->CSTAT & (0x00000004)));
    spi_index[iis_n]->TXREG = left_channel;
    while((spi_index[iis_n]->CSTAT & (0x00000004)));
    spi_index[iis_n]->TXREG = right_channel;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IIS 初始化 半双工模式工作
// 参数说明     iis_n           IIS 的模块号    参照 zf_driver_iis.h 内 iis_index_enum 枚举体定义
// 参数说明     mode            IIS 工作模式    参照 zf_driver_iis.h 内 iis_mode_enum 枚举体定义
// 参数说明     sampl_freq      IIS 采样频率    参照 zf_driver_iis.h 内 iis_sampl_freq_enum 枚举体定义
// 参数说明     channel_len     IIS 通道位宽    参照 zf_driver_iis.h 内 iis_channel_len_enum 枚举体定义
// 参数说明     data_len        IIS 数据位宽    参照 zf_driver_iis.h 内 iis_data_len_enum 枚举体定义
// 参数说明     ck_pin          IIS CK  引脚    参照 zf_driver_iis.h 内 iis_ck_pin_enum 枚举体定义
// 参数说明     sd_pin          IIS SD  引脚    参照 zf_driver_iis.h 内 iis_sd_pin_enum 枚举体定义
// 参数说明     mck_pin         IIS MCK 引脚    参照 zf_driver_iis.h 内 iis_mck_pin_enum 枚举体定义
// 参数说明     ws_pin          IIS WS  引脚    参照 zf_driver_iis.h 内 iis_ws_pin_enum 枚举体定义
// 返回参数     void
// 使用示例     iis_init(IIS_2, IIS_PHILIPS_MODE, IIS_48KHZ_SAMPL, IIS_CHANNEL_LEN_16BIT, IIS_DATA_LEN_16BIT, IIS2_CK_D3, IIS2_SD_E6, IIS2_MCK_E5, IIS2_WS_E4);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void iis_init (iis_index_enum iis_n, iis_mode_enum mode, iis_sampl_freq_enum sampl_freq, iis_channel_len_enum channel_len, iis_data_len_enum data_len, iis_ck_pin_enum ck_pin, iis_sd_pin_enum sd_pin, iis_mck_pin_enum mck_pin, iis_ws_pin_enum ws_pin)
{
    zf_assert((ck_pin & iis_n) == iis_n);                                       // ck_pin   与 iis_n 匹配
    zf_assert((sd_pin & iis_n) == iis_n);                                       // sd_pin   与 iis_n 匹配
    zf_assert((mck_pin & iis_n) % 3 == iis_n);                                  // mck_pin  与 iis_n 匹配
    zf_assert((ws_pin & iis_n) == iis_n);                                       // ws_pin   与 iis_n 匹配

    switch(iis_n)
    {
        case IIS_1: RCC_EnableAPB2Periphs(RCC_APB2_PERIPH_SPI1, ZF_ENABLE);    break;
        case IIS_2: RCC_EnableAPB1Periphs(RCC_APB1_PERIPH_SPI2, ZF_ENABLE);    break;
        case IIS_3: RCC_EnableAPB1Periphs(RCC_APB1_PERIPH_SPI3, ZF_ENABLE);    break;
        default:    break;
    }

    afio_init((gpio_pin_enum)((ck_pin & 0xFFF00) >> 8), GPO, (gpio_af_enum)((ck_pin & 0xF0) >> 4), GPO_AF_PUSH_PULL);      // 提取对应IO索引 AF功能编码
    afio_init((gpio_pin_enum)((sd_pin & 0xFFF00) >> 8), GPO, (gpio_af_enum)((sd_pin & 0xF0) >> 4), GPO_AF_PUSH_PULL);      // 提取对应IO索引 AF功能编码
    afio_init((gpio_pin_enum)((mck_pin & 0xFFF00) >> 8), GPO, (gpio_af_enum)((mck_pin & 0xF0) >> 4), GPO_AF_PUSH_PULL);      // 提取对应IO索引 AF功能编码
    afio_init((gpio_pin_enum)((ws_pin & 0xFFF00) >> 8), GPO, (gpio_af_enum)((ws_pin & 0xF0) >> 4), GPO_AF_PUSH_PULL);      // 提取对应IO索引 AF功能编码

    SPI2->CCTL = 0x00000048;
    SPI2->GCTL = 0x0000080C;

    uint32 register_temp = 0x00000000;
    float div_temp = 0.0;
    register_temp |= 0x00002C00;
    register_temp |= ((2 < (mck_pin & iis_n)) ? (0x00001000) : (0x00000000));
    register_temp |= ((uint32)mode << 4);
    register_temp |= ((uint32)channel_len << 0);
    register_temp |= (IIS_CHANNEL_LEN_16BIT == channel_len) ? (0x00000000) : ((uint32)data_len << 1);
    // sampl_freq = (bus_clock / div) / ((channel_len + 1) * 2))
    // bus_clock / sampl_freq / ((channel_len + 1) * 2)) = div
    div_temp = ((float)bus_clock / (float)sampl_freq / (float)((channel_len + 1) * 32) / 4.0 / (IIS_CHANNEL_LEN_16BIT == channel_len ? 2.0 : 1.0)) + 0.5;
    register_temp |= (uint32)div_temp << 16;
    SPI2->I2SCFGR = register_temp;

    SPI2->GCTL |= 0x00000001;
}
