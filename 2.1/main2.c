#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "OLED.h"

void EXTI15_10_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line15)==SET){
		EXTI_ClearITPendingBit(EXTI_Line15);
	};
}

int main(void){
	EXTI_InitTypeDef exti15;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef nvic10_15;
	chushihua();
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_4|GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	OLED_Init();
	OLED_ShowNum(1,1,4,1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
	exti15.EXTI_Line=EXTI_Line15;
	exti15.EXTI_LineCmd=ENABLE;
	exti15.EXTI_Mode =EXTI_Mode_Interrupt;
	exti15.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&exti15);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	nvic10_15.NVIC_IRQChannel=EXTI15_10_IRQn;
	nvic10_15.NVIC_IRQChannelCmd=ENABLE;
	nvic10_15.NVIC_IRQChannelPreemptionPriority=0;
	nvic10_15.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&nvic10_15);
	
	while(1){
		OLED_ShowBinNum(1,1,GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7),1);
	};
}
