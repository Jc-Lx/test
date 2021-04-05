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
	TPAD_Init(6);

	while (1)
	{
		if (TPAD_Scan(0))
		{
			LED1=!LED1;
		}
		t++;
		if (t==15)
		{
			t=0;LED0=!LED0;
		}
		delay_ms(10);
		
	}
}

