#include "control/parser.h"
#include "FreeRTOS.h"
#include "queue.h"

void parserInit(Parser *parser, uint16_t b_len) {
    parser->flag = 0;   // 解析完成标志位置零
    parser->FE = 0;     // 帧头FE标志位置零
    parser->FH = 0;     // 完整帧头标志位置零
    
    parser->buf_len = b_len;    // 缓冲区长度
    parser->buf = (uint8_t *)pvPortMalloc(sizeof(uint8_t) * b_len); // 分配缓冲区内存
    parser->data_len = 0;   // 当前数据长度置零
}

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
        uint8_t check_sum = 0;
        for (int i = 0; i < parser->buf[2] + 3; i++) {
            check_sum += parser->buf[i];
        }
        check_sum = ~check_sum;
        
        // 调整标志位
        parser->FE = 0;
        parser->FH = 0;
        parser->data_len = 0;
        
        // 校验和不为0时校验失败
        if (!check_sum) parser->flag = 1;
    }
    
    parser->data_len++;
    if (parser->data_len >= parser->buf_len) parser->data_len = 0;
}















