#ifndef __SYS_H
#define __SYS_H

#include "sysVar.h"								/*定义系统级常用的变量、数据类型和二进制表示宏*/
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"						/*在里面的stm32f1xx_hal_conf.h里面选择用哪些外设的HAL库——————！按需要进行修改！*/
#include "PeriphConfig.h"
#include "FIFO.h"								/*用Github上面的一个开源仓库，
												一个实现对任何形式数据的环形缓存，默认已经用于串口接收，参考程序看.h文件内
												开源仓库地址：https://github.com/geekfactory/FIFO.git*/
#include "malloc.h"								/*借鉴正点原子的 内存管理 章节的源代码自实现的malloc和free*/				
												
												
												
/*用户设备驱动和APP文件调用*/
#include "TFTLCD.h"
												
												

/*
编程规范：
	No.1：紧凑，通用，可迭代，高内聚低耦合，广泛借鉴

*/

/*
命名规范：（临时）
	0、函数命名规则为“属什么是什么做什么”，如：sys_irq_disable()
	1、系统使用外设功能的启用与否均用宏定义SYSTEM_SUPPORT_XX
	2、RTOS任务函数均使用xx_task()命名
	3、任务函数与任务名称一致，任务索引与函数一致，只是task部分简写为t
	4、函数注释规范如下：*/
	
	/*__________函数简述_____________*/
	/********************************
	*描述：函数详细描述——————！按需要进行修改！
	*参数：		1、...
				2、...
	*返回值：	1、NULL
	********************************/
	
	/*
	5、函数返回值0表示成功，非0表示失败，此非零数可以表示错误代码
	6、一类或者一个模块内的变量尽量用结构体打包，多组数据就用结构体数组
	7、MCU外设统称"外设"，即Periph，用户的MCU外围芯片和设备统称"器件"或"设备",即devices
	*/
	
/*应用规范：（临时，待整理）
	00、工程内现已经不含string、stdio和stdlib等不适合用于mcu平台的头文件，已经都用开源的线程安全软件或者自实现替换
	0、中断优先级分组选用分组4，即16级抢占优先级，0级响应优先级（即不用）
	1、IO尽量设计为低电平有效，高电平截止；按键IO尽量使用外部中断。
	2、使用C99模式编译！
	3、I2C使用软件实现
	4、FreeRTOS所有组件始终使用动态申请内存；FreeRTOS的配置文件除了用户按需更改区域，尽量不要动；一般不使用软件定时器，用硬件定时器或者周期延时替代即可
	5、尽量减少数据传输过程中的拷贝，常用指针、结构体指针当传输数据
	6、调用函数尽量不用NULL实参，尽量给一个具体的值
	7、外设如串口、PWM等的IO初始化在其初始化函数内，不用单独再初始化
	8、函数返回值：0表正确，正数表错误并带返回信息，并尽量用HAL类型返回值如：
		  HAL_OK       = 0x00U,
		  HAL_ERROR    = 0x01U,
		  HAL_BUSY     = 0x02U,
		  HAL_TIMEOUT  = 0x03U
	9、FSMC的块1的区3和区4分别留给外部RAM和LCD，本模板默认的，慎改
	10、把本模板各种外设中默认的东西都罗列在这里
		
*/

/*坑说明：
	1、由于历史遗留原因，本MDK工程编码位GB2312，所以用外部文件写程序时请选用GB2312编码来写
		但是如果已经使用UTF-8等编码编写了很多内容时，只需用notepad软件新建一个新文件，选择GB2312编码，然后复制原来UTF-8等编码格式的原文进来，就自动转为GB2312编码了
		
		
*/

/*
STM32F10xx系列命名规范：

STM32F
	  101 = 		基本型
	  102 = 		USB基本型， USB 2.0全速设备
	  103 = 		增强型
	  105或107 = 	互联型
			  T = 36脚
			  C = 48脚
			  R = 64脚
			  V = 100脚
			  Z = 144脚
			  I = 176脚
			   4 = 16KB(ld)
			   6 = 32KB(ld)
			   8 = 64KB(md)
			   B = 128KB(md)
			   C = 256KB(hd)
			   D = 384KB(hd)
			   E = 512KB(hd)
			   G = 1024KB(hd)
			    H = BGA封装
				T = LQFP封装
				 6 = -40~85℃
			FLASH               RAM
			
			16K(ld)				6K			ld:2个USART，3个16位定时器+1个16位电机控制定时器，1个SPI，1个I2C，USB，CAN，1个高级定时器，2个ADC，DMA1（7通道）
			32K(ld)				10K
			
			64K(md)				20K			md:3个USART，4个16位定时器+1个16位电机控制定时器，2个SPI，2个I2C，USB，CAN，1个高级定时器，2个ADC（通道数不定）
			128K(md)			20K						(x8/xB系列有1、2、3、4定时器)，DMA1（7通道）
			
			256K(hd)			48K或64K	hd:3+2个串口，8个以上16位定时器+2个以上16位电机控制定时器，2个基本定时器，3个SPI，2个I2S，2个I2C，USB，CAN，
			384K(hd)			64K				2个高级定时器，3个ADC（通道数不定），2个12bit DAC，1个SDIO，FSMC(100和144脚封装)，DMA1（7通道）、2（5通道）
			512K(hd)			64K
*/

/*
启动：	BOOT1	BOOT0		启动区域
		  x		  0			FLASH
		  0		  1			系统储存器(ISP下载)
		  1		  1			SRAM
*/

