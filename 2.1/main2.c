#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "OLED.h"

volatile uint32_t global_flag15 = 0;
volatile uint32_t global_flag11 = 0;
volatile int is =0;
volatile uint32_t global_flag_for_time2 = 0;

/*void EXTI15_10_IRQHandler(void){
	int size =1;
	int num = 0;
	if(EXTI_GetITStatus(EXTI_Line15)==SET){
		if(is==1){
			global_flag11+=1;
		num = global_flag11;
		while(num>=10){
			size++;
			num=num/10;
		}
		OLED_ShowNum(1,5,global_flag11,size);
		is=0;
		EXTI_ClearITPendingBit(EXTI_Line11);
		EXTI_ClearITPendingBit(EXTI_Line15);
		}else if(is==2){
			
			EXTI_ClearITPendingBit(EXTI_Line11);
			EXTI_ClearITPendingBit(EXTI_Line15);
		}else{
			is=2;
			EXTI_ClearITPendingBit(EXTI_Line15);
		}
	}else if(EXTI_GetITStatus(EXTI_Line11)==SET){
		
		if(is==2){
			global_flag11-=1;
		num = global_flag11;
		while(num>=10){
			size++;
			num=num/10;
		}
		if(num==9){
		OLED_ShowString(1, 5,"     "); 
		}
		OLED_ShowNum(1,5,global_flag11,size);
		is=0;
		EXTI_ClearITPendingBit(EXTI_Line11);
		EXTI_ClearITPendingBit(EXTI_Line15);
		}else if(is==1){
			
			EXTI_ClearITPendingBit(EXTI_Line11);
			EXTI_ClearITPendingBit(EXTI_Line15);
		}else{
			is=1;
			EXTI_ClearITPendingBit(EXTI_Line11);
		}
		
	}
}*/
void EXTI15_10_IRQHandler(void){
	int size =1;
	int num = 0;
	if(EXTI_GetITStatus(EXTI_Line15)==SET){
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)==0){
			global_flag15+=1;
			num = global_flag15;
			while(num>=10){
				size++;
				num=num/10;
			}
			OLED_ShowNum(1,5,global_flag15,size);
			
			
		}
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
	if(EXTI_GetITStatus(EXTI_Line11)==SET){
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==0){
			global_flag15-=1;
			num = global_flag15;
			while(num>=10){
				size++;
				num=num/10;
			}
			if(num==9){
				OLED_ShowString(1, 5,"     "); 
			}
			OLED_ShowNum(1,5,global_flag15,size);
			
		}
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
}
void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		global_flag_for_time2+=1;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
int main(void){
	uint32_t tmp;
	int num;
	int size=1;
	EXTI_InitTypeDef exti15;
	EXTI_InitTypeDef exti11;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef nvic10_15;
	NVIC_InitTypeDef time2_nvic;
	TIM_TimeBaseInitTypeDef time2;
	chushihua();
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_11|GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	OLED_Init();
	OLED_ShowNum(1,1,4,1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);
	exti15.EXTI_Line=EXTI_Line15;
	exti15.EXTI_LineCmd=ENABLE;
	exti15.EXTI_Mode =EXTI_Mode_Interrupt;
	exti15.EXTI_Trigger=EXTI_Trigger_Falling;
	exti11.EXTI_Line=EXTI_Line11;
	exti11.EXTI_LineCmd=ENABLE;
	exti11.EXTI_Mode =EXTI_Mode_Interrupt;
	exti11.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&exti15);
	EXTI_Init(&exti11);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	nvic10_15.NVIC_IRQChannel=EXTI15_10_IRQn;
	nvic10_15.NVIC_IRQChannelCmd=ENABLE;
	nvic10_15.NVIC_IRQChannelPreemptionPriority=1;
	nvic10_15.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&nvic10_15);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_InternalClockConfig(TIM2);
	time2.TIM_Prescaler=7199;
	time2.TIM_ClockDivision=TIM_CKD_DIV1;
	time2.TIM_CounterMode=TIM_CounterMode_Up;
	time2.TIM_RepetitionCounter=1;
	time2.TIM_Period=4999;
	TIM_TimeBaseInit(TIM2,&time2);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	time2_nvic.NVIC_IRQChannel=TIM2_IRQn;
	time2_nvic.NVIC_IRQChannelCmd=ENABLE;
	time2_nvic.NVIC_IRQChannelPreemptionPriority=0;
	time2_nvic.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&time2_nvic);
	TIM_Cmd(TIM2,ENABLE);
	while(1){
		if(tmp!=global_flag_for_time2){
			tmp=global_flag_for_time2;
			num= global_flag_for_time2;
			size=1;
			while(num>=10){
				size++;
				num=num/10;
			}
			OLED_ShowNum(2,1,global_flag_for_time2,size);
		}
	};
}
