#include "delay.h"
#include  "sys.h"
#include "led.h"
#include "beep.h"
#include "usart.h"
#include "timer.h"
#include "key.h"


int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	LED_Init();
	TIM3_Int_Init(4999,7199);
	
	while (1)
	{
		LED0=!LED0;
		delay_ms(200);
	}
	

}

