/**
  ******************************************************************************
  * @file    httpd_cg_ssi.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   Webserver SSI and CGI handlers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/


#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "lwip_comm.h"
#include <string.h>
#include <stdlib.h>

#if LWIP_HTTP

tSSIHandler ADC_Page_SSI_Handler;
uint32_t ADC_not_configured=1;

/* we will use character "t" as tag for CGI */
char const* TAGCHAR="t";
char const** TAGS=&TAGCHAR;

/* CGI handler for LED control */ 
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/* Html request for "/leds.cgi" will start LEDS_CGI_Handler */
const tCGI LEDS_CGI=
{
	"/leds.cgi", LEDS_CGI_Handler
};

/* Cgi call table, only one CGI used */
tCGI CGI_TAB[1];



/**
  * @brief  ADC_Handler : SSI handler for ADC page 
  */
/* 通过 SSI 技术将这些值嵌入到网页中 */
/* 更多详细用法直接看原子的 LWIP 例程即可，写的设备比较多*/
u16_t ADC_Handler(int iIndex, char *pcInsert, int iInsertLen)
{
  /* We have only one SSI handler iIndex = 0 */
  if (iIndex ==0)
  {  
    char Digit1=0, Digit2=0, Digit3=0, Digit4=0; 
    uint32_t ADCVal = 0;        

     /* configure ADC if not yet configured */
     if (ADC_not_configured ==1)       
     {
//        ADCx_Init();
        ADC_not_configured=0;
     }
     
     /* get ADC conversion value */
//     ADCVal = Get_ADC_Value(ADC_CHANNEL_5,10);
     
     /* convert to Voltage,  step = 0.8 mV */
     ADCVal = (uint32_t)(ADCVal*3300/4096);  

     /* get digits to display */     
     Digit1= ADCVal/1000;
     Digit2= (ADCVal-(Digit1*1000))/100 ;
     Digit3= (ADCVal-((Digit1*1000)+(Digit2*100)))/10;
     Digit4= ADCVal -((Digit1*1000)+(Digit2*100)+ (Digit3*10));
        
     /* prepare data to be inserted in html */
     *pcInsert       = (char)(Digit1+0x30);
     *(pcInsert + 1) = (char)(Digit2+0x30);
     *(pcInsert + 2) = (char)(Digit3+0x30);
     *(pcInsert + 3) = (char)(Digit4+0x30);
    
    /* 4 characters need to be inserted in html*/
    return 4;
  }
  return 0;
}

/**
  * @brief  CGI handler for LEDs control 
  */
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  uint32_t i=0;
  
  /* We have only one SSI handler iIndex = 0 */
  if (iIndex==0)
  {
    
    /* Check cgi parameter : example GET /leds.cgi?led=2&led=4 */
    for (i=0; i<iNumParams; i++)	/*遍历每一个参数项*/
    {								/*比如 example GET /leds.cgi?led=2&led=4 中就有两个参数项*/
      /* check parameter "led" */
		if (strcmp(pcParam[i] , "led")==0)	/*如果参数名为 led*/
		{
			/* switch led1 ON if 1 */
			if(strcmp(pcValue[i], "1") ==0)		/*如果参数名为 led 后面 的参数为 "1" 的话*/
			{
//			  			LED1=0;					/*执行相应的动作*/
			}
			/* switch led2 ON if 2 */
			else if(strcmp(pcValue[i], "LED2ON") ==0) /*如果参数名为 led 后面 的参数为 "LED2ON" 的话*/
			{
//			  			LED2=0;        
			}
			/* switch led OFF if 3 */
			else if(strcmp(pcValue[i], "LED2Off") ==0) 
			{
//			  			LED1=1;
//			  			LED2=1;        
			}
		}
	  
		if (strcmp(pcParam[i] , "beep")==0)	/*如果参数名为 beep*/
		{
			if(strcmp(pcValue[i], "bibiOnce") ==0)	/*同理*/
			{
				/*balabala...*/
				
			}else if(strcmp(pcValue[i], "bibi") ==0)
			{
				
			}
		}
    }
  }
  
  /* 成功执行动作后，页面也相应的改变成对应状态的
	if(LED1 == 0 && BEEP == 0) return "/STM32F407LED_ON_BEEP_OFF.shtml";
  */
  
  /* uri to send after cgi call*/
  return "/STM32F4x7LED.html";  
}

/**
 * Initialize SSI handlers
 */
void httpd_ssi_init(void)
{  
  /* configure SSI handlers (ADC page SSI) */
  http_set_ssi_handler(ADC_Handler, (char const **)TAGS, 1);
}

/**
 * Initialize CGI handlers
 */
void httpd_cgi_init(void)
{ 
  /* configure CGI handlers (LEDs control CGI) */
  CGI_TAB[0] = LEDS_CGI;									/*别漏掉这里，对于多个器件，借鉴原子的写法*/
  http_set_cgi_handlers(CGI_TAB, 1);
}

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
