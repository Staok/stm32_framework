#include "PeriphConfigCore.h"



/*_____________________________________\\\                               ///____________________________________________*
 *_________________________________________________外设初始化函数_______________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/

/*__________MCU外设初始化序列_____________*/
void sys_MCU_Init_Seq(void)
{
	/*
		时钟分配：
		HSE外接8Mhz晶振，LSE外接32.768Khz晶振
		HSE（8M）	LSE（32.768K）	LSI（32K）	HSI（16M）
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
	
		定时器不管16或者32位的，都按16位来用，也够用了
	*/
	Stm32_Clock_Init();
	
	
	/*关闭JTAG，启用SWD*/
	/*TODO：用本框架的GPIO初始化这里的SWD IO，先试验试验如果不初始化有没有什么问题*/
	sys_ENswd_DISjtag();
	
	/*设置MCO*/
	#if SYSTEM_MCO_PA8_OUT
		sys_MCO_Out_Enable();
	#endif

	/*按照设定初始化串口1、2、3*/
	#if SYSTEM_UART1_ENABLE
		sys_USART1_ENABLE();
		
		/*串口1接受协议：0为只接受以'\r\n'结尾的数据，1为以FIFO先进先出的环形缓存实现接受区，无协议*/
		USART1_SetMode(0);
	#endif
	#if SYSTEM_UART2_ENABLE
		sys_USART2_ENABLE();
	#endif
	#if SYSTEM_UART3_ENABLE
		sys_USART3_ENABLE();
	#endif
	
	/*设置RTC*/
	#if SYSTEM_RTC_ENABLE
		sys_RTC_Enable();
		//用户自行更改设置RTC和闹钟的日期和时间，在	sys_RTC_Enable() 里面		
	#endif
	/*设置CRC*/
	#if SYSTEM_CRC_ENABLE
		sys_CRC_ENABLE();
		if(HAL_CRC_Accumulate(&hcrc, (uint32_t *)aDataBuffer, BUFFER_SIZE) == uwExpectedCRCValue)
		{}else{FaultASSERT("CRC init",1,flag_Warning);}
	#endif
		
	#if SYSTEM_RNG_ENABLE
		sys_RNG_ENABLE();
	#endif
	
	printf_uart(UART1,"Author : Staok\r\nEmail : superxhy@qq.com\r\nRepo : https://github.com/Staok/stm32_framework\r\n");
	printf_uart(UART1,"Compile time : %s,%s\r\n",__DATE__,__TIME__);
	printf_uart(UART1,"Version : %s\r\n",Version_of_stm32_framework);
	printf_uart(UART1,"OK,then...System is starting...\r\n");
	
	
	/*获取HCLK频率并打印到串口1，外设时钟均来自此再分频*/
	sysCoreClock = HAL_RCC_GetHCLKFreq(); 
	printf_uart(UART1,"sysCoreClock/HCLK : %d\r\n",sysCoreClock);
	/*保存STM32内部UID识别码并打印，全球唯一识别码*/
	UIDw[0] = HAL_GetUIDw0();UIDw[1] = HAL_GetUIDw1();UIDw[2] = HAL_GetUIDw2(); 
	printf_uart(UART1,"UID : %d %d %d\r\n",UIDw[0],UIDw[1],UIDw[2]);
	
	#if SYSTEM_FLASH_IAP_ENABLE
//		u16 a[3];
//		printf_uart(UART1,"getWordSize((u8*)&a) : %d\r\n",getWordSize(a));
		//获取开机次数
		STMFLASH_Read(FLASH_SAVE_ADDR,	(u32*)&StartUpTimes,	getWordSize(StartUpTimes));
		StartUpTimes += 1;
		STMFLASH_Write(FLASH_SAVE_ADDR,	(u32*)&StartUpTimes,	getWordSize(StartUpTimes));
		printf_uart(UART1,"StartUpTimes : %d\r\n",StartUpTimes);
	#endif
	
	#if SYSTEM_ADC1_ENABLE
		/*说明ADC1的用途*/
		sys_ADC1_ENABLE();
	#endif
	
	#if SYSTEM_SPI1_ENABLE
		/*说明SPI1的用途*/
		sys_SPI1_ENABLE();
	#endif
	
	#if SYSTEM_SPI2_ENABLE
		/*说明SPI2的用途*/
		sys_SPI2_ENABLE();
	#endif
	
	#if SYSTEM_CAN1_ENABLE
		CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS1_6TQ,CAN_BS2_7TQ,6,CAN_MODE_NORMAL);	//42M外设，500Kbps波特率
//		CAN1_Mode_Init(CAN_SJW_1TQ,CAN_BS1_7TQ,CAN_BS2_8TQ,21,CAN_MODE_NORMAL);	//42M外设，125Kbps波特率
	#endif
	
	#if SYSTEM_StdbyWKUP_ENABLE
		/*使能待机-低功耗模式，默认长按PA0(WKUP)3秒关开机*/
		sys_StdbyWKUP_ENABLE();
	#endif
	
	#if (SYSTEM_DAC_OUT1_ENABLE) || (SYSTEM_DAC_OUT2_ENABLE)
		sys_DAC_ENABLE();
	#endif
	
	my_mem_init(InrRAM);	//初始化内部内存SRAM 		128KB
	my_mem_init(InrCCM);	//初始化内部辅助内存CCM 	64KB
	
	//初始化外部内存SRAM 1024KB，960KB可用
	#if (SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4SRAM)
		sys_FSMC_SRAM_ENABLE();
		my_mem_init(ExRAM1);
	#endif
	
}