/*
需要 256K(hd) 及以上IC的外设列表：
	SDIO
	DAC
	FSMC
	DMA2
	
	在STM32F103系列之间切换容量或者芯片时，需要配置以下几项：
	1、在 Device 里改芯片型号
	2、在 C/C++ 里面改 Symbols 到相应芯片型号，可选：
		STM32F103x6 —— STM32F103xB —— STM32F103xE ——  STM32F103xG 
	3、（第一步完成后这里就自动改了，再检查一下）在 Debug 里面的 下载Settings 里面的 Flash Download 里面改相应的小、中大容量下载算法
	4、在当前工程的CORE文件夹里 只保留 相应的startup.s文件，对于中容量为startup_stm32f103xb.s，以此类推
*/

#define SYSTEM_SUPPORT_OS		0				/*定义是否使用FreeRTOS，不是0就是1——————！按需要进行修改！
													FreeRTOS版本：v10.3.1
													默认用于任务的RAM堆栈大小为5KB，按需修改！
												注意：还要在FreeRTOSConfig.h里面的SYSTEM_SUPPORT_OS宏定义与此处保持一致，否则系统不会调用FreeRTOS的任务切换SVC中断函数，跑不起来！*/

#define SYSTEM_SUPPORT_Menu		1				/*模板固定搭配！提供一个菜单模板，把系统的输入、输出、执行功能的标志位控制全部打包！*/
/*
	以下两个函数推荐100ms周期运行
	keyProcess(); 	//获取键值输入函数；对于RTOS，直接放一个100ms周期任务；对于裸机，100ms周期运行（仅裸机已经放了） ！输入可以为独立按键、五项按键、AD Key、触摸按键、无线遥控、旋转编码器等
	menuProcess();	//菜单处理的主函数；同上
*/
#define SYSTEM_SUPPORT_MyString	1				/*提供一个实现了string.h大部分字符操作函数的库*/
/*具体作用看MyString.c文件里的注释
int mystrlen (const char *str);
char * mystrcat (char *dest, const char *src);
char * mystrncat (char *dest, const char *src, int n);
char * mystrcpy (char *dest, const char *src);
char * mystrncpy (char *dest, const char *src, int n);
int mystrcmp (const char *s1, const char *s2);
int mystrncmp (const char *s1, const char *s2, int n);
void * mymemset (void *s, int c, unsigned n);
void *mymemcpy(void *des,const void *src,size_t len);
char *mystrtok(char *s, const char *delim);
*/
int myatoi(const char *str);					/*提供一个字符串转整形的实现*/
u16 sys_GetsysRunTime(u16* mins,u16* secs,u16* _10ms);/*提供获取系统运行时间的函数，具体看源函数处注释*/
#define SYSTEM_SUPPORT_sprintf	1				/*模板固定搭配！包含且编译printf.h，github开源文件，无依赖，功能比较全。
													约占6KB，对于stm32够，对于其他小容量MCU则看“其他几个sprintf实现”文件夹里面的,不要纠结了。
													开源仓库地址：https://github.com/mpaland/printf
													提供API：
													int printf(const char* format, ...); 								不用
													int printf_uart(unsigned char uart,const char* format, ...); 		用，可选发送串口
																		串口选择可选：UART1/UART2/UART3，需要先使能相关串口
													int sprintf(char* buffer, const char* format, ...); 				不带字节数量限制
													int snprintf(char* buffer, size_t count, const char* format, ...); 	带字节数量限制，更安全
												注意：另在SYSTEM文件夹内提供strprintf.h，功能比较单一，但很小。
												*/
#define SYSTEM_SUPPORT_pid		0				/*提供一个pid算法实现库，集成了积分分离和变限积分，以及可选的不完全微分和微分先行，具体用法看pid.h里面*/
												/*里面另提供一个一阶低通滤波实现函数*/
												/*一阶低通滤波器 float FirstOrderLPF(float NewValue);*/
												

/*所有main的头文件都放在这里*/
#if SYSTEM_SUPPORT_OS
	#include "FreeRTOS.h"	//FreeRTOS使用,这里不要乱动	  
	#include "task.h"
	#include "TaskConfig.h"
#else
	#include "BareConfig.h"
#endif

#if SYSTEM_SUPPORT_Menu
	#include "sys_menu.h"
#endif

#if SYSTEM_SUPPORT_MyString
	#include "MyString.h"
#endif

#if SYSTEM_SUPPORT_sprintf
	#include "printf.h"
#endif

#if SYSTEM_SUPPORT_pid
	#include "pid.h"
#endif

/*_____MCU外设启用选择函数_____*/
#define SYSTEM_MCO_PA8_OUT		0				/*设置PA8为MCO输出，默认时钟源为HSE*/

/*配置使用RTC，确保LSE连接有36.768kHz的晶振，确保RTC有VBAT备用电源
说明：	按需要在配置函数内设置：时间，闹钟，读存BKP
		默认开启RTC闹钟中断，系统在待机模式下可选被闹钟唤醒
API:
	RTC_Set(2049,10,1,17,7,0); 				//设置RTC日期和时间，年月日时分秒，2049年10月1日，17点07分0秒
	RTC_Alarm_Set(2149,10,1,17,7,0);		//设置RTC闹钟日期和时间，年月日时分秒，2149年10月1日，17点07分0秒
	
	typedef struct //时间结构体
	{
		vu8 hour;
		vu8 min;
		vu8 sec;			
		//公历日月年周
		vu16 w_year;
		vu8  w_month;
		vu8  w_date;
		vu8  week;	
	}_calendar_obj;					 
	extern _calendar_obj calendar;				//用户用！RTC结构体，实际用时只需要读这个结构体获取时间即可
*/
#define SYSTEM_RTC_ENABLE		0				
/*配置使用CRC循环冗余校验
这个CRC计算模块使用常见的、在以太网中使用的计算多项式：
X32 + X26 + X23 + X22 + X16 + X12 + X11 + X10 +X8 + X7 + X5 + X4 + X2 + X + 1
写成16进制就是：0x04C11DB7
API：参数：const uint32_t aDataBuffer[BUFFER_SIZE]; #define BUFFER_SIZE    114
	HAL_CRC_Accumulate(&hcrc, (uint32_t *)aDataBuffer, BUFFER_SIZE); //返回值为uint32_t的计算结果*/
