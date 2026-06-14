/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "ili9320.h"
#include "ili9320_api.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/*神舟王103LED灯相关定义*/
#define RCC_GPIO_LED RCC_APB2Periph_GPIOF /*LED使用的GPIO时钟*/
#define LEDn 4							  /*神舟王103LED数量*/
#define GPIO_LED GPIOF					  /*神舟王103LED灯使用的GPIO组*/

#define DS1_PIN GPIO_Pin_10 /*DS1使用的GPIO管脚*/
#define DS2_PIN GPIO_Pin_9	/*DS2使用的GPIO管脚*/
#define DS3_PIN GPIO_Pin_8	/*DS3使用的GPIO管脚*/
#define DS4_PIN GPIO_Pin_7	/*DS4使用的GPIO管脚*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;

u8 count = 0;

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
int direction = 0;
int size = 0;
int i = 0;
int j = 0;
uint16_t data[100];
uint16_t adc_value[10];
const uint16_t Sine12bit[32] = {
	2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
	3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
	599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};

uint32_t DualSine12bit[32];
const uint16_t Square12bit[32] = {
	4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
	4095, 4095, 4095, 4095, 4095, 4095, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0};

// 三角波表 (32点) - 上升沿和下降沿
const uint16_t Triangle12bit[32] = {
	0, 264, 528, 792, 1056, 1320, 1584, 1848, 2047, 2311, 2575, 2839,
	3103, 3367, 3631, 3895, 4095, 3895, 3631, 3367, 3103, 2839, 2575,
	2311, 2047, 1848, 1584, 1320, 1056, 792, 528, 264};
// 字 "三", "角", "波" 合并数组
unsigned char char_san_jiao_bo[3][72] = {
	// "三"
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1F,
		0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x40, 0x0F, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x7F, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	// "角"
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x81, 0x00, 0x01, 0xFF, 0x80, 0x01,
		0x03, 0x00, 0x02, 0x02, 0x00, 0x06, 0x04, 0x10, 0x0F, 0xFF, 0xF8, 0x1A, 0x08, 0x10, 0x22, 0x08,
		0x10, 0x02, 0x08, 0x10, 0x03, 0xFF, 0xF0, 0x02, 0x08, 0x10, 0x02, 0x08, 0x10, 0x03, 0xFF, 0xF0,
		0x02, 0x08, 0x10, 0x02, 0x08, 0x10, 0x06, 0x08, 0x10, 0x04, 0x08, 0x10, 0x08, 0x08, 0x10, 0x10,
		0x09, 0xF0, 0x20, 0x08, 0x70, 0x00, 0x00, 0x00},
	// "波"
	{
		0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x10, 0x03, 0x00, 0x0C, 0x01, 0x00, 0x04, 0x01, 0x00, 0x04,
		0x01, 0x08, 0x01, 0xFF, 0xFC, 0x41, 0x41, 0x18, 0x21, 0x41, 0x10, 0x32, 0x41, 0x00, 0x12, 0x41,
		0x00, 0x02, 0x41, 0x10, 0x04, 0x7E, 0xF0, 0x04, 0x48, 0x30, 0x04, 0x44, 0x20, 0x08, 0x44, 0x40,
		0x78, 0x42, 0xC0, 0x18, 0x81, 0x80, 0x18, 0x81, 0x80, 0x19, 0x02, 0xC0, 0x19, 0x0C, 0x70, 0x1A,
		0x10, 0x3C, 0x04, 0xE0, 0x08, 0x00, 0x00, 0x00}};
