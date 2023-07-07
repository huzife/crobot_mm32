// includes
#include "zf_common_headfile.h"
#include "share_ware.h"
#include "FreeRTOS.h"
#include "task.h"

// defines

// variables

// functions
void freertosInit();

// code
int main (void) {
    clock_init(SYSTEM_CLOCK_120M);                                              // 初始化芯片时钟 工作频率为 120MHz

    // 此处编写用户代码 例如外设初始化代码等
    freertosInit();
    vTaskStartScheduler();
    
    // 此处编写用户代码 例如外设初始化代码等

    while(1) {
        // 此处编写需要循环执行的代码
        
        // 此处编写需要循环执行的代码
    }
}