/*__________器件外设初始化，并开机自检_____________*/
void sys_Device_Init_Seq(void)
{
	u8 init_return;
	
	/*用户应用的Device初始化序列——开始*/
	
	/*用户IO初始化，可选择初始化某个特定器件或者所有器件（ALL_Index）*/
	Devices_Init(UserDevices,TestLED_Index);	
	Devices_Init(UserDevices,KEY_Index);	
	Devices_Init(UserDevices,LCD_Index);	
//	Devices_Init(UserDevices,simuI2C_Index);
	
	/*LCD初始化*/
	#if (SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4LCD)
		LCD_with_FSMC_init_FSMC();
		LCD_with_FSMC_init_LCD();
	#else
		LCD_Init_no_FSMC();
	#endif
	
	/*STM32 USB 的初始化*/
	#if ( (SYSTEM_USB_ENABLE) && ( (USE_DEVICE_MODE) || (USE_HOST_MODE) ) )
		#if (USE_DEVICE_MODE)
			init_return = sys_USBD_User_Init(); if(init_return != 0){ FaultASSERT("sys_USBD_User_Init()",init_return,flag_Fault);}
		#endif
		
		#if (USE_HOST_MODE)
			init_return = sys_USBH_User_Init(); if(init_return != 0){ FaultASSERT("sys_USBH_User_Init()",init_return,flag_Fault);}
		#endif
	#endif
	
	/*FATFS ff14 初始化*/
	#if SYSTEM_FATFS_ENABLE
	init_return = FATFS_Init(); if(init_return != 0){ FaultASSERT("FATFS_Init()",init_return,flag_Fault);}
	
	init_return = f_mount(fs[0],"DEV_ExFLASH:",1);	//挂载外部SPI FLASH
		if(init_return == 0X0D)	//外部SPI FLASH磁盘的FAT文件系统错误，重新格式化
		{
			MKFS_PARM mkf_opts =	//格式化选项
			{ .fmt = FM_SFD 		//FM_FAT32 还是 FM_SFD 不清楚，后者为没有引导区
				//其他都保持默认就好~
			};
			init_return = f_mkfs("DEV_ExFLASH:",&mkf_opts,NULL,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
			if(init_return==0)
			{
				f_setlabel((const TCHAR *)"0:DEV_ExFLASH");						//设置Flash磁盘的名字
			}else FaultASSERT("f_mkfs DEV_ExFLASH",init_return,flag_Warning);	//格式化失败
			HAL_Delay(1000);
		}
	
	init_return = f_mount(fs[1],"DEV_SD:",1);		//挂载SDIO驱动的SD卡
		if(init_return != 0){ FaultASSERT("f_mount fs[1]",init_return,flag_Warning);}


	init_return = f_mount(fs[2],"DEV_USB:",1);		//挂载USB UOST MSC文件设备（U盘）
		if(init_return != 0){ FaultASSERT("f_mount fs[2]",init_return,flag_Warning);}

	
	init_return = f_mount(fs[3],"DEV_SPI_SD:",1);	//挂载SPI驱动的SD卡
		if(init_return != 0){ FaultASSERT("f_mount fs[3]",init_return,flag_Warning);}
	#endif
	
	/*LWIP 2.1.2 初始化*/
	#if SYS_SUPPORT_LWIP
		init_return = lwip_comm_init(&lwip_handle);
		if(init_return != 0)
		{
			lwip_comm_destroy(&lwip_handle); //初始化失败时恢复原样
			FaultASSERT("lwip_comm_init",init_return,flag_Warning);
		}
		
		#if LWIP_HTTP
			httpd_init();	//Web Server
		#endif
		
	#endif
	
	/*OLED初始化*/
//	OLED_Init();
	
	/*MPU6050初始化*/
//	MPU6050_Init();
	
	
	/*用户应用的Device初始化序列——结束*/
	
	/*__________启动模板的心跳，开启MCU的精彩一生__________*/
	/*初始化并启动TIM4*/
	#if STSTEM_TIM4_ENABLE
		sys_TIM4_ENABLE();
	#endif
	
	/*初始化并启动TIM3PWM通道*/
	#if STSTEM_TIM3PWM_ENABLE
		sys_TIM3PWM_ENABLE();
	#endif
	
	/*初始化高级定时器1*/
	#if STSTEM_TIM1PWM_ENABLE
		sys_TIM1PWM_ENABLE();
	#endif
	
	/*初始化高级定时器8*/
	#if STSTEM_TIM8PWM_ENABLE
		sys_TIM8PWM_ENABLE();
	#endif
	
	/*初始化时基定时器6*/
	#if STSTEM_TIM6_ENABLE
		sys_TIM6_ENABLE();
	#endif
	
	/*初始化时基定时器7*/
	#if STSTEM_TIM7_ENABLE
		sys_TIM7_ENABLE();
	#endif
	
	/*初始化多功能定时器2*/
	#if STSTEM_TIM2_ENABLE
		/*说明TIM2的用途*/
		sys_TIM2_ENABLE();
	#endif
	
	/*初始化看门狗*/
	#if SYSTEM_IWDG_ENABLE
		sys_IWDG_ENABLE();
	#endif
	
	
	buzzer_bibi_once; //响一声表示初始化结束
	printf_uart(UART1,"System init over\r\n");
}

/*____________运行错误提示和打印______________________________*/
/********************************
*描述：表示某步骤运行有问题，串口提示，灯提示，声提示
*参数：		错误或者警告信息
			故障代号
			错误类别（可选flag_Fault或flag_Warning）
*返回值：	1、NULL
********************************/
void FaultASSERT(char* FaultMessage,u8 code,u8 flag)
{
	char* faultMessage_buf = (char*)mymalloc(InrRAM,100 * sizeof(char));
	
	switch(flag)
	{
		case flag_Fault:
			sprintf_(faultMessage_buf,"Fault: %s, code:%d\r\n",FaultMessage,code);
			
			/*往串口1发送数据*/
			printf_uart(UART1,"%s",faultMessage_buf);
			printf_uart(UART1,"File&Line: %s,%d\r\n",__FILE__,__LINE__);
			POINT_COLOR = RED;
			LCD_ShowString(5,0,16,(u8*)faultMessage_buf);
			
			myfree(InrRAM,faultMessage_buf);
			//灯提示，声提示
			buzzer_bibi_on;
			for(;;)
			{
				//在错误的道路上，奔跑也没用，停止就是进步
			}
		case flag_Warning:
			sprintf_(faultMessage_buf,"Warning: %s, code:%d\r\n",FaultMessage,code);
			
			printf_uart(UART1,"%s",faultMessage_buf);
//			printf_uart(UART1,"File&Line: %s,%d\r\n",__FILE__,__LINE__);	//小小警告不必显示位置了
			POINT_COLOR = YELLOW;
			LCD_ShowString(5,0,16,(u8*)faultMessage_buf);
			
			myfree(InrRAM,faultMessage_buf);
			buzzer_bibi_once;
			break;
		default:break;
	}
	myfree(InrRAM,faultMessage_buf);
}

/*_____________获取系统运行的时间________________________*/
/********************************
*描述：获取系统运行的时间
*参数：		mins、secs、_10ms:系统运行时间的分钟数，在此基础上多出的秒数，和在此基础上多出的10毫秒数
*返回值：	1、系统运行的总秒数
********************************/
u16 sys_GetsysRunTime(u16* mins,u16* secs,u16* _10ms)
{
	if(mins != NULL)
		*mins = Timer_IT_flags._1min;
	if(secs != NULL)
		*secs = Timer_IT_flags._1sec;
	if(_10ms != NULL)
		*_10ms = Timer_IT_flags._10msec;
	return ((Timer_IT_flags._1min)*60 + Timer_IT_flags._1sec);
}

/*__________时钟系统配置函数_____________*/
/********************************
*描述：上电时必要进行的时钟配置，默认使用外部高速信号源，外部晶振5M，主频168M
*参数：		1、NULL
*返回值：	1、HAL_OK,成功
			2、HAL_ERROR,失败
********************************/
u8 Stm32_Clock_Init(void)
{
/*
		时钟分配：
		HSE外接8Mhz晶振，LSE外接32.768Khz晶振
		开启HSE（8M）、LSE（32.768K）和LSI（32K），关闭HSI（16M）
		PLLCLK = SYSCLK = AHBCLK = HCLK 均为168MHz；APB2外设为84MHz，APB2定时器为168MHz；APB1外设为42Mhz，APB1定时器为84Mhz
		
		ETH\FSMC\USB FS\USB HS\GPIO\DMA 的最高速度具体看手册
		RTC 时钟取自 LSE = 32.768K	启用RTC时会开
		IWDG时钟取自 LSI = 32K		启用看门狗时会开
					 HSI = 16M
		DCMI 54 Mbyte/s max
		
		APB2外设(84M)：SDIO\EXT IT\USART1\USART6\SPI1\ADC123
		APB2定时器(168M)：TIM1 TIM8 TIM9 TIM10 TIM11
		APB1外设(42M)：USART2\USART3\UART4\UART5\SPI2、I2S2\SPI3、I2S3\I2C123\bxCAN12\DAC12
		APB1定时器(84M)：TIM2 TIM3 TIM4 TIM5 TIM12 TIM13 TIM14 TIM6 TIM7
*/
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	  /* Enable Power Control clock */
	  __HAL_RCC_PWR_CLK_ENABLE();
	  /* The voltage scaling allows optimizing the power consumption when the device is 
		 clocked below the maximum system frequency, to update the voltage scaling value 
		 regarding system frequency refer to product datasheet.  */
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	
	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;	//8M
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;				//PLLP 168M
	RCC_OscInitStruct.PLL.PLLQ = 7;							//USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		return HAL_ERROR;
	}
	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;	//SYSCLK 168M
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;			//AHBCLK 168M
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;			//APB1CLK 42M
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;			//APB2CLK 84M

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		return HAL_ERROR;
	}
	
	  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
	  if (HAL_GetREVID() == 0x1001)
	  {
		/* Enable the Flash prefetch */
		__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	  }
	
	/** Enables the Clock Security System
	*/
	HAL_RCC_EnableCSS();
	
	
//	#if SYSTEM_SUPPORT_OS
//	  //这里为了兼容FreeRTOS，把systick设置为1ms中断
//		u32 reload;
//		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
//		reload=SystemCoreClock/1000000;				//每秒钟的计数次数 单位为M  
//		reload*=1000000/configTICK_RATE_HZ;			//根据configTICK_RATE_HZ设定溢出时间   
//		
//		SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
//		SysTick->LOAD=reload; 						//每1/configTICK_RATE_HZ秒中断一次	
//		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK
//		
//		HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);	
//		HAL_NVIC_EnableIRQ(SysTick_IRQn);
//	#else
//		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
//		
//		HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
//		HAL_NVIC_EnableIRQ(SysTick_IRQn);
//	#endif
	
	/*systick 统一设置成1ms中断*/
		u32 reload;
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
		reload = SystemCoreClock/1000000;				//每秒钟的计数次数 单位为M  
		reload *= (1000000/1000);						//设定溢出时间，1KHz
		
		SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
		SysTick->LOAD=reload; 						//每1/1000秒中断一次
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK
		
		HAL_NVIC_SetPriority(SysTick_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(SysTick_IRQn);
	
	return HAL_OK;
}

