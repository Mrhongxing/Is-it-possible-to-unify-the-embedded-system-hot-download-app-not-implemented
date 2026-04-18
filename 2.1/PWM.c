#include "stm32f10x.h"

void PWM_Init(void){
	NVIC_InitTypeDef time1_nvic;
	TIM_TimeBaseInitTypeDef time1;
	TIM_OCInitTypeDef tim1_oc1;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_InternalClockConfig(TIM1);
	time1.TIM_ClockDivision=TIM_CKD_DIV1;
	time1.TIM_CounterMode=TIM_CounterMode_Up;
	time1.TIM_Period=199;
	time1.TIM_Prescaler=7199;
	time1.TIM_RepetitionCounter=1;
	TIM_TimeBaseInit(TIM1,&time1);
	
	TIM_OCStructInit(&tim1_oc1);
	tim1_oc1.TIM_OCPolarity=TIM_OCPolarity_High;
	tim1_oc1.TIM_OCMode=TIM_OCMode_PWM1;
	tim1_oc1.TIM_OutputState=ENABLE;
	tim1_oc1.TIM_Pulse=0;
	TIM_OC1Init(TIM1,&tim1_oc1);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1,ENABLE);
}
