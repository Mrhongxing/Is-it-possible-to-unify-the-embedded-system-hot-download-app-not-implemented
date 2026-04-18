#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "knob.h"
#include "clc.h"
#include "PWM.h"

volatile uint32_t global_flag_for_time2 = 0;
volatile uint32_t global_flag_for_time2_tmp = 0;
volatile int voice=0;//音量
volatile int voice_tmp=5;//临时加载
volatile  int page =0;//页面
volatile int page_tmp=0;//临时页面
volatile int isload=0;//加载状态
volatile int isload_tmp=0;//临时加载状态
volatile int a = 0;

void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){
		global_flag_for_time2_tmp+=1;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
int main(void){
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	OLED_Init();
	Knob_Init('B',11,10);
	
	CLC_Init();
	PWM_Init();

	while(1){
		page=page_tmp;
		isload=isload_tmp;
		if(page==1){
			if(isload==0){
				OLED_Clear();
				OLED_ShowString(2,6,"voice");
				isload=1;
				isload_tmp=1;
			}
			if(voice>voice_tmp){
				
				for(int i = voice_tmp;i<=voice;i++){
					if(i!=0){
					OLED_ShowChar(3,i,' ');
					}
				}
				voice = voice_tmp;
				
			}else if(voice<voice_tmp){
				
				for(int i = voice;i<=voice_tmp;i++){
					if(i!=0){
					OLED_ShowChar(3,i,127);
					}
				}
				voice = voice_tmp;
				
			}
		}else if(page==0){
			if(isload==0){
				OLED_Clear();
				OLED_ShowString(1,4,"main page");
				isload=1;
				isload_tmp=1;
			}
			OLED_ShowNum(2,1,global_flag_for_time2_tmp,3);
		}
		
	};
}
