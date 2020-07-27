#ifndef __SYS_H
#define __SYS_H	

/*
命名规范：（临时）
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
	0、中断优先级分组选用分组4，即16级抢占优先级，0级响应优先级（即不用）
	1、IO尽量设计为低电平有效，高电平截止；按键IO尽量使用外部中断。
	2、使用C99模式编译！
	3、I2C使用软件实现
	4、FreeRTOS所有组件始终使用动态申请内存；FreeRTOS的配置文件除了用户按需更改区域，尽量不要动；一般不使用软件定时器，用硬件定时器或者周期延时替代即可
	5、尽量减少数据传输过程中的拷贝，常用指针、结构体指针当传输数据
	6、调用函数尽量不用NULL实参，尽量给一个具体的值
	7、外设如串口、PWM等的IO初始化在其初始化函数内，不用单独再初始化
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
			   4 = 16K字节
			   6 = 32K
			   8 = 64K
			   B = 128K
			   C = 256K
			   D = 384K
			   E = 512K
			    H = BGA封装
				T = LQFP封装
				 6 = -40~85℃
			FLASH               RAM
			
			16K(ld)				6K			ld:2个USART，2个16位定时器，1个SPI，1个I2C，USB，CAN，1个高级定时器，2个ADC
			32K(ld)				10K
			
			64K(md)				20K			md:3个USART，3个16位定时器，2个SPI，2个I2C，USB，CAN，1个高级定时器，1个ADC
			128K(md)			20K						(x8/xB系列有1、2、3、4定时器)
			
			256K(hd)			48K或64K	hd:5个串口，4个16位定时器，2个基本定时器，3个SPI，2个I2S，2个I2C，USB，CAN，
			384K(hd)			64K				2个高级定时器，3个ADC，1个DAC，1个SDIO，FSMC(100和144脚封装)
			512K(hd)			64K
*/

/*
启动：	BOOT1	BOOT0		启动区域
		  x		  0			FLASH
		  0		  1			系统储存器(ISP下载)
		  1		  1			SRAM
*/

#define SYSTEM_SUPPORT_OS		1				/*定义是否使用FreeRTOS，不是0就是1——————！按需要进行修改！
													FreeRTOS版本：v10.3.1
													默认用于任务的RAM堆栈大小为5KB，按需修改！
												*/
#define SYSTEM_SUPPORT_Menu		0				/*提供一个菜单模板，把系统的输入、输出、执行功能的标志位控制全部打包！注意，menu库包含了printf.h库和MyString.h库！*/
/*
	keyProcess(); 	//获取键值输入函数；对于RTOS，直接放一个任务；对于裸机，放while大循环里（默认没放）
	menuProcess();	//菜单处理的主函数；对于RTOS，直接放一个任务；对于裸机，放while大循环里（默认没放）
*/
#define SYSTEM_SUPPORT_MyString	0				/*提供一个实现了string.h大部分字符操作函数的库*/
/*具体作用看MyString.c文件里的注释
int mystrlen (const char *str);
char * mystrcat (char *dest, const char *src);
char * mystrncat (char *dest, const char *src, int n);
char * mystrcpy (char *dest, const char *src);
char * mystrncpy (char *dest, const char *src, int n);
int mystrcmp (const char *s1, const char *s2);
int mystrncmp (const char *s1, const char *s2, int n);
void * mymemset (void *s, int c, unsigned n);
*/
int myatoi(const char *str);					/*提供一个字符串转整形的实现*/
#define SYSTEM_SUPPORT_sprintf	1				/*包含且编译printf.h，github开源文件，无依赖，功能比较全。
													约占6KB，对于stm32够，对于其他小容量MCU则看“其他几个sprintf实现”文件夹里面的,不要纠结了。
													https://github.com/mpaland/printf
													提供API：
													int printf(const char* format, ...); 								不用
													int printf_uart(unsigned char uart,const char* format, ...); 		用，可选发送串口
																		串口选择可选：UART1/UART2/UART3，需要先使能相关串口
													int sprintf(char* buffer, const char* format, ...); 				不带字节数量限制
													int snprintf(char* buffer, size_t count, const char* format, ...); 	带字节数量限制，更安全
												注意：另在SYSTEM文件夹内提供strprintf.h，功能比较单一，但很小。
												*/