#define SYSTEM_CRC_ENABLE		1
/*
F103系列有以下8个定时器：其中x8/xB系列仅有1、2、3、4定时器，xE和以上有全八个。
高级定时器1、8（定时器中断，
	高级PWM（7路，具体在xxx引脚，重映射在xxx引脚，形式如HAL F1手册的225页的表）），
通用定时器2-5（2、5为32位）
	（4路，具体在xxx引脚，重映射在xxx引脚），定时中断，输入捕获，输出比较，PWM 生成(边缘或中间对齐模式)，单脉冲模式输出，正交编码中断
基本定时器6、7（仅定时器中断，无PWM等）
*/
#define STSTEM_TIM4_ENABLE		1			/*模板基本功能，必须开启！使用通用定时器4，提供时基，默认10ms周期中断*/
/*提供用于同步和计时的时基：
	Timer_IT_flags._10msec_flag		//10毫秒标志
	Timer_IT_flags._100msec_flag	//100毫秒标志
	Timer_IT_flags._300msec_flag	//300毫秒标志
	Timer_IT_flags._1sec_flag		//1秒标志
	Timer_IT_flags._1min_flag		//1分钟标志
	
	Timer_IT_flags._10msec			//10毫秒计数
	Timer_IT_flags._100msec			//100毫秒计数
	Timer_IT_flags._1sec			//1秒计数
	Timer_IT_flags._1min			//1分钟计数
*/

/*PWM即输出比较，基本原理就是，每个定时器都有唯一一个CNT计数值，随着时钟源加一，到了Period值就会产生溢出中断即定时中断，
PWM就是四个通道由四个独立的比较值，每个比较值与这个CNT计数值比较，从而产生四路独立的PWM*/
#define STSTEM_TIM3PWM_ENABLE	1			/*模板基本功能，必须开启！使用通用定时器3，默认不开启中断，提供四路PWM输出*/
											/*其中一路默认给蜂鸣器，蜂鸣器低电平有效！蜂鸣器控制需要用到TIM4！蜂鸣器连接在通道4上！
											改变占空比可调响度，改变频率可调声调*/
											/*默认：引脚上拉，输出极性为LOW，PWM1模式			(CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1)*/
		#define STSTEM_TIM3PWM_REMAP_PARTIAL	0	/*定时器3部分引脚重映射，Partial remap  	(CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)*/
		#define STSTEM_TIM3PWM_REMAP_ENABLE		0	/*定时器3全部引脚重映射，Full remap     	(CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9)*/
		#define STSTEM_TIM3PWM_CHANNEL_ENABLE	B0000_1000 /*输出通道选择，共四个通道，通道四必打开*/
													/*可选  B0000_0001|B0000_0010|B0000_0100|B0000_1000	*/
		#define STSTEM_TIM3PWM_TI_ENABLE		0	/*是否开启定时器3的定时中断，除非急需用，否则一般不开*/
		
		
		#define tim3prsc STSTEM_TIM3PWM_Period_5K		/*选择定时器输出频率，以下六条语句基本不动（重装值为720，这里是选择预分频系数）*/
			#define STSTEM_TIM3PWM_Period_2K	(50-1)
			#define STSTEM_TIM3PWM_Period_5K	(20-1)
			#define STSTEM_TIM3PWM_Period_10K	(10-1)
			#define STSTEM_TIM3PWM_Period_20K	(5-1)
			#define STSTEM_TIM3PWM_Period_50K	(2-1)
			#define tim3arr 720-1
		/*可用API：
					HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);		开启TIM3的PWM通道2
					HAL_TIM_PWM_Stop(&TIM3_Handler,TIM_CHANNEL_2);		关闭TIM3的PWM通道2，但看源码好像使所有通道都关闭了，待实验
					
					设置TIM3的PWM通道2的占空比百分数为88.8%，值需在0~100.0之间。默认向上计数，默认设置为当计数值小于此值时输出低电平。
					TIM3_set_Channel_Pulse(TIM3PWM_Channel_2,88.8);
					
					buzzer_bibi_once;			用户使用，蜂鸣器叫唤一声
					define buzzer_bibi_on;		用户使用，蜂鸣器间歇叫唤开启
					define buzzer_bibi_off;		用户使用，蜂鸣器间歇叫唤关闭
		*/


