#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include "Cmd.h"

typedef uint32_t (*func_t)(uint32_t, uint32_t);
typedef void (*void_func_t)(void);

uint32_t apps[50][3] = {{1, 0x0800F000, 0}};
uint8_t usart2_rx_buffer[100];
uint8_t usart2_rx_buffer_index = 0;
uint8_t app_now = 0;
uint8_t app_bake[3];
uint8_t app_temp_now = 0;
uint16_t tryint[16] = {
    0xB510,  // PUSH {R4, LR}      (原 0x10B5)
      // POP {R4, PC}       (原 0x10BD)  ← 但这个位置不对，见下文
    0xA203,  // ADR R2, #0x0C      (原 0x03A2)
    0x2101,  // MOVS R1, #0x01     (原 0x0121)
    0x2003,  // MOVS R0, #0x03     (原 0x0320)
    0xF7F1,  // BL 高位            (原 0xFFF8)
    0xFB6C,  // BL 低位            (原 0xFDB6)
    0xBD10,  // POP {R4, PC}       (原 0x10BD)
    0x0000,
    0x6548,  // "He"               (原 0x4865)
    0x6C6C,  // "ll"
    0x2C6F,  // "o,"               (原 0x6F2C)
    0x5320,  // " S"               (原 0x2053)
    0x4D54,  // "TM"               (原 0x544D)
    0x3233,  // "32"               (原 0x3332)
    0x0021,  // "!\0"              (原 0x2100)
    0x0000
};

void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		uint8_t data = USART_ReceiveData(USART2);
		if(data!=0){
			usart2_rx_buffer[usart2_rx_buffer_index] = data;
			usart2_rx_buffer_index++;
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

__attribute__((used)) uint32_t jump(uint32_t addr, uint32_t arg0, uint32_t arg1)
{
	addr |= 1;
	func_t f = (func_t)addr;
	return f(arg0, arg1);
}

__attribute__((used)) void void_jump(uint32_t addr)
{
	addr |= 1;
	void_func_t f = (void_func_t)addr;
	f();
}

__attribute__((used)) void try()
{
	OLED_ShowString(3, 1, "Hello, STM32!");
}

int main(void)
{
	uint32_t i = 0;
	uint8_t j = 0;
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// 配置 NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	// GPIO配置
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// 初始化 OLED 显示屏
	OLED_Init();

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// USART 配置
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	// 配置 Flash 延迟和预取缓冲
	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_PrefetchBufferCmd(ENABLE);
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(0x0800F000);
    /* for(i = 0; i < 16; i++)
	{
		FLASH_ProgramHalfWord(0x0800F000 + i*2, tryint[i]);
	} */
	FLASH_ProgramWord(0x0800F000, (uint32_t)&jump);
	FLASH_Lock();

	// 显示欢迎信息
	OLED_ShowString(1, 1, "Hello, STM32!");

	while (1)
	{
		j=read_cmd();
		if (app_temp_now != app_now)
		{
			for (int i = 0; i < 50; i++)
			{
				if (apps[i][0] != 0&&apps[i][0] == app_now)
				{
					void_jump(apps[i][1]);
					app_temp_now = app_now;
					break;
				}
			}
		}else
		{
			OLED_ShowString(3, 1, "No app");
			Delay_ms(1000);
			//void_jump(apps[0][1]);
		}
	}
}