/*__________定时器中断回调函数_____________*/
/********************************
*描述：回调函数，定时器中断服务函数调用，不要写入太多程序！
*参数：		1、NULL
*返回值：	1、NULL
********************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	#if STSTEM_TIM3PWM_ENABLE
		static u8 is_reversal = 0;
		static u8 is_runOnce4bibi = 0;	//运行了一次置一，一次也没运行时置零
		static u8 is_runOnce4biOnce = 0;
	#endif
	
	#if SYSTEM_UseTIM5ForTiming_ENABLE
		if(htim==(&TIM5_Handler))
		{
									//每 0.065536s中断一次
			TIM5_InterruptTimes++;	//u16类型
									//0.065536 * 65536 = 4,294.967296s，TIM5_InterruptTimes溢出
		}
	#endif
	
	/*注意：不要写入太多程序从而占用时间！*/
    if(htim==(&TIM4_Handler))
    {
		Timer_IT_flags._10msec_flag = TRUE;
		
		/*如果启用定时器2的正交解码功能，这里定时10ms周期计算正交计数值并计算返回速度 单位 转/秒*/
		#if (STSTEM_TIM2_ENABLE)&&(STSTEM_TIM2_asPWMorCap == 3)
			//如果接着速度的变量是x，下句应写成 x = peek_TIM2_Encoder_Speed(); 返回的是float类型
			peek_TIM2_Encoder_Speed();
		#endif
		
		if(++Timer_IT_flags._10msec % 10 == 0)
		{
			
			Timer_IT_flags._100msec_flag = TRUE;
			Timer_IT_flags._100msec++;
			
			#if SYSTEM_IWDG_ENABLE
				IWDG_Feed();//来喂狗了，狗狗乖哦
			#endif
			
		}
		
		if(Timer_IT_flags._10msec >= 30)
		{
			Timer_IT_flags._10msec = 0;
			Timer_IT_flags._300msec_flag = TRUE;
			
			
			#if STSTEM_TIM3PWM_ENABLE
				if(is_buzzer_once)
				{
					TIM3_set_Channel_Pulse(TIM3PWM_BuzzerCh,80.0); //打开蜂鸣器
					is_buzzer_once = FALSE;
					is_runOnce4biOnce = TRUE;
				}else{
					if(is_runOnce4biOnce)
					{
						TIM3_set_Channel_Pulse(TIM3PWM_BuzzerCh,0); //关闭蜂鸣器
						is_runOnce4biOnce = FALSE;
					}
				}
			#endif
			
		}
		
		if(Timer_IT_flags._100msec >= 10)
		{
			Timer_IT_flags._100msec = 0;
			Timer_IT_flags._1sec_flag = TRUE;
			Timer_IT_flags._1sec++;
			
			#if STSTEM_TIM3PWM_ENABLE
				if(is_buzzer_bibi)
				{
					if(is_reversal) TIM3_set_Channel_Pulse(TIM3PWM_BuzzerCh,80.0); //打开蜂鸣器
					else TIM3_set_Channel_Pulse(TIM3PWM_BuzzerCh,0); //关闭蜂鸣器
					is_runOnce4bibi = TRUE;
					is_reversal = ~is_reversal;
				}else{
					if(is_runOnce4bibi)
					{
						TIM3_set_Channel_Pulse(TIM3PWM_BuzzerCh,0);//关闭蜂鸣器
						is_runOnce4bibi = FALSE;
					}
					
				}
			#endif
		}
		
		if(Timer_IT_flags._1sec >= 60)
		{
            Timer_IT_flags._1sec = 0;
			Timer_IT_flags._1min_flag = TRUE;
			Timer_IT_flags._1min++;
			
			
			if((Timer_IT_flags._1min > 666)){
				Timer_IT_flags._1min = 0;
				
				//超过时间，可以设定强制关闭
				//_OutofTime_Running_flag = 1;
				//CTRL_DCDC_ON_OFF = DCDC_OFF;
			}
		}
    }
	
	#if STSTEM_TIM3PWM_ENABLE
		/*定时器3周期回调*/
		if(htim==(&TIM3_Handler))
		{
			//LED1=!LED1;
		}
	#endif
	
	#if STSTEM_TIM6_ENABLE
		/*定时器6周期回调*/
		if(htim==(&TIM6_Handler))
		{
			
		}
	#endif
	
	#if STSTEM_TIM7_ENABLE
		/*定时器7周期回调*/
		if(htim==(&TIM7_Handler))
		{
			
		}
	#endif
		
	#if STSTEM_TIM2_ENABLE
	/*定时器2周期回调*/
	if(htim==(&TIM2_Handler))
    {
		#if (STSTEM_TIM2_asPWMorCap == 1)					//使用输入捕获功能，定时器溢出一次则溢出次数加一
			if((TIM2CHx_CAPTURE_STA&0X80)==0)				//还未成功捕获
			{
				if(TIM2CHx_CAPTURE_STA&0X40)				//已经捕获到高电平了
				{
					if((TIM2CHx_CAPTURE_STA&0X3F)==0X3F)	//高电平太长了
					{
						TIM2CHx_CAPTURE_STA|=0X80;			//标记成功捕获了一次
						TIM2CHx_CAPTURE_VAL=0XFFFF;
					}else TIM2CHx_CAPTURE_STA++;
				}	 
			}
		#endif
		
		#if (STSTEM_TIM2_asPWMorCap == 3)					//使用正交解码功能，计数溢出次数记录
			if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&TIM2_Handler))
				EncoderOverflowCount--;       //向下计数溢出
			else
				EncoderOverflowCount++;       //向上计数溢出
		#endif
    }
	#endif
}

/*__________定时器底层配置函数_____________*/
/********************************
*描述：定时器底册驱动，开启时钟，设置中断优先级，此函数会被HAL_TIM_Base_Init()函数调用
*参数：		1、NULL
*返回值：	1、NULL
********************************/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //使能TIM4时钟
		__HAL_TIM_CLEAR_IT(&TIM4_Handler, TIM_IT_UPDATE);  // 清除更新中断标志位
		__HAL_TIM_ENABLE_IT(&TIM4_Handler,TIM_IT_UPDATE);  // 使能更新中断
		
		HAL_NVIC_SetPriority(TIM4_IRQn,1,0);    //设置中断优先级，抢占优先级1，响应优先级0。分组为4
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM4中断   
	}
	
	#if STSTEM_TIM6_ENABLE
		if(htim->Instance==TIM6)
		{
			__HAL_RCC_TIM6_CLK_ENABLE();            //使能TIM6时钟
			__HAL_TIM_CLEAR_IT(&TIM6_Handler, TIM_IT_UPDATE);  // 清除更新中断标志位
			__HAL_TIM_ENABLE_IT(&TIM7_Handler,TIM_IT_UPDATE);  // 使能更新中断
			
			HAL_NVIC_SetPriority(TIM6_DAC_IRQn,4,0);    //设置中断优先级，抢占优先级1，响应优先级0。分组为4
			HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);          //开启ITM6中断   
		}
	#endif
	
	#if STSTEM_TIM7_ENABLE
		if(htim->Instance==TIM7)
		{
			__HAL_RCC_TIM7_CLK_ENABLE();            //使能TIM7时钟
			__HAL_TIM_CLEAR_IT(&TIM7_Handler, TIM_IT_UPDATE);  // 清除更新中断标志位
			__HAL_TIM_ENABLE_IT(&TIM7_Handler,TIM_IT_UPDATE);  // 使能更新中断
			
			HAL_NVIC_SetPriority(TIM7_IRQn,4,0);    //设置中断优先级，抢占优先级1，响应优先级0。分组为4
			HAL_NVIC_EnableIRQ(TIM7_IRQn);          //开启ITM7中断   
		}
	#endif
	
	
	#if STSTEM_TIM3PWM_ENABLE
		if(htim->Instance==TIM3)
		{
			__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
			__HAL_TIM_CLEAR_IT(&TIM3_Handler, TIM_IT_UPDATE);  // 清除更新中断标志位
			
			#if STSTEM_TIM3PWM_TI_ENABLE
				__HAL_TIM_ENABLE_IT(&TIM3_Handler,TIM_IT_UPDATE);  // 使能更新中断
				HAL_NVIC_SetPriority(TIM3_IRQn,3,0);    //设置中断优先级，抢占优先级3，子优先级0
				HAL_NVIC_EnableIRQ(TIM3_IRQn);      //开启ITM3中断
			#endif
		}
	#endif
	
	
	#if STSTEM_TIM2_ENABLE
		if(htim->Instance==TIM2)
		{
			__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
			__HAL_TIM_CLEAR_IT(&TIM2_Handler, TIM_IT_UPDATE);  // 清除更新中断标志位
			
			#if STSTEM_TIM2_TI_ENABLE
				//__HAL_TIM_URS_ENABLE(&TIM2_Handler);               // 仅允许计数器溢出才产生更新中断
				__HAL_TIM_ENABLE_IT(&TIM2_Handler,TIM_IT_UPDATE);  // 使能更新中断
				HAL_NVIC_SetPriority(TIM2_IRQn,3,0);
				HAL_NVIC_EnableIRQ(TIM2_IRQn);
			#endif
		}
	#endif
		
	#if SYSTEM_UseTIM5ForTiming_ENABLE
		if(htim->Instance==TIM5)
		{
			__HAL_RCC_TIM5_CLK_ENABLE();            //使能TIM5时钟
			
			__HAL_TIM_CLEAR_IT(&TIM5_Handler, TIM_IT_UPDATE);  	// 清除更新中断标志位
			__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);				//清零TIM5定时器的计数器值
			
			HAL_NVIC_SetPriority(TIM5_IRQn,1,0);    //设置中断优先级，抢占优先级1
			HAL_NVIC_EnableIRQ(TIM5_IRQn);          //开启ITM5中断   
		}
	#endif
}