// 字 "正", "弦", "波" 合并数组
unsigned char char_zheng_xian_bo[3][72] = {
	// "正"
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x1C, 0x0F, 0xFF, 0xE0, 0x00,
		0x08, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x03, 0x08, 0x00, 0x02, 0x08,
		0x10, 0x02, 0x08, 0x38, 0x02, 0x0F, 0xC0, 0x02, 0x08, 0x00, 0x02, 0x08, 0x00, 0x02, 0x08, 0x00,
		0x02, 0x08, 0x00, 0x02, 0x08, 0x00, 0x02, 0x08, 0x08, 0x02, 0x08, 0x1C, 0x3D, 0xF7, 0xE0, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	// "弦"
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x81, 0x00, 0x1F, 0x81, 0x00, 0x01,
		0x80, 0x0C, 0x01, 0xFF, 0xF0, 0x01, 0x83, 0x00, 0x11, 0x83, 0x00, 0x1F, 0x86, 0x00, 0x10, 0x04,
		0x30, 0x10, 0x08, 0x20, 0x30, 0x18, 0x60, 0x3F, 0x9F, 0xC0, 0x01, 0x90, 0x80, 0x01, 0x01, 0x00,
		0x01, 0x02, 0x00, 0x01, 0x06, 0x20, 0x01, 0x04, 0x18, 0x01, 0x08, 0x0C, 0x03, 0x3F, 0xFC, 0x0E,
		0x18, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00},
	// "波"
	{
		0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x10, 0x03, 0x00, 0x0C, 0x01, 0x00, 0x04, 0x01, 0x00, 0x04,
		0x01, 0x08, 0x01, 0xFF, 0xFC, 0x41, 0x41, 0x18, 0x21, 0x41, 0x10, 0x32, 0x41, 0x00, 0x12, 0x41,
		0x00, 0x02, 0x41, 0x10, 0x04, 0x7E, 0xF0, 0x04, 0x48, 0x30, 0x04, 0x44, 0x20, 0x08, 0x44, 0x40,
		0x78, 0x42, 0xC0, 0x18, 0x81, 0x80, 0x18, 0x81, 0x80, 0x19, 0x02, 0xC0, 0x19, 0x0C, 0x70, 0x1A,
		0x10, 0x3C, 0x04, 0xE0, 0x08, 0x00, 0x00, 0x00}};
const u8 WangChar[3][72] = {
	// 字 "王"
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x1F, 0xFF, 0xF8, 0x00,
		0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18,
		0x00, 0x00, 0x18, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00,
		0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x0C, 0x3F, 0xFF, 0xFE, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	// 字 "瀚"
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xC0, 0x30, 0x40, 0xC0, 0x18, 0x50, 0xA0, 0x1B,
		0xF9, 0x90, 0x00, 0x41, 0x18, 0x00, 0x42, 0x0C, 0x47, 0xFC, 0x04, 0x25, 0x14, 0x8C, 0x39, 0x1B,
		0xFC, 0x19, 0xF0, 0xCC, 0x09, 0x14, 0xAC, 0x09, 0x16, 0xBC, 0x09, 0xF2, 0xAC, 0x08, 0x40, 0x8C,
		0x10, 0x43, 0xBC, 0x37, 0xFC, 0xEC, 0x10, 0x4C, 0x8C, 0x10, 0x40, 0x8C, 0x10, 0x40, 0x8C, 0x10,
		0x47, 0xB8, 0x10, 0x41, 0x18, 0x00, 0x00, 0x00},
	// 字 "霆"
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x07, 0xFF, 0x80, 0x10, 0x10, 0x08, 0x1F,
		0xFF, 0xFC, 0x10, 0x10, 0x08, 0x33, 0xD3, 0xD0, 0x00, 0x10, 0x00, 0x03, 0x99, 0x80, 0x02, 0x10,
		0x20, 0x3F, 0x07, 0xF0, 0x04, 0x19, 0x00, 0x08, 0x01, 0x00, 0x19, 0x81, 0x10, 0x17, 0x7F, 0xE0,
		0x21, 0x01, 0x00, 0x12, 0x01, 0x00, 0x0A, 0x01, 0x18, 0x06, 0x3E, 0xE0, 0x0F, 0x00, 0x00, 0x18,
		0xFF, 0xFC, 0x20, 0x0F, 0xF8, 0x00, 0x00, 0x00}};
const uint8_t char_tong[] = {
	// 字 "通"
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x3F, 0xF0, 0x0C, 0x0C, 0xC0, 0x0C,
	0x07, 0x00, 0x00, 0x03, 0x08, 0x00, 0x7F, 0xF8, 0x00, 0x42, 0x08, 0x04, 0x42, 0x08, 0x3C, 0x7F,
	0xF8, 0x04, 0x42, 0x08, 0x04, 0x42, 0x08, 0x04, 0x42, 0x08, 0x04, 0x7F, 0xF8, 0x04, 0x42, 0x08,
	0x04, 0x42, 0x08, 0x04, 0x42, 0x08, 0x0C, 0x42, 0x38, 0x33, 0x00, 0x10, 0x60, 0xC0, 0x02, 0x20,
	0x3F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};
