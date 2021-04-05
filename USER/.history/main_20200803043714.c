#include "delay.h"
#include  "sys.h"
#include "led.h"
#include "beep.h"
#include "usart.h"
#include "timer.h"
#include "key.h"
#include "tpad.h"
	
int main(void)
{	
	u8 t=0;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	LED_Init();
	TPAD_Init();
	
	while (1)
	{
		delay_ms(10);
		if (TIM5CH1_CAPTURE_STA&0x80)
		{
			temp=TIM5CH1_CAPTURE_STA&0x3f;
			temp*=65536;
			temp+=TIM5CH1_CAPTURE_VAL;
			printf("HIGH: %d us\r\n",temp);
			TIM5CH1_CAPTURE_STA=0;
		}
		
	}
}