#define SYSTEM_SUPPORT_pid		0				/*提供一个pid算法实现库，集成了积分分离和变限积分，以及可选的不完全微分和微分先行，具体用法看pid.h里面*/

/*所有main的头文件都放在这里*/
#if SYSTEM_SUPPORT_OS
	#include "FreeRTOS.h"	//FreeRTOS使用,这里不要乱动	  
	#include "task.h"
	#include "TaskConfig.h"
#else
	#include "BareConfig.h"
#endif

#include "sysVar.h"								/*定义系统级常用的变量、数据类型和二进制表示宏*/
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"						/*在里面的stm32f1xx_hal_conf.h里面选择用哪些外设的HAL库——————！按需要进行修改！*/
#include "PeriphConfig.h"
#include "ringbuf.h"							/*实现的一个环形缓存，用于串口接收模式*/

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
说明：按需要在配置函数内设置：时间，闹钟，读存BKP
其他：系统在待机模式下可选被闹钟唤醒*/
#define SYSTEM_RTC_ENABLE		0				
/*配置使用CRC循环冗余校验
这个CRC计算模块使用常见的、在以太网中使用的计算多项式：
X32 + X26 + X23 + X22 + X16 + X12 + X11 + X10 +X8 + X7 + X5 + X4 + X2 + X + 1
写成16进制就是：0x04C11DB7*/
#define SYSTEM_CRC_ENABLE		0
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
#define STSTEM_TIM2_ENABLE		1			/*通用定时器2，功能自定，默认分频系数为72，初始化函数在PeriphCconfig.c里面定义*/
	#define STSTEM_TIM2_TI_ENABLE	1		/*是否开启定时器2的定时中断*/
	
	#define STSTEM_TIM2_asPWMorCap	3		/*选择定时器2作为...注：PWM(输出比较)、输入捕获和正交解码三个功能不能共用！*/
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
	参考电压 Vref-必须连到VSSA，Vref+可连到 2.4V~VDDA，ADC输入电压必须小于Vref+
	ADC1本质就是一个AD转换器加16路选择器，所以ADC1同一时刻采样值只能是一个，如果用在规则组中扫描模式，无法得知当前结果是哪一路的，一个AD模块一个值
	ADC1的通道与引脚映射：
	通道：	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	   16		     17
	IO	：	A0	A1	A2	A3	A4	A5	A6	A7	B0	B1	C0	C1	C2	C3	C4	C5	内部温度	内部参考电压
*/
#define SYSTEM_ADC1_ENABLE		0			/*启否ADC1*/
	#define SYSTEM_ADC1_useScan		1		/*启否规则组的连续扫描，如果启用，则把下面定义的所有通道都放到规则组里，
												然后在定时器2中断中连续采集
												如果不启用，则为软件触发的单次转换
												是用连续扫描还是单次采集，定下后运行时不能改变
											*/
	#define SYSTEM_ADC1_useChanlNum	3		/*定义共用多少路通道*/
											/*定义共用哪些通道，可写B0in16~B15in16，和InrTemp(内部温度通道)
											如果只用采集内部温度，而不用其他通道，应设置：SYSTEM_ADC1_useChanlNum为1  SYSTEM_ADC1_useChanl单为InrTemp
											如果不用
											*/
	#define SYSTEM_ADC1_useChanl	B0in16|B1in16|B3in16
		#define InrTemp B16in16
		/*下面的定时器触发不用了，何必那么麻烦，直接调用启动ADC采集一次的函数一样...*/
