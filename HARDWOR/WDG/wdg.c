#include "wdg.h"
#include "LED.h"
u8 WWDG_CNT=0x7f;

void IWDG_Init(u8 prer,u16 rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(prer);
    IWDG_SetReload(rlr);
    IWDG_ReloadCounter();
    IWDG_Enable();
}
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();
}

void WWDG_Init(u8 tr,u8 wr,u32 fprer)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
    WWDG_CNT=tr&WWDG_CNT;
    WWDG_SetPrescaler(fprer);//SETING PRECALE
    WWDG_SetWindowValue(wr);//SETING WINDOWS
    WWDG_Enable(WWDG_CNT);//STAR SETTING COUNTER
    WWDG_ClearFlag();//CLEAR INTERUPUT
    WWDG_NVIC_Init();
    WWDG_EnableIT();
}
// RESETING WWDG COUNTER
void WWDG_Set_Counter(u8 cnt)
{
    WWDG_Enable(cnt);
}
void WWDG_NVIC_Init(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void WWDG_IRQHandler(void)
{
    WWDG_SetCounter(WWDG_CNT);
    WWDG_ClearFlag();
    LED1=!LED1;
}
