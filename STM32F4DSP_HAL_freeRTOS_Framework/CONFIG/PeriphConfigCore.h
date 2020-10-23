#ifndef __PERIPHCONFIGCORE_H
#define __PERIPHCONFIGCORE_H

/*
启动：	BOOT1	BOOT0		启动区域
		  x		  0			FLASH
		  0		  1			系统储存器(ISP下载)
		  1		  1			SRAM
*/
/*_________________________________PIN MAP_________________________________________
注：以下这些外设的IO在其初始化函数内已经初始化好，不用再单独初始化
						[IO]										[描述]
外设：	* PA8												MCO输出，默认时钟源为HSE
		
		* CH1/PA6	CH2/PA7		CH3/PB0		CH4/PB1			TIM3默认PWM口
		
		  CH1/PB4	CH2/PB5		CH3/PB0		CH4/PB1			TIM3部分重映射PWM口
		  CH1/PC6	CH2/PC7		CH3/PC8		CH4/PC9			TIM3完全重映射PWM口
		  
		* TX/PA9	RX/PA10		TX/PB6		RX/PB7			USART1默认引脚和重映射引脚
		  TX/PA2	RX/PA3		TX/PD5		RX/PD6			USART2默认引脚和重映射引脚
		  TX/PB10	RX/PB11		TX/PD8		RX/PD9			USART3默认引脚和重映射引脚
		  
		* CS/PA4	CLK/PA5		MISO/PA6	MOSI/PA7		SPI1默认引脚
		  CS/PB12	CLK/PB13	MISO/PB14 	MOSI/PB15		SPI2默认引脚
		  
		* 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15	ADC1各个通道引脚
		  A0 A1 A2 A3 A4 A5 A6 A7 B0 B1 C0 C1 C2 C3 C4 C5
		  
		* CH1/PA15	CH2/PB3		CH3/PB10	CH4/PB11		TIM2四个PWM通道
		
		* CH1/CH1N		CH2/CH2N		CH3/CH3N		CH4		BKIN	TIM8的PWM通道
		  PC6/PA7		PC7/PB0			PC8/PB1			PC9		PA6
		  
		  	CH1/CH1N		CH2/CH2N		CH3/CH3N		CH4		BKIN	TIM1的PWM通道
		* 	PA8/PB13		PA9/PB14		PA10/PB15		PA11	PB12	默认
		  	PA8/PA7			PA9/PB0			PA10/PB1		PA11	PA6		部分重映射
		  	PE9/PE8			PE11/PE10		PE13/PE12		PE14	PE15	完全重映射
		
		* PA0												StandBy待机低功耗模式的唤醒按键WKUP（占用0线外部中断）
		
		* PA4		PA5										DAC_OUT1  DAC_OUT2（hd容量系类外设）
		
		* PC8/SDIO_D0	PC9/SDIO/D1		PC10/SDIO_D2		SDIO固定引脚，用于接SD卡
		  PC11/SDIO_D3	PC12/SDIO_CK	PD2/SDIO_CMD
		  
		* 													FSMC引脚（A[0:25],D[0:15],NEx,NOE,NWE,NBL0,NBL1）
		  D2	D3	NOE	NWE	NE1			D13	D14 D15 	A16 	A17 	A18 	D0 		D1
		  PD0	PD1	PD4	PD5	PD7			PD8	PD9	PD10	PD11	PD12	PD13	PD14	PD15
		  
		  NBL0	NBL1	A23	A19	A20	A21	A22	D4	D5	D6	D7		D8		D9		D10		D11		D12
		  PE0	PE1		PE2	PE3	PE4	PE5	PE6	PE7	PE8	PE9	PE10	PE11	PE12	PE13	PE14	PE15
		  
		  A0	A1	A2	A3	A4	A5	A6		A7		A8		A9		A10	A11	A12	A13	A14	A15
		  PF0	PF1	PF2	PF3	PF4	PF5	PF12	PF13	PF14	PF15	PG0	PG1	PG2	PG3	PG4	PG5
		  
		  NE2	NE3		NE4		A24		A25
		  PG9	PG10	PG12	PG13	PG14
		  
		* ...TODO:DCMI、USB、CAN等外设的IO还没有列出


用户：	*
*/

/*_______________________________复用资源介绍___________________________________________

GPIO的AF复用功能完整表在 “stm32f407ze.pdf” 的 62页 开始

DMAx的数据流和通道分配完整表在 “dm00031020-stm32f405415-stm32f407417-stm32f427437-and-stm32f429439-advanced-armbased-32bit-mcus-stmicroelectronics”
			的 307页 开始

*/

/*_______________________________外设资源介绍___________________________________________
	STM32F407ZGT6：
		1024KB(hd)FLASH，128KB（SRAM1）+ 64KB（SRAM2）RAM，144脚LQFP封装，114个GPIO，-40~85℃
	
	时钟分配：
		HSE外接8Mhz晶振，LSE外接32.768Khz晶振
		开启HSE（8M）、LSE（32.768K）和LSI（32K），关闭HSI（16M）
		PLLCLK = SYSCLK = AHBCLK = HCLK 均为168MHz；APB2外设为84MHz，APB2定时器为168MHz；APB1外设为42Mhz，APB1定时器为84Mhz
		
		ETH\FSMC\USB FS\USB HS\GPIO\DMA 的最高速度具体看手册
		RTC 时钟取自 LSE = 32.768K 	（默认不开，使能RTC时会开）
		IWDG时钟取自 LSI = 32K 		（默认开）
					 HSI = 16M		（默认不开）
		DCMI 54 Mbyte/s max
		
		APB2外设(84M)：SDIO\EXT IT\USART1\USART6\SPI1\ADC123
		APB2定时器(168M)：TIM1 TIM8 TIM9 TIM10 TIM11
		APB1外设(42M)：USART2\USART3\UART4\UART5\SPI2、I2S2\SPI3、I2S3\I2C123\bxCAN12\DAC12
		APB1定时器(84M)：TIM2 TIM3 TIM4 TIM5 TIM12 TIM13 TIM14 TIM6 TIM7
	
		定时器不管16或者32位的，都按16位来用，也很够了
	本框架TIM分配：
		TIM4	10ms时基，用户不可写中断程序
		TIM3	产生辅助PWM，可中断提供辅助时基，通道按照其他外设使用情况灵活分配
		TIM2	多功能使用，可中断提供辅助时基，通道可用于PWM、正交编码解码、输入捕获
		TIM5	同上
		TIM1\8	主PWM，产生3对互补、带死区、带刹车和电平可调的PWM
		TIM6\7	官方推荐用于触发DAC（也可产生三角波）或者产生额外时基
	
*/

/*_______________________________外设优先级划分___________________________________________*/
/*
优先级分组为4，只有0~15的16级抢占优先级


优先级	0			1				2				3				4				5				...
	   保留	    TIM4(时基)	 	按键外部中断	按键外部中断	TIM6(辅助时基)
											    TIM3（PWNM）	TIM7(辅助时基)
											    TIM2（多功能）   
*/

/*
   	CPU内部 FLASH  			【0x0800 0000 - 0x080F FFFF】,容量512K(0x80000)字节
	CPU内部 SRAM1  			【0x2000 0000 - 0x2000 FFFF】,容量128K(0x20000)字节
	
	外部扩展SRAM 在块1区3   【0x6800 0000 - 0x681F FFFF】,容量1MB(0x100000)字节（512K个16bit）
	TFT LCD 地址 在块1区4   【0x6C000000 | 0x000007FE,  +2】, 仅占用2个端口地址（使用A10连接RS引脚）TODO：改为A0
	
	STM32F407ZGT6内存分配：
	普通内存：
		主要内存：		地址从：0X2000 0000 开始，共 128KB），这部分内存任何外设都可以访问。
		辅助内存CCM：	地址从：0X1000 0000 开始，共 64KB，这部分内存仅 CPU 可以访问，DMA 之类的不可以直接访问，使用时得特别注意！！
		外部SRAM：		地址从：0X6800 0000 开始，共 1024KB。
		都用内存管理来管理，大块内存使用应尽量使用malloc和free
	
*/

/*FPU开启*/
/*
	只需确保 system_stm32f4xx.c 文件的 SystemInit()中的FPU初始化环节被启用；
	一般来说MDK设置启用单精度(32bit)FPU就会开启相关宏，但是并没用...所以这里手动在SystemInit()函数中强行启用FPU初始化了
*/

/*________________________________________TODO：DSP使用_________________________________________*/
/*
移植：



可用API：







*/

/*________________________________模板固定搭配____________________________________________*/
/*所有main的头文件都放在这里*/
#include "sysVar.h"								/*定义系统级常用的变量、数据类型和二进制表示宏*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"						/*在里面的stm32f4xx_hal_conf.h里面选择用哪些外设的HAL库——————！按需要进行修改！*/

#include "malloc.h"							/*借鉴的 内存管理 章节的源代码自实现的malloc和free*/	
#include "sys_menu.h"							/*模板固定搭配！必须开启！提供一个菜单模板，把系统的输入、输出、执行功能的标志位控制全部打包！相当于io_ctrl*/
#include "MyString.h"							
												/*模板固定搭配！必须开启！提供一个实现了string.h大部分字符操作函数的库
													具体API看MyString.c文件里的注释
													int mystrlen (const char *str);
													char * mystrcat (char *dest, const char *src);
													char * mystrncat (char *dest, const char *src, int n);
													char * mystrcpy (char *dest, const char *src);
													char * mystrncpy (char *dest, const char *src, int n);
													int mystrcmp (const char *s1, const char *s2);
													int mystrncmp (const char *s1, const char *s2, int n);
													void * mymemset (void *s, int c, unsigned n);
													void *mymemcpy(void *des,const void *src,size_t len);
													char *mystrtok(char *s, const char *delim);*/
