#include "sensor/shtc3.h"
#include "driver/i2c.h"

uint16_t SHTC3_ID;
uint8_t SHTC3_ERR;//主要用来判断I2C写操作是否能收到从机的应答信号，如果没有收到应答信号说明异常，0：收到应答信号，非0：没有收到应答信号

uint16_t TEMP_DATA;
uint8_t HUM_DATA;


void SHTC3_Init() {
    shtc3_I2C_Init();
    SHTC3_WAKEUP();
    iic_write_16bit(SHTC3_I2C, SHTC3_WRITE_ADDR, SHTC3_SOFT_RESET_CMD);
}

void SHTC3_WAKEUP() {
    iic_write_16bit(SHTC3_I2C, SHTC3_WRITE_ADDR, SHTC3_WAKE_UP_CMD);
}

void SHTC3_SLEEP() {
    iic_write_16bit(SHTC3_I2C, SHTC3_WRITE_ADDR, SHTC3_SLEEP_CMD);
}


uint8_t SHTC3_CRC_CHECK(uint16_t DAT,uint8_t CRC_DAT) {
    uint8_t i,t,temp;
    uint8_t CRC_BYTE;
   
    CRC_BYTE = 0xFF;  
    temp = (DAT >> 8) & 0xFF; 
       
    for(t = 0;t < 2;t ++) {
        CRC_BYTE ^= temp;
        for(i = 0;i < 8;i ++) {
            if(CRC_BYTE & 0x80) {
                CRC_BYTE <<= 1;
                CRC_BYTE ^= 0x31;    
            }    
            else {
                CRC_BYTE <<= 1;    
            }
        }
        
        if(t == 0) {
            temp = DAT & 0xFF; 
        }
    }
    
    if(CRC_BYTE == CRC_DAT) {
        temp = 0;    
    }    
    else {
        temp = 1;    
    }
    
    return temp;
}


void SHTC3_MEASURE(SHTC3 *shtc3) {
    uint8_t data[6];
	uint8_t temp_check_num;
	uint8_t hum_check_num;
	uint16_t temp_measurement;
	uint16_t hum_measurement;
    
    SHTC3_WAKEUP();
    iic_write_16bit(SHTC3_I2C, SHTC3_ADDR, SHTC3_NOR_READ_TEMP_FIR_EN_CMD);
    iic_read_8bit_array(SHTC3_I2C, SHTC3_ADDR, data, 6);
    temp_measurement = (data[0] << 8) + data[1];
    temp_check_num = data[2];
    hum_measurement = (data[3] << 8) + data[4];
    hum_check_num = data[5];
    
    
    if (SHTC3_CRC_CHECK(temp_measurement, temp_check_num) == 0) {
        shtc3->temperature = (float)temp_measurement * 175 / 65536 - 45;
    }
    
    if (SHTC3_CRC_CHECK(hum_measurement, hum_check_num) == 0) {
        shtc3->humidity = (float)hum_measurement * 100 / 65536;
    }
    
    SHTC3_SLEEP();
}








