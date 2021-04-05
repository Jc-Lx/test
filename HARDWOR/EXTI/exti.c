#include  "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"

void INTERRUPUT_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    KEY_Init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    // GPIOE.3
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
    EXTI_InitStructure.EXTI_Line=EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);
    // GPIOE.4
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line=EXTI_Line4;
    // EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    // EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);
    // GPIOA.0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line=EXTI_Line0;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
    EXTI_Init(&EXTI_InitStructure);
    // NVIC KEY1
    NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0X02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0X01;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // NVIC KEY0
    NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0X02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0X00;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // NVIC WK_UP
    NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0X02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0X03;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);


}
void EXTI0_IRQHandler(void)
{
    delay_ms(10);
    if (WK_UP==1)
    {
        BEEP=!BEEP;
    }
    EXTI_ClearITPendingBit(EXTI_Line0);

}
void EXTI3_IRQHandler(void)
{
    delay_ms(10);
    if (KEY1==0)
    {
        LED0=!LED0;
    }
    EXTI_ClearITPendingBit(EXTI_Line3);
    
}
void EXTI4_IRQHandler(void)
{
    delay_ms(10);
    if (KEY0==0)
    {
        LED0=!LED0;
        LED1=!LED1;

    }
    EXTI_ClearITPendingBit(EXTI_Line4);  
}
