#include "stm32f10x.h"     // Device header
void hong(uint32_t time){
	time--;
};
int main(void){
	int i =0;
	int a = 0;
	GPIO_InitTypeDef wangC13;
	GPIO_InitTypeDef wangB12;
	GPIO_InitTypeDef wangB13;
	GPIO_InitTypeDef wangB14;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	wangC13.GPIO_Mode=GPIO_Mode_Out_PP;
	wangC13.GPIO_Pin=GPIO_Pin_13;
	wangC13.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&wangC13);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	wangB12.GPIO_Mode=GPIO_Mode_Out_PP;
	wangB12.GPIO_Pin=GPIO_Pin_12;
	wangB12.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&wangB12);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	wangB13.GPIO_Mode=GPIO_Mode_Out_PP;
	wangB13.GPIO_Pin=GPIO_Pin_13;
	wangB13.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&wangB13);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_StructInit(&wangB14);
	wangB14.GPIO_Mode=GPIO_Mode_Out_PP;
	wangB14.GPIO_Pin=GPIO_Pin_14;
	wangB14.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&wangB14);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	for(i=0;i<11;i++){
		if(a==0){
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			a=1;
		}else{
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			a=0;
		};
	};
	while(i){
	};
}
