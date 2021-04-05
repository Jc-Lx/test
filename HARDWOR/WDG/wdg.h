#ifndef __WDG_h
#define __WDG_H
#include "sys.h"



void IWDG_Init(u8 prer,u16 rlr);//tout=((4x2^prer)xrlr)/40
void IWDG_Feed(void);

void WWDG_Init(u8 tr,u8 wr,u32 fprer);//twwdg=(4096x2^WDGTBx(T[5:0]+1))/Fpclk;
void WWDG_Set_Counter(u8 cnt);
void WWDG_NVIC_Init(void);

#endif
