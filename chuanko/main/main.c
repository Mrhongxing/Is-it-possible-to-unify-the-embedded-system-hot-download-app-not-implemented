#include "stm32f10x.h"
#include "OLED.h"
int main(void) {
	GPIO_InitTypeDef GPIOs;
	USART_InitTypeDef USARTs;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_StructInit(&GPIOs);
	GPIOs.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOs.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIOs.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOs);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	USARTs.USART_BaudRate=9600;
	USARTs.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USARTs.USART_Mode
	USARTs.USART_Parity
	USARTs.USART_StopBits
	USARTs.USART_WordLength
	OLED_Init();
	OLED_ShowString(1,1,"aaa");
    while(1) {
        
    }
}