int myatoi(const char *str);					/*提供一个字符串转整形的实现*/
#include "lwrb.h"								/*提供一个FIFO实现，github开源库地址：*/
unsigned char BCD2HEX(unsigned char bcd_data); 	//提供BCD转为HEX子程序
unsigned char HEX2BCD(unsigned char hex_data); 	//提供HEX转为BCD子程序
#include "printf.h"								
												/*模板固定搭配！必须开启！包含且编译printf.h，github开源文件，无依赖，功能比较全。
													约占6KB，对于stm32够，对于其他小容量MCU则看“其他几个sprintf实现”文件夹里面的,不要纠结了。
													开源仓库地址：https://github.com/mpaland/printf
													提供API：
													int printf(const char* format, ...); 								不用
													int printf_uart(unsigned char uart,const char* format, ...); 		用，可选发送串口
																		串口选择可选：UART1/UART2/UART3，需要先使能相关串口
													int sprintf(char* buffer, const char* format, ...); 				不带字节数量限制
													int snprintf(char* buffer, size_t count, const char* format, ...); 	带字节数量限制，更安全
												注意：另在Middlewares文件夹内提供strprintf.h，功能比较单一，但很小。*/
#define STSTEM_TIM4_ENABLE		1				/*模板固定搭配！必须开启！使用通用定时器4，提供时基，默认10ms周期中断*/
#define STSTEM_TIM3PWM_ENABLE	0				/*模板基本功能，必须开启！使用通用定时器3，默认不开启中断，提供四路PWM输出*/
												/*默认通道4给蜂鸣器，蜂鸣器低电平有效！蜂鸣器控制需要用到TIM4！
												改变占空比可调响度，改变频率可调声调*/
												/*默认向上计数，输出极性为LOW，PWM1模式，即设置为当计数值小于此值时输出低电平。*/
												/*默认：引脚上拉							(CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1)*/
		#define STSTEM_TIM3PWM_REMAP_PARTIAL	0	/*定时器3部分引脚重映射，Partial remap  (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)*/
		#define STSTEM_TIM3PWM_REMAP_ENABLE		0	/*定时器3全部引脚重映射，Full remap     (CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9)*/
		#define STSTEM_TIM3PWM_CHANNEL_ENABLE	(B0000_1000) /*输出通道选择，共四个通道*/
													/*可选  B0000_0001|B0000_0010|B0000_0100|B0000_1000	*/
		#define TIM3PWM_BuzzerCh				TIM3PWM_Channel_4	//选择用于蜂鸣器的PWM通道
		#define STSTEM_TIM3PWM_TI_ENABLE		0	/*是否开启定时器3的定时中断，除非急需用，否则一般不开*/
		
		#define tim3prsc STSTEM_TIM3PWM_prsc_12K		/*选择定时器输出频率，以下六条语句基本不动（重装值为1000，这里是选择预分频系数）*/
			#define STSTEM_TIM3PWM_prsc_2K	(42-1)
			#define STSTEM_TIM3PWM_prsc_4K	(21-1)
			#define STSTEM_TIM3PWM_prsc_12K	(7-1)
			#define STSTEM_TIM3PWM_prsc_21K	(4-1)
			#define STSTEM_TIM3PWM_prsc_42K	(4-1)
			#define STSTEM_TIM3PWM_prsc_84K	(1-1)
			#define tim3arr 1000-1
		/*可用API：
					HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);		开启TIM3的PWM通道2
					HAL_TIM_PWM_Stop(&TIM3_Handler,TIM_CHANNEL_2);		关闭TIM3的PWM通道2，但看源码好像使所有通道都关闭了，待实验
					
					设置TIM3的PWM通道2的占空比百分数为88.8%，值需在0~100.0之间。即88.8%的低电平时间（本模板规范：低电平有效，即用电器工作电压）
					TIM3_set_Channel_Pulse(TIM3PWM_Channel_2,88.8);
					
					buzzer_bibi_once;			用户使用，蜂鸣器叫唤一声
					define buzzer_bibi_on;		用户使用，蜂鸣器间歇叫唤开启
					define buzzer_bibi_off;		用户使用，蜂鸣器间歇叫唤关闭
		*/


/*_____________________________________\\\                               ///____________________________________________*
 *___________________________________________________组件选择___________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/
#define SYSTEM_SUPPORT_pid		0				/*提供一个pid算法实现库，集成了积分分离和变限积分，以及可选的不完全微分和微分先行，具体用法看pid.h里面*/
												/*里面另提供一个一阶低通滤波实现函数*/
												/*一阶低通滤波器 float FirstOrderLPF(float NewValue);*/												
	#if SYSTEM_SUPPORT_pid						/*如果不包含，其他文件就无法调用此组件的API*/
		#include "pid.h"
	#endif

#define SYSTEM_SUPPORT_simuspi	0				/*是否调用软件模拟SPI，具体用法在.h文件里介绍*/
	#if SYSTEM_SUPPORT_simuspi
		#include "simuspi.h"
	#endif
	
#define SYSTEM_SUPPORT_simui2c	0				/*是否调用软件模拟I2C，具体用法在.h文件里介绍*/
	#if SYSTEM_SUPPORT_simui2c
		#include "simui2c.h"
	#endif

/*DEVICES所有头文件*/
#include "TFTLCD.h"
//#include "OLED.h"
//#include "mpu6050.h"


/*_____________________________________\\\                               ///____________________________________________*
 *_________________________________________________用户GPIO配置_________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/

/*在PeriphConfig.c文件里去配置用户GPIO*/

/*_____________________________________\\\                               ///____________________________________________*
 *___________________________________________________外设配置___________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/
/*
 *FreeRTOS启用
 */
#include "isUseFreeRTOS.h"		/*到这个文件里配置*/
/*FreeRTOS版本：v10.3.1，默认用于任务的RAM堆栈大小为30KB，按需修改，不要有没有语句的空任务！*/
	
	#if SYSTEM_SUPPORT_OS
		#include "FreeRTOS.h"	//这里不要乱动	  
		#include "task.h"
		#include "queue.h"
		#include "TaskConfig.h"
	#else
		#include "BareConfig.h"	
	#endif
	


#define SYSTEM_IWDG_ENABLE		0			/*开启独立看门狗，默认1S的喂狗周期，默认在TIM4定时中断里喂狗，用IWDG必开TIM4*/
											/*注：看门狗和低功耗待机模式不能同时开启，因为看门狗不能关闭，看门狗复位会唤醒低功耗状态*/
#define SYSTEM_MCO_PA8_OUT		0			/*设置PA8为MCO输出，默认时钟源为HSE（5M），五倍分频（最大分频）*/


/*配置使用RTC，确保LSE连接有36.768kHz的晶振，确保RTC有VBAT备用电源
	默认开启RTC闹钟中断，系统在待机模式下可选被闹钟唤醒
API:
	设置时分秒：HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);
	读取时分秒：HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);
	设置年月日：HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);
	读取年月日：HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);
	
	设置闹钟：HAL_StatusTypeDef HAL_RTC_SetAlarm_IT(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format);
	闹钟A的事件回调函数在.c里：void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
	例子：
		RTC_TimeTypeDef RTC_RealTime = {
			.Hours = xx,
			.Minutes = xx,
			.Seconds = xx,
		};
		RTC_DateTypeDef RTC_RealDate = {0}; //这两个和上面同理
		RTC_AlarmTypeDef RTC_AlarmA = {0};
		
		HAL_RTC_SetTime(&RTC_Handler, &sTime, RTC_FORMAT_BCD); 		//设置时分秒
		//日期不举例了，不常用
		HAL_RTC_SetAlarm_IT(&RTC_Handler, &sAlarm, RTC_FORMAT_BCD); //设置闹钟
		HAL_RTC_DeactivateAlarm(&RTC_Handler,RTC_ALARM_A);
		
	读取时间：
		RTC_TimeTypeDef RTC_RealTime;
		RTC_DateTypeDef	RTC_RealDate;
		HAL_RTC_GetTime(&RTC_Handler, &RTC_RealTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&RTC_Handler, &RTC_RealDate, RTC_FORMAT_BIN);
		sprintf(RTC_buf,"%d-%d-%d-%d   %d-%d-%d\r\n",RTC_RealDate.Year + 1970,RTC_RealDate.Month,RTC_RealDate.Date,RTC_RealDate.WeekDay,RTC_RealTime.Hours,RTC_RealTime.Minutes,RTC_RealTime.Seconds);
		printf_uart(UART1,"%s",RTC_buf);
*/
#define SYSTEM_RTC_ENABLE		1



/*硬件产生32位随机数 RNG*/
#define SYSTEM_RNG_ENABLE		0
	extern u8 GenerateRNG_Ready;	//RNG产生好标志位
	extern u32 randomNum32bit; 		//存32位RNG容器
/*可用API：
	调用一次 HAL_RNG_GenerateRandomNumber_IT(&RNG_Handle); 来触发产生随机数
	查询 GenerateRNG_Ready，当其为1时，可从 randomNum32bit 取32位随机数
	随后把 GenerateRNG_Ready 清零
*/


/*配置使用CRC循环冗余校验
这个CRC计算模块使用常见的、在以太网中使用的计算多项式：
X32 + X26 + X23 + X22 + X16 + X12 + X11 + X10 +X8 + X7 + X5 + X4 + X2 + X + 1
写成16进制就是：0x04C11DB7
API：参数：const uint32_t aDataBuffer[BUFFER_SIZE]; #define BUFFER_SIZE    114
	HAL_CRC_Accumulate(&hcrc, (uint32_t *)aDataBuffer, BUFFER_SIZE); //返回值为uint32_t的计算结果*/
#define SYSTEM_CRC_ENABLE		1


/*
 *串口配置
 */
