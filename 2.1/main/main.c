#include "stm32f10x.h"
#include "Delay.h"

int mainw() {
    int i = 0;
    int a = 0;
    
    // 直接使用数组存储引脚配置，而不是结构体数组
    struct {
        GPIO_TypeDef* port;
        uint16_t pin;
    } pins[] = {
        {GPIOA, GPIO_Pin_8},
        {GPIOA, GPIO_Pin_9},
        {GPIOA, GPIO_Pin_10},
        {GPIOB, GPIO_Pin_13},
        {GPIOB, GPIO_Pin_14},
        {GPIOB, GPIO_Pin_15}
    };
    int pinCount = sizeof(pins) / sizeof(pins[0]);
    
    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
    
    // 初始化 PC13
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    
    // 批量初始化所有引脚
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    for(i = 0; i < pinCount; i++) {
        GPIO_InitStruct.GPIO_Pin = pins[i].pin;
        GPIO_Init(pins[i].port, &GPIO_InitStruct);
        // 初始设为高电平（LED灭，假设低电平点亮）
        GPIO_SetBits(pins[i].port, pins[i].pin);
    }
    
    // 主循环 - 依次点亮LED
    while(1) {
        // 先关闭所有LED
        for(i = 0; i < pinCount; i++) {
            GPIO_SetBits(pins[i].port, pins[i].pin);  // 灭
        }
        
        // 点亮当前LED（假设低电平有效，改为 GPIO_ResetBits）
        GPIO_ResetBits(pins[a].port, pins[a].pin);  // 亮
        
        Delay_s(1);  // 延时1秒
        
        a++;
        if(a >= pinCount) a = 0;
    }
}
