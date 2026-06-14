#include "stm32f10x.h"
#include <math.h>
#include "Delay.h"

extern uint32_t global_flag15;
extern int voice_tmp;
extern int page_tmp;
extern int isload_tmp;
extern int voice;
extern int a;
int s =0;

void EXTI15_10_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line11)==SET){
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0){
			if(voice_tmp<16){
			voice_tmp+=1;
			};
		}
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line10)==SET){
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0){
			if(voice_tmp>0)
			voice_tmp-=1;
		}
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	if(EXTI_GetITStatus(EXTI_Line13)==SET){
		if(page_tmp==1){
		page_tmp=0;
		isload_tmp=0;
		voice=0;}else if(page_tmp==0){
		page_tmp=1;
		isload_tmp=0;
		voice=0;}
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetITStatus(EXTI_Line15)==SET){
		if(a==0||a==5){
			if(s==0){
			s=1;
			a=25;
			TIM_SetCompare1(TIM1,25);
			Delay_ms(200);
			s=0;
			}
		}else if(s==0){
			s=1;
			a=5;
			TIM_SetCompare1(TIM1,5);
			Delay_ms(200);
			s=0;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

void Knob_Init(char IO,uint8_t pin1,uint8_t pin2){
	EXTI_InitTypeDef exti2;
	EXTI_InitTypeDef exti1;
	EXTI_InitTypeDef exti12;
	EXTI_InitTypeDef exti15;
	NVIC_InitTypeDef nvic10_15;
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	exti1.EXTI_Line=EXTI_Line10;
	exti1.EXTI_LineCmd=ENABLE;
	exti1.EXTI_Mode =EXTI_Mode_Interrupt;
	exti1.EXTI_Trigger=EXTI_Trigger_Falling;
	exti2.EXTI_Line=EXTI_Line11;
	exti2.EXTI_LineCmd=ENABLE;
	exti2.EXTI_Mode =EXTI_Mode_Interrupt;
	exti2.EXTI_Trigger=EXTI_Trigger_Falling;
	exti15.EXTI_Line=EXTI_Line15;
	exti15.EXTI_LineCmd=ENABLE;
	exti15.EXTI_Mode =EXTI_Mode_Interrupt;
	exti15.EXTI_Trigger=EXTI_Trigger_Falling;
	exti12.EXTI_Line=EXTI_Line13;
	exti12.EXTI_LineCmd=ENABLE;
	exti12.EXTI_Mode =EXTI_Mode_Interrupt;
	exti12.EXTI_Trigger=EXTI_Trigger_Falling;
	nvic10_15.NVIC_IRQChannel=EXTI15_10_IRQn;
	nvic10_15.NVIC_IRQChannelCmd=ENABLE;
	nvic10_15.NVIC_IRQChannelPreemptionPriority=1;
	nvic10_15.NVIC_IRQChannelSubPriority=0;
	if(IO=='A'){
		GPIO_Init(GPIOA, &GPIO_InitStruct);
	}else if(IO=='B'){
		GPIO_Init(GPIOB, &GPIO_InitStruct);
	}else if(IO=='C'){
		GPIO_Init(GPIOC, &GPIO_InitStruct);
	}
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);
	EXTI_Init(&exti1);
	EXTI_Init(&exti2);
	EXTI_Init(&exti15);
	EXTI_Init(&exti12);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_Init(&nvic10_15);
}