#define SYSTEM_UART1_ENABLE			1		/*使能串口1	       TX/PA9, RX/PA10		*/
#define SYSTEM_UART1_REMAP_ENABLE	0		/*串口1引脚重映射：TX/PB6, RX/PB7		*/
#define SYSTEM_UART1_BOUND			115200	/*串口1波特率*/

#define SYSTEM_UART2_ENABLE			0		/*使能串口2	       TX/PA2, RX/PA3		*/
#define SYSTEM_UART2_REMAP_ENABLE	0		/*串口2引脚重映射：TX/PD5, RX/PD6		*/
#define SYSTEM_UART2_BOUND			115200	/*串口2波特率*/

#define SYSTEM_UART3_ENABLE			0		/*使能串口3	       TX/PB10, RX/PB11		*/
#define SYSTEM_UART3_REMAP_ENABLE	0		/*串口3引脚重映射：TX/PD8,  RX/PD9		*/
#define SYSTEM_UART3_BOUND			115200	/*串口3波特率*/
/*默认均为：8位数据，1位停止，无校验，收发模式，开启接受中断*/
/*注：串口2、3的接收回调函数没有补全，用时再补，和串口1的同理*/
/*缓存大小去*/
/*可用的API： 目前串口接收的问题就是一次涌入数据过多会崩，可能是存储数据时栈溢出，但是暂时找不到在哪里，小数据量接收没大问题
	发送：	注：不要用printf！
				参数：UART1~UART3
				printf_uart(UART1,"Fault : %d\t",x);
				也可以用	sprintf(char* buffer, const char* format, ...); 				不带字节数量限制
							snprintf(char* buffer, size_t count, const char* format, ...); 	带字节数量限制，更安全
			如果没有启用 SYSTEM_SUPPORT_sprintf ，则可用以下：
			参数：UART1_Handler~UART3_Handler ， 发送数据的字符类型指针 ， 数据长度(字节数} ， 超时时间
				HAL_UART_Transmit(&UART1_Handler,UART_BUF,12,10); 				发送函数
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET); 	等待发送结束
	接收：（以下只针对串口1说明）（一次接收字符不要超过200个字节，如果超出则丢弃200之前的数据）
			接受协议设置(默认协议0)：USART1_SetMode(0);	串口1接受协议：0为只接受以'\r\n'结尾的数据（数据以去掉\r\n保存），1协议暂时保留，等以后加上可以解析别的协议如modbus等等
				接受协议设置可用随时切换，切换后接收方式也随下面的说明换。
			统一接收函数： 返回HAL_OK即有数据可以取，反之为HAL_ERROR；is_print：是否打印接收到的数据；buf用于存放接收到的数据，必须先给个足够大的容器
				char sys_USART1_RX_Fetch(u8 is_print, char* buf)
				用例：
					u16 num4uart1;
					char* buf4uart1 = mymalloc(InrRAM,(USART1_RX_BUF_MaxNum > USART1_RX_FIFO_MaxNum ? USART1_RX_BUF_MaxNum : USART1_RX_FIFO_MaxNum));
					if(buf4uart1 != NULL)
					{
						if(sys_USART1_RX_Fetch(FALSE, buf4uart1,&num4uart1) == HAL_OK)
						{
							printf_uart(UART1,"%s-%d",buf4uart1,num4uart1);
						}
					}
					myfree(InrRAM,buf4uart1);
			关于两个协议0、1的接收细节，已经在sys_USART1_RX_Fetch()内实现，一般不用看
*/



/*使用CAN1，默认不使用中断
默认引脚：默认上拉
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
	可以按照 stm32f207ie.pdf 手册的 59页 开始自行选择引脚，可选的还挺多的
默认帧：
	固定格式:ID为0X12,标准帧,数据帧
*/
#define SYSTEM_CAN1_ENABLE			0
/*可用API：
	发送： u8 CAN1_Send_Msg(u8* msg,u8 len); —————— msg:数据指针,最大为8个字节；len:数据长度(最大为8)，返回HAL_ERROR失败，返回HAL_OK成功
	接收： u8 CAN1_Receive_Msg(u8 *buf);	 —————— buf:数据缓存区;返回值:0,无数据被收到,其他则为接收的数据长度;	
*/



/*高级定时器1，专门用于产生PWM信号：三路互补PWM输出、死区控制、刹车信号输入*/
/*引脚：
				CH1/CH1N		CH2/CH2N		CH3/CH3N		CH4		BKIN（刹车信号输入）
默认			PA8/PB13		PA9/PB14		PA10/PB15		PA11	PB12
部分重映射		PA8/PA7			PA9/PB0			PA10/PB1		PA11	PA6
完全重映射		PE9/PE8			PE11/PE10		PE13/PE12		PE14	PE15
*/
/*默认并推荐不开启定时中断；引脚默认没有上下拉；初始化后默认没有打开相应通道的PWM输出，需要手动开启输出*/
/*定时器工作在向上计数时，PWM信号边缘对齐；定时器工作在向上和向下计数模式时，PWM信号中心对齐*/
#define STSTEM_TIM1PWM_ENABLE		0				/*是否启用并初始化TIM1的三路互补PWM（前三个通道）和一路PWM（通道4）*/
			#define STSTEM_TIM1PWM_useBreak	1		/*是否启用BKIN刹车信号控制输入，默认低电平有效*/
													/*选择每一个通道的有效电平和空闲电平（可能是刹车时的电平），默认死区3us，可修改*/
			#define STSTEM_TIM1PWM_REMAP_PARTIAL  0	/*定时器1部分引脚重映射，Partial remap，	引脚如上边所示*/
			#define STSTEM_TIM1PWM_REMAP_ENABLE	  0	/*定时器1全部引脚重映射，Full remap，		引脚如上边所示*/
			/*输出通道选择，共四个通道，可以相与打开多个通道，互补通道默认成对打开*/
			#define STSTEM_TIM1PWM_CHANNEL_ENABLE (B0000_0001|B0000_0010|B0000_0100|B0000_1000)		/*必须加括号括起来*/
			#define tim1prsc STSTEM_TIM1PWM_prsc_4K		/*选择定时器输出频率，以下六条语句基本不动（重装值为2000，这里是选择预分频系数）*/
			#define STSTEM_TIM1PWM_prsc_2K	(42-1)
			#define STSTEM_TIM1PWM_prsc_4K	(21-1)
			#define STSTEM_TIM1PWM_prsc_12K	(7-1)
			#define STSTEM_TIM1PWM_prsc_21K	(4-1)
			#define STSTEM_TIM1PWM_prsc_42K	(4-1)
			#define STSTEM_TIM1PWM_prsc_84K	(1-1)
				#define tim1arr 2000-1
/*可用API：
		启动通道1~4的PWM输出
		HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_1);
		
		启动互补通道1~3的PWM输出
		HAL_TIMEx_PWMN_Start(&TIM1_Handler,TIM_CHANNEL_1);
		
		关闭通道的PWM输出（通道选择同上）
		HAL_TIM_PWM_Stop(&TIM1_Handler,TIM_CHANNEL_1);
		HAL_TIMEx_PWMN_Stop(&TIM1_Handler,TIM_CHANNEL_1);
		
		设置TIM1的PWM通道2的占空比百分数为88.8%，值需在0~100.0之间。即88.8%的低电平时间（本模板规范：低电平有效，即用电器工作电压）
		TIM1_set_Channel_Pulse(TIM1PWM_Channel_2,88.8);
*/

/*hd系列外设，其他与上同理*/
/*高级定时器8，专门用于产生PWM信号：三路互补PWM输出、死区控制、刹车信号输入*/
/*引脚：
				CH1/CH1N		CH2/CH2N		CH3/CH3N		CH4		BKIN（刹车信号输入）
				PC6/PA7			PC7/PB0			PC8/PB1			PC9		PA6
*/
/*默认并推荐不开启定时中断；引脚默认没有上下拉；初始化后默认没有打开相应通道的PWM输出，需要手动开启输出*/
/*定时器工作在向上计数时，PWM信号边缘对齐；定时器工作在向上和向下计数模式时，PWM信号中心对齐*/
/*没有重映射功能*/
#define STSTEM_TIM8PWM_ENABLE		0				/*是否启用并初始化TIM8的三路互补PWM（前三个通道）和一路PWM（通道4）*/
			#define STSTEM_TIM8PWM_useBreak	1		/*是否启用BKIN刹车信号控制输入，默认低电平有效*/
													/*选择每一个通道的有效电平和空闲电平（可能是刹车时的电平），默认死区3us，可修改*/
			/*输出通道选择，共四个通道，可以相与打开多个通道，互补通道默认成对打开*/
			#define STSTEM_TIM8PWM_CHANNEL_ENABLE (B0000_0001|B0000_0010|B0000_0100|B0000_1000)		/*必须加括号括起来*/
			#define tim8prsc STSTEM_TIM8PWM_prsc_4K		/*选择定时器输出频率，以下六条语句基本不动（重装值为2000，这里是选择预分频系数）*/
			#define STSTEM_TIM8PWM_prsc_2K	(42-1)
			#define STSTEM_TIM8PWM_prsc_4K	(21-1)
			#define STSTEM_TIM8PWM_prsc_12K	(7-1)
			#define STSTEM_TIM8PWM_prsc_21K	(4-1)
			#define STSTEM_TIM8PWM_prsc_42K	(4-1)
			#define STSTEM_TIM8PWM_prsc_84K	(1-1)
				#define tim8arr 2000-1