/*通过用定时器2：16位，四个独立通道可用于：输入捕获、输入比较、PWM、单脉冲，多种途径触发DMA中断*/
#define STSTEM_TIM2_ENABLE		0			/*通用定时器2，功能自定，默认分频系数为72，初始化函数在PeriphCconfig.c里面定义*/
	#define STSTEM_TIM2_TI_ENABLE	1		/*是否开启定时器2的定时中断*/
	
	#define STSTEM_TIM2_asPWMorCap	2		/*选择定时器2作为...注：PWM(输出比较)、输入捕获和正交解码三个功能不能共用！*/
											/*写3作为正交编码器的解码使用，只能使用CH1和CH2，即PA15和PB3，默认使用两路边沿触发，计数值为单路上升沿数的四倍
												正交解码使用到TIM2的定时中断，必须打开！但在初始化时已经默认打开*/
											/*写2作为普通定时器中断使用*/
											/*写1作为输入捕获：可以获取高电平或者低电平的时间，默认不分频，不滤波
												输入捕获使用到TIM2的定时中断，必须打开！但在初始化时已经默认打开*/
											/*写0作为PWM：默认PWM1模式，向上计数，低电平有效，一个周期内占空比越大低电平时间越长
												引脚说明：			没有重映射			两种部分重映射			完全重映射
												TIM2四个通道：		PA0 PA1 PA2 PA3		含有前面的引脚		PA15 PB3 PB10 PB11
												注意：由于PA2 PA3串口2占用，PA0 PA1由ADC占用，所以TIM2的PWM只用完全重映射！！
														但PB10 PB11串口3占用，要么不用串口3，要么串口3重映射
											*/
			/*PWM输出比较功能选项*/
			#define tim2arr STSTEM_TIM2PWM_Period_5K			/*选择定时器输出频率（预分频系数为72，这里选择重装值）*/
				#define STSTEM_TIM2PWM_Period_1K	(1000-1)	/*注意：这里是选择重装值，也是占空比的分辨率，频率高则分辨率低，反之亦然，慎选*/
				#define STSTEM_TIM2PWM_Period_2K	(500-1)
				#define STSTEM_TIM2PWM_Period_5K	(200-1)
				#define STSTEM_TIM2PWM_Period_10K	(100-1)
				#define STSTEM_TIM2PWM_Period_20K	(50-1)
				#define STSTEM_TIM2PWM_Period_50K	(20-1)
				#define STSTEM_TIM2PWM_Period_100K	(10-1)
				#define STSTEM_TIM2PWM_Period_200K	(5-1)
			#define STSTEM_TIM2PWM_CHANNEL_ENABLE	B0000_0001|B0000_0010 /*输出通道选择，共四个通道，可以相与打开多个通道，PA15 PB3 PB10 PB11*/
			/*可用API：
				HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_2);		开启TIM2的PWM通道2
				HAL_TIM_PWM_Stop(&TIM2_Handler,TIM_CHANNEL_2);		关闭TIM2的PWM通道2，但看源码好像使所有通道都关闭了，待实验
				
				设置TIM2的PWM通道2的占空比百分数为88.8%，值需在0~100.0之间。默认向上计数，默认设置为当计数值小于此值时输出低电平。
				TIM2_set_Channel_Pulse(TIM2PWM_Channel_2,88.8);
			*/
			
			/*输入捕获功能选项*/
			#define STSTEM_TIM2_Cap_trigV	1			/*写1高电平触发一次输入捕获，写0相反，写2双边沿触发*/
			#define STSTEM_TIM2_Cap_Channel	B0000_0001	/*选择用哪个通道当输入捕获，只能四选一！！*/
			/*可用API：
				float Peek_TIM2_Cap_Val();		获取最近一次TIM2输入捕获的脉冲时间，单位 毫秒，按照设置的捕获沿进行，可以随时随地调用此函数
			*/
			
			/*正交解码功能选项*/
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

/*
	ADC：STN32的是12位逐次逼近型，分ADC1、2和3，其中ADC1和ADC2的引脚一样（除了内部通道）；
	最大速率1MHz，1us（在AD时钟14MHz（最高），采样时间为1.5个周期得到），总转换时间为 (采样时间 + 12.5个周期)，采样时间长则采样精度高；
	参考电压 Vref-必须连到VSSA，Vref+可连到 2.4V~VDDA，ADC输入电压必须小于Vref+，默认Vref+为3.3V，否则自行更改转换计算
	ADC1本质就是一个AD转换器加16路选择器，所以ADC1同一时刻采样值只能是一个，如果用在规则组中扫描模式，无法得知当前结果是哪一路的，一个AD模块一个值
	ADC1的通道与引脚映射：
	通道：	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	   16		     17
	IO	：	A0	A1	A2	A3	A4	A5	A6	A7	B0	B1	C0	C1	C2	C3	C4	C5	内部温度	内部参考电压
*/
#define SYSTEM_ADC1_ENABLE		0			/*启否ADC1*/
	#define SYSTEM_ADC1_useScan		1		/*启否规则组的连续扫描，如果启用，则把下面定义的所有通道都放到规则组里，并使用DAM1的通道1把转换结果放到目标位置
												如果不启用，则为软件触发的单次转换*/
		#define SYSTEM_ADC1_useCircular	1	/*只在扫描模式下有效；开启则自动循环ADC转换，只需要判断标志位和读数即可，不需要软件手动触发开启一次ADC转换*/
	#define SYSTEM_ADC1_useChanlNum	3		/*定义共用多少路通道*/
	#define SYSTEM_ADC1_useChanl	B1in16|B4in16|B5in16 /*定义共用哪些通道，可写B1in16~B15in16（分别代表通道0~14），和InrTemp(内部温度通道)
														（第16个ADC1通道留作内部温度标志所以不可用）*/
													/*如果只用采集内部温度，而不用其他通道，应设置：SYSTEM_ADC1_useChanlNum为1  SYSTEM_ADC1_useChanl单为InrTemp*/
		#define InrTemp B16in16
		extern u16 adValue[SYSTEM_ADC1_useChanlNum];/*扫描模式下DMA1把ADC转换结果传送的目标位置*/
		extern u8 adValueDone; 						/*扫描模式下转换完成标志*/
	/*可用的API：返回的都是原始ADC值
			凡是启用 SYSTEM_ADC1_useScan 扫描模式，即触发一次，自动扫描所有规则组里的通道————适合快速
						自动用DMA1通道1发送到adValue[]，并置位adValueDone
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
		
#define SYSTEM_IWDG_ENABLE		1			/*开启独立看门狗，默认1S的喂狗周期，默认在TIM4定时中断里喂狗，用IWDG必开TIM4*/
											/*注：看门狗和低功耗待机模式不能同时开启，因为看门狗不能关闭，看门狗复位会唤醒低功耗状态*/

