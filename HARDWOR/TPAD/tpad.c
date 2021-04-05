#include "tpad.h"
#include "delay.h"
#include "usart.h"

#define TPAD_ARR_MAX_VAL 0xFFFF
vu16 tpad_default_val=0;

void TPAD_Rest(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    // gpioa.1 ->pp
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_1);//discharge

    delay_ms(5);

    TIM_SetCounter(TIM5,0);
    TIM_ClearITPendingBit(TIM5,TIM_IT_CC2|TIM_IT_Update);

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA,&GPIO_InitStructure);//charge
}
u16 TPAD_Get_Val(void)
{
    TPAD_Rest();
    while (TIM_GetFlagStatus(TIM5,TIM_IT_CC2)==RESET)
    {
        if(TIM_GetCounter(TIM5)>TPAD_ARR_MAX_VAL-500) return TIM_GetCounter(TIM5);
    }
    return TIM_GetCapture2(TIM5);
}
u16 TPAD_Get_MaxVal(u8 n)
{
    u16 temp=0,res=0;
    while (n--)
    {
        temp=TPAD_Get_Val();
        if(temp>res)res=temp;
    }
    return res;
    
}
u8 TPAD_Init(u8 psc)
{
    u16 buf[10];
    u16 temp;
    u8 j,i;
    TIM5_CH2_Cap_Init(TPAD_ARR_MAX_VAL,psc-1);
    for ( i = 0; i < 10; i++)
    {
        buf[i]=TPAD_Get_Val();
        delay_ms(10);
    }
    for ( i = 0; i < 9; i++)
    {
        for ( j = i+1; j < 10; j++)
        {
            if (buf[i]>buf[j])
            {
                temp=buf[i];
                buf[i]=buf[j];
                buf[j]=temp;
            }
            
        }
        
    }
    temp=0;
    for ( i = 2; i < 8; i++) temp+=buf[i];
    tpad_default_val=temp/6;
    printf("tpad_default_val:%d\r\n",tpad_default_val);
    if (tpad_default_val>TPAD_ARR_MAX_VAL/2) return 1;
    return 0;  
}
#define TPAD_GATE_VAL 	100	
u8 TPAD_Scan(u8 mode)
{
    static u8 keyen=0;
    u8 res=0,sample=3;
    u16 rval;
    if (mode)
    {
        sample=6;
        keyen=0;
    }
    rval=TPAD_Get_MaxVal(sample);
    if (rval>(tpad_default_val+TPAD_GATE_VAL))
    {
        if (keyen==0) 
        {
        res=1;
        printf("r:%d\r\n",rval);
        }
        printf("keyen:%d\r\n",keyen);
        keyen=3; 
        printf("keyen:%d\r\n",keyen);   
    }
    if (keyen)
    {
        keyen--;
    }
    return res;   
}
void TIM5_CH2_Cap_Init(u16 arr,u16 psc)
{
    TIM_ICInitTypeDef TIM5_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    //GPIOA.1 IN_FLOATING
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    // initialize tim5
    TIM_TimeBaseStructure.TIM_Period=arr;
    TIM_TimeBaseStructure.TIM_Prescaler=psc;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
    // initialize tim5 cap
    TIM5_ICInitStructure.TIM_Channel=TIM_Channel_2;
    TIM5_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
    TIM5_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
    TIM5_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
    TIM5_ICInitStructure.TIM_ICFilter=0x03;
    TIM_ICInit(TIM5,&TIM5_ICInitStructure);
    
    TIM_Cmd(TIM5,ENABLE);
}