/*__________定时器PWM初始化底层配置函数_____________*/
/********************************
*描述：定时器PWM底层驱动，开启时钟，设置GPIO，此函数会被HAL_TIM_PWM_Init()函数调用
*参数：		1、NULL
*返回值：	1、NULL
********************************/
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3时钟
		
		if(STSTEM_TIM3PWM_REMAP_ENABLE)	//如果全部重映射
		{
			//__HAL_AFIO_REMAP_TIM3_ENABLE();			
			//TIM3通道引脚全部重映射使能，具体怎么映射看库函数注释
			__HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0001)	GPIO_Initure.Pin=GPIO_PIN_6;	//如果开启通道1，PC6
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0010)	GPIO_Initure.Pin=GPIO_PIN_7;	//如果开启通道2，PC7
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0100)	GPIO_Initure.Pin=GPIO_PIN_8;	//如果开启通道3，PC8
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_1000)	GPIO_Initure.Pin=GPIO_PIN_6;	//如果开启通道4，PC9
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;          //上拉，所以硬件上最好低电平有效，高电平截止
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF2_TIM3;
			HAL_GPIO_Init(GPIOC,&GPIO_Initure); 
		}else if(STSTEM_TIM3PWM_REMAP_PARTIAL)	//如果部分重映射
		{
			//__HAL_AFIO_REMAP_TIM3_PARTIAL();		
			//TIM3通道引脚部分重映射使能，具体怎么映射看库函数注释
			__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0001)	GPIO_Initure.Pin=GPIO_PIN_4;	//如果开启通道1，PB4
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0010)	GPIO_Initure.Pin=GPIO_PIN_5;	//如果开启通道2，PB5
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0100)	GPIO_Initure.Pin=GPIO_PIN_0;	//如果开启通道3，PB0
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_1000)	GPIO_Initure.Pin=GPIO_PIN_1;	//如果开启通道4，PB1
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;          //上拉，所以硬件上最好低电平有效，高电平截止
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF2_TIM3;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure); 			
		}else{									//没有重映射
			
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;          //上拉，所以硬件上最好低电平有效，高电平截止
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0001)	
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_6;	//如果开启通道1，PA6
				GPIO_Initure.Alternate = GPIO_AF2_TIM3;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			}
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0010)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_7;	//如果开启通道2，PA7
				GPIO_Initure.Alternate = GPIO_AF2_TIM3;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			}
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0100)
			{
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_0;	//如果开启通道3，PB0
				GPIO_Initure.Alternate = GPIO_AF2_TIM3;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_1000)
			{
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_1;	//如果开启通道4，PB1
				GPIO_Initure.Alternate = GPIO_AF2_TIM3;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure);
			}
		}
	}
	#if (STSTEM_TIM2_ENABLE) && (STSTEM_TIM2_asPWMorCap == 0)
	if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();			//使能定时器2时钟
		//__HAL_AFIO_REMAP_TIM2_ENABLE();			/*TIM2通道引脚完全重映射使能 (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11)*/
		
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_Initure.Pull=GPIO_NOPULL;          //上拉，所以硬件上最好低电平有效，高电平截止
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
		GPIO_Initure.Alternate = GPIO_AF1_TIM2;
		
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_0001)	//CH1/ETR/PA15
		{
			__HAL_RCC_GPIOA_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_15;
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		}
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_0010)	//CH2/PB3
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_3;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		}
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_0100)	//CH3/PB10
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_10;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		}
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_1000)	//CH4/PB11
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_11;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		}
	}
	#endif
	
	#if STSTEM_TIM1PWM_ENABLE
	if(htim->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();			//使能定时器1时钟
		
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;
		GPIO_Initure.Pull=GPIO_NOPULL;
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_Initure.Alternate = GPIO_AF1_TIM1;
		
		/*引脚：
						CH1/CH1N		CH2/CH2N		CH3/CH3N		CH4		BKIN（刹车信号输入）
		默认			PA8/PB13		PA9/PB14		PA10/PB15		PA11	PB12
		部分重映射		PA8/PA7			PA9/PB0			PA10/PB1		PA11	PA6
		完全重映射		PE9/PE8			PE11/PE10		PE13/PE12		PE14	PE15
		*/
		if(STSTEM_TIM1PWM_REMAP_PARTIAL)
		{
			//__HAL_AFIO_REMAP_TIM1_PARTIAL();
			
			if(STSTEM_TIM1PWM_useBreak)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_6;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			}
			
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0001)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_7;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_8;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0010)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_9;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_0;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0100)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_10;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_1;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_1000)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_11;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			}
		}else if(STSTEM_TIM1PWM_REMAP_ENABLE)
		{
			//__HAL_AFIO_REMAP_TIM1_ENABLE();
			__HAL_RCC_GPIOE_CLK_ENABLE();
			
			if(STSTEM_TIM1PWM_useBreak)
			{
				GPIO_Initure.Pin=GPIO_PIN_15;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure);
			}
			
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0001)
			{
				GPIO_Initure.Pin=GPIO_PIN_9;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_8;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0010)
			{
				GPIO_Initure.Pin=GPIO_PIN_11;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_10;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0100)
			{
				GPIO_Initure.Pin=GPIO_PIN_13;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_12;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_1000)
			{
				GPIO_Initure.Pin=GPIO_PIN_14;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
			}
		}else{
			/*引脚：
							CH1/CH1N		CH2/CH2N		CH3/CH3N		CH4		BKIN（刹车信号输入）
			默认			PA8/PB13		PA9/PB14		PA10/PB15		PA11	PB12
			部分重映射		PA8/PA7			PA9/PB0			PA10/PB1		PA11	PA6
			完全重映射		PE9/PE8			PE11/PE10		PE13/PE12		PE14	PE15
			*/
			
			if(STSTEM_TIM1PWM_useBreak)
			{
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_12;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure);
			}
			
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0001)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_8;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_13;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0010)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_9;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_14;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0100)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_10;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_15;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_1000)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_11;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			}
		}
		
	}
	#endif
	
	#if STSTEM_TIM8PWM_ENABLE
	if(htim->Instance==TIM8)
	{
		/*引脚：
						CH1/CH1N		CH2/CH2N		CH3/CH3N		CH4		BKIN（刹车信号输入）
						PC6/PA7			PC7/PB0			PC8/PB1			PC9		PA6
		*/
		__HAL_RCC_TIM8_CLK_ENABLE();			//使能定时器8时钟
		
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;
		GPIO_Initure.Pull=GPIO_NOPULL;
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_Initure.Alternate = GPIO_AF3_TIM8;
		
		if(STSTEM_TIM8PWM_useBreak)
		{
			__HAL_RCC_GPIOA_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_6;
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		}
		
		if(STSTEM_TIM8PWM_CHANNEL_ENABLE & B0000_0001)
		{
			__HAL_RCC_GPIOA_CLK_ENABLE();
			__HAL_RCC_GPIOC_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_6;
			HAL_GPIO_Init(GPIOC,&GPIO_Initure);
			GPIO_Initure.Pin=GPIO_PIN_7;
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		}
		if(STSTEM_TIM8PWM_CHANNEL_ENABLE & B0000_0010)
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
			__HAL_RCC_GPIOC_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_0;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);
			GPIO_Initure.Pin=GPIO_PIN_7;
			HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		}
		if(STSTEM_TIM8PWM_CHANNEL_ENABLE & B0000_0100)
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
			__HAL_RCC_GPIOC_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_8;
			HAL_GPIO_Init(GPIOC,&GPIO_Initure);
			GPIO_Initure.Pin=GPIO_PIN_1;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		}
		if(STSTEM_TIM8PWM_CHANNEL_ENABLE & B0000_1000)
		{
			__HAL_RCC_GPIOC_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_9;
			HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		}
	}
	#endif
}


