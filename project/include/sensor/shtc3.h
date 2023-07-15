#ifndef _SHTC3_H
#define _SHTC3_H

#include "zf_common_headfile.h"

//SHTC3 命令
#define SHTC3_WRITE_ADDR                ((SHTC3_ADDR << 1) & 0xFE)
#define SHTC3_READ_ADDR                 ((SHTC3_ADDR << 1) | 0x01)

#define SHTC3_SOFT_RESET_CMD            0x805D//软件复位命令
#define SHTC3_READ_ID_CMD               0xEFC8//读取SHTC3 ID命令
#define SHTC3_WAKE_UP_CMD               0x3517//将芯片从睡眠模式唤醒命令
#define SHTC3_SLEEP_CMD                 0xB098//使芯片进入休眠命令

//Normal Mode Clock Stretching Enable Measurement Command
#define SHTC3_NOR_READ_TEMP_FIR_EN_CMD  0x7CA2//
#define SHTC3_NOR_READ_HUM_FIR_EN_CMD   0x5C24//

//Normal Mode Clock Stretching Disable Measurement Command
#define SHTC3_NOR_READ_TEMP_FIR_DIS_CMD 0x7866//
#define SHTC3_NOR_READ_HUM_FIR_DIS_CMD  0x58E0//

//Low Power Mode Clock Stretching Enable Measurement Command
#define SHTC3_LOW_READ_TEMP_FIR_EN_CMD  0x6458//
#define SHTC3_LOW_READ_HUM_FIR_EN_CMD   0x44DE//

//Low Power Mode Clock Stretching Disable Measurement Command
#define SHTC3_LOW_READ_TEMP_FIR_DIS_CMD 0x609C//
#define SHTC3_LOW_READ_HUM_FIR_DIS_CMD  0x401A//

// shtc3传感器结构体
typedef struct {
    __IO float temperature;
    __IO float humidity;
} SHTC3;

void SHTC3_Init();
void SHTC3_WAKEUP();
void SHTC3_SLEEP();
uint8_t SHTC3_CRC_CHECK(uint16_t DAT,uint8_t CRC_DAT);
void SHTC3_MEASURE(SHTC3 *shtc3);

#endif  // _SHTC3_H