unsigned char char_xin[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x03, 0x03, 0x00, 0x02, 0x03, 0x08, 0x06,
	0xFF, 0xFC, 0x04, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x0C, 0x00, 0x10, 0x1C, 0x3F, 0xE0, 0x14, 0x00,
	0x00, 0x24, 0x00, 0x10, 0x44, 0x7F, 0xF8, 0x44, 0x00, 0x00, 0x04, 0x00, 0x00, 0x04, 0x20, 0x18,
	0x04, 0x3F, 0xF0, 0x04, 0x20, 0x10, 0x04, 0x20, 0x10, 0x04, 0x20, 0x10, 0x04, 0x3F, 0xF0, 0x04,
	0x20, 0x10, 0x04, 0x20, 0x10, 0x00, 0x00, 0x00};
const uint8_t char_ban[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x01, 0x88, 0x00, 0x3F, 0xCB, 0xFC, 0x04,
	0x08, 0x40, 0x04, 0x08, 0x40, 0x04, 0x08, 0x40, 0x04, 0x08, 0x40, 0x04, 0x28, 0x40, 0x3F, 0xA8,
	0x40, 0x04, 0x68, 0x4C, 0x04, 0x49, 0xF0, 0x04, 0x08, 0x40, 0x04, 0x08, 0x40, 0x04, 0x18, 0x40,
	0x05, 0x90, 0x40, 0x0E, 0x10, 0x40, 0x78, 0x20, 0x40, 0x60, 0x60, 0x44, 0x00, 0x87, 0xFE, 0x01,
	0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00};

void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		Delay(150);
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) == 0)
		{
			if (direction == 1)
			{
				direction = 0;
			}
			else
			{
				direction = 1;
			}
		}
	}

	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		Delay(150);
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
		{
			if (j == 2)
			{
				j = 0;
			}
			else if(j==0)
			{
				j = 1;
			}else if(j==1){
				j=2;
			}
		}
		GPIO_SetBits(GPIO_LED, DS1_PIN | DS2_PIN | DS3_PIN | DS4_PIN);
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		if (j == 1)
		{
			if (direction == 0)
			{
				GPIO_SetBits(GPIO_LED, DS1_PIN | DS2_PIN | DS3_PIN | DS4_PIN);
				GPIO_ResetBits(GPIO_LED, DS4_PIN << (count % 4)); /*点亮一个LED灯*/
			}
			else
			{
				GPIO_SetBits(GPIO_LED, DS1_PIN | DS2_PIN | DS3_PIN | DS4_PIN);
				GPIO_ResetBits(GPIO_LED, DS1_PIN >> (count % 4)); /*点亮一个LED灯*/
			}
			count++;
		}
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	}
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		data[size] = USART_ReceiveData(USART1) - '0';
		size++;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