/*可用API：
		启动通道1~4的PWM输出
		HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_1);
		
		启动互补通道1~3的PWM输出
		HAL_TIMEx_PWMN_Start(&TIM8_Handler,TIM_CHANNEL_1);
		
		关闭通道的PWM输出（通道选择同上）
		HAL_TIM_PWM_Stop(&TIM8_Handler,TIM_CHANNEL_1);
		HAL_TIMEx_PWMN_Stop(&TIM8_Handler,TIM_CHANNEL_1);
		
		设置TIM1的PWM通道2的占空比百分数为88.8%，值需在0~100.0之间。即88.8%的低电平时间（本模板规范：低电平有效，即用电器工作电压）
		TIM8_set_Channel_Pulse(TIM8PWM_Channel_2,88.8);
*/



/*通过用定时器2：16位，四个独立通道可用于：输入捕获、输入比较、PWM、单脉冲，多种途径触发DMA中断*/
#define STSTEM_TIM2_ENABLE		0			/*通用定时器2，功能自定，默认分频系数为72，初始化函数在PeriphCconfig.c里面定义*/
	#define STSTEM_TIM2_TI_ENABLE	0		/*是否开启定时器2的定时中断*/
	
	#define STSTEM_TIM2_asPWMorCap	3		/*选择定时器2作为...注：PWM(输出比较)、输入捕获和正交解码三个功能不能共用！*/
											/*写3作为正交编码器的解码使用，只能使用CH1和CH2，即PA15和PB3（已经默认上拉），默认使用两路边沿触发，计数值为单路上升沿数的四倍
												正交解码使用到TIM2的定时中断，必须打开！但在初始化时已经默认打开*/
											/*写2作为普通定时器中断使用*/
											/*写1作为输入捕获：可以获取高电平或者低电平的时间，默认不分频，不滤波
												输入捕获使用到TIM2的定时中断，必须打开！但在初始化时已经默认打开*/
											/*写0作为PWM：默认PWM1模式，向上计数，低电平有效，一个周期内占空比越大低电平时间越长
												引脚说明：			没有重映射			两种部分重映射			完全重映射
												TIM2四个通道：		PA0 PA1 PA2 PA3		含有前面的引脚		PA15 PB3 PB10 PB11
												注意：由于PA2 PA3串口2占用，PA0 PA1由ADC占用，所以TIM2的PWM只用完全重映射！！
														但PB10 PB11串口3占用，要么不用串口3，要么串口3重映射
												初始化后自动打开相应通道的PWM输出
											*/
			/*PWM输出比较功能选项*/
			#define tim2arr STSTEM_TIM2PWM_Period_5K			/*选择定时器输出频率（预分频系数为84，计数频率为1Mhz，这里选择重装值）*/
				#define STSTEM_TIM2PWM_Period_1K	(1000-1)	/*注意：这里是选择重装值，也是占空比的分辨率，频率高则分辨率低，反之亦然，慎选*/
				#define STSTEM_TIM2PWM_Period_2K	(500-1)
				#define STSTEM_TIM2PWM_Period_5K	(200-1)
				#define STSTEM_TIM2PWM_Period_10K	(100-1)
				#define STSTEM_TIM2PWM_Period_20K	(50-1)
				#define STSTEM_TIM2PWM_Period_50K	(20-1)
				#define STSTEM_TIM2PWM_Period_100K	(10-1)
				#define STSTEM_TIM2PWM_Period_200K	(5-1)
			#define STSTEM_TIM2PWM_CHANNEL_ENABLE	(B0000_0001|B0000_0010) /*输出通道选择，共四个通道，可以相与打开多个通道，PA15 PB3 PB10 PB11*/
			/*可用API：
				HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_2);		开启TIM2的PWM通道2
				HAL_TIM_PWM_Stop(&TIM2_Handler,TIM_CHANNEL_2);		关闭TIM2的PWM通道2，但看源码好像使所有通道都关闭了，待实验
				
				设置TIM2的PWM通道2的占空比百分数为88.8%，值需在0~100.0之间。即88.8%的低电平时间（本模板规范：低电平有效，即用电器工作电压）
				TIM2_set_Channel_Pulse(TIM2PWM_Channel_2,88.8);
			*/
			
			/*输入捕获功能选项，自动设置庄装在值为0xff，计数频率为1Mhz*/
			#define STSTEM_TIM2_Cap_trigV	1			/*写1高电平触发一次输入捕获，写0相反，写2双边沿触发*/
			#define STSTEM_TIM2_Cap_Channel	B0000_0001	/*选择用哪个通道当输入捕获，只能四选一！！*/
			/*可用API：
				float Peek_TIM2_Cap_Val();		获取最近一次TIM2输入捕获的脉冲时间，单位 毫秒，float类型，按照设置的捕获沿进行，可以随时随地调用此函数
			*/
			
			/*正交解码功能选项，自动设置庄装在值为0xff，计数频率为1Mhz*/
			// 4 : 			使用定时器编码器接口捕获AB相的上升沿和下降沿，一个脉冲*4
			// ENCODER：	编码器线数(转速一圈输出脉冲数)
			// SPEEDRATIO：	电机减数比，内部电机转动圈数与电机输出轴转动圈数比，即减速齿轮比，编码器一般和电机转速一致，即直接把编码器安装到电机轴上！
			#define ENCODER     300    	// 编码器线数
			#define SPEEDRATIO  1   	// 电机减速比
			#define PPR         (SPEEDRATIO*ENCODER*4) // Pulse/r 每圈可捕获的脉冲数，不用动！
			/*可用API：
				得到当前计数值：int32_t peek_TIM2_Encoder_Value(void); //用于表示编码器当前的绝对位置
				速度：只需去定时器溢出中断函数HAL_TIM_PeriodElapsedCallback的TIM4中用一个速度变量接着 peek_TIM2_Encoder_Speed()返回的速度值 单位 转/秒
			*/
			

/*hd系列外设，基本定时器6、7，只能用于定时中断，提供更多的同步功能*/
#define STSTEM_TIM6_ENABLE		0
	#define tim6arr STSTEM_TIM6_Period_5K			/*选择定时器6输出频率（预分频系数为84，这里选择重装值）*/
		#define STSTEM_TIM6_Period_1K	(1000-1)
		#define STSTEM_TIM6_Period_2K	(500-1)
		#define STSTEM_TIM6_Period_5K	(200-1)
		#define STSTEM_TIM6_Period_10K	(100-1)
		#define STSTEM_TIM6_Period_20K	(50-1)
		#define STSTEM_TIM6_Period_50K	(20-1)
		#define STSTEM_TIM6_Period_100K	(10-1)
		#define STSTEM_TIM6_Period_200K	(5-1)
#define STSTEM_TIM7_ENABLE		0
	#define tim7arr STSTEM_TIM6_Period_5K			/*选择定时器7输出频率（预分频系数为84，这里选择重装值）*/
		#define STSTEM_TIM7_Period_1K	(1000-1)
		#define STSTEM_TIM7_Period_2K	(500-1)
		#define STSTEM_TIM7_Period_5K	(200-1)
		#define STSTEM_TIM7_Period_10K	(100-1)
		#define STSTEM_TIM7_Period_20K	(50-1)
		#define STSTEM_TIM7_Period_50K	(20-1)
		#define STSTEM_TIM7_Period_100K	(10-1)
		#define STSTEM_TIM7_Period_200K	(5-1)
/*
	关闭定时中断：（先清除中断标志位，再关闭中断）
		__HAL_TIM_CLEAR_IT(&TIM6_Handler, TIM_IT_UPDATE);
		HAL_TIM_Base_Stop_IT(&TIM6_Handler);
		
		__HAL_TIM_CLEAR_IT(&TIM76_Handler, TIM_IT_UPDATE);
		HAL_TIM_Base_Stop_IT(&TIM7_Handler);
		
		注： __HAL_TIM_CLEAR_FLAG(&TIM6_Handler,TIM_FLAG_UPDATE);和
			 __HAL_TIM_CLEAR_IT(&TIM6_Handler, TIM_IT_UPDATE);效果应该一样
*/


/*对于ADC和DAC，如果Vref+连接到3.0V基准，则要修改程序！*/
/*hd系列外设，DAC两个输出通道 PA4和PA5，默认初始化为12位右对齐，速度最快为250K左右，输出范围 0~Vref+，默认Vref+为3.3V，否则自行更改转换计算*/
/*可选输出 噪声波形 和 三角波波形 ，默认不适用外部触发，可选触发源在初始化函数中修改（有定时器中断和外部中断线9）*/
#define SYSTEM_DAC_OUT1_ENABLE	0
	#define SYSTEM_DAC_OUT1_TriangleWave_ENABLE	1
	#define SYSTEM_DAC_OUT1_NoiseWave_ENABLE	0
#define SYSTEM_DAC_OUT2_ENABLE	0
	#define SYSTEM_DAC_OUT2_TriangleWave_ENABLE	0
	#define SYSTEM_DAC_OUT2_NoiseWave_ENABLE	1
/*可用API:
	设置通道1输出电压,vol:0~3.30V
		void DAC_Set_Ch1_Vol(float vol)
	设置通道2输出电压,vol:0~3.30V
		void DAC_Set_Ch2_Vol(float vol)
*/

