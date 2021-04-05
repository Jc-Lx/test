#include "delay.h"
#include  "sys.h"
#include "led.h"
#include "beep.h"
#include "usart.h"
#include "timer.h"
#include "key.h"

extern u8  TIM5CH1_CAPTURE_STA;	
extern u16	TIM5CH1_CAPTURE_VAL;	
int main(void)
{	
	u32 temp=0; 
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	LED_Init();
	TIM3_PWM_Init(899,0);//arr 899,psc 0
	TIM5_Cap_Init(0xffff,71);
	
	while (1)
	{
		delay_ms(10);
		if (TIM5CH1_CAPTURE_STA&0x80)
		{
			temp=TIM5CH1_CAPTURE_STA&0x3f;
			temp*=65536;
			temp+=TIM5CH1_CAPTURE_val;
			printf("HIGH: %d us\r\n",temp);
			TIM5CH1_CAPTURE_STA=0;
		}
		
	}
}