int main(void)
{

	EXTI_InitTypeDef exti3;
	NVIC_InitTypeDef nvic3;
	TIM_TimeBaseInitTypeDef tim5;
	USART_InitTypeDef usart1;
	ADC_InitTypeDef adc1;
	DMA_InitTypeDef dma1;
	TIM_TimeBaseInitTypeDef tim2;
	DMA_InitTypeDef DMA_InitStructure;
	DAC_InitTypeDef dac1;
	int Idx = 0;
	int i = 0;
	int temp_j = j;

	/* 配置神舟王103LED灯使用的GPIO管脚模式*/
	RCC_APB2PeriphClockCmd(RCC_GPIO_LED | RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1, ENABLE); /*使能LED灯使用的GPIO时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 | RCC_APB1Periph_DAC | RCC_APB1Periph_TIM2, ENABLE);																	/*使能TIM5时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);																							/*使能DMA1时钟*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);

	ili9320_Initializtion();
	ili9320_Clear(0xF800);

	ili9320_PutChar(0, 10, '2', 0x0000, 0xffff);
	ili9320_PutChar(8, 10, '0', 0x0000, 0xffff);
	ili9320_PutChar(16, 10, '2', 0x0000, 0xffff);
	ili9320_PutChar(24, 10, '3', 0x0000, 0xffff);
	ili9320_PutChar(32, 10, '1', 0x0000, 0xffff);
	ili9320_PutChar(40, 10, '4', 0x0000, 0xffff);
	ili9320_PutChar(48, 10, '9', 0x0000, 0xffff);
	ili9320_PutChar(56, 10, '2', 0x0000, 0xffff);
	ili9320_PutChar(64, 10, '4', 0x0000, 0xffff);
	ili9320_PutChar(72, 10, '1', 0x0000, 0xffff);
	ili9320_PutChar(80, 10, '1', 0x0000, 0xffff);
	for (i = 0; i < 3; i++)
	{
		LCD_DrawChinaChar(40, 20 * i, WangChar[i], 0x0000, 0xffff);
	}
	for (i = 0; i < 3; i++)
	{
		LCD_DrawChinaChar(100, 20 * i, char_zheng_xian_bo[i], 0x0000, 0xffff);
	}
	LCD_DrawChinaChar(60, 0, char_tong, 0x0000, 0xffff);
	LCD_DrawChinaChar(60, 20, char_xin, 0x0000, 0xffff);
	ili9320_PutChar(44, 64, '3', 0x0000, 0xffff);
	LCD_DrawChinaChar(60, 50, char_ban, 0x0000, 0xffff);
	/*外部中断配置*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	exti3.EXTI_Line = EXTI_Line3;
	exti3.EXTI_Mode = EXTI_Mode_Interrupt;
	exti3.EXTI_Trigger = EXTI_Trigger_Falling;
	exti3.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti3);
	exti3.EXTI_Line = EXTI_Line1;
	EXTI_Init(&exti3);
	nvic3.NVIC_IRQChannel = EXTI3_IRQn;
	nvic3.NVIC_IRQChannelCmd = ENABLE;
	nvic3.NVIC_IRQChannelPreemptionPriority = 1;
	nvic3.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic3);
	nvic3.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_Init(&nvic3);
	nvic3.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Init(&nvic3);
	nvic3.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Init(&nvic3);

	/*神舟王103使用的LED灯相关的GPIO口初始化*/
	GPIO_InitStructure.GPIO_Pin = DS1_PIN | DS2_PIN | DS3_PIN | DS4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_LED, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIO_LED, DS1_PIN | DS2_PIN | DS3_PIN | DS4_PIN); /*关闭所有的LED指示灯*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 输入模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // ADC通道5对应的GPIO口初始化
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*系统时钟配置函数*/
	TIM_TimeBaseStructInit(&tim5);				/*TIM5时间基数配置结构体初始化*/
	tim5.TIM_ClockDivision = TIM_CKD_DIV1;		/*时钟分频*/
	tim5.TIM_CounterMode = TIM_CounterMode_Up;	/*向上计数模式*/
	tim5.TIM_Period = 10000 - 1;				/*自动重装载寄存器周期值*/
	tim5.TIM_Prescaler = 7200 - 1;				/*预分频值*/
	TIM_TimeBaseInit(TIM5, &tim5);				/*根据TIM_TimeBaseInitTypeDef中指定的参数初始化TIM5*/
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update); /*清除TIM5更新标志位*/
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);	/*使能TIM5更新中断*/
	TIM_Cmd(TIM5, ENABLE);						/*使能TIM5*/
	TIM_TimeBaseStructInit(&tim2);
	tim2.TIM_Period = 20;
	tim2.TIM_Prescaler = 720-1;
	tim2.TIM_ClockDivision = 0x0;
	tim2.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &tim2);
	/* TIM2 TRGO selection */
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	dac1.DAC_Trigger = DAC_Trigger_T2_TRGO;
	dac1.DAC_WaveGeneration = DAC_WaveGeneration_None;
	dac1.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_2, &dac1);
	/* Fill Sine32bit table */
	for (Idx = 0; Idx < 32; Idx++)
	{
		DualSine12bit[Idx] = (Sine12bit[Idx] << 16) + (Sine12bit[Idx]);
	}
	USART_StructInit(&usart1);
	usart1.USART_BaudRate = 9600;
	usart1.USART_WordLength = USART_WordLength_8b;
	usart1.USART_StopBits = USART_StopBits_1;
	usart1.USART_Parity = USART_Parity_No;
	usart1.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &usart1);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	// TIM_SetAutoReload(TIM5, 10000 - 1);
	DMA_DeInit(DMA1_Channel1);
	dma1.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	dma1.DMA_MemoryBaseAddr = (uint32_t)adc_value;
	dma1.DMA_DIR = DMA_DIR_PeripheralSRC;
	dma1.DMA_BufferSize = 1;
	dma1.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma1.DMA_MemoryInc = DMA_MemoryInc_Disable;
	dma1.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	dma1.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	dma1.DMA_Mode = DMA_Mode_Circular;
	dma1.DMA_Priority = DMA_Priority_Low;
	dma1.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &dma1);
	DMA_Cmd(DMA1_Channel1, ENABLE);

	DMA_DeInit(DMA2_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40007414;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&DualSine12bit;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 32;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel4, &DMA_InitStructure);
	/* Enable DMA2 Channel4 */
	DMA_Cmd(DMA2_Channel4, ENABLE);
	/* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
	automatically connected to the DAC converter. */
	DAC_Cmd(DAC_Channel_2, ENABLE);
	/* Enable DMA for DAC Channel2 */
	DAC_DMACmd(DAC_Channel_2, ENABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);

	adc1.ADC_Mode = ADC_Mode_Independent;
	adc1.ADC_ScanConvMode = DISABLE;
	adc1.ADC_ContinuousConvMode = ENABLE;
	adc1.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	adc1.ADC_DataAlign = ADC_DataAlign_Right;
	adc1.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &adc1);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_55Cycles5);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1))
		;
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1))
		;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	/* DMA_DeInit(DMA2_Channel4);
	DMA_InitTypeDef dma2;
	dma2.DMA_PeripheralBaseAddr =0x4001004C;
	dma2.DMA_MemoryBaseAddr = (uint32_t)data;
	dma2.DMA_DIR = DMA_DIR_PeripheralSRC;
	dma2.DMA_BufferSize = 100;
	dma2.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma2.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma2.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	dma2.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	dma2.DMA_Mode = DMA_Mode_Circular;
	dma2.DMA_Priority = DMA_Priority_Low;
	dma2.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel4, &dma2);
	DMA_Cmd(DMA2_Channel4, ENABLE);
	DAC_Cmd(DAC_Channel_1, ENABLE); */

	while (1)
	{
		if (temp_j == j)
		{
		}
		else
		{
			if (j == 1)
			{
				for (Idx = 0; Idx < 32; Idx++)
				{
					DualSine12bit[Idx] = (Sine12bit[Idx] << 16) + (Sine12bit[Idx]);
					temp_j = j;
				}
			}
			else if (j == 0)
			{
				for (Idx = 0; Idx < 32; Idx++)
				{
					DualSine12bit[Idx] = (Square12bit[Idx] << 16) + (Square12bit[Idx]);
					temp_j = j;
				}
			}else if(j==2){
				for (Idx = 0; Idx < 32; Idx++)
				{
					DualSine12bit[Idx] = (Triangle12bit[Idx] << 16) + (Triangle12bit[Idx]);
					temp_j = j;
				}
			}
		};
		/* ili9320_PutChar(0, 0, '0'+(uint8_t)((adc_value[0]/100000)%10), 0x0000, 0xffff);
		ili9320_PutChar(8, 0, '0'+(uint8_t)((adc_value[0]/10000)%10), 0x0000, 0xffff);
		ili9320_PutChar(16, 0, '0'+(uint8_t)((adc_value[0]/1000)%10), 0x0000, 0xffff);
		ili9320_PutChar(24, 0, '0'+(uint8_t)((adc_value[0]/100)%10), 0x0000, 0xffff);
		ili9320_PutChar(32, 0, '0'+(uint8_t)((adc_value[0]/10)%10), 0x0000, 0xffff);
		ili9320_PutChar(40, 0, '0'+(uint8_t)(adc_value[0]%10), 0x0000, 0xffff);

		USART_SendData(USART1, '0'+(uint8_t)((adc_value[0]/10000)%10));
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, '0'+(uint8_t)((adc_value[0]/1000)%10));
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, '0'+(uint8_t)((adc_value[0]/100)%10));
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, '0'+(uint8_t)((adc_value[0]/10)%10));
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1,'0'+(uint8_t)(adc_value[0]%10));
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); */
	}
}

/*******************************************************************************
 * Function Name  : Delay
 * Description    : Inserts a delay time.
 * Input          : nCount: specifies the delay time length.
 * Output         : None
 * Return         : None
 *******************************************************************************/
/* void Delay(vu32 nCount)
{
	for (; nCount != 0; nCount--)
		;
} */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