/*DMA用法简单列举*/
/*
配置前 首先查看 “DMA1各通道一览表”
DMA配置的一般过程：（以储存器传输到UART1的TX为例子，USART1的TX连接在DMA1的通道4上）
	定义句柄：
		DMA_HandleTypeDef  UART1TxDMA_Handler;      //DMA句柄
	初始化序列：
		__HAL_RCC_DMA1_CLK_ENABLE();			//DMA1时钟使能 
		
		__HAL_LINKDMA(&UART1_Handler,hdmatx,UART1TxDMA_Handler);    		//将DMA与USART1联系起来(发送DMA)
		//参数说明：外设句柄(无论是发送方还是接收方) ， 外设句柄中的DMA_HandleTypeDef类型变量 ， DMA1句柄
		
		UART1TxDMA_Handler.Instance=DMA1_Channel4;                          //通道选择
		UART1TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设
											//还可选 ： DMA_PERIPH_TO_MEMORY 外设到存储器 或 DMA_MEMORY_TO_MEMORY 储存器到储存器
		UART1TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式 外设一般都是非增量（地址不增）
		UART1TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式 储存器一般都是增量（地址要增）
		UART1TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位 或者16位 或者32位 外设、存储器要一致，并且能接受
		UART1TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
		UART1TxDMA_Handler.Init.Mode=DMA_NORMAL;                            //以ADC为例：如果ADC选择连续模式，这里是循环，如果不开启连续模式，这里是正常模式
		UART1TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级 最高、高、中、低
		
		HAL_DMA_DeInit(&UART1TxDMA_Handler);   								//DMA复位
		HAL_DMA_Init(&UART1TxDMA_Handler);									//DMA初始化
	中断初始化：（可选）
		HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 3, 0);                     //DMA中断优先级
		HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);                             //使能DMA中断
	中断回调函数：
		//DMA通道1中断服务函数，完成传送一次时的中断
		void DMA1_Channel1_IRQHandler(void)
		{
			HAL_DMA_IRQHandler(&ADC1rxDMA_Handler); //清除中断标志和其他，勿动！
		}

		//HAL_DMA_IRQHandler的回调函数，ADC转换完成所有规则组通道后的中断函数
		void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
		{
			//要干的活
		}
		
	查询法得到传输是否完成：（通道4为例）（没实验过）
		if(__HAL_DMA_GET_FLAG(&UART1TxDMA_Handler,DMA_FLAG_TC4))//等待DMA1通道4传输完成
		{
			__HAL_DMA_CLEAR_FLAG(&UART1TxDMA_Handler,DMA_FLAG_TC4);//清除DMA1通道4传输完成标志
			//要干的活
		}
	
可用API：
	得到当前还剩多少数据待发：	u16 pro = HAL_DMA_GET_COUNTER(&UART1TxDMA_Handler); //比如一次传输count个数据，获取到的是剩下的， 1-pro/count 是进度百分比
	
	对于串口：	使用DMA发送		原型： HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
									例子：HAL_UART_Transmit_DMA(&UART1_Handler,SendBuff,SEND_BUF_SIZE);
				使用DMA接收		原型： HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
	
	对于更多外设：	参考上面ADC外设的DMA用法注解，和periphconfig.c里面的ADC DMA的中断服务函数，以及网上
*/


/*开启串口，x8/xB系列有三个串口，最好不超过2M位每秒。默认均为：8位数据，1位停止，无校验，收发模式，开启接受中断*/
/*串口方面可用API，看SYSTEM_SUPPORT_sprintf宏定义的注释*/
/*注：串口2、3的接收回调函数没有补全，用时再补，和串口1的同理*/
#define SYSTEM_UART1_ENABLE			1		/*使能串口1	       TX/PA9, RX/PA10		*/
#define SYSTEM_UART1_REMAP_ENABLE	0		/*串口1引脚重映射：TX/PB6, RX/PB7		*/
#define SYSTEM_UART1_BOUND			115200	/*串口1波特率*/

/*注：串口2在目前不能用，没有反应，不知道是HAL库问题还是芯片问题*/
#define SYSTEM_UART2_ENABLE			0		/*使能串口2	       TX/PA2, RX/PA3		*/
#define SYSTEM_UART2_REMAP_ENABLE	0		/*串口2引脚重映射：TX/PD5, RX/PD6，可以设置，但对于C8T6无此引脚*/
#define SYSTEM_UART2_BOUND			115200	/*串口2波特率*/

