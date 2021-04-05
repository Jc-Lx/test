#include "led.h"
#include "beep.h"
#include "rtc.h" 
#include "delay.h"
#include "sys.h"
#include "usart.h" 
#include "usmart.h" 
//主函数//
 int main(void)
 {  
 u8 t=0,temp2; 
 delay_init();                                      //延时函数初始化//   
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //设置中断优先级分组为组2：2位抢占优先级，2位响应优先级//
 uart_init(115200);                                 //串口初始化为115200//
 LED_Init();                                        //LED端口初始化//
 BEEP_Init();
 BEEP=1;
 delay_ms(1000);
 BEEP=0;
 LED0=0;                                            //LED0亮//
 usmart_dev.init(SystemCoreClock/1000000);          //初始化USMART// 
 RTC_Init();                                        //RTC初始化//    
 while(1)
 { 
  temp2=           
  printf("%d\r\n",);
  if(t!=calendar.sec)
  {
   t=calendar.sec;                                  //如果calendar.sec不更新，则t不更新//
   LED0=!LED0;                                      //每次calendar.sec更新，LED0和LED1交替闪烁//
   LED1=!LED1;
  } 
  delay_ms(10);          
 }  
 }


 