#if STSTEM_TIM4_ENABLE
	struct TIM4_IT_FLAGS Timer_IT_flags = 
	{
		._10msec_flag = 0,
		._10msec = 0,
		._100msec_flag = 0,
		._100msec = 0,
		._300msec_flag = 0,
		._1sec_flag  = 0,
		._1sec = 0,
		._1min_flag = 0,
		._1min = 0
	};

	//通用定时器4中断初始化，arr：自动重装值，psc：时钟预分频数，定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us
	//此处默认频率为100Hz，10ms产生一次中断
	TIM_HandleTypeDef TIM4_Handler;

	void sys_TIM4_ENABLE(void)
	{
		TIM_ClockConfigTypeDef sClockSourceConfig = {0};
		TIM_MasterConfigTypeDef sMasterConfig = {0};
		
		TIM4_Handler.Instance=TIM4;                          		//通用定时器4
		TIM4_Handler.Init.Prescaler=(8400-1);                     	//分频系数
		TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    		//向上计数器
		TIM4_Handler.Init.Period=(100-1);                        	//自动装载值
		TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;		//时钟分频因子
		TIM4_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		HAL_TIM_Base_Init(&TIM4_Handler);
		
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		HAL_TIM_ConfigClockSource(&TIM4_Handler, &sClockSourceConfig);
		
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		HAL_TIMEx_MasterConfigSynchronization(&TIM4_Handler, &sMasterConfig);
		
		HAL_TIM_Base_Start_IT(&TIM4_Handler); //使能定时器4和定时器4更新中断：TIM_IT_UPDATE 
	}

	//定时器4中断服务函数
	void TIM4_IRQHandler(void)
	{
		HAL_TIM_IRQHandler(&TIM4_Handler);
	}
#endif

#if STSTEM_TIM3PWM_ENABLE

TIM_HandleTypeDef 	TIM3_Handler;      	//定时器句柄
TIM_OC_InitTypeDef 	TIM3_CH1Handler,TIM3_CH2Handler,TIM3_CH3Handler,TIM3_CH4Handler;	//定时器3通道1-4句柄
void sys_TIM3PWM_ENABLE(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	TIM3_Handler.Instance=TIM3;          	//定时器3
	TIM3_Handler.Init.Prescaler=tim3prsc;       //定时器分频
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
	TIM3_Handler.Init.Period=tim3arr;          //自动重装载值
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	TIM3_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&TIM3_Handler);
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&TIM3_Handler, &sClockSourceConfig);
	
	HAL_TIM_PWM_Init(&TIM3_Handler);       //初始化PWM
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM3_Handler, &sMasterConfig);
	
    #if STSTEM_TIM3PWM_TI_ENABLE
		HAL_TIM_Base_Start_IT(&TIM3_Handler); 	//使能定时器3和定时器3更新中断：TIM_IT_UPDATE
	#else
		HAL_TIM_Base_Start(&TIM3_Handler);		//不带IT的开启定时器
	#endif
	
	if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0010)
	{
		TIM3_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
		TIM3_CH2Handler.Pulse=tim3arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
		/*输出比较极性为低，对于PWM1模式时，当计数值小于比较值时输出低电平，PWM2与PWM1正好相反，注意规范定义低电平有效，高电平截止*/
		/*即当计数值小于比较值时输出有效，器件可以工作，即这个比较值越大，器件工作时间越长，即 占空比 越大*/
		TIM3_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW;
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH2Handler,TIM_CHANNEL_2);//配置TIM3通道2
		__HAL_TIM_ENABLE_OCxPRELOAD(&TIM3_Handler, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);//开启PWM通道2
	}
	if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0001)
	{
		TIM3_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
		TIM3_CH1Handler.Pulse=tim3arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
		TIM3_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
		TIM3_CH1Handler.OCFastMode = TIM_OCFAST_DISABLE;
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH1Handler,TIM_CHANNEL_1);//配置TIM3通道1
		__HAL_TIM_ENABLE_OCxPRELOAD(&TIM3_Handler, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_1);//开启PWM通道1
	}
	if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0100)
	{
		TIM3_CH3Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
		TIM3_CH3Handler.Pulse=tim3arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
		TIM3_CH3Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH3Handler,TIM_CHANNEL_3);//配置TIM3通道3
		__HAL_TIM_ENABLE_OCxPRELOAD(&TIM3_Handler, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_3);//开启PWM通道3
	}
	if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_1000)
	{
		TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
		TIM3_CH4Handler.Pulse=tim3arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
		TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);//配置TIM3通道4
		__HAL_TIM_ENABLE_OCxPRELOAD(&TIM3_Handler, TIM_CHANNEL_4);
		HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);//开启PWM通道4
	}

}


//设置TIM3通道的占空比
//percent:占空比百分数
void TIM3_set_Channel_Pulse(u8 channel,float percent)
{
	float compare;
	if(percent < 0) percent = 0;
	if(percent > 100) percent = 100.0f;
	percent /= 100.0f;
	compare = ((float)tim3arr) * percent;
	switch(channel)
	{
		case TIM3PWM_Channel_1: TIM3->CCR1=(u32)compare;break;
		case TIM3PWM_Channel_2: TIM3->CCR2=(u32)compare;break;
		case TIM3PWM_Channel_3: TIM3->CCR3=(u32)compare;break;
		case TIM3PWM_BuzzerCh: TIM3->CCR4=(u32)compare;break;
		default:break;
	}
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}
#endif




#if SYSTEM_UseTIM5ForTiming_ENABLE

	u16 TIM5_InterruptTimes = 0;

	TIM_HandleTypeDef TIM5_Handler;
	
	void sys_TIM5_ENABLE(void)
	{
		TIM_ClockConfigTypeDef sClockSourceConfig = {0};
		TIM_MasterConfigTypeDef sMasterConfig = {0};
		
		TIM5_Handler.Instance=TIM5;                          		//通用定时器5
		TIM5_Handler.Init.Prescaler=(84-1);                     	//分频系数		计数频率1Mkz,即1us
		TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    		//向上计数器	0.065536s溢出一次
		TIM5_Handler.Init.Period=(65536-1);                        	//自动装载值	
		TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;		//时钟分频因子
//		TIM5_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		HAL_TIM_Base_Init(&TIM5_Handler);
		
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		HAL_TIM_ConfigClockSource(&TIM5_Handler, &sClockSourceConfig);
		
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		HAL_TIMEx_MasterConfigSynchronization(&TIM5_Handler, &sMasterConfig);
		
		HAL_TIM_Base_Start_IT(&TIM5_Handler); //使能定时器5和定时器5更新中断：TIM_IT_UPDATE 
	}

	//定时器5中断服务函数
	void TIM5_IRQHandler(void)
	{
		HAL_TIM_IRQHandler(&TIM5_Handler);
	}
	
	//测量(*FunctionForTiming)()函数运行时间，返回单位为us的运行时间，没有超时退出机制，不适合在有操作系统的环境中用
	//最长可测量约4290s，0.065536 * 65536 = 4,294.967296s，TIM5_InterruptTimes溢出
	u32 Timing(void (*FunctionForTiming)())
	{
		u32 FunctionRunTime_us;
		
		TIM5_InterruptTimes = 0;
		__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);
		(*FunctionForTiming)();
		FunctionRunTime_us = (u32)__HAL_TIM_GET_COUNTER(&TIM5_Handler) + (u32)TIM5_InterruptTimes * 65536;
		
		return FunctionRunTime_us;
	}
	
#endif
#if SYSTEM_UART1_ENABLE||SYSTEM_UART2_ENABLE||SYSTEM_UART3_ENABLE

UART_HandleTypeDef UART1_Handler,UART2_Handler,UART3_Handler; //UART句柄
u8 aRxBuffer1[RXBUFFERSIZE],aRxBuffer2[RXBUFFERSIZE],aRxBuffer3[RXBUFFERSIZE];//HAL库使用的串口接收缓冲

/*printf函数和printf_uart会调用此函数发送单个字节，需填入串口发送，只用printf_uart就行了*/
void _putchar(char character)
{
	// send char to console etc.
	//如果使用RTOS，这里是否应该停止其调度器，在结束处再打开
	switch(uart2sent)/*uart2sent全局变量，用于判断发送到哪一个串口*/
	{
		case UART1:/*发往串口1*/
			HAL_UART_Transmit(&UART1_Handler, (uint8_t *)&character, 1, 0xA); 	//超时时间为0xA，为10ms
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET){}		//等待发送完成
				return ; 
		case UART2:/*发往串口2*/
			HAL_UART_Transmit(&UART2_Handler, (uint8_t *)&character, 1, 0x12C);
			while(__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_TC)!=SET){}
				return ;
		case UART3:/*发往串口3*/
			HAL_UART_Transmit(&UART3_Handler, (uint8_t *)&character, 1, 0x12C);
			while(__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_TC)!=SET){}
				return ;
		default:/*默认发往串口1*/
			HAL_UART_Transmit(&UART1_Handler, (uint8_t *)&character, 1, 0xA);
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET){}
				return ;
	}
}