/*	对于ADC和DAC，如果Vref+连接到3.0V基准，则要修改程序！
	ADC：STN32的是12位逐次逼近型，分ADC1、2和3，其中ADC1和ADC2的引脚一样（除了内部通道）；
	最大速率1MHz，1us（在AD时钟14MHz（最高），采样时间为1.5个周期得到），总转换时间为 (采样时间 + 12.5个周期)，采样时间长则采样精度高；
	参考电压 Vref-必须连到VSSA，Vref+可连到 2.4V~VDDA，ADC输入电压必须小于Vref+，默认Vref+为3.3V，否则自行更改转换计算
	ADC1本质就是一个AD转换器加16路选择器，所以ADC1同一时刻采样值只能是一个，如果用在规则组中扫描模式，无法得知当前结果是哪一路的，一个AD模块一个值
	ADC1的通道与引脚映射：
	通道：	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	   16		     17
	IO	：	A0	A1	A2	A3	A4	A5	A6	A7	B0	B1	C0	C1	C2	C3	C4	C5	内部温度	内部参考电压
	标志：B1in16					  B8in16					  B15in16	 InrTemp
*/
#define SYSTEM_ADC1_ENABLE		0			/*启否ADC1*/
	#define SYSTEM_ADC1_useScan		1		/*启否规则组的连续扫描，如果启用，则把下面定义的所有通道都放到规则组里，并使用DMA2 Stream0的通道0把转换结果放到目标位置
												如果不启用，则为软件触发的单次转换*/
		#define SYSTEM_ADC1_useCircular	1	/*只在扫描模式下有效；开启则自动循环ADC转换，只需要判断标志位和读数即可，不需要软件手动触发开启一次ADC转换*/
	#define SYSTEM_ADC1_useChanlNum	4		/*定义共用多少路通道*/
	#define SYSTEM_ADC1_useChanl	B4in16|B5in16|B8in16|InrTemp /*定义共用哪些通道，可写B1in16~B15in16（分别代表通道0~14），和InrTemp(内部温度通道)
														（第16个ADC1通道留作内部温度标志所以不可用）*/
													/*如果只用采集内部温度，而不用其他通道，应设置：SYSTEM_ADC1_useChanlNum为1  SYSTEM_ADC1_useChanl单设为InrTemp*/
		#define InrTemp B16in16
		extern u16 adValue[SYSTEM_ADC1_useChanlNum];/*扫描模式下DMA1把ADC转换结果传送的目标位置*/
		extern u8 adValueDone; 						/*扫描模式下转换完成标志*/
/*可用的API：返回的都是原始ADC值
		凡是启用 SYSTEM_ADC1_useScan 扫描模式，即触发一次，自动扫描所有规则组里的通道————适合快速
					自动用DMA2 Stream0的通道0发送到adValue[]，并置位adValueDone
			读取顺序：
				调用 HAL_ADC_Start_DMA(&ADC1_Handler, (uint32_t*)&adValue,SYSTEM_ADC1_useChanlNum); 一次则开启一次规则组全部通道的转换
					调用1次则把规则组所有通道转换一遍，然后判断标志位adValueDone等着在adValue[]取数就行了
				如果 SYSTEM_ADC1_useCircular 打开：只需要执行一次！一次！ HAL_ADC_Start_DMA(&ADC1_Handler, (uint32_t*)&adValue,SYSTEM_ADC1_useChanlNum); 即可
				
				判断 adValueDone 是否为1，是则从 adValue[x] 读值即可，如果启用 内部温度 通道，其值保存在adValue[SYSTEM_ADC1_useChanlNum-1]
				读取完后对 adValueDone 清零
				停止转换为 HAL_ADC_Stop_DMA(&ADC1_Handler);
				例子：
					char ADC_buf[20];
					if(adValueDone)
					{
						adValueDone = 0;
						//HAL_ADC_Stop_DMA(&ADC1_Handler);
						sprintf(ADC_buf,"adValue:%d %d %d",adValue[0],adValue[1],adValue[2]);
						LCD_ShowString(10,180,16,(u8*)ADC_buf,0);
					}
		凡是没有启动 SYSTEM_ADC1_useScan 一次触发一个通道并手动取数————适合慢速
			读取顺序：
				调用 u32 Get_Adc_Average(u32 ch,u8 times); 返回值为本次ADC值
				其中 ch 通道 形参选择想转换的通道，可选ADC_CHANNEL_0~15，可以为ADC_CHANNEL_TEMPSENSOR，其中result[0]保存本次本通道的转换结果
				例子：
					adValue[0] = (u16)Get_Adc_Average(ADC_CHANNEL_0,10);
					adValue[1] = (u16)Get_Adc_Average(ADC_CHANNEL_3,10);
					adValue[2] = (u16)Get_Adc_Average(ADC_CHANNEL_4,10);
					sprintf(ADC_buf,"adValue:%d %d %d",adValue[0],adValue[1],adValue[2]);
					LCD_ShowString(10,180,16,(u8*)ADC_buf,0);
				
		注意：采集温度通道返回的是ADC的原始值，用 float Get_Temprate(u32 adcx) 把原始值转换为实际温度值(float类型)
*/


/*开启硬件SPI，x8/xB系列有两个SPI，最高18M位每秒
默认：尽量只用其中一个，多个器件用多个SS使能端，不提供引脚重映射，默认一次发送八位bits数据，SS引脚用户单独定义！
		master模式，MSB First，SS低电平选中器件
		SCK空闲时刻为高电平(时钟极性CPOL=1)（默认）：
			若使用第二跳变沿数据被采样(时钟相位CPHA=1)：数据线在 SCK 的偶数边沿采样（默认）
			若使用第一跳变沿数据被采样(时钟相位CPHA=0)：数据线在 SCK 的奇数边沿采样
引脚：
	SPI1->CS	SPI1->CLK	SPI1->MISO	SPI1->MOSI 	—————— 	SPI2->CS	SPI2->CLK	SPI2->MISO	SPI2->MOSI
	PA4			PA5			PA6			PA7					PB12		PB13		PB14		PB15
*/
#define SYSTEM_SPI1_ENABLE		0		/*使能SPI1*/
#define SYSTEM_SPI2_ENABLE		0		/*使能SPI2*/
/*提供API：
	用户自定SS引脚：
		到PeriphConfig.c里面的
			sys_SPI1_SS_io_Init();		自行更改
			sys_SPI2_SS_io_Init();		自行更改
		以及 到PeriphConfig.h里面的
			#define	SPI1_CS PAout(4)	自行更改
			#define	SPI2_CS PBout(12)	自行更改
	写读函数：(目前只写了SPI1，SPI2如果要用的话同理)
		u8 SPI1_ReadWriteByte_8Byte(u8 TxData) 		读写一个字节
		u8 SPI1_ReadWriteByte_16Byte(u8* TxData) 	连写两个字节并读回，传入一个包含2字节的u8*指针
		u8 SPI1_WriteByte_8Byte(u8 TxData)			写一个字节
		u8 SPI1_WriteByte_16Byte(u8* TxData)		连写两个字节，传入一个包含2字节的u8*指针
		例子：
			u8 Max7219_adr_data[Max7219_regNum][2] = 
			{
				{0x09,0xFF},
				{0x0A,0x08},
				...
			}
			for(i = 0;i < Max7219_regNum;i++)
				SPI1_WriteByte_16Byte(Max7219_adr_data[i]);
	
	设置SPI速度函数：参数可选：
					SPI_BAUDRATEPRESCALER_2 SPI_BAUDRATEPRESCALER_4 SPI_BAUDRATEPRESCALER_8 SPI_BAUDRATEPRESCALER_16 
					SPI_BAUDRATEPRESCALER_32 SPI_BAUDRATEPRESCALER_64 SPI_BAUDRATEPRESCALER_128 SPI_BAUDRATEPRESCALER_256
		void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);	//SPI1由APB2(60M)分频得来
		void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);	//SPI2由APB1(30M)分频得来
*/


/*低功耗模式：
睡眠： WFI操作进入，任意中断恢复，CPU时钟关，其他外设时钟维持																		（不可用）
停止：HSI、HSE时钟关，电压调节器开关需设置，任一外部中断唤醒，典型电流值为20uA														（不可用）
待机：HSI、HSE时钟关，电压调节器关，SRAM和寄存器内容丢失，仅电源控制寄存器和备份区域不受影响
		唤醒条件：WKUP按键上升沿，RTC警告事件，复位按键，看门狗复位（！），典型电流值为2uA，为三个模式中最低 	类似于开关机		（可用）
		WKUP 在PA0
*/
#define SYSTEM_StdbyWKUP_ENABLE	1		/*使用待机-低功耗模式（占用0线外部中断）*/
										/*注：看门狗和低功耗待机模式不能同时开启，因为看门狗不能关闭，看门狗复位会唤醒低功耗状态*/
/*当启用 SYSTEM_StdbyWKUP_ENABLE 后，PA0作为WKUP按键，默认长按3秒进入待机状态，再次按下则恢复，进入待机模式函数在PA0的外部中断里*/
//对于PA0，按下为高电平，按下3s后松开，延时一秒（给时间做保存数据等关机前的准备）后进入待机状态（关机），再次按下开机
/*WKUP IO不用外接下拉电阻，在配置时STM32内部已经下拉*/
/*使用说明：
	当启用FreeRTOS时，是循环检测PA0是否为高电平持续3s，需要在任务里放一个检测函数：sys_CheckWKUP_4RTOS()，已经放了
	当使用裸机时，PA0被设置为上升沿外部中断，在外部中断中执行检测高电平是否持续3s，外部中断线0中已经放了
	需要在Sys_Enter_Standby()函数里放一些关机前的准备工作事务
*/



