#include "delay.h"
#include  "sys.h"
#include "led.h"
#include "beep.h"
#include "usart.h"
#include "timer.h"
#include "key.h"
#include "usmart.h"
#include "rtc.h"

int main(void)
{
	u8 t=0;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init();
	LED_Init();
	BEEP_Init();
	BEEP=1;
	delay_ms(1000);
	BEEP=0;
	LED0=0;
	usmart_dev.init(SystemCoreClock/1000000);
	RTC_Init();
}
	
