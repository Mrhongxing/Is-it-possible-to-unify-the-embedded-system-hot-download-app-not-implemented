/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Private typedef -----------------------------------------------------------*/
				 
/* Private define ------------------------------------------------------------*/

/*神舟王103LED灯相关定义*/
#define RCC_GPIO_LED                    RCC_APB2Periph_GPIOF    /*LED使用的GPIO时钟*/
#define LEDn                            4                       /*神舟王103LED数量*/
#define GPIO_LED                        GPIOF                   /*神舟王103LED灯使用的GPIO组*/

#define DS1_PIN                         GPIO_Pin_10              /*DS1使用的GPIO管脚*/
#define DS2_PIN                         GPIO_Pin_9				/*DS2使用的GPIO管脚*/
#define DS3_PIN                         GPIO_Pin_8  			/*DS3使用的GPIO管脚*/
#define DS4_PIN                         GPIO_Pin_7				/*DS4使用的GPIO管脚*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;
u8 count=0;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void Delay(vu32 nCount);
void Turn_On_LED(u8 LED_NUM);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int direction =0;
int on =0;

void EXTI3_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET){
		Delay(15000);
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0){
			direction=1;
		}
	}
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)==0);
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI1_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET){
		Delay(15000);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0){
			on=1;
		}
	}
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0);
	EXTI_ClearITPendingBit(EXTI_Line1);
}

int main(void)
{
	int i=0;
	int j=0;
	EXTI_InitTypeDef exti3;
	NVIC_InitTypeDef nvic3;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  	/* 配置神舟王103LED灯使用的GPIO管脚模式*/
  	RCC_APB2PeriphClockCmd(RCC_GPIO_LED|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); /*使能LED灯使用的GPIO时钟*/

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	
	exti3.EXTI_Line=EXTI_Line3;
	exti3.EXTI_Mode=EXTI_Mode_Interrupt;
	exti3.EXTI_Trigger=EXTI_Trigger_Falling;
	exti3.EXTI_LineCmd=ENABLE;
	EXTI_Init(&exti3);
	
	exti3.EXTI_Line=EXTI_Line1;
	EXTI_Init(&exti3);
	
	nvic3.NVIC_IRQChannel=EXTI3_IRQn;
	nvic3.NVIC_IRQChannelCmd=ENABLE;
	nvic3.NVIC_IRQChannelPreemptionPriority=1;
	nvic3.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&nvic3);
	
	nvic3.NVIC_IRQChannel=EXTI1_IRQn;
	NVIC_Init(&nvic3);
	
  	GPIO_InitStructure.GPIO_Pin = DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  	GPIO_Init(GPIO_LED, &GPIO_InitStructure);  /*神舟王103使用的LED灯相关的GPIO口初始化*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  	GPIO_SetBits(GPIO_LED,DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN);/*关闭所有的LED指示灯*/

  	while(1)
  	{	
		if(on==1){
			if(i==1){i=0;}else{i=1;}
			on=0;
			Delay(0x2FFFFF);
			
		}
		if(direction==1){
			if(j==1){j=0;}else{j=1;}
			
			direction=0;
			Delay(0x2FFFFF);
		}
		if(i==1){
		if(j==1){
		GPIO_SetBits(GPIO_LED,DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN);/*关闭所有的LED指示灯*/
		Turn_On_LED(count%4);	//点亮一个LED灯		
		count++;
		Delay(0x2FFFFF);
		}else{
			GPIO_SetBits(GPIO_LED,DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN);/*关闭所有的LED指示灯*/
			switch(count%4)
	{
        case 0:
          GPIO_ResetBits(GPIO_LED,DS4_PIN);  /*点亮DS1灯*/
          break;
        case 1:
          GPIO_ResetBits(GPIO_LED,DS3_PIN);  /*点亮DS2灯*/
          break;
        case 2:
          GPIO_ResetBits(GPIO_LED,DS2_PIN);  /*点亮DS3灯*/
          break;
        case 3:
          GPIO_ResetBits(GPIO_LED,DS1_PIN);  /*点亮DS4灯*/
          break;          
        default:
		  GPIO_ResetBits(GPIO_LED,DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN); /*点亮所有的灯*/
          break;
	}
		count++;
		Delay(0x2FFFFF);
		}
  	}else{
	GPIO_SetBits(GPIO_LED,DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN);}
	}
}

/*点亮对应灯*/
void Turn_On_LED(u8 LED_NUM)
{
	switch(LED_NUM)
	{
        case 0:
          GPIO_ResetBits(GPIO_LED,DS1_PIN);  /*点亮DS1灯*/
          break;
        case 1:
          GPIO_ResetBits(GPIO_LED,DS2_PIN);  /*点亮DS2灯*/
          break;
        case 2:
          GPIO_ResetBits(GPIO_LED,DS3_PIN);  /*点亮DS3灯*/
          break;
        case 3:
          GPIO_ResetBits(GPIO_LED,DS4_PIN);  /*点亮DS4灯*/
          break;          
        default:
		  GPIO_ResetBits(GPIO_LED,DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN); /*点亮所有的灯*/
          break;
	}
}



/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
