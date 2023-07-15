#include "driver/i2c.h"

void shtc3_I2C_Init() {
    iic_init(SHTC3_I2C, SHTC3_ADDR, 100000, SHTC3_I2C_SCL, SHTC3_I2C_SDA);
}