//	#define SYSTEM_ADC1_useTIM2trig	1		/*定时器2触发ADC采集转换，由于ADC1触发源没有TIM2TRGO，
//												所以是在TIM2的定时中断中软件触发实现，必须打开TIM2的定时中断！*/
	#define SYSTEM_ADC1_useDMA1		1		/*使用DAM1把转换结果放到目标位置*/
		extern u16 adValue[SYSTEM_ADC1_useChanlNum];						/*DMA1把ADC转换结果传送的目标位置*/
		extern u8 adValueDone;
	/*可用的API：
			凡是启用 SYSTEM_ADC1_useDMA1 即用DMA传输ADC数据，读取顺序：
				如果启用 SYSTEM_ADC1_useScan 循环一遍采集
					先调用 HAL_ADC_Start(&ADC1_Handler); 调用SYSTEM_ADC1_useChanlNum次， 转换一遍每一个规则通道
					判断 adValueDone 是否为1，是则从 adValue[x] 读值即可，如果启用 内部温度 通道，其值保存在adValue[SYSTEM_ADC1_useChanlNum-1]
					adValueDone 清零
				如果没有启用 SYSTEM_ADC1_useScan 循环一遍采集
					先注入本次单次通道的规则，以下函数其参数只可以改通道，通道可以为ADC_CHANNEL_0~ADC_CHANNEL_15 和 ADC_CHANNEL_TEMPSENSOR
						ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_TEMPSENSOR,1, ADC_SAMPLETIME_55CYCLES_5);
					再调用调用 HAL_ADC_Start(&ADC1_Handler); 启动一次
					判断 adValueDone 是否为1，是则从 adValue[x] 读值即可
					adValueDone 清零
			凡是没有启动 SYSTEM_ADC1_useDMA1 即没用用DMA传输ADC数据，读取顺序：
				如果启用 SYSTEM_ADC1_useScan 循环一遍采集
					调用 Get_Adc_Average(u32 ch,u8 times,u32* result); 
					其中result为返回值，用result[0]~result[SYSTEM_ADC1_useChanlNum-1]分别保存每一通道AD值，如果启用 内部温度 通道，其值保存在result[SYSTEM_ADC1_useChanlNum-1]
					其中 ch 通道 形参无效
				如果没有启用 SYSTEM_ADC1_useScan 循环一遍采集
					调用 Get_Adc_Average(u32 ch,u8 times,u32* result); 
					其中 ch 通道 形参选择想转换的通道，可以为ADC_CHANNEL_TEMPSENSOR，其中result[0]保存本次本通道的转换结果
					
			注意：采集温度通道返回的是ADC的原始值，用 float Get_Temprate(u32 adcx) 把原始值转换为实际温度值(float类型)
	*/
		
#define SYSTEM_IWDG_ENABLE		1			/*开启独立看门狗，默认1S的喂狗周期，默认在TIM4定时中断里喂狗，用IWDG必开TIM4*/

/*开启串口，x8/xB系列有三个串口，最好不超过2M位每秒。默认均为：8位数据，1位停止，无校验，收发模式，开启接受中断*/
/*串口方面可用API，看SYSTEM_SUPPORT_sprintf宏定义的注释*/
/*注：串口2、3的接收回调函数没有补全，用时再补，和串口1的同理*/
#define SYSTEM_UART1_ENABLE			1		/*使能串口1	       TX/PA9, RX/PA10		*/
#define SYSTEM_UART1_REMAP_ENABLE	0		/*串口1引脚重映射：TX/PB6, RX/PB7		*/
#define SYSTEM_UART1_BOUND			115200	/*串口1波特率*/

#define SYSTEM_UART2_ENABLE			0		/*使能串口2	       TX/PA2, RX/PA3		*/
#define SYSTEM_UART2_REMAP_ENABLE	0		/*串口2引脚重映射：TX/PD5, RX/PD6，可以设置，但对于C8T6无此引脚*/
#define SYSTEM_UART2_BOUND			115200	/*串口2波特率*/

