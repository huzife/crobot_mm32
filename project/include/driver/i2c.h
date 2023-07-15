#ifndef _I2C_H
#define _I2C_H

#include "zf_common_headfile.h"

#define SHTC3_I2C IIC_1
#define SHTC3_ADDR 0x70
#define SHTC3_I2C_SCL IIC1_SCL_B8
#define SHTC3_I2C_SDA IIC1_SDA_B9

void shtc3_I2C_Init();

#endif  // _I2C_H