#if SYSTEM_UART1_ENABLE

char USART1_RX_BUF[USART1_RX_BUF_MaxNum]; 				/*串口1的模式0数据接受区*/
char USART1_RX_FIFO_buf[USART1_RX_FIFO_MaxNum]; 		/*串口1的模式1的FIFO数据接受区*/
lwrb_t USART1_RX_FIFO_Handle;							/*用于串口1模式1的FIFO的句柄*/

void sys_USART1_ENABLE(void)
{
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    		//USART1
	UART1_Handler.Init.BaudRate=(uint32_t)SYSTEM_UART1_BOUND;	//波特率
	
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   		//字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    		//一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    		//无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   		//无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    		//收发模式
	UART1_Handler.Init.OverSampling = UART_OVERSAMPLING_16;		//不知道有没有影响（TODO:试验有没有这句话有什么变化）
	HAL_UART_Init(&UART1_Handler);					    		//HAL_UART_Init()会使能UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer1, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	

	lwrb_init(&USART1_RX_FIFO_Handle, USART1_RX_FIFO_buf, sizeof(USART1_RX_FIFO_buf));
}

u8 sys_USART1_RX_Fetch(u8 is_print, char *buf,u16 *RX_ByteNum)
{
	if(!((USART1_RX_CONFIG) & (USART1_RX_MODE_mask)))
	{
		/*0为只接受以'\r\n'结尾的数据*/
		if(USART1_isDone)
		{
			if(is_print)
				printf_uart(UART1,"%s",USART1_RX_BUF);
			
			if(USART1_RX_ByteNum >= USART1_RX_BUF_MaxNum) /*防止栈溢出*/
			{
				mystrncpy(buf,USART1_RX_BUF,USART1_RX_BUF_MaxNum);
			}else{
				mystrncpy(buf,USART1_RX_BUF,USART1_RX_ByteNum);
			}
			//buf = USART1_RX_BUF;
			*RX_ByteNum = USART1_RX_ByteNum;
			/*USART1_RX_BUF共有USART1_RX_ByteNum个有效字节，不带\r\n的字符串，第USART1_RX_ByteNum的位置是结束符\0*/
			USART1_SetUnDone;
			return HAL_OK;
		}else
		{
			return HAL_ERROR;
		}
	}else{
		/*1为以FIFO先进先出的环形缓存实现接受区，无协议*/
		
		char buf_buf = NULL;
		u16 RX_ByteNum_buf = 0;
		
		while(lwrb_get_full(&USART1_RX_FIFO_Handle))
		{
			lwrb_peek(&USART1_RX_FIFO_Handle,0,&buf_buf,1);
			mystrcat(buf,&buf_buf);
			RX_ByteNum_buf++;
		}
		//加个结束符
		mystrcat(buf,(char*)'\0');
		
		*RX_ByteNum = RX_ByteNum_buf;
		
		if(buf_buf != NULL)
		{
			return HAL_OK;
		}else
		{
			return HAL_ERROR;
		}
	}
}

#endif

#if SYSTEM_UART2_ENABLE
void sys_USART2_ENABLE(void)
{
	//UART 初始化设置
	UART2_Handler.Instance=USART2;					    		//USART2
	UART2_Handler.Init.BaudRate=(uint32_t)SYSTEM_UART2_BOUND;	//波特率
	
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   		//字长为8位数据格式
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    		//一个停止位
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    		//无奇偶校验位
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   		//无硬件流控
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    		//收发模式
	HAL_UART_Init(&UART2_Handler);					    		//HAL_UART_Init()会使能UART2
	
	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}


u8 sys_USART2_RX_Fetch(u8 is_print, char *buf,u16 *RX_ByteNum)
{
	return HAL_OK;
}
#endif
#if SYSTEM_UART3_ENABLE
void sys_USART3_ENABLE(void)
{
	//UART 初始化设置
	UART3_Handler.Instance=USART3;					    		//USART3
	UART3_Handler.Init.BaudRate=(uint32_t)SYSTEM_UART3_BOUND;	//波特率
	
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   		//字长为8位数据格式
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    		//一个停止位
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    		//无奇偶校验位
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   		//无硬件流控
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    		//收发模式
	HAL_UART_Init(&UART3_Handler);					    		//HAL_UART_Init()会使能UART3
	
	HAL_UART_Receive_IT(&UART3_Handler, (u8 *)aRxBuffer3, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}

u8 sys_USART3_RX_Fetch(u8 is_print, char *buf,u16 *RX_ByteNum)
{
	return HAL_OK;
}
#endif


//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
		//__HAL_RCC_AFIO_CLK_ENABLE();
		#if SYSTEM_UART1_REMAP_ENABLE
			//__HAL_AFIO_REMAP_USART1_ENABLE();
			__HAL_RCC_GPIOB_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_6;			//PB6 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF7_USART1;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB6

			GPIO_Initure.Pin=GPIO_PIN_7;			//PB7 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用模式
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB7
		#else 
			__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
			GPIO_Initure.Pin=GPIO_PIN_9;			//PA9 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF7_USART1;
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

			GPIO_Initure.Pin=GPIO_PIN_10;			//PA10 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用模式	
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		#endif
		
		HAL_NVIC_SetPriority(USART1_IRQn,3,0);			//抢占优先级3，子优先级0
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
	}
	
	if(huart->Instance==USART2)//如果是串口2，进行串口2 MSP初始化
	{
		__HAL_RCC_USART2_CLK_ENABLE();			//使能USART2时钟
		//__HAL_RCC_AFIO_CLK_ENABLE();
		#if SYSTEM_UART2_REMAP_ENABLE
			//__HAL_AFIO_REMAP_USART2_ENABLE();
			__HAL_RCC_GPIOD_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_5;			//PD5 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF7_USART2;
			HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PD5

			GPIO_Initure.Pin=GPIO_PIN_6;			//PD6 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;		
			HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PD6
		#else 
			__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
			GPIO_Initure.Pin=GPIO_PIN_2;			//PA2 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF7_USART2;
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA2

			GPIO_Initure.Pin=GPIO_PIN_3;			//PA3 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;			
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA3
		#endif
		
		HAL_NVIC_SetPriority(USART2_IRQn,4,0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//使能USART2中断通道
	}
	
	if(huart->Instance==USART3)//如果是串口3，进行串口3 MSP初始化
	{
		__HAL_RCC_USART3_CLK_ENABLE();			//使能USART3时钟
		//__HAL_RCC_AFIO_CLK_ENABLE();
		#if SYSTEM_UART3_REMAP_ENABLE
			//__HAL_AFIO_REMAP_USART3_ENABLE();
			__HAL_RCC_GPIOD_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_8;			//PD8 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF7_USART3;
			HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PD8

			GPIO_Initure.Pin=GPIO_PIN_9;			//PD9 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;			
			HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PD9
		#else 
			__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOB时钟
			GPIO_Initure.Pin=GPIO_PIN_10;			//PB10 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF7_USART3;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB10

			GPIO_Initure.Pin=GPIO_PIN_11;			//PB11 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;			
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB11
		#endif
		
		HAL_NVIC_SetPriority(USART3_IRQn,4,0);
		HAL_NVIC_EnableIRQ(USART3_IRQn);				//使能USART3中断通道
	}
}



/*以下四行放在了.h里面*/
//#define USART1_RX_DONE_mask 0x8000
//#define USART1_RX_MODE_mask 0x4000
//#define USART1_RX_Rec_r_mask 0x2000
//#define USART1_RX_Num_mask 0x1fff /*0001 1111 1111 1111*/

u16 USART1_RX_CONFIG = 0;	/*    x            x           x        x xxxx xxxx xxxx      */
							/*(接受完成) (协议模式0/1) (接收到\r)   串口1接受数据字节计数 */
							/*串口1接受协议：0为只接受以'\r\n'结尾的数据，1为以FIFO先进先出的环形缓存实现接受区，无协议*/
