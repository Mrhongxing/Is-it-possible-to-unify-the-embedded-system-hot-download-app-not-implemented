#include "stm32f10x.h"     // Device header
void hong(uint32_t time){
	time--;
};
int main(void){
	int i =0;
	GPIO_InitTypeDef wang;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	wang.GPIO_Mode=GPIO_Mode_Out_PP;
	wang.GPIO_Pin=GPIO_Pin_13;
	wang.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&wang);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	for(i=0;i<500000;i++){
		
	}
}
