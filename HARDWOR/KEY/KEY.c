#include "key.h"
#include "sys.h"
#include "delay.h"
//按键初始化
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//设置成上拉输入
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4;
    
    GPIO_Init(GPIOE,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//设置成上拉输入
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//初始化WK_UP-->GPIOA.0

    GPIO_Init(GPIOA,&GPIO_InitStructure);
}
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;//按键松开标志位
    if (mode){key_up=1;}//支持连续按键
    if (key_up && (KEY1==0 || KEY0==0 || WK_UP==1) ) 
    {
        delay_ms(10);
        key_up=0;
        if (KEY0==0) {return KEY0_PRES;}
      
        else if (KEY1==0) {return KEY1_PRES;}
        else if (WK_UP==1) {return WKUP_PRES;}
       
    }else if (KEY0==1&&KEY1==1&&WK_UP==0)key_up=1;
    return 0;
}