#define SYSTEM_UART3_ENABLE			0		/*使能串口3	       TX/PB10, RX/PB11		*/
#define SYSTEM_UART3_REMAP_ENABLE	0		/*串口3引脚重映射：TX/PD8,  RX/PD9，可以设置，但对于C8T6无此引脚*/
#define SYSTEM_UART3_BOUND			115200	/*串口3波特率*/
/*可用的API： 目前串口接收的问题就是一次涌入数据过多会崩，可能是存储数据时栈溢出，但是暂时找不到在哪里，小数据量接收没大问题
	发送：	如果启用 SYSTEM_SUPPORT_sprintf ，则可用：不要用printf！
				参数：UART1~UART3
				printf_uart(UART1,"Fault : %d\t",x);
				也可以用	sprintf(char* buffer, const char* format, ...); 				不带字节数量限制
							snprintf(char* buffer, size_t count, const char* format, ...); 	带字节数量限制，更安全
			如果没有启用 SYSTEM_SUPPORT_sprintf ，则可用以下：
			参数：UART1_Handler~UART3_Handler ， 发送数据的字符类型指针 ， 数据长度(字节数} ， 超时时间
				HAL_UART_Transmit(&UART1_Handler,UART_BUF,12,10); 				发送函数
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET); 	等待发送结束
	接收：（以下只针对串口1说明）（一次接收字符不要超过200个字节，如果超出则丢弃之后的数据）
			接受协议设置(默认协议0)：USART1_SetMode(0);	串口1接受协议：0为只接受以'\r\n'结尾的数据（数据以去掉\r\n保存），1为以FIFO先进先出的环形缓存实现接受区，数据原样保存，无协议
				接受协议设置可用随时切换，切换后接收方式也随下面的说明换。
			统一接收函数： 返回returnOK即有数据可以取，反之为ReturnErr；is_print：是否打印接收到的数据；buf用于存放接收到的数据，必须先给个足够大的容器
							char sys_USART1_RX_Fetch(u8 is_print, char* buf)
							用例：
								char buf4uart1[(USART1_RX_BUF_MaxNum > USART1_RX_FIFO_MaxNum) ? (USART1_RX_BUF_MaxNum):(USART1_RX_FIFO_MaxNum)];
								if(sys_USART1_RX_Fetch(FALSE, buf4uart1) == ReturnOK)
								{
									printf_uart(UART1,"%s-",buf4uart1);
									printf_uart(UART1,"%d",mystrlen(buf4uart1));
								}
			以下两个是协议0、1的接收细节，已经在sys_USART1_RX_Fetch()内实现，一般不用看
			协议0：
				接受区缓存大小设置：#define USART1_RX_BUF_MaxNum 200 这个语句在下面串口宏定义区里面
				判断是否接收完成一次：USART1_isDone		用于判断是否接受完成一次 例子： if(USART1_isDone){表示接收完成一次}
				查询本次接收完成一次后接收到的字符数量：USART1_RX_ByteNum u8类型 不包含"\r\n"两个字符！
				接收缓存区：	USART1_RX_BUF[x] ，x : 0~USART1_RX_ByteNum
				接受成功一次之后，处理完数据之后，清接收完成标志：USART1_SetUnDone;
			协议1：
				以FIFO储存区接收保存数据，FIFO初始化函数在sys.c里面的串口初始化函数中
				接受区缓存大小设置：#define USART1_RX_FIFO_MaxNum 200 这个语句在下面串口宏定义区里面
				while (!fifo_is_empty(Uart1_fifo))
				{
					char/int/float/struct... fifodata;
					fifo_get(Uart1_fifo, &fifodata);	//fifodata可以为各种字符型、整形、浮点型、结构体型等等
					printf_uart(UART1,"%c",fifodata); 
				}
				由于没有协议，只是储存，所以没有接收完成标志
				
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
	
	设置SPI速度函数： APB2为72MHz，APB1为二分频为36Mhz
		void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);	//SPI1由APB2分频得来
		void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);	//SPI2由APB1分频得来
*/

/*低功耗模式：
睡眠： WFI操作进入，任意中断恢复，CPU时钟关，其他外设时钟维持																		（不可用）
停止：HSI、HSE时钟关，电压调节器开关需设置，任一外部中断唤醒，典型电流值为20uA														（不可用）
待机：HSI、HSE时钟关，电压调节器关，SRAM和寄存器内容丢失，仅电源控制寄存器和备份区域不受影响
		唤醒条件：WKUP按键上升沿，RTC警告事件，复位按键，看门狗复位（！），典型电流值为2uA，为三个模式中最低 	类似于开关机		（可用）
		WKUP 在PA0
*/
#define SYSTEM_StdbyWKUP_ENABLE	0		/*使用待机-低功耗模式（占用0线外部中断）*/
										/*注：看门狗和低功耗待机模式不能同时开启，因为看门狗不能关闭，看门狗复位会唤醒低功耗状态*/
/*当启用 SYSTEM_StdbyWKUP_ENABLE 后，PA0作为WKUP按键，默认长按3秒进入待机状态，再次按下则恢复，进入待机模式函数在PA0的外部中断里*/
//对于PA0，按下为高电平，按下3s后松开，延时一秒（给时间做保存数据等关机前的准备）后进入待机状态（关机），再次按下开机
/*WKUP IO不用外接下拉电阻，在配置时STM32内部已经上拉*/

/*不同容量的FLASH组织方式：
小容量：32个	1K字节/页
中容量：128个	1K字节/页
大容量：256个	2K字节/页
*/
#define SYSTEM_FLASH_IAP_ENABLE	0		/*启用对内部FLASH储存空间编程*/
	#define STM32_FLASH_WREN	1		/*启用写功能，否则只读不写*/
	#define STM32_FLASH_SIZE 	64 		/*所选STM32的FLASH容量大小(单位为KB)*/
	#define FLASH_SAVE_ADDR  0X0800F000					//设置FLASH 保存地址(必须为页的首地址，且必须为2的倍数！且其值要大于代码所占用FLASH的大小+0X08000000)
	#define FLASH_SAVE_ADDR1 (0X08000000 + (u32)(60*1024))