/*hd系列外设，FSMC*/
/*FSMC为16位数据线，分为四个256MB的块（28跟地址线）（地址从0x6000 0000开始，第二个块开始在0x7000 0000，以此类推），每一块分为四个64MB的区（BANK1~4）*/
/*第一个块用于NOR/SRAM，第二、三个块用于NAND，第四个块用于PC卡，块一属于NOR FLASH控制器，块二、三和四属于NAND FLASH控制器*/
/*
一般接法：（CE前面加一个N，表示CE低有效，其他同理）
	FSMC			SRAM					LCD
	NEx				NCE（低选中）			CS（低选中）
	NOE				NOE（低使能输出）		RD（低读）
	NWE				NWE（高读低写）			WR（低写）
											RS（高数据低命令）——可接至FSMC的一个地址脚如A10
	NUB				NUB（低选高八位）
	NLB				NLB（低选低八位）
	A[x:0]			A[x:0]（地址，可乱序）
	D[15:0]									D[15:0]（数据）
*/
/*原子的FSMC驱动LCD历程的解释：时序选择模式A，RS连接到了A10
	typedef struct
	{
		vu16 LCD_REG;	//0111 1111 1110 0X7FE
		vu16 LCD_RAM;	//1000 0000 0000 0X7FE+0x02
	}LCD_TypeDef;
	#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
	#define LCD             ((LCD_TypeDef *) LCD_BASE)
	
	0x7FE为：	0111 1111 1110（因为FSMC选择了16位模式，实际地址脚为0x7FE再右移1）
	右移之后    0011 1111 1111 此为LCD->LCD_REG的 实际地址脚 情况，此时A10为0，LCD->LCD_RAM的地址为LCD->LCD_REG地址+2
	LCD->LCD_REG 的地址脚 0011 1111 1111		A10为0 	RS低地址
	LCD->LCD_RAM 的地址脚 0100 0000 0001		A10为1	RS高数据
	这样命令和数据就区分开了
	
	然后读写：（读写相关的CS、WR和RD脚等的电平由FSMC自动设置）
		写：	LCD->LCD_REG = LCD_Reg;				//要写的寄存器地址 
				LCD->LCD_RAM = LCD_RegValue;		//写入一个16位数据
		读：	LCD_WR_REG(LCD_Reg);				//先写寄存器地址
				vu16 LCD_RegValue = LCD->LCD_RAM;	//再读出一个16位数据
*/
/*可以用FSMC同时管理LCD和SRAM（对于MCU来说，外部FLASH可以用SPI的FLASH当作文件系统使用（如存图、当USB U盘或者IAP等等），不用上NOR/NAND之流，那是对MPU的）
	这是对于不带LTDC LCD外设的如STM32F429以下的如STM32F1和STM32F40x系列而言
    对于STM32F429，可以用LTDC LCD单独驱动LCD
*/
/*
  如果实际使用了大容量系列和100脚以上系列的片子但是没有LTDC，推荐用FSMC同时管理LCD和SRAM*/
#define SYSTEM_FSMC_ENABLE	0				//是否启用FSMC
	#define SYSTEM_FSMC_use4LCD		0		//启用FSMC用于驱动LCD，则相关代码被编译，相关API可用
	#define SYSTEM_FSMC_use4SRAM	1		//启用FSMC用于驱动SRAM，则相关代码被编译，相关API可用
/*
用于LCD的部分：
	  可用API详看TFTLCD.h

用于SRAM的部分：
	这里默认用的 块1（NORSRAM块）的区域3（区域4留给LCD喽~）（块1的区3和区4分别留给外部RAM和LCD，本模板默认，否则还得改地址）
	默认用于驱动IS62WV51216，地址线范围为A0~A18，数据线范围D0~D15，（NUB、NLB、NWE、NOE、NCE五根控制线都对应接好）
	底层API：（不推荐直接调用，使用自实现的malloc和free）（以字节为单位）
		u32 testsram[250000] __attribute__((at(SRAM1_BANK3_ADDR))); 	//地址定义在外部SRAM中，实际不要这样用，所有外部SRAM数据用malloc管理！！
		void FSMC_SRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n); 	//写 参数：数据指针，外部SRAM内的地址(从0开始)，写入的字节数
		void FSMC_SRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n);   	//读 类似上
*/

/*STM32F407ZGT6内存分配：
普通内存：
	主要内存：		地址从：0X2000 0000 开始，共 128KB），这部分内存任何外设都可以访问。
	辅助内存CCM：	地址从：0X1000 0000 开始，共 64KB，这部分内存仅 CPU 可以访问，DMA 之类的不可以直接访问，使用时得特别注意！！
	外部SRAM：		地址从：0X6800 0000 开始，共 1024KB。
	都用内存管理来管理，大块内存使用应尽量使用malloc和free
*/
/*
内存管理：先到malloc.h里面定义有几块RAM，RAM的标志位，和各个RAM给内存管理分配的空间
	应用层API：（分配内存的数量以字节为单位）
		my_mem_init(InrRAM);				//初始化内部内存池（系统自带）
		my_mem_init(ExRAM1);				//初始化外部内存池（如果不开启SYSTEM_FSMC_use4SRAM则没有这句话）
		
		void myfree(u8 memx,void *ptr);  			//内存释放
		void *mymalloc(u8 memx,u32 size);			//内存分配
		void *myrealloc(u8 memx,void *ptr,u32 size);//重新分配内存
*/


/*不同容量的FLASH组织方式：
大容量：256个	2K字节/页*/
#define SYSTEM_FLASH_IAP_ENABLE	0			/*启用对内部FLASH储存空间编程*/
	#define STM32_FLASH_WREN	1			/*启用写功能，否则只读不写*/
	#define FLASH_SAVE_FATFS_SIZE	124		/*内部FLASH给FATFS划分的大小，单位KB*/
	/*以下不用动了*/
	#define STM32F407ZG_FLASH_SIZE 	1024 		/*所选STM32的FLASH容量大小(单位为KB)*/
	#define STM32_FLASH_BASE 0x08000000 		//STM32 FLASH的起始地址
		//设置FLASH 保存地址(必须为页的首地址，且必须为2的倍数！且其值要大于代码所占用FLASH的大小+0X08000000)
	#define FLASH_SAVE_FATFS_ADDR 	(STM32_FLASH_BASE + (u32)((STM32F407ZG_FLASH_SIZE - FLASH_SAVE_FATFS_SIZE)*1024))   //最后124KB留给FATFS作为文件系统（如果使用FATFS的话）
	#define FLASH_SAVE_ADDR  		(STM32_FLASH_BASE + (u32)((STM32F407ZG_FLASH_SIZE - FLASH_SAVE_FATFS_SIZE - 2)*1024)) //留2KB作为参数保存
/*可用API：（下载程序时不要擦除 整个芯片FLASH，只擦除有程序的扇区）
	注：当启用本功能后，在系统初始化序列中将加入读存开机次数的一个用例，一个u16的变量会储存在最后一个或倒数第二个页
	读：	STMFLASH_Read(FLASH_SAVE_ADDR,	(u32*)&StartUpTimes,	getWordSize(StartUpTimes));
	写：	STMFLASH_Write(FLASH_SAVE_ADDR,	(u32*)&StartUpTimes,	getWordSize(StartUpTimes));
*/


/*hd系列外设，六根线（四位数据线，时钟线和命令线），SDIO最高传输速度12M字节每秒，目前只支持用于SD卡*/
/*支持的四种卡：SD2.0 高容量卡（SDHC，最大32G），SD2.0 标准容量卡（SDSC，最大 2G），SD1.x 卡和 MMC 卡）*/
/*
初始化后获取到的卡信息存在：
	1、SD卡信息结构体	HAL_SD_CardInfoTypeDef  SDCardInfo;
			成员：
				uint32_t CardType;             			卡类型：						Specifies the card Type
																CARD_SDSC					SD Standard Capacity
																CARD_SDHC_SDXC				SD High Capacity <32Go, SD Extended Capacity <2To
																CARD_SECURED
				uint32_t CardVersion;                 	卡版本：CARD_V1_X、CARD_V2_X	Specifies the card version
				uint32_t Class;                        	卡级别							Specifies the class of the card class 
				uint32_t RelCardAdd;                  	卡相对地址						Specifies the Relative Card Address
				uint32_t BlockNbr;                     	块数量（存储器内部的事）		Specifies the Card Capacity in blocks
				uint32_t BlockSize;                   	块大小（存储器内部的事）		Specifies one block size in bytes
				uint32_t LogBlockNbr;                 	逻辑块数量						Specifies the Card logical Capacity in blocks
				uint32_t LogBlockSize;                	逻辑块大小（扇区数目）			Specifies logical block size in bytes
				注：我们只需要关注逻辑块和逻辑块大小即可，其他是SD卡内部的事情，逻辑块和逻辑扇区一回事，一个逻辑块为了保证兼容都是512字节大小
					BLOCK SIZE ： 8 即每个BLOCK有8个扇区（这个是存储器内部的事，用户不关心）
					也就是我们只关心：
						SECTOR SIZE ：一般为512字节
						SECTOR COUNT：等于总容量字节数/512
	
	2、SD卡制造CID		HAL_SD_CardCIDTypeDef SDCard_CID;
			成员：
				__IO uint8_t  ManufacturerID;  制造商ID		Manufacturer ID
				__IO uint16_t OEM_AppliID;     OEM/Application ID
				__IO uint32_t ProdName1;       Product Name part1
				__IO uint8_t  ProdName2;       Product Name part2
				__IO uint8_t  ProdRev;         Product Revision
				__IO uint32_t ProdSN;          Product Serial Number
				__IO uint8_t  Reserved1;       Reserved1
				__IO uint16_t ManufactDate;    Manufacturing Date
				__IO uint8_t  CID_CRC;         CID CRC
				__IO uint8_t  Reserved2;       Always 1
*/
/*
引脚：
PC8			SDIO_D0
PC9			SDIO/D1
PC10		SDIO_D2
PC11		SDIO_D3
PC12		SDIO_CK
PD2			SDIO_CMD
*/
/*备注：可用开发用DMA读写SD卡，这样更省时间，读写的时候就不用关中断了，更新：但是DMA的话还有中断处理，数据量大的话易前后混淆，增加程序复杂度，算啦*/
#define SYSTEM_SDIO_SD_ENABLE	0
/*底层API：（不推荐直接读写！要用文件系统FATFS按照文件读写，并且SDIO的SD初始化就在FATFS初始化里）
	一个块的大小：SDCardInfo.LogBlockSize
	SD卡块的数量：SDCardInfo.LogBlockNbr
	u8 SD_ReadDisk(buf,secaddr,seccnt);			//读取从第secaddr块开始的seccnt个块的内容，返回地址到buf（大概率一个块为512KB，buf必须先准备好足够空间）
												//返回 HAL_OK （值为0） 为成功，其他为失败
														
	u8 SD_WriteDisk(buf,secaddr,seccnt);		//把buf内的内容从secaddr块开始写入，连续写seccnt个块
												//返回 HAL_OK （值为0） 为成功，其他为失败
	
	void show_sdcard_info(void);				//通过串口1打印SD卡相关信息
	
	计算SD卡容（单位为字节）：
		uint64_t CardCap = (uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);	 //计算SD卡容量（单位为字节）
		CardCap>>20 //此为转换为MB单位
*/

