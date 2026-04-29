#include "stm32f10x.h"

void CLC_Init(void){
	NVIC_InitTypeDef time2_nvic;
	TIM_TimeBaseInitTypeDef time2;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	//GPIO_InitTypeDef GPIO_InitStruct;
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	//GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    //GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_Init(GPIOA,&GPIO_InitStruct);
	//TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_Inverted,0x01);
	
	time2.TIM_Prescaler=7199;
	time2.TIM_ClockDivision=TIM_CKD_DIV1;
	time2.TIM_CounterMode=TIM_CounterMode_Up;
	time2.TIM_RepetitionCounter=1;
	time2.TIM_Period=9999;
	TIM_TimeBaseInit(TIM2,&time2);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	time2_nvic.NVIC_IRQChannel=TIM2_IRQn;
	time2_nvic.NVIC_IRQChannelCmd=ENABLE;
	time2_nvic.NVIC_IRQChannelPreemptionPriority=0;
	time2_nvic.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&time2_nvic);
	TIM_Cmd(TIM2,ENABLE);
}
