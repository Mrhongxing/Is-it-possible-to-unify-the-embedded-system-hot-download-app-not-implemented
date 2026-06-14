#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"

#define SCL_PORT GPIOB
#define SDA_PORT GPIOB
#define SCL_PIN(x) GPIO_WriteBit(SCL_PORT, GPIO_Pin_10, (BitAction)(x))
#define SDA_PIN(x) GPIO_WriteBit(SDA_PORT, GPIO_Pin_11, (BitAction)(x))

void SendMessage(uint8_t id, uint8_t position, uint8_t *message,int size)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
		;
	I2C_Send7bitAddress(I2C2, id, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C2, position);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
		;
	while (size--)
	{
		I2C_SendData(I2C2, *message++);
		if (size)
		{
			while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
				;
		}
		else
		{
			while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
				;
		}
	}
	I2C_GenerateSTOP(I2C2, ENABLE);
}

void ReceiveMessage(uint8_t id, uint8_t position, uint8_t *message, uint8_t size)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
		;
	I2C_Send7bitAddress(I2C2, id, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		;
	I2C_SendData(I2C2, position);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;
	I2C_GenerateSTART(I2C2, ENABLE);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
		;
	I2C_Send7bitAddress(I2C2, id, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		;
	if (size == 1)
	{
		I2C_AcknowledgeConfig(I2C2, DISABLE);
		I2C_GenerateSTOP(I2C2, ENABLE);
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
			;
		message[0] = I2C_ReceiveData(I2C2);
		I2C_AcknowledgeConfig(I2C2, ENABLE);
	}
	else if (size > 1)
	{
		while (--size)
		{
			while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
				;
			*message++ = I2C_ReceiveData(I2C2);
		}
		I2C_AcknowledgeConfig(I2C2, DISABLE);
		I2C_GenerateSTOP(I2C2, ENABLE);
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
			;
		*message = I2C_ReceiveData(I2C2);
		I2C_AcknowledgeConfig(I2C2, ENABLE);
	}
}

void Test()
{
	uint8_t id = 0xD0;
	I2C_GenerateSTART(I2C2, ENABLE);
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
	{
		OLED_ShowString(2, 1, "Start Fail");
	};
	I2C_Send7bitAddress(I2C2, id, I2C_Direction_Transmitter);
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)
	{
		OLED_ShowString(2, 1, "Address Fail");
	}
	I2C_GenerateSTOP(I2C2, ENABLE);
	OLED_ShowString(2, 1, "               ");
}

char *itoa(uint16_t x)
{
	static char str[9];
	int16_t a = (int16_t)x;
	int b= (int)a;
	float temp = 36.53f + (b / 340.0f);
	int tempInt = (int)(temp * 100);
	str[0] = ((tempInt % 10000) / 1000) + '0';
	str[1] = ((tempInt % 1000) / 100) + '0';
	str[2]='.';
	str[3] = ((tempInt % 100) / 10) + '0';
	str[4] = (tempInt % 10) + '0';
	str[5]='\'';
	str[6]='C';
	str[7] = '\0';
	return str;
}

int main()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	uint8_t message[20];
	uint8_t id = 0xD0;
	SPI_InitTypeDef SPI_InitStruct;

	// 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	// 初始化GPIO
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);

	// 初始化OLED
	OLED_Init();
	OLED_ShowString(1, 1, "GPIOOK");

	// 初始化I2C
	I2C_DeInit(I2C2);
	I2C_StructInit(&I2C_InitStruct);
	I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C2, &I2C_InitStruct);
	I2C_Cmd(I2C2, ENABLE);

	// 测试I2C通信
	Test();
	message[0] = 0x18;
	SendMessage(id, 0x1B, &message[0],1);
	message[0] = 0x00;
	SendMessage(id, 0x1C, &message[0],1);
	message[0] = 0x06;
	SendMessage(id, 0x1A, &message[0],1);
	message[0] = 0x00;
	SendMessage(id, 0x6B, &message[0],1);
	OLED_ShowString(2, 1, "                ");

	// 初始化SPI
	SPI_I2S_DeInit(SPI1);
	SPI_StructInit(&SPI_InitStruct); 
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;//时钟空闲状态为低电平
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//数据在第一个时钟边沿被捕获
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;//软件管理NSS信号
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;//CRC校验多项式
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
	while (1)
	{
		ReceiveMessage(id, 0x41, message, 2);
		uint16_t x = (message[0] << 8) | message[1];
		
		OLED_ShowString(2, 1, "X:");
		OLED_ShowString(2, 3, itoa(x));
		
	}
}