/*使用FATFS文件系统管理SD卡，FATSF支持FAT12/FAT16/FAT32/exFAT，官网（含API说明）：http://elm-chan.org/fsw/ff/00index_e.html*/
/*配置相关：在 ffconf.h 文件里
	注意：	默认没有支持相对路径，如需可在FATFS配置文件内打开
			默认配置支持4个文件设备，默认使用UTF-8编码
	配置：	定义有哪些设备，比如DEV_ExFLASH（外部FLASH，MMC/NAND/SPI FLASH等）、DEV_SD、DEV_InrFlash（内部FLASH）、DEV_USB等等
			在diskio.c里面定义读写以上这些设备的底层函数：（底层函数，应用层不应调用）
				DSTATUS disk_initialize (BYTE pdrv);
				DSTATUS disk_status (BYTE pdrv);	//由于存储设备自己实现的读写函数内部含有忙判断，所以这个获取设备状态函数不用
				DRESULT disk_read (BYTE pdrv, BYTE* buff, LBA_t sector, UINT count);
				DRESULT disk_write (BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count);
				DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);
	配置：	在ffsystem.c里面已经用自实现的内存分配和释放函数，默认用STM32内部的RAM，如要使用外部RAM可去手动改为 ExRAM1
	配置：	FATFS的配置文件里有一个没有完全配置好：ffconf.h里在最后的RTOS支持
*/
#define SYSTEM_FATFS_ENABLE	0 //TODO:FATFS初始化还没有放到系统初始化序列，等原子的API移植好了，这个宏用于控制是否初始化和相关API的编译控制
/*初始化步骤：
	要做的：在初始化系列里加上FATFS初始化SD卡
			先为每个文件设备句柄申请空间
			挂载
			
			更多文件操作可用参考原子FATFS历程里的 fattester.c 和 exfuns.c 里面的获取剩余空间、识别文件类型、得到文件夹大小、文件和文件夹复制等有用的API等等
			
			FATFS可用API：参考原子的，罗列下来API，参考官方文档等
*/



/*_____________系统变量和函数（莫要乱动撒）_______________*/
void sys_ENswd_DISjtag(void);
extern u16	StartUpTimes;					/*用于保存开机次数，储存在最后一个或倒数第二个页*/
extern uint32_t UIDw[3]; 					/*保存STM32内部UID识别码，全球唯一识别码*/
extern uint32_t sysCoreClock; 				/*获取HCLK频率，外设时钟均来自此再分频*/

/*蜂鸣器连接在TIM3的通道4上*/
extern u8 is_buzzer_once;
extern u8 is_buzzer_bibi;
#define buzzer_bibi_once	is_buzzer_once = 1;  //用户使用，蜂鸣器叫唤一声
#define buzzer_bibi_on		is_buzzer_bibi = 1;  //用户使用，蜂鸣器间歇叫唤开启
#define buzzer_bibi_off		is_buzzer_bibi = 0;  //用户使用，蜂鸣器间歇叫唤关闭

void delay_us(unsigned int time);
u16 sys_GetsysRunTime(u16* mins,u16* secs,u16* _10ms);/*提供获取系统运行时间的函数，具体看源函数处注释*/

void sys_MCU_Init_Seq(void);				/*MCU外设初始化序列，所有初始化写到这里面*/
void sys_Device_Init_Seq(void);				/*器件外设初始化，并开机自检*/

void FaultASSERT(char* FaultMessage);		/*表示初始化有问题，串口提示，指示灯或者蜂鸣器进行提示*/
u8 Stm32_Clock_Init(void);					/*时钟系统配置*/


/*_____________核心外设变量和函数（莫要乱动撒）_______________*/
/*_______________________________时基定时器4___________________________________*/
#if STSTEM_TIM4_ENABLE

	__packed struct TIM4_IT_FLAGS
	{
		u8 _10msec_flag;	//10毫秒标志和计数
		u16 _10msec;
		
		u8 _100msec_flag;	//100毫秒标志和计数
		u16 _100msec;
		
		u8 _300msec_flag;	//300毫秒标志
		
		u8 _1sec_flag;		//1秒标志和计数
		u16 _1sec;
		
		u8 _1min_flag;		//1分钟标志和计数
		u16 _1min;
	};
	extern struct TIM4_IT_FLAGS Timer_IT_flags;
	extern TIM_HandleTypeDef TIM4_Handler;      //通用定时器4句柄 
	void sys_TIM4_ENABLE(void);
#endif

/*_______________________________辅助PWM定时器3___________________________________*/
#if STSTEM_TIM3PWM_ENABLE
	extern TIM_HandleTypeDef 	TIM3_Handler;      	//定时器3句柄
	extern TIM_OC_InitTypeDef 	TIM3_CH1Handler,TIM3_CH2Handler,TIM3_CH3Handler,TIM3_CH4Handler;	//定时器3通道1-4句柄
	#define TIM3PWM_Channel_1	1
	#define TIM3PWM_Channel_2	2
	#define TIM3PWM_Channel_3	3
	#define TIM3PWM_Channel_4	4
	void sys_TIM3PWM_ENABLE(void);
	void TIM3_set_Channel_Pulse(u8 channel,float percent);
#endif


/*_______________________________串口们___________________________________*/
#if SYSTEM_UART1_ENABLE||SYSTEM_UART2_ENABLE||SYSTEM_UART3_ENABLE
	
	#define USART1_RX_BUF_MaxNum 	200 	/*定义用协议0只接受以'\r\n'结尾的数据时可以存在字符数组里面的最大字节数目*/
	#define USART1_RX_FIFO_MaxNum	200		/*定义用协议1用FIFO存储数据时可以存的最大字节数目*/
	
	#define UART1	1
	#define UART2	2
	#define UART3	3
	#define RXBUFFERSIZE   1 					//缓存大小
	extern u8 aRxBuffer1[RXBUFFERSIZE],aRxBuffer2[RXBUFFERSIZE],aRxBuffer3[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
	extern UART_HandleTypeDef UART1_Handler,UART2_Handler,UART3_Handler; //UART句柄
	void sys_USART1_ENABLE(void);
	void sys_USART2_ENABLE(void);
	void sys_USART3_ENABLE(void);
	
	
	#define USART1_RX_DONE_mask 0x8000
	#define USART1_RX_MODE_mask 0x4000
	#define USART1_RX_Rec_r_mask 0x2000
	#define USART1_RX_Num_mask 0x1fff /*0001 1111 1111 1111*/
	
	extern u16 USART1_RX_CONFIG;	/*    x            x           x        x xxxx xxxx xxxx      */
									/*(接受完成) (协议模式0/1) (接收到\r)   串口1接受数据字节计数 */
									/*串口1接受协议：0为只接受以'\r\n'结尾的数据，1为以FIFO先进先出实现接受区，无协议*/
	
	#define USART1_SetDone 		USART1_RX_CONFIG |= (USART1_RX_DONE_mask) /*设置串口1接收完成标志位*/
	#define USART1_SetUnDone 	USART1_RX_CONFIG &= (USART1_RX_MODE_mask) /*在任务中处理完后对串口1标志位进行复位，除了mode位，其他位都写0*/
	#define USART1_SetMode(x) 	USART1_RX_CONFIG |= (((u16)x)<<14) 		/*用户可用，在任务中设置串口1接收协议*/
	#define USART1_isDone 		((USART1_RX_CONFIG) & (USART1_RX_DONE_mask))/*用户用，用于判断是否接受完成一次*/
	#define USART1_RX_ByteNum 	((USART1_RX_CONFIG) & (USART1_RX_Num_mask))/*用户可用，返回接收的字符数量*/
	
//	extern QueueHandle_t Uart1_fifo;
	u8 sys_USART1_RX_Fetch(u8 is_print, char *buf,u16 *RX_ByteNum);
	u8 sys_USART2_RX_Fetch(u8 is_print, char *buf,u16 *RX_ByteNum);
	u8 sys_USART3_RX_Fetch(u8 is_print, char *buf,u16 *RX_ByteNum);
	
	
#endif

/*_______________________________CAN1___________________________________*/
#if SYSTEM_CAN1_ENABLE
	u8 CAN1_Receive_Msg(u8 *buf);
	u8 CAN1_Send_Msg(u8* msg,u8 len);
#endif
/*_______________________________MCO___________________________________*/
#if SYSTEM_MCO_PA8_OUT
	void sys_MCO_Out_Enable(void);				/*MCO在PA8上输出启动，默认时钟源HSE*/
#endif
/*_______________________________IWDG___________________________________*/
#if SYSTEM_IWDG_ENABLE
	extern IWDG_HandleTypeDef IWDG_Handler; //独立看门狗句柄
	void sys_IWDG_ENABLE(void);
	void IWDG_Feed(void);
#endif

/*_______________________________CRC___________________________________*/
#if SYSTEM_CRC_ENABLE
	#define BUFFER_SIZE    114
	extern CRC_HandleTypeDef hcrc;
	extern const uint32_t aDataBuffer[BUFFER_SIZE];
	extern uint32_t uwExpectedCRCValue;
	void sys_CRC_ENABLE(void);
	void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc);
