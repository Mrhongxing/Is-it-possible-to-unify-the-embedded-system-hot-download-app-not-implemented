#include "stm32f10x.h"
#include <string.h>

extern uint8_t usart2_rx_buffer[100];
extern uint8_t usart2_rx_buffer_index;
extern uint32_t apps[50][3];

void send_cmd(const char* cmd){
    int i =0;
    for (i = 0; cmd[i] != '\0'; i++) {
        USART_SendData(USART2, cmd[i]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    }
    
}

uint8_t read_cmd(void){
    if (strstr(usart2_rx_buffer, "app list")) {
        memset(usart2_rx_buffer, 0, sizeof(usart2_rx_buffer));
        usart2_rx_buffer_index = 0;
        send_cmd("id    adress    backadress\r\n");
        for (int i = 0; i < 50; i++) {
            if (apps[i][0] != 0) {
                char app_info[20];
                sprintf(app_info, "app%d ", apps[i][0]);
                send_cmd(app_info);
                sprintf(app_info, "0x%08X ", apps[i][1]);
                send_cmd(app_info);
                sprintf(app_info, "0x%08X\r\n", apps[i][2]);
                send_cmd(app_info);
            }else{
                break;
            }
        }
        return 1;
    }
    return 0;
}


