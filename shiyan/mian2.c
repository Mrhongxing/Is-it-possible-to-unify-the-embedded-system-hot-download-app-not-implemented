/******************** (C) COPYRIGHT 2013 www.armjishu.com  ********************
 * 文件名  ：main.c
 * 描述    ：实现STM32F103ZE神舟王开发板的TIM定时器功能实验
 * 实验平台：STM32神舟开发板
 * 标准库  ：STM32F10x_StdPeriph_Driver V3.5.0
 * 作者    ：www.armjishu.com 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "ili9320.h"
#include "ili9320_api.h"
#include <stdio.h>
#include <string.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SysTick_Handler_User(void);

extern u16 	TemperatureThreshold;
extern u8 	UsartReceive;
extern void LCD_DisplayWelcomeStr(u8 Line);
#define Baudrate 			115200
#define RxBufferSize1 		64			//接收缓冲区大小
uint8_t RxBuffer1[RxBufferSize1];//接收缓冲区
__IO uint8_t RxCounter1 = 	0x00; 	//接收字节计数变量
u16 StartXVal;
u8 str[] = " Welcome to 1234567890  ";
u8 len = sizeof(str)-1;

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
  
/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
void Delay(vu32 nCount)
{
	for(; nCount != 0; nCount--);
}  
*******************************************************************************/

void Usart1_Init(uint32_t Brate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/*使能串口1和串口2使用的GPIO时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*使能串口1时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	
	/*串口1 RX管脚配置*/
	/* Configure USART1 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*串口1 TX管脚配置*/ 
	/* Configure USART1 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* 使能串口1的中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	 USART_InitStructure.USART_BaudRate = Brate;              //串口的波特率，例如115200 最高达4.5Mbits/s
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据字长度(8位或9位)
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //可配置的停止位-支持1或2个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;         //无奇偶校验  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //双工模式，使能发送和接收
  
    USART_Init(USART1, &USART_InitStructure);  // 调用STM32的USART初始化底层函数
  
  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能接收中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//使能空闲中断
    /* 使能STM32的USART功能模块 */
    USART_Cmd(USART1, ENABLE);
	
    SystemCoreClockUpdate();
	printf("\n\r");
	printf("\n\r 实验五 USART 实验例程 \n\r");
    printf("\n\r 系统内核时钟频率(SystemCoreClock)为：%dHz \n\r",SystemCoreClock);
	printf("\n\r USART1设置完毕，波特率：%d \n\r",Brate);
	printf("\n\r 数据位 8bits \n\r");
	printf("\n\r 停止位 1bit  \n\r");
	printf("\n\r 检验位 None  \n\r");
	printf("\n\r 流控制 None  \n\r");
	printf("\n\r");
	
}

/**-------------------------------------------------------
  * @函数名 main
  * @功能   主函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
int main(void)
{
    /*!< 在系统启动文件(startup_stm32f10x_xx.s)中已经调用SystemInit()初始化了时钟, 
         所以main函数不需要再次重复初始化时钟。默认初始化系统主时钟为72MHz。
         SystemInit()函数的实现位于system_stm32f10x.c文件中。*/ 
	u8 i;

	/* 初始化USART1 波特率=115200 */
	Usart1_Init(Baudrate);
	
	ili9320_Initializtion();
	
    /* Infinite loop 主循环 */
    while (1)
    {
// 		
		  
	  }
// 		ili9320_Clear(Green);
    
}

