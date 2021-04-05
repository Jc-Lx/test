 #include  "timer.h"
 #include  "LED.h"
 #include "usart.h"
// arr  auto reload value
// psc  clock presplitting frequency
// Tclk tim3 input clock frequency
// Tout=((arr+1)*(psc+1))/Tclk

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

    //initialize tim3
    TIM_TimeBaseStructure.TIM_Period=arr;//settitng arr
    TIM_TimeBaseStructure.TIM_Prescaler=psc;//setting psc
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//set clock division
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//open interruput tim3

    //initialize nvic
    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;    
    NVIC_Init(&NVIC_InitStructure); 
     
    TIM_Cmd(TIM3,ENABLE);
}
//interruput tim3
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
    {
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
        LED1=!LED1;
    }
}
// tim3 pwm 
void TIM3_PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//set gpiob afio clock

    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//tim3_ch2->pb5
    // set gpiob.5 output tim3_ch2 pwm
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    // initialize tim3
    TIM_TimeBaseStructure.TIM_Period=arr;
    TIM_TimeBaseStructure.TIM_Prescaler=psc;
    TIM_TimeBaseStructure.TIM_ClockDivision=0;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
    // initialize tim3 channel2 pwm
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2; 
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OC2Init(TIM3,&TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
    TIM_Cmd(TIM3,ENABLE);
}
// tim5 cap
void TIM5_Cap_Init(u16 arr,u16 psc)
{
    TIM_ICInitTypeDef TIM5_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
    // initialize tim5
    TIM_TimeBaseStructure.TIM_Period=arr;
    TIM_TimeBaseStructure.TIM_Prescaler=psc;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
    // initialize tim5 cap
    TIM5_ICInitStructure.TIM_Channel=TIM_Channel_1;
    TIM5_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
    TIM5_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
    TIM5_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
    TIM5_ICInitStructure.TIM_ICFilter=0x00;
    TIM_ICInit(TIM5,&TIM5_ICInitStructure);
    //initialize tim5 nvic
    NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);
    TIM_Cmd(TIM5,ENABLE);
}
u8  TIM5CH1_CAPTURE_STA=0;
u16	TIM5CH1_CAPTURE_VAL;
// tim5 interruput handler
void TIM5_IRQHandler(void)
{
    if ((TIM5CH1_CAPTURE_STA&0x80 )!=1)
    {
        if (TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET)
        {
            if (TIM5CH1_CAPTURE_STA&0x40)
            {
                if ((TIM5CH1_CAPTURE_STA&0x3f)==0x3f)
                {
                    TIM5CH1_CAPTURE_STA|=0x80;
                    TIM5CH1_CAPTURE_VAL=0xffff;
                }else
                {
                    TIM5CH1_CAPTURE_STA++;
                }     
            } 
        }
    
        if (TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET)
        {
            if (TIM5CH1_CAPTURE_STA&0x40)
            {
                TIM5CH1_CAPTURE_STA|=0x80;
                TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
                TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);
            }
            else
            {
                TIM5CH1_CAPTURE_STA=0;
                TIM5CH1_CAPTURE_VAL=0;
                TIM_SetCounter(TIM5,0);
                TIM5CH1_CAPTURE_STA|=0x40;
                TIM_OC1PreloadConfig(TIM5,TIM_ICPolarity_Falling);
            }  
        }
    }
 TIM_ClearITPendingBit(TIM5,TIM_IT_CC1|TIM_IT_Update);
    
}
