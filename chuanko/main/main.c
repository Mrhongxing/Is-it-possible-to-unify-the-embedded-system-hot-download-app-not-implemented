#include "stm32f10x.h"
#include "OLED.h"
#include "stdio.h"
#include "Delay.h"
#include "HTTP.h"
#include <stdio.h>
#include "CLock.h"

uint16_t recives[1000];
GPIO_InitTypeDef GPIOs;
int number=1;
int isChanged=0;
volatile uint32_t timer_ms = 0;
int timerReLoad=0;

// 重定向 printf 到哪个串口（根据你的需要选择）
// 如果要用 USART1 输出调试信息：
int fputc(int ch, FILE *f)
{
    // 等待发送完成
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, (uint8_t)ch);
    return ch;
}

// 如果需要接收，可以同时实现 fgetc（可选）
int fgetc(FILE *f)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(USART1);
}
int main(void) {
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
			}
			//j=USART1_connectToServe("192.168.1.21");
	
			//k=USART1_HTTP(2," ");
		}
		//if(j==1){
			//OLED_ShowString(2,1,"               ");
			//OLED_ShowString(2,1,"Ok");
			//show=0;
		//}else if(show==0){
			//OLED_ShowString(2,1,"               ");
			//OLED_ShowString(2,1,"serveConnectError");
			//show=1;
		//}
		//OLED_ShowString(1,1,"               ");
		//OLED_ShowNum(1,1,timer_ms,6);
		//if(k==1){
			//OLED_ShowString(3,1,"sendOK");
		//}else{
			//OLED_ShowString(3,1,"sendError");
			//j=USART1_connectToServe("192.168.1.21");
			//k=USART1_HTTP(2," ");
		//}
	}
}
