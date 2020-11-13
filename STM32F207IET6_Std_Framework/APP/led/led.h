#ifndef _led_H
#define _led_H

#include "system.h"

/*  LED时钟端口、引脚定义 */
#define LED1_PORT 			GPIOF   
#define LED1_PIN 			GPIO_Pin_9
#define LED1_PORT_RCC		RCC_AHB1Periph_GPIOF

#define LED2_PORT 			GPIOF   
#define LED2_PIN 			GPIO_Pin_10
#define LED2_PORT_RCC		RCC_AHB1Periph_GPIOF


#define LED1 PFout(9)  	
#define LED2 PFout(10) 

void LED_Init(void);


#endif
