#include "stm32f10x.h"
/**-------------------------------------------------------
  * 短按一次KEY1 返回值1
  * 短按一次KEY2 返回值2
  * 短按一次KEY3 返回值3
  * 短按一次KEY4 返回值4
  * 没有按键按下 返回值0
***------------------------------------------------------*/
uint32_t KEYScan(void)
{
    if(0 == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3))
    {
         /* 去抖动 */
        //delay(250000);
        if(0 == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3))
        {
            return 1;//KEY1
        }
    }

    if(0 == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
    {
        //delay(250000);
        if(0 == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
        {
            return 2;//KEY2
        }
    }

    if(0 == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
    {
        //delay(250000);
        if(0 == GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
        {
            return 3;//KEY3
        }
    }

    if(0 == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {
        //delay(250000);
        if(0 == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        {
            return 4;//KEY4
        }
    }
    
    return 0;
}