/*
FIFO先进先出环形队列示意：	头指针出只读出，尾指针出只写入。
							 *		*		*		*		*	...
							 0头	1		2		3		4尾（新入的数据存在尾指针上，随着存储，尾指针向后移动）
							列队头的数据处理完了后，‘0’地址空间的数据进行释放掉，列队头指向下一个可以处理数据的地址‘1’，以此类推
							在一定内存内，尾指针一直移动到最后，这时如果0地址空闲，则把尾指针指向0，从而形成环形
*/
/*以下五行放在了.h里面*/
//#define USART1_SetMode(x) 	USART1_RX_CONFIG |= (((u16)x)<<14) /*用户可用，在任务中设置串口1接收协议*/
//#define USART1_isDone 		(USART1_RX_CONFIG & USART1_RX_DONE_mask)/*用户用，用于判断是否接受完成一次*/
//#define USART1_RX_ByteNum 	(USART1_RX_CONFIG & USART1_RX_Num_mask)/*用户可用，返回接收的字符数量*/
//#define USART1_SetDone 		USART1_RX_CONFIG |= USART1_RX_DONE_mask /*设置串口1接收完成标志位*/
//#define USART1_SetUnDone 		USART1_RX_CONFIG &= USART1_RX_MODE_mask /*用户可用，在任务中处理完后对串口1标志位进行复位，除了mode位，其他位都写0*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
	{	
		if(!((USART1_RX_CONFIG) & (USART1_RX_MODE_mask))) /*协议0：只接受以'\r\n'结尾的数据：从USART1_RX_BUF[]接受数据*/
		{ 
			if(!((USART1_RX_CONFIG) & (USART1_RX_DONE_mask))) /*如果接收未完成*/
			{
				if((USART1_RX_CONFIG) & (USART1_RX_Rec_r_mask)) /*如果接收到了\r*/
				{
					if(aRxBuffer1[0] == '\n') /*如果接受到了\r\n*/
					{
						USART1_RX_BUF[USART1_RX_ByteNum] = '\0'; /*加个结束符\0*/
						USART1_SetDone;
						//USART1_RX_CONFIG++; 
						/*接受完成后的 接受数据字节计数 不包含 '\r\n' 两个*/
					}else{USART1_SetUnDone;} /*没有接收到\n，则重新开始*/
				}else
				{
					if(aRxBuffer1[0] == '\r') {
						USART1_RX_CONFIG |= (USART1_RX_Rec_r_mask);
						//USART1_RX_CONFIG++; /*接受完成后的 接受数据字节计数 不包含 '\r\n' 两个*/
					}else{
						USART1_RX_BUF[(USART1_RX_CONFIG) & (USART1_RX_Num_mask)] = aRxBuffer1[0];
						USART1_RX_CONFIG++;
						if(((USART1_RX_CONFIG) & (USART1_RX_Num_mask)) > (USART1_RX_BUF_MaxNum-1)) USART1_SetUnDone;
					}
					
				}
			}
		}else 		/*协议1：以FIFO先进先出的环形缓存实现接受区，无协议，需要及时拿走，如果缓存满了则新入数据丢失*/
		{
			if(lwrb_get_free(&USART1_RX_FIFO_Handle))
				lwrb_write(&USART1_RX_FIFO_Handle, &aRxBuffer1[0], 1);
			
			
		}
	}else if(huart->Instance==USART2)//如果是串口2
	{
		
	}else if(huart->Instance==USART3)//如果是串口3
	{
		
	}
}

/*TODO：下面是不是可用精简精简...*/
#if SYSTEM_UART1_ENABLE
void USART1_IRQHandler(void)	//串口1中断服务程序
{ 
	u32 timeout=0;
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntEnter();  
		taskENTER_CRITICAL();           //进入临界区
	#endif
	
	HAL_UART_IRQHandler(&UART1_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer1, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
	}
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntExit();  	
		taskEXIT_CRITICAL();            //退出临界区
	#endif
}
#endif

#if SYSTEM_UART2_ENABLE
void USART2_IRQHandler(void)	//串口2中断服务程序
{ 
	u32 timeout=0;
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntEnter();    
		taskENTER_CRITICAL();           //进入临界区
	#endif
	
	HAL_UART_IRQHandler(&UART2_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART2_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
	}
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntExit();  			
		taskEXIT_CRITICAL();            //退出临界区	
	#endif
}
#endif

#if SYSTEM_UART3_ENABLE
void USART3_IRQHandler(void)	//串口3中断服务程序
{ 
	u32 timeout=0;
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntEnter();    
		taskENTER_CRITICAL();           //进入临界区
	#endif
	
	HAL_UART_IRQHandler(&UART3_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART3_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART3_Handler, (u8 *)aRxBuffer3, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
	}
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntExit();  			
		taskEXIT_CRITICAL();            //退出临界区
	#endif
}
#endif

#endif


#if SYSTEM_IWDG_ENABLE
	IWDG_HandleTypeDef IWDG_Handler; //独立看门狗句柄
	//初始化独立看门狗
	//prer:分频数:IWDG_PRESCALER_4~IWDG_PRESCALER_256
	//rlr:自动重装载值,0~0XFFF.
	//时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms)
	/*以下设置为 320ms周期看门狗 大概...*/
	void sys_IWDG_ENABLE(void)
	{
		IWDG_Handler.Instance=IWDG;
		IWDG_Handler.Init.Prescaler=IWDG_PRESCALER_256;	//设置IWDG分频系数
		IWDG_Handler.Init.Reload=32-1;		//重装载值
		HAL_IWDG_Init(&IWDG_Handler);		//初始化IWDG,默认会开启独立看门狗	
	}
	//喂独立看门狗
	void IWDG_Feed(void)
	{   
		HAL_IWDG_Refresh(&IWDG_Handler); 	//喂狗
	}
#endif


#if SYSTEM_MCO_PA8_OUT
void sys_MCO_Out_Enable(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_5);
	
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}
#endif

#if SYSTEM_CRC_ENABLE
CRC_HandleTypeDef hcrc;
void sys_CRC_ENABLE(void)
{
	hcrc.Instance = CRC;
	HAL_CRC_Init(&hcrc);
}
void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc)
{
	if(hcrc->Instance==CRC)
	{
	/* 外设时钟使能 */
	__HAL_RCC_CRC_CLK_ENABLE();
	}
}
/* 私有变量 ------------------------------------------------------------------*/
const uint32_t aDataBuffer[BUFFER_SIZE] =
  {
    0x00001021, 0x20423063, 0x408450a5, 0x60c670e7, 0x9129a14a, 0xb16bc18c,
    0xd1ade1ce, 0xf1ef1231, 0x32732252, 0x52b54294, 0x72f762d6, 0x93398318,
    0xa35ad3bd, 0xc39cf3ff, 0xe3de2462, 0x34430420, 0x64e674c7, 0x44a45485,
    0xa56ab54b, 0x85289509, 0xf5cfc5ac, 0xd58d3653, 0x26721611, 0x063076d7,
    0x569546b4, 0xb75ba77a, 0x97198738, 0xf7dfe7fe, 0xc7bc48c4, 0x58e56886,
    0x78a70840, 0x18612802, 0xc9ccd9ed, 0xe98ef9af, 0x89489969, 0xa90ab92b,
    0x4ad47ab7, 0x6a961a71, 0x0a503a33, 0x2a12dbfd, 0xfbbfeb9e, 0x9b798b58,
    0xbb3bab1a, 0x6ca67c87, 0x5cc52c22, 0x3c030c60, 0x1c41edae, 0xfd8fcdec,
    0xad2abd0b, 0x8d689d49, 0x7e976eb6, 0x5ed54ef4, 0x2e321e51, 0x0e70ff9f,
    0xefbedfdd, 0xcFFCbf1b, 0x9f598f78, 0x918881a9, 0xb1caa1eb, 0xd10cc12d,
    0xe16f1080, 0x00a130c2, 0x20e35004, 0x40257046, 0x83b99398, 0xa3fbb3da,
    0xc33dd31c, 0xe37ff35e, 0x129022f3, 0x32d24235, 0x52146277, 0x7256b5ea,
    0x95a88589, 0xf56ee54f, 0xd52cc50d, 0x34e224c3, 0x04817466, 0x64475424,
    0x4405a7db, 0xb7fa8799, 0xe75ff77e, 0xc71dd73c, 0x26d336f2, 0x069116b0,
    0x76764615, 0x5634d94c, 0xc96df90e, 0xe92f99c8, 0xb98aa9ab, 0x58444865,
    0x78066827, 0x18c008e1, 0x28a3cb7d, 0xdb5ceb3f, 0xfb1e8bf9, 0x9bd8abbb,
    0x4a755a54, 0x6a377a16, 0x0af11ad0, 0x2ab33a92, 0xed0fdd6c, 0xcd4dbdaa,
    0xad8b9de8, 0x8dc97c26, 0x5c644c45, 0x3ca22c83, 0x1ce00cc1, 0xef1fff3e,
    0xdf7caf9b, 0xbfba8fd9, 0x9ff86e17, 0x7e364e55, 0x2e933eb2, 0x0ed11ef0
  };