/*可用API：（下载程序时不要擦除 整个芯片FLASH，只擦除有程序的扇区）
	注：当启用本功能后，在系统初始化序列中将加入读存开机次数的一个用例，一个u16的变量会储存在最后一个或倒数第二个页
	例：	const u8 TEXT_Buffer[]={"STM32F103 FLASH TEST"};	//用于存入的数据
			u8 datatemp[sizeof(TEXT_Buffer)];					//用于接读出的数据
	写：（一次写入长度最多为一个页的字节数，对于中小容量的为1KB，对于大容量的为2KB！），最后一个参数是以字节为单位！
		STMFLASH_Write(	FLASH_SAVE_ADDR1,	(u16*)TEXT_Buffer,	sizeof(TEXT_Buffer));
	读：STMFLASH_Read(	FLASH_SAVE_ADDR1,	(u16*)datatemp,		sizeof(TEXT_Buffer));
*/

/*hd系列外设，DAC两个输出通道 PA4和PA5，默认初始化为12位右对齐，速度最快为250K左右，输出范围 0~Vref+，默认Vref+为3.3V，否则自行更改转换计算*/
/*可选输出 噪声波形 和 三角波波形 ，默认不适用外部触发，可选触发源在初始化函数中修改（有定时器中断和外部中断线9）*/
#define SYSTEM_DAC_OUT1_ENABLE	0
#define SYSTEM_DAC_OUT2_ENABLE	0
/*可用API:
	设置通道1输出电压,vol:0~3.30V
		void DAC_Set_Ch1_Vol(float vol)
	设置通道2输出电压,vol:0~3.30V
		void DAC_Set_Ch2_Vol(float vol)
*/

/*hd系列外设，六根线（四位数据线，时钟线和命令线），SDIO最高传输速度12M字节每秒，目前只支持用于SD卡*/
/*支持的四种卡：SD2.0 高容量卡（SDHC，最大32G），SD2.0 标准容量卡（SDSC，最大 2G），SD1.x 卡和 MMC 卡）*/
/*
初始化后获取到的卡信息存在：
	HAL_SD_CardInfoTypeDef  SDCardInfo;	//SD卡信息结构体
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
	
	HAL_SD_CardCIDTypeDef SDCard_CID;  //SD卡制造CID
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
/*备注：可用开发用DMA读写SD卡，这样更省时间，读写的时候就不用关中断了*/
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
#define SYSTEM_FATFS_ENABLE	0
/*初始化步骤：
	要做的：在初始化系列里加上FATFS初始化SD卡
			先为每个文件设备句柄申请空间
			挂载
			
			更多文件操作可用参考原子FATFS历程里的 fattester.c 和 exfuns.c 里面的获取剩余空间、识别文件类型、得到文件夹大小、文件和文件夹复制等有用的API等等
			
			FATFS可用API：参考原子的，罗列下来API，参考官方文档等
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
	右移之后  0011 1111 1111 此为LCD->LCD_REG的 实际地址脚 情况，此时A10为0，LCD->LCD_RAM的地址为LCD->LCD_REG地址+2
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
	#define SYSTEM_FSMC_use4SRAM	0		//启用FSMC用于驱动SRAM，则相关代码被编译，相关API可用
/*
用于LCD的部分：
	  要做的：	LCD的相关代码还没有移植，LCD的驱动可以完全移用原子的LCD历程
				但是还保留现在的LCD驱动，用预编译命令控制，我说的明白了吗，到用的时候好好改

用于SRAM的部分：
	这里默认用的 块1（NORSRAM块）的区域3（区域4留给LCD喽~）（块1的区3和区4分别留给外部RAM和LCD，本模板默认，否则还得改地址）
	默认用于驱动IS62WV51216，地址线范围为A0~A18，数据线范围D0~D15，（NUB、NLB、NWE、NOE、NCE五根控制线都对应接好）
	底层API：（不推荐直接调用，使用自实现的malloc和free）（以字节为单位）
		u32 testsram[250000] __attribute__((at(SRAM1_BANK3_ADDR))); 	//地址定义在外部SRAM中
		void FSMC_SRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n); 	//写 参数：数据指针，外部SRAM内的地址(从0开始)，写入的字节数
		void FSMC_SRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n);   	//读 类似上
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



/*_____________系统函数_______________*/
extern u16	StartUpTimes;			/*用于保存开机次数，储存在最后一个或倒数第二个页*/
extern uint32_t UIDw[3]; /*保存STM32内部UID识别码，全球唯一识别码*/
extern uint32_t sysCoreClock; /*获取HCLK频率，外设时钟均来自此再分频*/

void sys_MCU_Init_Seq(void);				/*MCU外设初始化序列，所有初始化写到这里面*/
void sys_Device_Init_Seq(void);				/*器件外设初始化，并开机自检*/

extern uint8_t is_quitFault;
void FaultASSERT(char* FaultMessage);				/*表示初始化有问题，串口提示，指示灯或者蜂鸣器进行提示*/
int8_t Stm32_Clock_Init(uint32_t PLL);		/*时钟系统配置*/

