#include "delay.h"
#include  "sys.h"
#include "led.h"
#include "beep.h"
#include "usart.h"
#include "timer.h"
#include "key.h"


int main(void)
{	
	u16 led0pwmval=0;
	u8 dir=1;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	LED_Init();
	TIM3_PWM_Init(899,0);//arr 899,psc 0
	
	while (1)
	{
		delay_ms(10);
		if(dir)led0pwmval++;
		else led0pwmval--;
		if(led0pwmval>300)dir=0;
		if(led0pwmval==0)dir=1;
		TIM_SetCompare2(TIM3,led0pwmval);
		printf("working\r\n");
		// delay_ms(200);
	}
}

