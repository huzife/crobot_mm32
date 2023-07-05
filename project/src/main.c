// includes
#include "zf_common_headfile.h"
#include "share_ware.h"
#include "FreeRTOS.h"
#include "task.h"

// defines

// variables

// functions
void freertos_init();

// code
int main (void) {
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz
//    debug_init();                                                               // 初始化默认 Debug UART

    // 此处编写用户代码 例如外设初始化代码等
    uartDMAInit(D_UART_INDEX, D_UART_BAUDRATE, D_UART_TX_PIN, D_UART_RX_PIN);
    
    freertos_init();
    vTaskStartScheduler();
    // 此处编写用户代码 例如外设初始化代码等

    while(1) {
        // 此处编写需要循环执行的代码
        
        // 此处编写需要循环执行的代码
    }
}
