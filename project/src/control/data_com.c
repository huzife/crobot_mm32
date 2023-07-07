#include "control/data_com.h"
#include "share_ware.h"
#include "FreeRTOS.h"
#include "queue.h"

/**
 *  函数功能：计算累加校验和
 *  入口参数：数据首地址、数据长度
 *  返 回 值：校验和
 */
uint8_t calcCheckSum(uint8_t *data, uint32_t len) {
    uint8_t sum = 0;
    for (int i = 0; i < len; i++) {
        sum += data[i];
    }
    
    return ~sum;
}

/**
 *  函数功能：数据解析器初始化
 *  入口参数：数据解析器结构体指针、缓冲区长度
 *  返 回 值：None
 */
void parserInit(Parser *parser, uint16_t b_len) {
    parser->flag = 0;   // 解析完成标志位置零
    parser->FE = 0;     // 帧头FE标志位置零
    parser->FH = 0;     // 完整帧头标志位置零
    
    parser->buf_len = b_len;    // 缓冲区长度
    parser->buf = (uint8_t *)pvPortMalloc(sizeof(uint8_t) * b_len); // 分配缓冲区内存
    parser->data_len = 0;   // 当前数据长度置零
}

/**
 *  函数功能：解析数据
 *  入口参数：数据解析器结构体指针、一个字节数据
 *  返 回 值：None
 */
void parse(Parser *parser, uint8_t data) {
    // 未完成则继续解析数据
    if (!parser->flag) {
        // 存入当前数据
        parser->buf[parser->data_len] = data;
        
        switch(data) {
            case 0xFE: {
                parser->FE = 1;
                break;
            }
            case 0xEF: {
                if (parser->FE) {
                    parser->FE = 0; // 清除FE标志
                    parser->FH = 1; // 完整帧头
                    parser->buf[0] = 0xFE;
                    parser->buf[1] = 0xEF;
                    parser->buf[2] = parser->buf_len;
                    parser->data_len = 1;
                }
                break;
            }
            default: {
                parser->FE = 0;
                break;
            }
        }
    }
    
    // 判断是否接收到完整的一帧数据
    if (parser->FH && (parser->data_len > parser->buf[2] + 2)) {
        // 累加法计算校验和
        uint8_t check_sum = calcCheckSum((uint8_t *)parser->buf, parser->buf[2] + 4);
        
        // 调整标志位
        parser->FE = 0;
        parser->FH = 0;
        parser->data_len = 0;
        
        // 校验和为0时校验成功
        if (!check_sum) parser->flag = 1;
    }
    
    parser->data_len++;
    if (parser->data_len >= parser->buf_len) parser->data_len = 0;
}

/**
 *  函数功能：数据帧处理
 *  入口参数：数据帧首地址
 *  返 回 值：None
 */
void processData(uint8_t *buf) {
    FunctionCode code = (FunctionCode)buf[3];
    switch (code) {
        case NONE: break;
        
        // 功能1：设置车体速度
        case SET_SPEED: {
            timClear(HEARTBEAT_TIM);
            // 读取线速度x
            for (int i = 0; i < 4; i++) {
                float_hex.hex[3 - i] = buf[4 + i];
            }
            kinematics_inverse.linear_x = float_hex.float_value;
            
            // 读取线速度x
            for (int i = 0; i < 4; i++) {
                float_hex.hex[3 - i] = buf[8 + i];
            }
            kinematics_inverse.linear_y = float_hex.float_value;
            
            // 读取线速度x
            for (int i = 0; i < 4; i++) {
                float_hex.hex[3 - i] = buf[12 + i];
            }
            kinematics_inverse.angular_z = float_hex.float_value;
            
            // 其他字节
            com_tx_data[0] = 0xFE;
            com_tx_data[1] = 0xEF;
            com_tx_data[2] = 0x01;
            com_tx_data[3] = 0x01;
            com_tx_data[4] = calcCheckSum((uint8_t *)com_tx_data, 4);
            
            // 发送数据帧
            DMA1->CH[3].CNDTR = 5;
            enableDMA(COM_UART_TX_CH);
            break;
        }
        
        // 功能2：获取车体速度
        case GET_SPEED_INFO: {
            // 获取线速度x
            float_hex.float_value = kinematics_forward.linear_x;
            for (int i = 0; i < 4; i++) {
                com_tx_data[4 + i] = float_hex.hex[3 - i];
            }
            
            // 获取线速度y
            float_hex.float_value = kinematics_forward.linear_y;
            for (int i = 0; i < 4; i++) {
                com_tx_data[8 + i] = float_hex.hex[3 - i];
            }
            
            // 获取角速度z
            float_hex.float_value = kinematics_forward.angular_z;
            for (int i = 0; i < 4; i++) {
                com_tx_data[12 + i] = float_hex.hex[3 - i];
            }
            
            // 其他字节
            com_tx_data[0] = 0xFE;
            com_tx_data[1] = 0xEF;
            com_tx_data[2] = 0x01;
            com_tx_data[3] = 0x02;
            com_tx_data[16] = calcCheckSum((uint8_t *)com_tx_data, 16);
            
            // 发送数据帧
            DMA1->CH[3].CNDTR = 17;
            enableDMA(COM_UART_TX_CH);
            break;
        }
    }
}