#if SYSTEM_UART1_ENABLE||SYSTEM_UART2_ENABLE||SYSTEM_UART3_ENABLE
	
	#define USART1_RX_BUF_MaxNum 	500 	/*定义用协议0只接受以'\r\n'结尾的数据时可以存在字符数组里面的最大字节数目*/
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
	

	extern char USART1_RX_BUF[USART1_RX_BUF_MaxNum]; /*串口1的模式1数据接受区*/
	extern u16 USART1_RX_CONFIG;	/*    x            x           x        x xxxx xxxx xxxx      */
									/*(接受完成) (协议模式0/1) (接收到\r)   串口1接受数据字节计数 */
									/*串口1接受协议：0为只接受以'\r\n'结尾的数据，1为以FIFO先进先出实现接受区，无协议*/
									
	#define USART1_SetDone 		USART1_RX_CONFIG |= USART1_RX_DONE_mask /*设置串口1接收完成标志位*/
	#define USART1_SetUnDone 	USART1_RX_CONFIG &= USART1_RX_MODE_mask /*在任务中处理完后对串口1标志位进行复位，除了mode位，其他位都写0*/
	#define USART1_Set_r_UnDone	USART1_RX_CONFIG &= (!USART1_RX_Rec_r_mask)
	#define USART1_SetMode(x) 	USART1_RX_CONFIG |= (((u16)x)<<14) 		/*用户可用，在任务中设置串口1接收协议*/
	#define USART1_isDone 		(USART1_RX_CONFIG & USART1_RX_DONE_mask)/*用户用，用于判断是否接受完成一次*/
	#define USART1_RX_ByteNum 	(USART1_RX_CONFIG & USART1_RX_Num_mask)/*用户可用，返回接收的字符数量*/
	
//	extern char USART1_RX_FIFO_buf[USART1_RX_FIFO_MaxNum]; /*串口1的模式0的FIFO数据接受区*/
//	extern fifo_t Test_fifo;
//	extern fifo_t Test2_fifo;
	extern fifo_t Uart1_fifo;
	char sys_USART1_RX_Fetch(u8 is_print, char* buf);
	
	
#endif

#if SYSTEM_MCO_PA8_OUT
	void sys_MCO_Out_Enable(void);				/*MCO在PA8上输出启动，默认时钟源HSE*/
#endif

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
	
#if STSTEM_TIM3PWM_ENABLE
	/*蜂鸣器连接在通道4上*/
	extern u8 is_buzzer_once;
	extern u8 is_buzzer_bibi;
	#define buzzer_bibi_once	is_buzzer_once = 1;  //用户使用，蜂鸣器叫唤一声
	#define buzzer_bibi_on		is_buzzer_bibi = 1;  //用户使用，蜂鸣器间歇叫唤开启
	#define buzzer_bibi_off		is_buzzer_bibi = 0;  //用户使用，蜂鸣器间歇叫唤关闭
	extern TIM_HandleTypeDef 	TIM3_Handler;      	//定时器3句柄
	extern TIM_OC_InitTypeDef 	TIM3_CH1Handler,TIM3_CH2Handler,TIM3_CH3Handler,TIM3_CH4Handler;	//定时器3通道1-4句柄
	#define TIM3PWM_Channel_1	1
	#define TIM3PWM_Channel_2	2
	#define TIM3PWM_Channel_3	3
	#define TIM3PWM_Channel_4	4
	void sys_TIM3PWM_ENABLE(void);
	void TIM3_set_Channel_Pulse(u8 channel,float percent);
#endif

#if SYSTEM_IWDG_ENABLE
	extern IWDG_HandleTypeDef IWDG_Handler; //独立看门狗句柄
	void sys_IWDG_ENABLE(void);
	void IWDG_Feed(void);
#endif

#if SYSTEM_CRC_ENABLE
	#define BUFFER_SIZE    114
	extern CRC_HandleTypeDef hcrc;
	extern const uint32_t aDataBuffer[BUFFER_SIZE];
	extern uint32_t uwExpectedCRCValue;
	void sys_CRC_ENABLE(void);
	void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc);
#endif

#if SYSTEM_RTC_ENABLE
	extern RTC_HandleTypeDef RTC_Handler;  //RTC句柄
	//时间结构体
	typedef struct 
	{
		vu8 hour;
		vu8 min;
		vu8 sec;			
		//公历日月年周
		vu16 w_year;
		vu8  w_month;
		vu8  w_date;
		vu8  week;	
	}_calendar_obj;					 
	extern _calendar_obj calendar;				//用户用！RTC结构体，实际用时只需要读这个结构体获取时间即可
	char sys_RTC_Enable(void);						/*RTC配置*/
	u8 Is_Leap_Year(u16 year);					//平年,闰年判断
	u8 RTC_Get(void);         					//获取时间，在RTC秒中断中执行，自动更新calendar时间，用户不用管
	u8 RTC_Get_Week(u16 year,u8 month,u8 day);
	u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);		//用户用！设置时间	
	u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);	//用户用！设置闹钟
	//HAL_StatusTypeDef HAL_RTC_SetAlarm_IT (RTC_HandleTypeDef * hrtc, RTC_AlarmTypeDef * sAlarm,uint32_t Format);
#endif


void delay_init(void);
void delay_ms(uint16_t nms);
void delay_xms(uint16_t nms);
void delay_us(uint32_t nus);


/*Author:		MaxwellXyao*/
#define BIT(n) 					(1 << n) 				//位mask
#define bit_SetTrue(x,mask) 	(x |= mask)				//该位设置为真
#define bit_SetFalse(x,mask)	(x &= ~mask)			//该位设置为假
#define bit_Toggle(x,mask) 		(x ^= mask)				//位切换
#define bit_IsTrue(x,mask) 		((x & mask) != 0)		//该位是否真
#define bit_IsFalse(x,mask) 	((x & mask) == 0)		//该位是否假

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(uint32_t addr);	//设置堆栈地址

#endif


