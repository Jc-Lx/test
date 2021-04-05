#include "sys.h"
#include "usart.h"	  


#if 1
#pragma import(__use_no_semihosting)
//????????????
struct __FILE
{
    int handle;
};
FILE __stdout;
//??_sys_exit()??????????
void _sys_exit(int x)
{
    x=x;
}
//???fputc??
int fputc(int ch,FILE *f)
{
    while ((USART1->SR&0X40)==0);//????,??????
    USART1->DR=(u8)ch;
    return ch;

}
#endif

#if EN_USART1_RX//?????????1??????,??,??usart1——sr??????????,???????
u8 USART_RX_BUF[USART_REC_LEN];//???????????????????????
u16 USART_RX_STA=0;//????????

void uart_init(u32 bound)
{
    //GPIO????
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
    //USART1_TX GPIOA.9???
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//??????	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //USART1_RX	  GPIOA.10³õÊ¼»¯
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
    GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIO.10
    //??1?????NVIC??
    NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//IRQn????
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//?????3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;//????3
    NVIC_Init(&NVIC_InitStructure);
    //???????
    USART_InitStructure.USART_BaudRate=bound;
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//?????????
    USART_InitStructure.USART_Parity=USART_Parity_No;//?????
    USART_InitStructure.USART_StopBits=USART_StopBits_1;//?????
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;//?????8?????

    USART_Init(USART1,&USART_InitStructure);//?????1
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//????????
    USART_Cmd(USART1,ENABLE);//????1

}

void USART1_IRQHandler(void)                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
	{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //½ÓÊÕÖÐ¶Ï(½ÓÊÕµ½µÄÊý¾Ý±ØÐëÊÇ0x0d 0x0a½áÎ²)
		{
		Res =USART_ReceiveData(USART1);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
		
		if((USART_RX_STA&0x8000)==0)//½ÓÊÕÎ´Íê³É
			{
			if(USART_RX_STA&0x4000)//½ÓÊÕµ½ÁË0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//½ÓÊÕ´íÎó,ÖØÐÂ¿ªÊ¼
				else USART_RX_STA|=0x8000;	//½ÓÊÕÍê³ÉÁË 
				}
			else //»¹Ã»ÊÕµ½0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//½ÓÊÕÊý¾Ý´íÎó,ÖØÐÂ¿ªÊ¼½ÓÊÕ	  
					}		 
				}
			}   		 
     } 

} 
#endif	