#define SYSTEM_UART3_ENABLE			0		/*使能串口3	       TX/PB10, RX/PB11		*/
#define SYSTEM_UART3_REMAP_ENABLE	0		/*串口3引脚重映射：TX/PD8,  RX/PD9，可以设置，但对于C8T6无此引脚*/
#define SYSTEM_UART3_BOUND			115200	/*串口3波特率*/
/*可用的API：
	发送：	如果启用 SYSTEM_SUPPORT_sprintf ，则可用：不要用printf！
				参数：UART1~UART3
				printf_uart(UART1,"Fault : %d\t",x);
				也可以用	sprintf(char* buffer, const char* format, ...); 				不带字节数量限制
							snprintf(char* buffer, size_t count, const char* format, ...); 	带字节数量限制，更安全
			如果没有启用 SYSTEM_SUPPORT_sprintf ，则可用：
			参数：UART1_Handler~UART3_Handler ， 发送数据的字符类型指针 ， 数据长度(字节数} ， 超时时间
				HAL_UART_Transmit(&UART1_Handler,UART_BUF,12,10); 				发送函数
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET); 	等待发送结束
	接收：（以下只针对串口1说明）（一次接收字符不要超过200个字节，如果超出则丢弃之后的数据）
			接受协议设置(默认协议0)：USART1_SetMode(0);	串口1接受协议：0为只接受以'\r\n'结尾的数据，1为以FIFO先进先出的环形缓存实现接受区，无协议
			接受协议设置可用随时切换，切换后接收方式也随下面的说明换。
			协议0：
				接受区缓存大小设置：#define USART1_RX_BUF_MaxNum 200 这个语句在下面串口宏定义区里面
				判断是否接收完成一次：USART1_isDone		用于判断是否接受完成一次 例子： if(USART1_isDone){表示接收完成一次}
				查询本次接收完成一次后接收到的字符数量：USART1_RX_ByteNum u8类型 不包含"\r\n"两个字符！
				接收缓存区：	USART1_RX_BUF[x] ，x : 0~USART1_RX_ByteNum
				接受成功一次之后，处理完数据之后，清接收完成标志：USART1_SetUnDone;
			协议1：
				接受区缓存大小设置：#define ringbuf_MaxNum 200 这个语句在ringbuf.h里面
				res = ReadDataFromRingbuff(&RingBuff_forUSART1); 返回值如果为 ReturnOK 则为成功，若为 ReturnErr 则失败，不要从下一条语句拿取数据
				RingBuff_forUSART1.data 从这里拿取数据 char类型
				可用一直拿到 ReadDataFromRingbuff 返回为 ReturnErr 为止，由于没有协议，只是储存，所以没有接收完成标志
				
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
#define SYSTEM_SPI1_ENABLE		1		/*使能SPI1*/
#define SYSTEM_SPI2_ENABLE		0		/*使能SPI2*/
/*提供API：
	用户自定SS引脚：
		到PeriphConfig.c里面的
			sys_SPI1_SS_io_Init();		自行更改
			sys_SPI2_SS_io_Init();		自行更改
		以及 到PeriphConfig.h里面的
			#define	SPI1_CS PAout(4)	自行更改
			#define	SPI2_CS PBout(12)	自行更改
	写读一体函数：
		u8 SPI1_ReadWriteByte(u8 TxData);
		u8 SPI2_ReadWriteByte(u8 TxData);
	设置SPI速度函数： APB2为72MHz，APB1为二分频为36Mhz
		void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);	//SPI1由APB2分频得来
		void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);	//SPI2由APB1分频得来
*/

/*低功耗模式：
睡眠： WFI操作进入，任意中断恢复，CPU时钟关，其他外设时钟维持																		（不可用）
停止：HSI、HSE时钟关，电压调节器开关需设置，任一外部中断唤醒，典型电流值为20uA														（不可用）
待机：HSI、HSE时钟关，电压调节器关，SRAM和寄存器内容丢失，仅电源控制寄存器和备份区域不受影响
		唤醒条件：WKUP按键上升沿，RTC警告事件，复位按键，看门狗复位，典型电流值为2uA，为三个模式中最低 	类似于开关机				（可用）
		WKUP 在PA0
*/
#define SYSTEM_StdbyWKUP_ENABLE	0		/*使用待机-低功耗模式*/
/*当启用 SYSTEM_StdbyWKUP_ENABLE 后，PA0作为WKUP按键，默认长按3秒进入待机状态，再次按下则恢复，进入待机模式函数在PA0的外部中断里*/
/*WKUP IO不用外接下拉电阻，在配置时STM32内部已经上拉*/