#endif
/*_______________________________HASH_RNG___________________________________*/
#if SYSTEM_RNG_ENABLE
	extern RNG_HandleTypeDef RNG_Handle;
	void sys_RNG_ENABLE(void); 
#endif
/*_______________________________RTC___________________________________*/
#if SYSTEM_RTC_ENABLE
	extern RTC_HandleTypeDef RTC_Handler;  //RTC句柄

	void sys_RTC_Enable(void);						/*RTC配置*/

#endif

/*_______________________________定时器2___________________________________*/
#if STSTEM_TIM2_ENABLE
	extern TIM_HandleTypeDef TIM2_Handler;
	void sys_TIM2_ENABLE(void); //写在预编译外面为了外部文件能够调用
	#if (STSTEM_TIM2_asPWMorCap == 0)
		/*PWM私有变量*/
		#define TIM2PWM_Channel_1	1
		#define TIM2PWM_Channel_2	2
		#define TIM2PWM_Channel_3	3
		#define TIM2PWM_Channel_4	4
		extern TIM_OC_InitTypeDef 	TIM2_CH1Handler,TIM2_CH2Handler,TIM2_CH3Handler,TIM2_CH4Handler;
		void TIM2_set_Channel_Pulse(u8 channel,float percent);
	#elif (STSTEM_TIM2_asPWMorCap == 1)
		/*输入捕获私有变量*/
		extern u8  	TIM2CHx_CAPTURE_STA;	    				
		extern u16	TIM2CHx_CAPTURE_VAL;
		float Peek_TIM2_Cap_Val(void);
		extern TIM_IC_InitTypeDef TIM2_CHxConfig;
		void Process_TIM2_IC_CallBack_Channel_1(void);
		void Process_TIM2_IC_CallBack_Channel_2(void);
		void Process_TIM2_IC_CallBack_Channel_3(void);
		void Process_TIM2_IC_CallBack_Channel_4(void);
	#elif (STSTEM_TIM2_asPWMorCap == 3)
		/*正交解码私有变量*/
		extern TIM_Encoder_InitTypeDef sEncoderConfig;
		extern int32_t EncoderOverflowCount;//定时器溢出次数
		float peek_TIM2_Encoder_Speed(void);
		int32_t peek_TIM2_Encoder_Value(void);
	#endif
#endif

/*_______________________________TIM1PWM___________________________________*/

#if STSTEM_TIM1PWM_ENABLE
		void sys_TIM1PWM_ENABLE(void);
		extern TIM_HandleTypeDef TIM1_Handler;
		#define TIM1PWM_Channel_1	1
		#define TIM1PWM_Channel_2	2
		#define TIM1PWM_Channel_3	3
		#define TIM1PWM_Channel_4	4
		void TIM1_set_Channel_Pulse(u8 channel,float percent);
#endif

/*_______________________________TIM8PWM___________________________________*/

#if STSTEM_TIM8PWM_ENABLE
		extern TIM_HandleTypeDef TIM8_Handler;
		void sys_TIM8PWM_ENABLE(void);
		#define TIM8PWM_Channel_1	1
		#define TIM8PWM_Channel_2	2
		#define TIM8PWM_Channel_3	3
		#define TIM8PWM_Channel_4	4
		void TIM8_set_Channel_Pulse(u8 channel,float percent);
#endif

/*_______________________________TIM6___________________________________*/

#if STSTEM_TIM6_ENABLE
	void sys_TIM6_ENABLE(void);
	extern TIM_HandleTypeDef TIM6_Handler;
#endif
/*_______________________________TIM7___________________________________*/

#if STSTEM_TIM7_ENABLE
	void sys_TIM7_ENABLE(void);
	extern TIM_HandleTypeDef TIM7_Handler;
#endif


/*_______________________________ADC1___________________________________*/

#if SYSTEM_ADC1_ENABLE
	//void ADC_RegularChannelConfig(ADC_HandleTypeDef *AdcHandle, uint32_t Channel, uint32_t Rank, uint32_t SamplingTime);
	//u16 Get_Adc(u32 ch);
	#if SYSTEM_ADC1_useScan
		extern DMA_HandleTypeDef  ADC1rxDMA_Handler;
		void ADC_DMA_Cfg(void);
	#endif
	
	extern ADC_HandleTypeDef ADC1_Handler;
	void sys_ADC1_ENABLE(void);
	u32 Get_Adc_Average(u32 ch,u8 times);
	float Get_Temprate(u32 adcx);
#endif

/*_______________________________SPI1、2________________________________*/

#if (SYSTEM_SPI1_ENABLE) || (SYSTEM_SPI1_ENABLE)
extern SPI_HandleTypeDef SPI1_Handler;  //SPI1句柄
extern SPI_HandleTypeDef SPI2_Handler;  //SPI2句柄
void sys_SPI1_ENABLE(void);
void sys_SPI2_ENABLE(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);

//写读8字节
u8 SPI1_ReadWriteByte_8Byte(u8 TxData);
//写读16字节，传入一个包含2字节的u8*指针
u8 SPI1_ReadWriteByte_16Byte(u8* TxData);
//写8字节
u8 SPI1_WriteByte_8Byte(u8 TxData);
//写16字节，传入一个包含2字节的u8*指针
u8 SPI1_WriteByte_16Byte(u8* TxData);

u8 SPI2_ReadWriteByte(u8 TxData);

void sys_SPI1_SS_io_Init(void);
void sys_SPI2_SS_io_Init(void);

#endif
/*_________________________________低功耗StandbyMode________________________________*/
#if SYSTEM_StdbyWKUP_ENABLE
	void sys_StdbyWKUP_ENABLE(void);
	void sys_CheckWKUP_4RTOS(void);
	#define WKUP_KD HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //PA0 检测是否外部WK_UP按键按下
	u8 Check_WKUP(void);  			//检测WKUP脚的信号
	void Sys_Enter_Standby(void);	//系统进入待机模式
#endif

/*____________________________FLASH编程___________________________________*/

#if SYSTEM_FLASH_IAP_ENABLE
	
	void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//从指定地址开始读出指定长度的数据
	#define getWordSize(x)		sizeof(x)/4 + ((sizeof(x) % 4) ? 1 : 0)
	
	#if STM32_FLASH_WREN	//如果使能了写   
		#define FLASH_WAITETIME  50000          //FLASH等待超时时间

		//FLASH 扇区的起始地址
		#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
		#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
		#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
		#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
		#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
		#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
		#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes  
		#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
		#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
		#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
		#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
		#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes 
	 
		void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		//从指定地址开始写入指定长度的数据
	#endif
	
#endif

/*_____________________________________DAC________________________________________*/

#if ((SYSTEM_DAC_OUT1_ENABLE) || (SYSTEM_DAC_OUT2_ENABLE))
	extern DAC_HandleTypeDef DAC1_Handler;//DAC句柄
	void sys_DAC_ENABLE(void);
	//void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac);
	void DAC_Set_Ch1_Vol(float vol);
	void DAC_Set_Ch2_Vol(float vol);
#endif



/*____________________SDIO SD_____________________________________________*/

#if SYSTEM_SDIO_SD_ENABLE
	extern HAL_SD_CardInfoTypeDef  	SDCardInfo;              	//SD卡信息
	extern HAL_SD_CardCIDTypeDef	SDCard_CID;					//SD卡CID信息
	extern SD_HandleTypeDef        	SDCARD_Handler;     		//SD卡句柄
	#define SD_TIMEOUT 			((uint32_t)100000000)  			//超时时间

	u8 SD_Init(void);
	u8 SD_ReadDisk(u8* buf,u32 sector,u32 cnt);
	u8 SD_WriteDisk(u8 *buf,u32 sector,u32 cnt);
	void show_sdcard_info(void);						//通过串口1打印SD卡相关信息

#endif

/*____________________________FSMC for SARM_____________________________________________*/
#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4SRAM))

	extern SRAM_HandleTypeDef SRAM_Handler;    //SRAM句柄
	//使用NOR/SRAM的 Bank1.sector3,地址位HADDR[27,26]=10 
	//对IS61LV25616/IS62WV25616,地址线范围为A0~A17 
	//对IS61LV51216/IS62WV51216,地址线范围为A0~A18
	#define SRAM1_BANK3_ADDR    ((u32)(0x68000000))	//计算： (0x6000 0000 | ((u32)64*1024*1024*(3 - 1)))
	
	void sys_FSMC_SRAM_ENABLE(void);
	void FSMC_SRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n);
	void FSMC_SRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n);
	
#endif


/*____________________________FSMC for LCD_____________________________________________*/
#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4LCD))
	extern SRAM_HandleTypeDef TFTSRAM_Handler;    //SRAM句柄(用于控制LCD)

	void LCD_with_FSMC_init_FSMC(void);
	//LCD地址结构体
	typedef struct
	{
		vu16 LCD_REG;
		vu16 LCD_RAM;
	} LCD_TypeDef;
	//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 
	//注意设置时STM32内部会右移一位对其! 			    
	#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
	#define LCD             ((LCD_TypeDef *) LCD_BASE)
#endif


#define Version_of_stm32_framework "2.4"

/*Author:		MaxwellXyao*/
#define BIT(n) 					(1 << n) 				//位mask
#define bit_SetTrue(x,mask) 	(x |= mask)				//该位设置为真
#define bit_SetFalse(x,mask)	(x &= ~mask)			//该位设置为假
#define bit_Toggle(x,mask) 		(x ^= mask)				//位切换
#define bit_IsTrue(x,mask) 		((x & mask) != 0)		//该位是否真
#define bit_IsFalse(x,mask) 	((x & mask) == 0)		//该位是否假

//以下为汇编函数
void WFI_SET(void);				//执行WFI指令
void INTX_DISABLE(void);		//关闭所有中断
void INTX_ENABLE(void);			//开启所有中断
void MSR_MSP(uint32_t addr);	//设置堆栈地址

#endif




