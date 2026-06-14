/* #include "stm32f10x.h"
#include "OLED.h"
#include "stdio.h"
#include "Delay.h"
#include "HTTP.h"
#include <stdio.h>
#include "CLock.h" */

/* uint16_t recives[1000];
GPIO_InitTypeDef GPIOs;
int number=1;
int isChanged=0;
volatile uint32_t timer_ms = 0;
int timerReLoad=0; */

// 重定向 printf 到哪个串口（根据你的需要选择）
// 如果要用 USART1 输出调试信息：
/* int fputc(int ch, FILE *f)
{
	// 等待发送完成
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, (uint8_t)ch);
	return ch;
} */

// 如果需要接收，可以同时实现 fgetc（可选）
/* int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
} */
/* int main2(void) {
	int i=0;
	int s=0;
	int j=0;
	int k=0;
	int h = 0;
	int a =0;
	int show=0;
	char password[100];
	char name[100];
	int havewifi=0;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	GPIO_StructInit(&GPIOs);
	GPIOs.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOs.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIOs.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOs);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	GPIOs.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_11;
	GPIO_Init(GPIOA,&GPIOs);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);

	TIM2_Init();

	OLED_Init();

	OLED_ShowString(1,1,"STMOK");

	i=USART1_Init();

	h=USART1_openHotspot();



	while(1){
		if(i==1){
			show=0;
		}else{
			OLED_ShowString(2,1,"               ");
			OLED_ShowString(2,1,"wifiModeError");
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			Delay_ms(1000);
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			i=USART1_Init();
			show=1;
		}
		if(havewifi==0){
		if(h==1){
			show=0;
			OLED_ShowString(2,1,"OK");
		}else if(show==0&&h==2){
			OLED_ShowString(2,1,"               ");
			OLED_ShowString(2,1,"wifiConfigError");
			h=USART1_openHotspot();
			show=1;
		}else if(show==0){
			OLED_ShowString(2,1,"               ");
			OLED_ShowNum(2,1,show,1);
			h=USART1_openHotspot();
			show=1;
		}
		a=ReadConnectIPD();
		if(a==1){
			USART1_sendHTML("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h2>WiFiPZ</h2>\r\n<form action='/config' method='get'>\r\nWiFiMC: <input type='text' name='ssid'><br>\r\nWiFiMM: <input type='password' name='pwd'><br>\r\n<input type='submit' value='LJ'>\r\n</form>\r\n\r\n\r\n");
		}else if(a==2){
			readHTML(&name[100],&password[100]);
			havewifi=1;
		}
		}else if(havewifi==1){
			if(s==1){
				OLED_ShowString(3,1,"wifiOK");
			}else{
				s=USART1_connectToWifi(&name[100],&password[100]);
			} */
// j=USART1_connectToServe("192.168.1.21");

// k=USART1_HTTP(2," ");
//}
// if(j==1){
// OLED_ShowString(2,1,"               ");
// OLED_ShowString(2,1,"Ok");
// show=0;
//}else if(show==0){
// OLED_ShowString(2,1,"               ");
// OLED_ShowString(2,1,"serveConnectError");
// show=1;
//}
// OLED_ShowString(1,1,"               ");
// OLED_ShowNum(1,1,timer_ms,6);
// if(k==1){
// OLED_ShowString(3,1,"sendOK");
//}else{
// OLED_ShowString(3,1,"sendError");
// j=USART1_connectToServe("192.168.1.21");
// k=USART1_HTTP(2," ");
//}
/* 	}
} */

#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

typedef uint32_t (*func_t)(uint32_t, uint32_t);
typedef void (*void_func_t)(void);

uint32_t jump(uint32_t addr, uint32_t arg0, uint32_t arg1)
{
	addr |= 1;
	func_t f = (func_t)addr;
	return f(arg0, arg1);
}

void void_jump(uint32_t addr)
{
	addr |= 1;
	void_func_t f = (void_func_t)addr;
	f();
}

__attribute__((used)) void try()
{
	OLED_ShowString(3, 1, "Hello, STM32!");
}

uint32_t apps[50][3] = {{1, 0x0800F000, 0}};
uint8_t app_now = 0;
uint8_t app_bake[3];
uint8_t app_temp_now = 0;
uint16_t tryint[16] = {
    0xB510,  // PUSH {R4, LR}      (原 0x10B5)
      // POP {R4, PC}       (原 0x10BD)  ← 但这个位置不对，见下文
    0xA203,  // ADR R2, #0x0C      (原 0x03A2)
    0x2101,  // MOVS R1, #0x01     (原 0x0121)
    0x2003,  // MOVS R0, #0x03     (原 0x0320)
    0xF7F1,  // BL 高位            (原 0xFFF8)
    0xFB6C,  // BL 低位            (原 0xFDB6)
    0xBD10,  // POP {R4, PC}       (原 0x10BD)
    0x0000,
    0x6548,  // "He"               (原 0x4865)
    0x6C6C,  // "ll"
    0x2C6F,  // "o,"               (原 0x6F2C)
    0x5320,  // " S"               (原 0x2053)
    0x4D54,  // "TM"               (原 0x544D)
    0x3233,  // "32"               (原 0x3332)
    0x0021,  // "!\0"              (原 0x2100)
    0x0000
};

int main(void)
{
	uint32_t i = 0;
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
	// 使能 GPIOB 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// 配置 GPIOB Pin 6 和 Pin 7 为推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	// 初始化 OLED 显示屏
	OLED_Init();
	// 配置 Flash 延迟和预取缓冲
	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_PrefetchBufferCmd(ENABLE);
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(0x0800F000);
    for(i = 0; i < 16; i++)
	{
		FLASH_ProgramHalfWord(0x0800F000 + i*2, tryint[i]);
	}
	
	FLASH_Lock();
	// 显示欢迎信息
	OLED_ShowString(1, 1, "Hello, STM32!");
	while (1)
	{
		if (app_temp_now != app_now)
		{
			for (int i = 0; i < 50; i++)
			{
				if (apps[i][0] != 0&&apps[i][0] == app_now)
				{
					void_jump(apps[i][1]);
					app_temp_now = app_now;
					break;
				}
			}
		}else
		{
			OLED_ShowString(3, 1, "No app");
			Delay_ms(1000);
			void_jump(apps[0][1]);
		}
	}
}