/* Expected CRC Value */
uint32_t uwExpectedCRCValue = 0x379E9F06;
#endif


#if SYSTEM_RTC_ENABLE

u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表
//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//year,month,day：公历年月日 
//返回值：星期号 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}

RTC_HandleTypeDef RTC_Handler;		//RTC句柄

/*初始化RTC时钟,同时检测时钟是否工作正常
BKP->DR1用于保存是否第一次配置的设置*/
void sys_RTC_Enable(void)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
//	RTC_AlarmTypeDef sAlarm = {0};
	
	RTC_Handler.Instance=RTC;
	RTC_Handler.Init.HourFormat = RTC_HOURFORMAT_24;
	RTC_Handler.Init.SynchPrediv = 273;			
	RTC_Handler.Init.AsynchPrediv = 120; 		//这里最大127.两个预分频吧应该是，乘起来等于32768
	RTC_Handler.Init.OutPut = RTC_OUTPUT_DISABLE;
	RTC_Handler.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RTC_Handler.Init.OutPutType = RTC_OUTPUT_TYPE_PUSHPULL;
	HAL_RTC_Init(&RTC_Handler);
	
	if(HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR1)!=0X5050)//是否第一次配置
	{
		__HAL_RCC_PWR_CLK_ENABLE();	//使能电源时钟PWR
		HAL_PWR_EnableBkUpAccess();	//取消备份区域写保护
		
		sTime.Hours		=	0x23;		//直接BCD格式写
		sTime.Minutes	= 	0x30;
		sTime.Seconds	= 	0x29;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		HAL_RTC_SetTime(&RTC_Handler, &sTime, RTC_FORMAT_BCD);
		
		sDate.Month = RTC_MONTH_OCTOBER;
		sDate.Date = 0x10;				//直接BCD格式写
		sDate.Year = 0x50;
		sDate.WeekDay = RTC_Get_Week(((u16)BCD2HEX(sDate.Year) + 1970),BCD2HEX(sDate.Month),BCD2HEX(sDate.Date));
		HAL_RTC_SetDate(&RTC_Handler, &sDate, RTC_FORMAT_BCD);
		
		/* Enable the Alarm A*/
//		sAlarm.AlarmTime.Hours	 = 	0x23;	//直接BCD格式写
//		sAlarm.AlarmTime.Minutes = 	0x31;
//		sAlarm.AlarmTime.Seconds = 	0x29;
//		sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//		sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
//		sAlarm.AlarmMask = RTC_ALARMMASK_SECONDS;					//可选mask时分秒和所有
//		sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;	//可选日期和星期
//		sAlarm.AlarmDateWeekDay = 0x1;	//不懂..
//		sAlarm.Alarm = RTC_ALARM_A;
//		HAL_RTC_SetAlarm_IT(&RTC_Handler, &sAlarm, RTC_FORMAT_BCD); //默认不开启闹钟
//		HAL_RTC_DeactivateAlarm(&RTC_Handler,RTC_ALARM_A);
		
		HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR1,0X5050);//标记已经初始化过了
		
		HAL_PWR_DisableBkUpAccess();	//备份区域写保护
		__HAL_RCC_PWR_CLK_DISABLE();	//失能电源时钟PWR
	}

}

/*RTC底层驱动，时钟配置
此函数会被HAL_RTC_Init()调用
hrtc:RTC句柄*/
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
	
	RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;//LSE配置
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  //RTC使用LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
    PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;//外设为RTC
    PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;//RTC时钟源为LSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    
    __HAL_RCC_RTC_ENABLE();//RTC时钟使能
	
	HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}
/*__________RTC读存_____________*/

//RTC闹钟A中断的回调函数
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	if(hrtc->Instance == RTC)
	{
		printf_uart(UART1,"RTC Alarm A IRQ!!!");
	}
	
}
#endif





#if SYSTEM_RNG_ENABLE
RNG_HandleTypeDef RNG_Handle;
void sys_RNG_ENABLE(void)
{
	RNG_Handle.Instance = RNG;
	HAL_RNG_Init(&RNG_Handle);
}

void HAL_RNG_MspInit(RNG_HandleTypeDef* rngHandle)
{
    /* RNG clock enable */
    __HAL_RCC_RNG_CLK_ENABLE();

    /* RNG interrupt Init */
    HAL_NVIC_SetPriority(RNG_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(RNG_IRQn);

}

void HASH_RNG_IRQHandler(void)
{
  HAL_RNG_IRQHandler(&RNG_Handle);
}
//调用一次 HAL_RNG_GenerateRandomNumber_IT(&RNG_Handle); 来触发产生随机数
u8 GenerateRNG_Ready = 0;	//RNG产生好标志位
u32 randomNum32bit = 0; 	//存32位RNG容器
void HAL_RNG_ReadyDataCallback(RNG_HandleTypeDef *hrng, uint32_t random32bit)
{
	GenerateRNG_Ready = 1;
	randomNum32bit = random32bit;
}
#endif
/*_____________系统变量和函数（莫要乱动撒）_______________*/
void sys_ENswd_DISjtag(void)
{
	/*PA13 A14为SWD必要线。PA15 B3 B4为额外用于JTAG的线*/
	GPIO_InitTypeDef GPIO_Initure;
	
	GPIO_Initure.Pin = GPIO_PIN_13;
	GPIO_Initure.Mode = GPIO_MODE_AF_PP;
	GPIO_Initure.Pull=GPIO_NOPULL;
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Initure.Alternate = GPIO_AF0_SWJ;
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	GPIO_Initure.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
	
	GPIO_Initure.Pin = GPIO_PIN_15;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Initure.Alternate = NULL;
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	GPIO_Initure.Pin = GPIO_PIN_3|GPIO_PIN_4;
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}
u16	StartUpTimes = 0;			/*用于保存开机次数，储存在最后一个或倒数第二个页*/
uint32_t UIDw[3]; 				/*保存STM32内部UID识别码，全球唯一识别码*/
uint32_t sysCoreClock; 			/*获取HCLK频率，外设时钟均来自此再分频*/

u8 is_buzzer_once = 0;
u8 is_buzzer_bibi = 0;


void delay_us(unsigned int time)
{    
   u8 i;  
   while(time--)
   {
	   for(i = 0;i < 168;i++)
	   {
			__NOP();
	   }
   }
}

unsigned char BCD2HEX(unsigned char bcd_data) //BCD转为HEX子程序 
{
    unsigned char temp; 
    temp=(bcd_data/16*10 + bcd_data%16); 
    return temp; 
} 
 
unsigned char HEX2BCD(unsigned char hex_data) //HEX转为BCD子程序 
{ 
    unsigned char temp; 
    temp=(hex_data/10*16 + hex_data%10); 
    return temp; 
}

/*字符串转整形，stm32不支持标准的atoi,这里自己实现*/
int myatoi(const char *str)
{
	int s=0;
	uint8_t falg=0;
	
	while(*str==' ')
	{
		str++;
	}

	if(*str=='-'||*str=='+')
	{
		if(*str=='-')
		falg=1;
		str++;
	}

	while(*str>='0'&&*str<='9')
	{
		s=s*10+*str-'0';
		str++;
		if(s<0)
		{
			s=2147483647;
			break;
		}
	}
	return s*(falg?-1:1);
}


/* 实现伪随机数的支持*/
unsigned int Curl_rand(void)
{
	static unsigned int gtimes = 0; 	//记录被调用了多少次
	static unsigned int randseed = 50; 	//随机数种子
	
	if(++gtimes > (randseed & 0x000f)) //当被调用(randseed & 0x000f)次后更新一下随机数种子
	{
		sys_GetsysRunTime(NULL,NULL,(u16*)&randseed);
		gtimes = 0;
	}
	
	/* 返回一个无符号32位整型的伪随机数. */
	randseed = randseed * 3971038;
	return (randseed << 7) | ((randseed >> 7) & 0xFFFF);
}

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
//void WFI_SET(void)
//{
//	__ASM volatile("wfi");		  
//}
////关闭所有中断
//void INTX_DISABLE(void)
//{		  
//	__ASM volatile("cpsid i");
//}
////开启所有中断
//void INTX_ENABLE(void)
//{
//	__ASM volatile("cpsie i");		  
//}
////设置栈顶地址
////addr:栈顶地址
//__asm void MSR_MSP(uint32_t addr) 
//{
//    MSR MSP, r0 			//set Main Stack value
//    BX r14
//}
 
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}