/*不同容量的FLASH组织方式：
小容量：32个	1K字节/页
中容量：128个	1K字节/页
大容量：256个	2K字节/页
*/
#define SYSTEM_FLASH_IAP_ENABLE	0		/*启用对内部FLASH储存空间编程*/
	#define STM32_FLASH_WREN	1		/*启用写功能，否则只读不写*/
	#define STM32_FLASH_SIZE 	64 		/*所选STM32的FLASH容量大小(单位为KB)*/
	#define FLASH_SAVE_ADDR  0X0800F000					//设置FLASH 保存地址(必须为页的首地址！且其值要大于本代码所占用FLASH的大小+0X08000000)
	#define FLASH_SAVE_ADDR1 (0X08000000 + (u32)(60*1024))
/*可用API：
	注：当启用本功能后，在系统初始化序列中将加入读存开机次数的一个用例，一个u16的变量会储存在最后一个或倒数第二个页
	例：	const u8 TEXT_Buffer[]={"STM32F103 FLASH TEST"};	//用于存入的数据
			u8 datatemp[sizeof(TEXT_Buffer)];					//用于接读出的数据
	写：(一次写入长度最多为一个页的字节数，对于中小容量的为1KB，对于大容量的为2KB！)
		STMFLASH_Write(	FLASH_SAVE_ADDR1,	(u16*)TEXT_Buffer,	sizeof(TEXT_Buffer));
	读：STMFLASH_Read(	FLASH_SAVE_ADDR1,	(u16*)datatemp,		sizeof(TEXT_Buffer));
*/

/*_____________系统函数_______________*/
//extern static uint8_t Init_OK_Num;

extern uint32_t UIDw[3]; /*保存STM32内部UID识别码，全球唯一识别码*/
extern uint32_t sysCoreClock; /*获取HCLK频率，外设时钟均来自此再分频*/

void sys_MCU_Init_Seq(void);				/*MCU外设初始化序列，所有初始化写到这里面*/
void sys_Device_Init_Seq(void);				/*器件外设初始化，并开机自检*/

extern uint8_t is_quitFault;
void FaultASSERT(uint8_t errNum,char* message,uint8_t* is_quit);				/*表示初始化有问题，串口提示，指示灯或者蜂鸣器进行提示，并进入死循环*/
uint8_t Stm32_Clock_Init(uint32_t PLL);		/*时钟系统配置*/

#if SYSTEM_UART1_ENABLE||SYSTEM_UART2_ENABLE||SYSTEM_UART3_ENABLE
	#define UART1	1
	#define UART2	2
	#define UART3	3
	#define RXBUFFERSIZE   1 					//缓存大小
	extern u8 aRxBuffer1[RXBUFFERSIZE],aRxBuffer2[RXBUFFERSIZE],aRxBuffer3[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
	extern UART_HandleTypeDef UART1_Handler,UART2_Handler,UART3_Handler; //UART句柄
	void sys_USART1_ENABLE(void);
	void sys_USART2_ENABLE(void);
	void sys_USART3_ENABLE(void);
	
	#define USART1_RX_BUF_MaxNum 200
	extern char USART1_RX_BUF[USART1_RX_BUF_MaxNum]; /*串口1的模式1数据接受区*/
	extern u16 USART1_RX_CONFIG;	/*    x            x           x        x xxxx xxxx xxxx      */
									/*(接受完成) (协议模式0/1) (接收到\r)   串口1接受数据字节计数 */
									/*串口1接受协议：0为只接受以'\r\n'结尾的数据，1为以FIFO先进先出实现接受区，无协议*/
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
	uint8_t sys_RTC_Enable(void);						/*RTC配置*/
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


