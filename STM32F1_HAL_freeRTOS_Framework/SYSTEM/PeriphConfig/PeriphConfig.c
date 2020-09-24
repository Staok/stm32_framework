#include "PeriphConfig.h"

/*
______________________________【PIN MAP】__________________________________________
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
		* PA0												StandBy待机低功耗模式的唤醒按键WKUP（占用0线外部中断）
		* PA4		PA5										DAC_OUT1  DAC_OUT2（hd容量系类外设）
		* PC8/SDIO_D0	PC9/SDIO/D1		PC10/SDIO_D2		SDIO固定引脚，用于接SD卡
		  PC11/SDIO_D3	PC12/SDIO_CK	PD2/SDIO_CMD
		  * 												FSMC引脚（A[0:25],D[0:15],NEx,NOE,NWE,NBL0,NBL1）
		  D2	D3	NOE	NWE	NE1/NCE2	D13	D14 D15 	A16 	A17 	A18 	D0 		D1
		  PD0	PD1	PD4	PD5	PD7			PD8	PD9	PD10	PD11	PD12	PD13	PD14	PD15
		  
		  NBL0	NBL1	A23	A19	A20	A21	A22	D4	D5	D6	D7		D8		D9		D10		D11		D12
		  PE0	PE1		PE2	PE3	PE4	PE5	PE6	PE7	PE8	PE9	PE10	PE11	PE12	PE13	PE14	PE15
		  
		  A0	A1	A2	A3	A4	A5	A6		A7		A8		A9		A10	A11	A12	A13	A14	A15
		  PF0	PF1	PF2	PF3	PF4	PF5	PF12	PF13	PF14	PF15	PG0	PG1	PG2	PG3	PG4	PG5
		  
		  NE2/NCE3	NCE4_1/NE3	NCE4_2	NE4		A24		A25
		  PG9		PG10		PG11	PG12	PG13	PG14
		  
		* ...


用户：	*
*/

/*_______________________________外设优先级划分___________________________________________*/
/*
优先级分组为4，只有0~15的16级抢占优先级

  优先级
	0			1				2				3				4				5				...
   保留	   TIM4(时基)	 	按键外部中断	按键外部中断
											   TIM3
											   TIM2
*/

/*___________________________器件IO配置___________________________________________*/

/*STEP1:去.h文件里的"定义都有什么器件"里面写都有哪些器件*/

/*STEP2:定义一共有多少个器件*/
#define devicesNum	3

/*STEP3:定义每个器件所用到的IO和其配置（只是定义，但可以分别选择初始化或者不初始化）*/
/*参数说明：参数必须在以下列举中选
	PIN：	GPIO_PIN_0~GPIO_PIN_15
	MODE：	GPIO_MODE_INPUT、GPIO_MODE_ANALOG、GPIO_MODE_AF_INPUT							输入
			GPIO_MODE_OUTPUT_PP、GPIO_MODE_OUTPUT_OD、GPIO_MODE_AF_PP、GPIO_MODE_AF_OD		输出
			GPIO_MODE_IT_RISING、GPIO_MODE_IT_FALLING、GPIO_MODE_IT_RISING_FALLING  		选用IT表示启用EXTI，最多16线EXTI，如PA0和PB0共用EXTI0
	上下拉：	GPIO_NOPULL、GPIO_PULLUP、GPIO_PULLDOWN
	翻转速度：	GPIO_SPEED_FREQ_LOW、GPIO_SPEED_FREQ_MEDIUM、GPIO_SPEED_FREQ_HIGH
	GPIOx：		GPIOA~GPIOE
	默认状态:	GPIO_PIN_SET（1u）或GPIO_PIN_RESET（0u）
	EXTI优先级：0~15
	启否EXTI：	TRUE或者FALSE
*/
GPIO_Init_Struct TestLED_IO_Struct[] = 
{	/*	PIN				MODE			  上下拉		翻转速度		  	GPIOx 	  默认状态     EXTI优先级	启否EXTI*/
	{{GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOA,  GPIO_PIN_RESET,		15,		  FALSE}
};
//用于菜单的外部输入按键
GPIO_Init_Struct KEY_IO_Struct[] =
{	/*	PIN				MODE			  上下拉		翻转速度		  	GPIOx 	  默认状态     EXTI优先级	启否EXTI*/
	{{GPIO_PIN_1, GPIO_MODE_IT_FALLING, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH},	GPIOA,  GPIO_PIN_SET,		2,		  TRUE},
	{{GPIO_PIN_2, GPIO_MODE_IT_FALLING, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH},	GPIOA,  GPIO_PIN_SET,		2,		  TRUE}
};

GPIO_Init_Struct LCD_IO_Struct[] = 
{	/*	PIN				MODE			  上下拉		翻转速度		  	GPIOx 	  默认状态     EXTI优先级	启否EXTI*/
	{{GPIO_PIN_3, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
	{{GPIO_PIN_4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
	{{GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
	{{GPIO_PIN_6, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
	{{GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
	
	{{GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
	{{GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
	{{GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
	{{GPIO_PIN_11, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
	{{GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
	{{GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
	{{GPIO_PIN_14, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
	{{GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  FALSE},
};


/*STEP3.5:填写用于外部中断的IO的中断标志位*/
//当IO使用外部中断时，用一个标志位记录中断是否发生，把长时间的处理函数放到中断外面！
u8 key_Up_Interrupted 		= 	FALSE; 
u8 key_Down_Interrupted 	= 	FALSE;
//继续：去PeriphConfig.h里面修改用于外部中断的IO的标志位定义和读入函数宏定义
//继续：再去外部中断回调函数HAL_GPIO_EXTI_Callback()中填写IO的外部中断标志为置位

/*STEP4:按照器件的数量，按照"定义都有什么器件"里面的顺序！完善每个器件的信息*/
Devices_Init_Struct UserDevices[devicesNum] = 
{
	{	
		.deviceIO_Struct 	= 	TestLED_IO_Struct	,		//器件IO配置结构体
		.deviceIndex 		= 	TestLED_Index		,		//器件enum格式索引
		.deviceName 		= 	"TestLED"			,		//器件名称
		.device_IOnum 		= 	1							//器件有多少个IO口
	},
	{
		.deviceIO_Struct 	= 	KEY_IO_Struct	,
		.deviceIndex		= 	KEY_Index		,
		.deviceName 		= 	"Menu Key"		,
		.device_IOnum 		= 	2
	},
	{	
		.deviceIO_Struct 	= 	LCD_IO_Struct	,		//器件IO配置结构体
		.deviceIndex 		= 	LCD_Index		,		//器件enum格式索引
		.deviceName 		= 	"LCD"			,		//器件名称
		.device_IOnum 		= 	13						//器件有多少个IO口
	}
};


/*STEP5:去.h文件里定义IO控制的宏和改变入出模式的宏*/



/*初始化SPI1的SS器件选中引脚	默认PA4，用户可改*/
void sys_SPI1_SS_io_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull=GPIO_PULLUP;
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;
    
	/*主要改这三行*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_Initure.Pin=GPIO_PIN_4;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	SPI1_CS = IO_High;			                	//SPI1默认不选中
}
/*初始化SPI2的SS器件选中引脚	默认PB12，用户可改*/
void sys_SPI2_SS_io_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull=GPIO_PULLUP;
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;
    
	/*主要改这三行*/
	__HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_Initure.Pin=GPIO_PIN_12;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
	SPI2_CS = IO_High;			                	//SPI2默认不选中
}

/*___________________________器件IO配置函数___________________________________________*/
void Devices_Init(Devices_Init_Struct* Devices , enum devicesIndex_enum device2Init)
{
	static u8 dIndex;
	if(device2Init == ALL_Index)
	{
		for(dIndex = 0;dIndex < devicesNum;dIndex++)	//遍历所有器件
		{
			deviceIO_Init(Devices,(enum devicesIndex_enum)dIndex);
		}
	}else{
		deviceIO_Init(Devices,device2Init);				////特定器件初始化
	}
}

void deviceIO_Init(Devices_Init_Struct* Devices , enum devicesIndex_enum device2Init)
{
	u8 dIndex,iIndex;
	dIndex = (u8)device2Init;
	for(iIndex = 0;iIndex < Devices[dIndex].device_IOnum;iIndex++)	//遍历某一个器件的所有IO
	{
		//时钟
		if(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx == GPIOA)
			__HAL_RCC_GPIOA_CLK_ENABLE();
		else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx == GPIOB)
			__HAL_RCC_GPIOB_CLK_ENABLE();
		else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx == GPIOC)
			__HAL_RCC_GPIOC_CLK_ENABLE();
		else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx == GPIOD)
			__HAL_RCC_GPIOD_CLK_ENABLE();
		else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx == GPIOE)
			__HAL_RCC_GPIOE_CLK_ENABLE();
		
		//填入IO配置
		HAL_GPIO_Init(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx, \
					 &Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure);
		
		//如果使用EXTI，配置中断线和优先级
		if(   (Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Mode == GPIO_MODE_IT_RISING) \
			||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Mode == GPIO_MODE_IT_FALLING) \
			||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Mode == GPIO_MODE_IT_RISING_FALLING) )
		{		
			if(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_0)
			{	
				HAL_NVIC_SetPriority(EXTI0_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI0_IRQn); else HAL_NVIC_DisableIRQ(EXTI0_IRQn);
			}else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_1)
			{
				HAL_NVIC_SetPriority(EXTI1_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI1_IRQn); else HAL_NVIC_DisableIRQ(EXTI1_IRQn);
			}else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_2)
			{
				HAL_NVIC_SetPriority(EXTI2_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI2_IRQn); else HAL_NVIC_DisableIRQ(EXTI2_IRQn);
			}else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_3)
			{
				HAL_NVIC_SetPriority(EXTI3_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI3_IRQn); else HAL_NVIC_DisableIRQ(EXTI3_IRQn);
			}else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_4)
			{
				HAL_NVIC_SetPriority(EXTI4_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI4_IRQn); else HAL_NVIC_DisableIRQ(EXTI4_IRQn);
			}else if( (Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_5) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_6) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_7) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_8) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_9) )
			{
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn); else HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
			}else if( (Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_10) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_11) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_12) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_13) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_14) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_15) )
			{
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); else HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
			}
		}
		
		//配置默认IO状态
		HAL_GPIO_WritePin(  Devices[dIndex].deviceIO_Struct[iIndex].GPIOx, \
							Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin, \
							Devices[dIndex].deviceIO_Struct[iIndex].defaultState);
	}
}


/*中断服务函数*/
void EXTI0_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);}
void EXTI1_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);}
void EXTI2_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);}
void EXTI3_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);}
void EXTI4_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);}
void EXTI9_5_IRQHandler(void)		//需要自己判断的！
{ 
//	if(PAin(5))		//提前配置PA5为外部中断时
//		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}
void EXTI15_10_IRQHandler(void)		//需要自己判断的！
{ 
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}


/*外部中断回调函数*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
			/*如果使用RTOS则不初始化PA0的中断，使用裸机则使用PA0的外部中断*/
			#if SYSTEM_StdbyWKUP_ENABLE
				if(Check_WKUP())
				{
					Sys_Enter_Standby();//进入待机模式
				}
			#endif
            break;
		case GPIO_PIN_1:
			/*当配置GPIOA的引脚1为菜单按键的上按键时*/
			key_Up_Interrupted = TRUE;
			break;
        case GPIO_PIN_2:
			/*当配置GPIOA的引脚2为菜单按键的下按键时*/
			key_Down_Interrupted = TRUE;
            break;
        case GPIO_PIN_3:
            break;
        case GPIO_PIN_4:
            break;
    }
}

/*________________________________________用户定时器2配置_________________________________________________________*/
TIM_HandleTypeDef TIM2_Handler;
#if STSTEM_TIM2_ENABLE

TIM_HandleTypeDef TIM2_Handler;

#if (STSTEM_TIM2_asPWMorCap == 0)
	TIM_OC_InitTypeDef 	TIM2_CH1Handler,TIM2_CH2Handler,TIM2_CH3Handler,TIM2_CH4Handler;
#elif (STSTEM_TIM2_asPWMorCap == 1)
	TIM_IC_InitTypeDef TIM2_CHxConfig;
#elif (STSTEM_TIM2_asPWMorCap == 3)
	TIM_Encoder_InitTypeDef sEncoderConfig;
#endif

void sys_TIM2_ENABLE(void)
{
	TIM2_Handler.Instance = 		TIM2;                          	//通用定时器2
    TIM2_Handler.Init.Prescaler=	(72-1);                    		//分频系数
    TIM2_Handler.Init.CounterMode=	TIM_COUNTERMODE_UP;    			//向上计数器
    TIM2_Handler.Init.Period=		tim2arr;                       	//自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;			//时钟分频因子
    
	#if (STSTEM_TIM2_asPWMorCap == 2)
		HAL_TIM_Base_Init(&TIM2_Handler);
		
		#if (!STSTEM_TIM2_TI_ENABLE)
			HAL_TIM_Base_Start(&TIM2_Handler);	//不带IT的开启定时器
		#else
			HAL_TIM_Base_Start_IT(&TIM2_Handler);//带IT的开启定时器
		#endif
		
	#endif
	
	
	#if (STSTEM_TIM2_asPWMorCap == 0)
		/*作为PWM*/
		HAL_TIM_PWM_Init(&TIM2_Handler);       //初始化PWM
		if(STSTEM_TIM2PWM_CHANNEL_ENABLE & B0000_0001)
		{
			TIM2_CH1Handler.OCMode=TIM_OCMODE_PWM1; 		//模式选择PWM1
			TIM2_CH1Handler.Pulse=tim2arr/2;            	//设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
			TIM2_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; 	//输出比较极性为低 
			HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH1Handler,TIM_CHANNEL_1);//配置TIM2通道1
			HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_1);	//开启PWM通道1
		}
		if(STSTEM_TIM2PWM_CHANNEL_ENABLE & B0000_0010)
		{
			TIM2_CH2Handler.OCMode=TIM_OCMODE_PWM1;
			TIM2_CH2Handler.Pulse=tim2arr/2;
			TIM2_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW;
			HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH2Handler,TIM_CHANNEL_2);
			HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_2);
		}
		if(STSTEM_TIM2PWM_CHANNEL_ENABLE & B0000_0100)
		{
			TIM2_CH3Handler.OCMode=TIM_OCMODE_PWM1;
			TIM2_CH3Handler.Pulse=tim2arr/2;
			TIM2_CH3Handler.OCPolarity=TIM_OCPOLARITY_LOW;
			HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH3Handler,TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_3);
		}
		if(STSTEM_TIM2PWM_CHANNEL_ENABLE & B0000_1000)
		{
			TIM2_CH4Handler.OCMode=TIM_OCMODE_PWM1;
			TIM2_CH4Handler.Pulse=tim2arr/2;
			TIM2_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW;
			HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH4Handler,TIM_CHANNEL_4);
			HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_4);
		}
	#elif (STSTEM_TIM2_asPWMorCap == 1)
		/*作为Cap*/
		TIM2_Handler.Init.Period=0XFFFF;       //自动装载值
		HAL_TIM_IC_Init(&TIM2_Handler);			//初始化输入捕获时基参数
		
		if(STSTEM_TIM2_Cap_trigV == 1)
			TIM2_CHxConfig.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
		else if(STSTEM_TIM2_Cap_trigV == 0)
			TIM2_CHxConfig.ICPolarity=TIM_ICPOLARITY_FALLING;    //下降沿捕获
		else if(STSTEM_TIM2_Cap_trigV == 2)
			TIM2_CHxConfig.ICPolarity=TIM_ICPOLARITY_BOTHEDGE;    //双边沿捕获
		
		TIM2_CHxConfig.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
		TIM2_CHxConfig.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
		TIM2_CHxConfig.ICFilter=0;                          //配置输入滤波器，不滤波
		
		switch(STSTEM_TIM2_Cap_Channel)
		{
			case B0000_0001:				//选用通带1作为输入捕获
				HAL_TIM_IC_ConfigChannel(&TIM2_Handler,&TIM2_CHxConfig,TIM_CHANNEL_1);//配置TIM2通道1
				HAL_TIM_IC_Start_IT(&TIM2_Handler,TIM_CHANNEL_1);   //开启TIM2的捕获通道1，并且开启捕获中断
				break;
			case B0000_0010:				//选用通带2作为输入捕获
				HAL_TIM_IC_ConfigChannel(&TIM2_Handler,&TIM2_CHxConfig,TIM_CHANNEL_2);
				HAL_TIM_IC_Start_IT(&TIM2_Handler,TIM_CHANNEL_2);
				break;
			case B0000_0100:				//选用通带3作为输入捕获
				HAL_TIM_IC_ConfigChannel(&TIM2_Handler,&TIM2_CHxConfig,TIM_CHANNEL_3);
				HAL_TIM_IC_Start_IT(&TIM2_Handler,TIM_CHANNEL_3);
				break;
			case B0000_1000:				//选用通带4作为输入捕获
				HAL_TIM_IC_ConfigChannel(&TIM2_Handler,&TIM2_CHxConfig,TIM_CHANNEL_4);
				HAL_TIM_IC_Start_IT(&TIM2_Handler,TIM_CHANNEL_4);
				break;
			default:break;
		}
		__HAL_TIM_ENABLE_IT(&TIM2_Handler,TIM_IT_UPDATE);   //使能更新中断
	#elif (STSTEM_TIM2_asPWMorCap == 3)
		TIM2_Handler.Init.Prescaler=	0;
		TIM2_Handler.Init.Period=		0xFFFF;

	  sEncoderConfig.EncoderMode        = TIM_ENCODERMODE_TI12;
	  sEncoderConfig.IC1Polarity        = TIM_ICPOLARITY_RISING;
	  sEncoderConfig.IC1Selection       = TIM_ICSELECTION_DIRECTTI;
	  sEncoderConfig.IC1Prescaler       = TIM_ICPSC_DIV1;
	  sEncoderConfig.IC1Filter          = 0;

	  sEncoderConfig.IC2Polarity        = TIM_ICPOLARITY_RISING;
	  sEncoderConfig.IC2Selection       = TIM_ICSELECTION_DIRECTTI;
	  sEncoderConfig.IC2Prescaler       = TIM_ICPSC_DIV1;
	  sEncoderConfig.IC2Filter          = 0;
	  __HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
	  
	  HAL_TIM_Encoder_Init(&TIM2_Handler, &sEncoderConfig);
	  
	__HAL_TIM_CLEAR_IT(&TIM2_Handler, TIM_IT_UPDATE);  // 清除更新中断标志位
	__HAL_TIM_URS_ENABLE(&TIM2_Handler);               // 仅允许计数器溢出才产生更新中断
	__HAL_TIM_ENABLE_IT(&TIM2_Handler,TIM_IT_UPDATE);  // 使能更新中断

	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_TIM_Encoder_Start(&TIM2_Handler, TIM_CHANNEL_ALL);
	
	#endif
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Handler);
}

#if (STSTEM_TIM2_asPWMorCap == 3)				//如果使用正交解码
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==TIM2)
  {
    /* 基本定时器外设时钟使能 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

    __HAL_AFIO_REMAP_TIM2_ENABLE();
	  
	/*PA15 PB3*/
	  
    /* 定时器通道1功能引脚IO初始化 */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* 定时器通道2功能引脚IO初始化 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
}

/*在定时器溢出中断回调函数里面进行加减操作*/
int32_t EncoderOverflowCount = 0;//定时器溢出次数

/*获取一次编码器的当前计数值，用于表示编码器当前的绝对位置*/
int32_t peek_TIM2_Encoder_Value(void)
{
	return ( int32_t )__HAL_TIM_GET_COUNTER(&TIM2_Handler);
}

/*获取一次正交编码器的旋转速度 单位 转/秒，本函数放到TIM4的10ms定时中断中*/
float peek_TIM2_Encoder_Speed(void)
{
	float Speed = 0;
	int32_t CaptureNumber=0;     // 输入捕获数
	
	/* 速度定时器编码器接口捕获值 */
	CaptureNumber = ( int32_t )__HAL_TIM_GET_COUNTER(&TIM2_Handler) + EncoderOverflowCount*65536;
	
	Speed = (float)(CaptureNumber / PPR);
	Speed *= 100.0;	//本函数10ms计算一次，这里换算成1秒
	
	/* 数据清零,等待下一秒重新记录数据 */
	EncoderOverflowCount = 0;
	__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
	
	return Speed;
}

#endif

#if (STSTEM_TIM2_asPWMorCap == 1)				//如果使用Cap功能

//定时器2的IC功能底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_IC_Init()调用
//htim:定时器句柄
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
		__HAL_AFIO_REMAP_TIM2_ENABLE();			/*TIM2通道引脚完全重映射使能 (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11)*/
		
		if(STSTEM_TIM2_Cap_trigV == 1)				//上升沿捕获
			GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
		else if(STSTEM_TIM2_Cap_trigV == 0)			//下降沿捕获
			GPIO_Initure.Pull=GPIO_PULLUP;        	//上拉
		else if(STSTEM_TIM2_Cap_trigV == 2)			//双边沿捕获
			GPIO_Initure.Pull=GPIO_NOPULL;        	//无拉
		
		GPIO_Initure.Mode=GPIO_MODE_AF_INPUT; 	//复用输入
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		
		switch(STSTEM_TIM2_Cap_Channel)
		{
			case B0000_0001:					//选用通带1作为输入捕获
				__HAL_RCC_GPIOA_CLK_ENABLE();	//开启GPIOA时钟
				GPIO_Initure.Pin=GPIO_PIN_15;            //PA15
				HAL_GPIO_Init(GPIOA,&GPIO_Initure);
				break;
			case B0000_0010:					//选用通带2作为输入捕获
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_3;            //PB3
				HAL_GPIO_Init(GPIOB,&GPIO_Initure);
				break;
			case B0000_0100:					//选用通带3作为输入捕获
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_10;            //PB10
				HAL_GPIO_Init(GPIOB,&GPIO_Initure);
				break;
			case B0000_1000:					//选用通带4作为输入捕获
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_11;            //PB11
				HAL_GPIO_Init(GPIOB,&GPIO_Initure);
				break;
			default:break;
		}
		
		HAL_NVIC_SetPriority(TIM2_IRQn,2,0);    //设置中断优先级，抢占优先级3，子优先级0
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断通道  
	}
}

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数
u8  TIM2CHx_CAPTURE_STA=0;							//输入捕获状态		    				
u16	TIM2CHx_CAPTURE_VAL;							//输入捕获值(TIM5是16位)

/*获取最近一次TIM2输入捕获的脉冲时间，单位 毫秒，按照设置的捕获沿进行，可以随时随地调用此函数*/
float Peek_TIM2_Cap_Val(void)
{
	u8 Tim2overFlowTimes = 0;
	float temp = 0;
	if(TIM2CHx_CAPTURE_STA&0X80)        			//成功捕获到了一次高电平
	{
		Tim2overFlowTimes = TIM2CHx_CAPTURE_STA & 0X3F;
		temp = (float)Tim2overFlowTimes;
		temp *= 65536.0;		 	    			//溢出时间总和
		temp += (float)TIM2CHx_CAPTURE_VAL;      	//加上当前计数值得到总的高电平时间
		TIM2CHx_CAPTURE_STA = 0;          			//开启下一次捕获
		temp /= 1000.0;								//单位 毫秒
		return temp;
	}else{
		return 0.0;
	}
}

//定时器输入捕获中断处理回调函数，该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行
{
	if(htim==(&TIM2_Handler))
	{
			switch(STSTEM_TIM2_Cap_Channel)
			{
				case B0000_0001: //通道1
					Process_TIM2_IC_CallBack_Channel_1();
					break;
				case B0000_0010: //通带2
					Process_TIM2_IC_CallBack_Channel_2();
					break;
				case B0000_0100: //通道3
					Process_TIM2_IC_CallBack_Channel_3();
					break;
				case B0000_1000: //通带4
					Process_TIM2_IC_CallBack_Channel_4();
					break;
				default:break;
			}
	}	
}

void Process_TIM2_IC_CallBack_Channel_1(void)
{
	if((STSTEM_TIM2_Cap_trigV == 1)||(STSTEM_TIM2_Cap_trigV == 0))				//上升沿或者下降沿捕获
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)				//还未成功捕获
		{
			if(TIM2CHx_CAPTURE_STA&0X40)				//捕获到一个下降沿 		
			{	  			
				TIM2CHx_CAPTURE_STA|=0X80;				//标记成功捕获到一次高电平脉宽
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_1);//获取当前的捕获值.
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
				if(STSTEM_TIM2_Cap_trigV == 1)	//如果配置为上升沿捕获
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//配置TIM2通道1上升沿捕获
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//定时器2通道1设置为下降沿捕获
				}
				
			}else  										//还未开始,第一次捕获上升沿
			{
				TIM2CHx_CAPTURE_STA=0;					//清空
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;				//标记捕获到了上升沿
				__HAL_TIM_DISABLE(&TIM2_Handler);      	//关闭定时器2
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
				
				if(STSTEM_TIM2_Cap_trigV == 1)	//如果配置为上升沿捕获
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//定时器2通道1设置为下降沿捕获
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//定时器2通道1设置为上升沿捕获
				}
				
				__HAL_TIM_ENABLE(&TIM2_Handler);		//使能定时器2
			}		    
		}
	}else if(STSTEM_TIM2_Cap_trigV == 2)				//双沿捕获
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)				//还未成功捕获
		{
			if(TIM2CHx_CAPTURE_STA&0X40)				//已经捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA|=0X80;				//标记成功捕获到一次脉宽
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_1);//获取当前的捕获值
			}else										//还未捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA=0;					//清空
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;				//标记捕获到了一个沿
				__HAL_TIM_DISABLE(&TIM2_Handler);      	//关闭定时器2
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				__HAL_TIM_ENABLE(&TIM2_Handler);		//使能定时器2
			}
		}
	}
}

void Process_TIM2_IC_CallBack_Channel_2(void)
{
	if((STSTEM_TIM2_Cap_trigV == 1)||(STSTEM_TIM2_Cap_trigV == 0))
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)
		{
			if(TIM2CHx_CAPTURE_STA&0X40)	
			{	  			
				TIM2CHx_CAPTURE_STA|=0X80;	
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_2);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_2); 
				if(STSTEM_TIM2_Cap_trigV == 1)
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);
				}
				
			}else 
			{
				TIM2CHx_CAPTURE_STA=0;
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;
				__HAL_TIM_DISABLE(&TIM2_Handler);
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_2); 
				
				if(STSTEM_TIM2_Cap_trigV == 1)	
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);
				}
				
				__HAL_TIM_ENABLE(&TIM2_Handler);
			}		    
		}
	}else if(STSTEM_TIM2_Cap_trigV == 2)				//双沿捕获
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)				//还未成功捕获
		{
			if(TIM2CHx_CAPTURE_STA&0X40)				//已经捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA|=0X80;				//标记成功捕获到一次脉宽
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_2);//获取当前的捕获值
			}else										//还未捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA=0;					//清空
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;				//标记捕获到了一个沿
				__HAL_TIM_DISABLE(&TIM2_Handler);      	//关闭定时器2
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				__HAL_TIM_ENABLE(&TIM2_Handler);		//使能定时器2
			}
		}
	}
}

void Process_TIM2_IC_CallBack_Channel_3(void)
{
	if((STSTEM_TIM2_Cap_trigV == 1)||(STSTEM_TIM2_Cap_trigV == 0))
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)
		{
			if(TIM2CHx_CAPTURE_STA&0X40)	
			{	  			
				TIM2CHx_CAPTURE_STA|=0X80;	
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_3);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_3); 
				if(STSTEM_TIM2_Cap_trigV == 1)
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);
				}
				
			}else 
			{
				TIM2CHx_CAPTURE_STA=0;
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;
				__HAL_TIM_DISABLE(&TIM2_Handler);
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_3); 
				
				if(STSTEM_TIM2_Cap_trigV == 1)	
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);
				}
				
				__HAL_TIM_ENABLE(&TIM2_Handler);
			}		    
		}
	}else if(STSTEM_TIM2_Cap_trigV == 2)				//双沿捕获
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)				//还未成功捕获
		{
			if(TIM2CHx_CAPTURE_STA&0X40)				//已经捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA|=0X80;				//标记成功捕获到一次脉宽
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_3);//获取当前的捕获值
			}else										//还未捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA=0;					//清空
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;				//标记捕获到了一个沿
				__HAL_TIM_DISABLE(&TIM2_Handler);      	//关闭定时器2
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				__HAL_TIM_ENABLE(&TIM2_Handler);		//使能定时器2
			}
		}
	}
}
void Process_TIM2_IC_CallBack_Channel_4(void)
{
	if((STSTEM_TIM2_Cap_trigV == 1)||(STSTEM_TIM2_Cap_trigV == 0))
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)
		{
			if(TIM2CHx_CAPTURE_STA&0X40)	
			{	  			
				TIM2CHx_CAPTURE_STA|=0X80;	
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_4);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_4); 
				if(STSTEM_TIM2_Cap_trigV == 1)
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);
				}
				
			}else 
			{
				TIM2CHx_CAPTURE_STA=0;
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;
				__HAL_TIM_DISABLE(&TIM2_Handler);
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_4); 
				
				if(STSTEM_TIM2_Cap_trigV == 1)	
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);
				}
				
				__HAL_TIM_ENABLE(&TIM2_Handler);
			}		    
		}
	}else if(STSTEM_TIM2_Cap_trigV == 2)				//双沿捕获
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)				//还未成功捕获
		{
			if(TIM2CHx_CAPTURE_STA&0X40)				//已经捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA|=0X80;				//标记成功捕获到一次脉宽
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_4);//获取当前的捕获值
			}else										//还未捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA=0;					//清空
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;				//标记捕获到了一个沿
				__HAL_TIM_DISABLE(&TIM2_Handler);      	//关闭定时器2
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				__HAL_TIM_ENABLE(&TIM2_Handler);		//使能定时器2
			}
		}
	}
}


#endif
#if (STSTEM_TIM2_asPWMorCap == 0)				//如果使用PWM
	//设置TIM2通道的占空比
	//percent:占空比百分数
	void TIM2_set_Channel_Pulse(u8 channel,float percent)
	{
		float compare;
		if(percent < 0) percent = 0;
		if(percent > 100) percent = 100.0;
		percent /= 100.0;
		compare = (float)tim2arr * percent;
		switch(channel)
		{
			case TIM2PWM_Channel_1: TIM2->CCR1=(u32)compare;break;
			case TIM2PWM_Channel_2: TIM2->CCR2=(u32)compare;break;
			case TIM2PWM_Channel_3: TIM2->CCR3=(u32)compare;break;
			case TIM2PWM_Channel_4: TIM2->CCR4=(u32)compare;break;
			default:break;
		}
	}
#endif

#endif


/*________________________________________用户ADC1配置_________________________________________________________*/
#if SYSTEM_ADC1_ENABLE

ADC_HandleTypeDef ADC1_Handler;		//ADC句柄

void sys_ADC1_ENABLE(void)
{
	u8 i;
	i = 1;
	RCC_PeriphCLKInitTypeDef ADC_CLKInit;
	
	#if SYSTEM_ADC1_useScan
		/*先初始化DMA，再初始化ADC*/
		ADC_DMA_Cfg();
		//HAL_ADC_Start_DMA(&ADC1_Handler, (uint32_t*)&adValue,12);	//开始DMA，指定接收数组，最后一个参数为数据长度
	#endif
	
	ADC_CLKInit.PeriphClockSelection=RCC_PERIPHCLK_ADC;			//ADC外设时钟
	ADC_CLKInit.AdcClockSelection=RCC_ADCPCLK2_DIV6;			//分频因子6时钟为72M/6=12MHz
	HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);					//设置ADC时钟
	
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
	
	if(!SYSTEM_ADC1_useScan)
		ADC1_Handler.Init.ScanConvMode=DISABLE;                  //非扫描模式
	else ADC1_Handler.Init.ScanConvMode=ENABLE;					 //扫描模式
	
	if(SYSTEM_ADC1_useCircular)
		ADC1_Handler.Init.ContinuousConvMode=ENABLE;                //连续转换 开启：本次转换玩规则通道内全部通道后又自动启动下一次转换；不开启：触发一次转换一次；
    else ADC1_Handler.Init.ContinuousConvMode=DISABLE;
	
	if(SYSTEM_ADC1_useScan)
		ADC1_Handler.Init.NbrOfConversion=SYSTEM_ADC1_useChanlNum;   	//n个转换在规则序列中
	else ADC1_Handler.Init.NbrOfConversion=1;							//只转换规则序列1
	
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
	ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;  	 //软件触发
    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 
	
	HAL_ADCEx_Calibration_Start(&ADC1_Handler);					 //校准ADC
	
	if(SYSTEM_ADC1_useScan)		//如果启用扫描，则把所有通道都加入到规则组里
	{               
		if((SYSTEM_ADC1_useChanl) & B1in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_0, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B2in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_1, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B3in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_2, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B4in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_3, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B5in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_4, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B6in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_5, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B7in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_6, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B8in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_7, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B9in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_8, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B10in16)	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_9, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B11in16)	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_10, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B12in16)	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_11, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B13in16)	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_12, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B14in16)	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_13, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		if((SYSTEM_ADC1_useChanl) & B15in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_14, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		//if((SYSTEM_ADC1_useChanl) & 0x10) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_15, (uint32_t)(i++), ADC_SAMPLETIME_55CYCLES_5);
		
		if((SYSTEM_ADC1_useChanl) & B16in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_TEMPSENSOR, (uint32_t)(i++), ADC_SAMPLETIME_239CYCLES_5);
	}
	
}

void ADC_RegularChannelConfig(ADC_HandleTypeDef *AdcHandle, uint32_t Channel, uint32_t Rank, uint32_t SamplingTime)
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;      
    ADC1_ChanConf.Channel      = Channel;                      		//通道
    ADC1_ChanConf.Rank         = Rank;                          	//第几个序列
    ADC1_ChanConf.SamplingTime = SamplingTime;                  	//采样时间 
    HAL_ADC_ConfigChannel(AdcHandle,&ADC1_ChanConf);              	//通道配置
}

//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
/*	
	通道：	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	   16		     17
	IO	：	A0	A1	A2	A3	A4	A5	A6	A7	B0	B1	C0	C1	C2	C3	C4	C5	内部温度	内部参考电压
*/
	if(hadc->Instance == ADC1)
	{
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
		__HAL_RCC_GPIOB_CLK_ENABLE();
		
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
		GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
		               
		if((SYSTEM_ADC1_useChanl) & B1in16) {
			GPIO_Initure.Pin=GPIO_PIN_0; 
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			}
		if((SYSTEM_ADC1_useChanl) & B2in16) {
			GPIO_Initure.Pin=GPIO_PIN_1; 
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			}
		if((SYSTEM_ADC1_useChanl) & B3in16) {
			GPIO_Initure.Pin=GPIO_PIN_2; 
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			}
		if((SYSTEM_ADC1_useChanl) & B4in16) {
			GPIO_Initure.Pin=GPIO_PIN_3; 
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			}
		if((SYSTEM_ADC1_useChanl) & B5in16) {GPIO_Initure.Pin=GPIO_PIN_4; HAL_GPIO_Init(GPIOA,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B6in16) {GPIO_Initure.Pin=GPIO_PIN_5; HAL_GPIO_Init(GPIOA,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B7in16) {GPIO_Initure.Pin=GPIO_PIN_6; HAL_GPIO_Init(GPIOA,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B8in16) {GPIO_Initure.Pin=GPIO_PIN_7; HAL_GPIO_Init(GPIOA,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B9in16) {GPIO_Initure.Pin=GPIO_PIN_0; HAL_GPIO_Init(GPIOB,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B10in16) {GPIO_Initure.Pin=GPIO_PIN_1; HAL_GPIO_Init(GPIOB,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B11in16) {GPIO_Initure.Pin=GPIO_PIN_0; HAL_GPIO_Init(GPIOC,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B12in16) {GPIO_Initure.Pin=GPIO_PIN_1; HAL_GPIO_Init(GPIOC,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B13in16) {GPIO_Initure.Pin=GPIO_PIN_2; HAL_GPIO_Init(GPIOC,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B14in16) {GPIO_Initure.Pin=GPIO_PIN_3; HAL_GPIO_Init(GPIOC,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B15in16) {GPIO_Initure.Pin=GPIO_PIN_4; HAL_GPIO_Init(GPIOC,&GPIO_Initure);}
		//if((SYSTEM_ADC1_useChanl) & B166in16) {GPIO_Initure.Pin=GPIO_PIN_5; HAL_GPIO_Init(GPIOC,&GPIO_Initure);}  留作内部温度通道，第16个ADC1通道不可用
	}
}


u16 adValue[SYSTEM_ADC1_useChanlNum];		  /*DMA1把ADC转换结果传送的目标位置*/
u8 adValueDone = 0;			/*DMA把ADC1的值传送到adValue完成标志*/

//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_Adc(u32 ch)   
{
	ADC_ChannelConfTypeDef ADC1_ChanConf;
	if (!SYSTEM_ADC1_useScan)
	{	
		ADC1_ChanConf.Channel=ch;                                   //通道
		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;      //采样时间               
		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
		
		HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
		HAL_ADC_PollForConversion(&ADC1_Handler,2);                 //轮询转换
		return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        	//返回最近一次ADC1规则组的转换结果
	}else
	{
		return 1357;
	}
}

u32 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val = 0;
	u8 t;
	
	for(t=0;t < times;t++)
	{
		temp_val += Get_Adc(ch);
		delay_ms(1);
	}
	return (temp_val/times);
}

/*把AD采集温度通道的原始值，转化为温度值*/
float Get_Temprate(u32 adcx)
{
 	float temperate;
	temperate = ((float)adcx)*(3.3/4096.0);				//电压值
	temperate = (1.43-temperate)/0.0043 + 25.0;				//转换为温度值 	 
	return temperate;
}

#if SYSTEM_ADC1_useScan
DMA_HandleTypeDef  ADC1rxDMA_Handler; //DMA句柄

//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
void ADC_DMA_Cfg(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();

    ADC1rxDMA_Handler.Instance = DMA1_Channel1;                                 //DMA通道选择
    ADC1rxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;                     //外设到存储器
    ADC1rxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                        //外设非增量模式
    ADC1rxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                             //存储器增量模式
    ADC1rxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;        //外设数据长度:16位
    ADC1rxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;          //存储器数据长度:16位
	
	if(SYSTEM_ADC1_useCircular)
		ADC1rxDMA_Handler.Init.Mode=DMA_CIRCULAR;                                 	//如果ADC选择连续模式，这里是循环，如果不开启连续模式，这里是正常模式
	else ADC1rxDMA_Handler.Init.Mode=DMA_NORMAL;
   
	ADC1rxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;                       //优先级

    ADC1rxDMA_Handler.XferCpltCallback = HAL_DMA_IRQHandler;
    
	HAL_DMA_DeInit(&ADC1rxDMA_Handler);                                      //DMA复位
    HAL_DMA_Init(&ADC1rxDMA_Handler);                                        //DMA初始化 
        
		/*			数据源   数据源句柄中的DMA变量	  使用的DMA句柄*/
	__HAL_LINKDMA(&ADC1_Handler,DMA_Handle,ADC1rxDMA_Handler);               //将DMA与ADC联系起来(发送DMA)
        
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 3, 0);                          //DMA中断优先级 必须开，勿动！
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);                                  //使能DMA中断
        
}

//DMA通道1中断服务函数，完成传送一次时的中断，不用，因为要等AD全部规则通道转换完后再产生一次中断
void DMA1_Channel1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&ADC1rxDMA_Handler); /*清除中断标志，并判断是否完成全部转换，勿动！*/
}

//HAL_DMA_IRQHandler的回调函数，ADC转换完成所有规则组通道后的中断函数
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	adValueDone = 1; /*标志本次规则组通道全部转换完成，勿动！*/
}

#endif

#endif






/*___________________________________________用户SPI1、2配置_______________________________________________*/
#if ((SYSTEM_SPI1_ENABLE)||(SYSTEM_SPI2_ENABLE))

//SPI底层驱动，时钟使能，引脚配置
//此函数会被HAL_SPI_Init()调用
//hspi:SPI句柄
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    /*
	SPI1->CS	SPI1->CLK	SPI1->MISO	SPI1->MOSI 	—————— 	SPI2->CS	SPI2->CLK	SPI2->MISO	SPI2->MOSI
	PA4			PA5			PA6			PA7					PB12		PB13		PB14		PB15
	*/
	if(hspi->Instance == SPI1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();       //使能GPIOA时钟
		__HAL_RCC_SPI1_CLK_ENABLE();        //使能SPI1时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
		
		//PB13,14,15
		GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出，MISO配置为复用推挽没问题
		GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;        //快速            
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	}else if(hspi->Instance == SPI2)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();       //使能GPIOB时钟
		__HAL_RCC_SPI2_CLK_ENABLE();        //使能SPI2时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
		
		//PB13,14,15
		GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出，MISO配置为复用推挽没问题
		GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;        //快速            
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	}

}

#endif

#if SYSTEM_SPI1_ENABLE

SPI_HandleTypeDef SPI1_Handler;  //SPI1句柄

void sys_SPI1_ENABLE(void)
{
    SPI1_Handler.Instance=SPI1;                         //SPI1
    SPI1_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    
	SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    
	SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI1_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI1_Handler);//初始化
    
	sys_SPI1_SS_io_Init();
	
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //使能SPI1
	
    //SPI1_ReadWriteByte(0Xff);                           //启动传输
}

//SPI速度设置函数
//SPI1速度=APB2/分频系数
/*参数可选：
SPI_BAUDRATEPRESCALER_2 SPI_BAUDRATEPRESCALER_4 SPI_BAUDRATEPRESCALER_8 SPI_BAUDRATEPRESCALER_16 
SPI_BAUDRATEPRESCALER_32 SPI_BAUDRATEPRESCALER_64 SPI_BAUDRATEPRESCALER_128 SPI_BAUDRATEPRESCALER_256
*/
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&SPI1_Handler);            //关闭SPI
    SPI1_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    SPI1_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&SPI1_Handler);             //使能SPI
    
}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte_8Byte(u8 TxData)
{
    u8 Rxdata;
	SPI1_CS = IO_Low;
    HAL_SPI_TransmitReceive(&SPI1_Handler,&TxData,&Rxdata,1, 100);
	SPI1_CS = IO_High;	
 	return Rxdata;          		    //返回收到的数据		
}

//传入一个包含2字节的u8*指针
u8 SPI1_ReadWriteByte_16Byte(u8* TxData)
{
    u8 Rxdata;
	SPI1_CS = IO_Low;
    HAL_SPI_TransmitReceive(&SPI1_Handler,TxData,&Rxdata, 2, 100);
	SPI1_CS = IO_High;	
 	return Rxdata;          		    //返回收到的数据		
}

u8 SPI1_WriteByte_8Byte(u8 TxData)
{
	u8 state;
	SPI1_CS = IO_Low;
	state = (u8)(HAL_SPI_Transmit(&SPI1_Handler, &TxData, 1, 100));	
	SPI1_CS = IO_High;
	return state;
}

//传入一个包含2字节的u8*指针
u8 SPI1_WriteByte_16Byte(u8* TxData)
{
	u8 state;
	SPI1_CS = IO_Low;
	state = (u8)(HAL_SPI_Transmit(&SPI1_Handler, TxData, 2, 100));	
	SPI1_CS = IO_High;
	return state;
}

#endif

#if SYSTEM_SPI2_ENABLE

SPI_HandleTypeDef SPI2_Handler;  //SPI2句柄

void sys_SPI2_ENABLE(void)
{
    SPI2_Handler.Instance=SPI2;                         //SPI2
    SPI2_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI2_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI2_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    
	SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI2_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    
	SPI2_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI2_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
    SPI2_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI2_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI2_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI2_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI2_Handler);//初始化
    
	sys_SPI2_SS_io_Init();
	
    __HAL_SPI_ENABLE(&SPI2_Handler);                    //使能SPI2
	
    SPI2_ReadWriteByte(0Xff);                           //启动传输
}

//SPI速度设置函数
//SPI2速度=APB1/分频系数
/*参数可选：
SPI_BAUDRATEPRESCALER_2 SPI_BAUDRATEPRESCALER_4 SPI_BAUDRATEPRESCALER_8 SPI_BAUDRATEPRESCALER_16 
SPI_BAUDRATEPRESCALER_32 SPI_BAUDRATEPRESCALER_64 SPI_BAUDRATEPRESCALER_128 SPI_BAUDRATEPRESCALER_256
*/
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&SPI2_Handler);            //关闭SPI
    SPI2_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    SPI2_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&SPI2_Handler);             //使能SPI
    
}

//SPI2 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI2_Handler,&TxData,&Rxdata,1, 100); 
 	return Rxdata;          		    //返回收到的数据		
}
#endif


/*__________________________________低功耗StandbyMode__________________________________________*/
#if SYSTEM_StdbyWKUP_ENABLE

void sys_StdbyWKUP_ENABLE(void)
{
	#if !SYSTEM_SUPPORT_OS
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;  //中断,上升沿
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//快速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	/*PA0外部中断函数在 HAL_GPIO_EXTI_Callback 里*/
    
//    //检查是否是正常开机
//    if(Check_WKUP()==0)
//    {
//        Sys_Enter_Standby();//不是开机，进入待机模式
//    }

    HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	#else
	
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //输入检测
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//快速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	#endif
}


//系统进入待机模式
void Sys_Enter_Standby(void)
{
	while(PAin(0) == 1){;}
	delay_ms(500);
	while(PAin(0) == 1){;}
	delay_ms(500);
    __HAL_RCC_APB2_FORCE_RESET();       //复位所有IO口 
	__HAL_RCC_PWR_CLK_ENABLE();         //使能PWR时钟
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);                  //清除Wake_UP标志
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);           //设置WKUP用于唤醒
    HAL_PWR_EnterSTANDBYMode();                         //进入待机模式     
}

//检测WKUP脚的信号
//返回值1:连续按下3s以上
//      0:错误的触发	
u8 Check_WKUP(void) 
{
	u8 t=0;	//记录按下的时间
	while(1)
	{
		if(WKUP_KD)
		{
			t++;			//已经按下了 
			delay_ms(30);
			if(t>=100)		//按下超过3秒钟
			{
				return 1; 	//按下3s以上了
			}
		}else 
		{ 
			return 0; //按下不足3秒
		}
	}
}

void sys_CheckWKUP_4RTOS(void)
{
	static u8 t=0;	//记录按下的时间
	if(WKUP_KD)
	{
		t++;			//已经按下了 
		delay_ms(30);
		if(t>=100)		//按下超过3秒钟
		{
			Sys_Enter_Standby();//按下3s以上了，进入待机模式
		}
	}else 
	{} //按下不足3秒
}

#endif

/*________________________________________用户FLASH编程_________________________________________________________*/
#if SYSTEM_FLASH_IAP_ENABLE

#if STM32_FLASH_WREN	//如果使能了写   
//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
} 


#if STM32_FLASH_SIZE<256
	#define STM_SECTOR_SIZE 1024 //字节
#else 
	#define STM_SECTOR_SIZE	2048
#endif

#define FLASH_WAITETIME  	50000          	//FLASH等待超时时间
#define STM32_FLASH_BASE 0x08000000 		//STM32 FLASH的起始地址

u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节

//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //扇区地址
	u16 secoff;	   //扇区内偏移地址(16位字计算)
	u16 secremain; //扇区内剩余地址(16位字计算)	   
 	u16 i;    
	u32 offaddr;   //去掉0X08000000后的地址
	
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
	
	HAL_FLASH_Unlock();					//解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)	//校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)				//需要擦除
		{
			FLASH_PageErase(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);	//擦除这个扇区
			FLASH_WaitForLastOperation(FLASH_WAITETIME);            	//等待上次操作完成
			CLEAR_BIT(FLASH->CR, FLASH_CR_PER);							//清除CR寄存器的PER位，此操作应该在FLASH_PageErase()中完成！
																		//但是HAL库里面并没有做，应该是HAL库bug！
			for(i=0;i<secremain;i++)//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
		}else 
		{
			FLASH_WaitForLastOperation(FLASH_WAITETIME);       	//等待上次操作完成
			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 
		}
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		   	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=secremain*2;	//写地址偏移(16位数据地址,需要*2)	   
		   	NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	HAL_FLASH_Lock();		//上锁
}
#endif

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

//读取指定地址的半字(16位数据) 
//faddr:读地址 
//返回值:对应数据.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

#endif


/*__________________________DAC________________________________________*/
#if ((SYSTEM_DAC_OUT1_ENABLE) || (SYSTEM_DAC_OUT2_ENABLE)) && ((STM32F103xG) || (STM32F103xE))

DAC_HandleTypeDef DAC1_Handler;//DAC句柄

void sys_DAC_ENABLE(void)
{
	DAC_ChannelConfTypeDef DACCH1_Config;
	DAC_ChannelConfTypeDef DACCH2_Config;
	
	DAC1_Handler.Instance=DAC;
	HAL_DAC_Init(&DAC1_Handler);                 			//初始化DAC
    
		/*通道1配置*/
		DACCH1_Config.DAC_Trigger=DAC_TRIGGER_NONE;             		//不使用触发功能
							/*可选：
								DAC_TRIGGER_T6_TRGO
								DAC_TRIGGER_T7_TRGO
								DAC_TRIGGER_T8_TRGO
								DAC_TRIGGER_T2_TRGO
								DAC_TRIGGER_T4_TRGO
								DAC_TRIGGER_EXT_IT9
								DAC_TRIGGER_SOFTWARE*/
		DACCH1_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;		//DAC1输出缓冲关闭
		/*通道2配置*/
		DACCH2_Config.DAC_Trigger=DAC_TRIGGER_NONE;             		//不使用触发功能
		DACCH2_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;		//DAC1输出缓冲关闭
		
		if(SYSTEM_DAC_OUT1_ENABLE)
			HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH1_Config,DAC_CHANNEL_1);//DAC通道1配置
			HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_1);  			//开启DAC通道1
		if(SYSTEM_DAC_OUT2_ENABLE)
			HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH2_Config,DAC_CHANNEL_2);//DAC通道2配置
			HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_2);  			//开启DAC通道2
    
    
}

//DAC底层驱动，时钟配置，引脚 配置
//此函数会被HAL_DAC_Init()调用
//hdac:DAC句柄
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{      
	if(SYSTEM_DAC_OUT1_ENABLE)
	{
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_DAC_CLK_ENABLE();             //使能DAC时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
		
		GPIO_Initure.Pin=GPIO_PIN_4;            //PA4
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
		GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	}
	
	if(SYSTEM_DAC_OUT2_ENABLE)
	{
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_DAC_CLK_ENABLE();             //使能DAC时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
		
		GPIO_Initure.Pin=GPIO_PIN_5;            //PA5
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
		GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	}
}

//设置通道1输出电压
//vol:0~3.30V
void DAC_Set_Ch1_Vol(float vol)
{
	float temp = vol;
	temp = (temp/3.30)*4096.0;
    HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_1,DAC_ALIGN_12B_R,(uint32_t)temp);//12位右对齐数据格式设置DAC值
}

//设置通道2输出电压
//vol:0~3.30V
void DAC_Set_Ch2_Vol(float vol)
{
	float temp = vol;
	temp = (temp/3.30)*4096.0;
    HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_2,DAC_ALIGN_12B_R,(uint32_t)temp);//12位右对齐数据格式设置DAC值
}


#endif

/*____________________SDIO SD_____________________________________________*/
#if (SYSTEM_SDIO_SD_ENABLE) && ((STM32F103xG) || (STM32F103xE))

SD_HandleTypeDef        SDCARD_Handler;     		//SD卡句柄
HAL_SD_CardInfoTypeDef  SDCardInfo;                 //SD卡信息
HAL_SD_CardCIDTypeDef	SDCard_CID;					//SD卡CID信息


//SD卡初始化
//返回值:0 初始化正确；其他值，初始化错误
u8 SD_Init(void)
{
    u8 SD_Error;
    
    //初始化时的时钟不能大于400KHZ 
    SDCARD_Handler.Instance=SDIO;
    SDCARD_Handler.Init.ClockEdge=SDIO_CLOCK_EDGE_RISING;          		//上升沿     
    SDCARD_Handler.Init.ClockBypass=SDIO_CLOCK_BYPASS_DISABLE;     		//不使用bypass模式，直接用HCLK进行分频得到SDIO_CK
    SDCARD_Handler.Init.ClockPowerSave=SDIO_CLOCK_POWER_SAVE_DISABLE;   //空闲时不关闭时钟电源
    SDCARD_Handler.Init.BusWide=SDIO_BUS_WIDE_1B;                       //1位数据线
    SDCARD_Handler.Init.HardwareFlowControl=SDIO_HARDWARE_FLOW_CONTROL_ENABLE;//开启硬件流控
    SDCARD_Handler.Init.ClockDiv=SDIO_TRANSFER_CLK_DIV;            		//SD传输时钟频率最大25MHZ
	
    SD_Error=HAL_SD_Init(&SDCARD_Handler);
    if(SD_Error!=HAL_OK) return 1;
	
    SD_Error=HAL_SD_ConfigWideBusOperation(&SDCARD_Handler,SDIO_BUS_WIDE_4B);//使能宽总线模式
    if(SD_Error!=HAL_OK) return 2;
    return 0;
}


//SDMMC底层驱动，时钟使能，引脚配置，DMA配置
//此函数会被HAL_SD_Init()调用
//hsd:SD卡句柄
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    DMA_HandleTypeDef TxDMAHandler,RxDMAHandler;
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_SDIO_CLK_ENABLE();    //使能SDIO时钟
    //__HAL_RCC_DMA2_CLK_ENABLE();    //使能DMA2时钟 
    __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();   //使能GPIOD时钟
    
    //PC8,9,10,11,12
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //推挽复用
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     //初始化
    
    //PD2
    GPIO_Initure.Pin=GPIO_PIN_2;            
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化
}

/*
//得到卡信息
//cardinfo:卡信息存储区
	HAL_SD_GetCardInfo(&SDCARD_Handler,&SDCardInfo);
*/

//读SD卡
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;
u8 SD_ReadDisk(u8* buf,u32 sector,u32 cnt)
{
    u8 sta = HAL_OK;
	u32 timeout=SD_TIMEOUT;
	long long lsector=sector;
    INTX_DISABLE();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
	sta=HAL_SD_ReadBlocks(&SDCARD_Handler, (uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//多个sector的读操作
	
	//等待SD卡读完
	while(HAL_SD_GetCardState(&SDCARD_Handler) != HAL_SD_CARD_READY)
    {
		if(timeout-- == 0)
		{	
			sta = HAL_BUSY;
		}
    }
    INTX_ENABLE();//开启总中断
    return sta;
}

//写SD卡
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;	
u8 SD_WriteDisk(u8 *buf,u32 sector,u32 cnt)
{   
    u8 sta = HAL_OK;
	u32 timeout=SD_TIMEOUT;
	long long lsector=sector;
    INTX_DISABLE();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
	sta=HAL_SD_WriteBlocks(&SDCARD_Handler,(uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//多个sector的写操作

	//等待SD卡读完
	while(HAL_SD_GetCardState(&SDCARD_Handler) != HAL_SD_CARD_READY)
    {
		if(timeout-- == 0)
		{	
			sta = HAL_BUSY;
		}
    }  
	INTX_ENABLE();//开启总中断
    return sta;
}


//通过串口打印SD卡相关信息
void show_sdcard_info(void)
{
	uint64_t CardCap;	//SD卡容量

	HAL_SD_GetCardCID(&SDCARD_Handler,&SDCard_CID);		//获取CID
	HAL_SD_GetCardInfo(&SDCARD_Handler,&SDCardInfo); 	//获取SD卡信息                  
	switch(SDCardInfo.CardType)
	{
		case CARD_SDSC:
		{
			if(SDCardInfo.CardVersion == CARD_V1_X)
				printf_uart(UART1,"Card Type:SDSC V1\r\n");
			else if(SDCardInfo.CardVersion == CARD_V2_X)
				printf_uart(UART1,"Card Type:SDSC V2\r\n");
		}
		break;
		case CARD_SDHC_SDXC:
			printf_uart(UART1,"Card Type:SDHC\r\n");
		break;
	}	
	CardCap=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);				//计算SD卡容量
  	printf_uart(UART1,"Card ManufacturerID:%d\r\n",SDCard_CID.ManufacturerID);					//制造商ID
 	printf_uart(UART1,"Card RCA:%d\r\n",SDCardInfo.RelCardAdd);									//卡相对地址
	printf_uart(UART1,"LogBlockNbr:%d \r\n",(u32)(SDCardInfo.LogBlockNbr));						//显示逻辑块数量
	printf_uart(UART1,"LogBlockSize:%d \r\n",(u32)(SDCardInfo.LogBlockSize));					//显示逻辑块大小
	printf_uart(UART1,"Card Capacity:%d MB\r\n",(u32)(CardCap>>20));							//显示容量（右移20位，以MB显示）
 	printf_uart(UART1,"Card BlockSize:%d\r\n\r\n",SDCardInfo.BlockSize);						//显示块大小
}

#endif

#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4SRAM)) && ((STM32F103xG) || (STM32F103xE))

SRAM_HandleTypeDef SRAM_Handler;    //SRAM句柄

//SRAM初始化
void sys_FSMC_SRAM_ENABLE(void)
{	
	GPIO_InitTypeDef GPIO_Initure;
	FSMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
    
    __HAL_RCC_FSMC_CLK_ENABLE();                //使能FSMC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();               //使能GPIOD时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();               //使能GPIOE时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();               //使能GPIOF时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();               //使能GPIOG时钟
    
	//PD0,1,4,5,8~15
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|\
					 GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|\
					 GPIO_PIN_14|GPIO_PIN_15;  
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速   
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);     	
    
	//PE0,1,7~15
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|\
					 GPIO_PIN_10| GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|\
					 GPIO_PIN_15;    
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);    
    
	//PF0~5,12~15
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|\
					 GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;              
	HAL_GPIO_Init(GPIOF,&GPIO_Initure);     
    
	//PG0~5,10
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_10;              
	HAL_GPIO_Init(GPIOG,&GPIO_Initure);      
	
	SRAM_Handler.Instance=FSMC_NORSRAM_DEVICE;                
	SRAM_Handler.Extended=FSMC_NORSRAM_EXTENDED_DEVICE;    
    
	SRAM_Handler.Init.NSBank=FSMC_NORSRAM_BANK3;     					//使用NE3
	SRAM_Handler.Init.DataAddressMux=FSMC_DATA_ADDRESS_MUX_DISABLE; 	//地址/数据线不复用
	SRAM_Handler.Init.MemoryType=FSMC_MEMORY_TYPE_SRAM;   				//SRAM
	SRAM_Handler.Init.MemoryDataWidth=FSMC_NORSRAM_MEM_BUS_WIDTH_16; 	//16位数据宽度
	SRAM_Handler.Init.BurstAccessMode=FSMC_BURST_ACCESS_MODE_DISABLE; 	//是否使能突发访问,仅对同步突发存储器有效,此处未用到
	SRAM_Handler.Init.WaitSignalPolarity=FSMC_WAIT_SIGNAL_POLARITY_LOW;	//等待信号的极性,仅在突发模式访问下有用
	SRAM_Handler.Init.WaitSignalActive=FSMC_WAIT_TIMING_BEFORE_WS;   	//存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
	SRAM_Handler.Init.WriteOperation=FSMC_WRITE_OPERATION_ENABLE;    	//存储器写使能
	SRAM_Handler.Init.WaitSignal=FSMC_WAIT_SIGNAL_DISABLE;           	//等待使能位,此处未用到
	SRAM_Handler.Init.ExtendedMode=FSMC_EXTENDED_MODE_DISABLE;        	//读写使用相同的时序
	SRAM_Handler.Init.AsynchronousWait=FSMC_ASYNCHRONOUS_WAIT_DISABLE;	//是否使能同步传输模式下的等待信号,此处未用到
	SRAM_Handler.Init.WriteBurst=FSMC_WRITE_BURST_DISABLE;           	//禁止突发写
  
	//FMC读时序控制寄存器
	FSMC_ReadWriteTim.AddressSetupTime=0x00;       	//地址建立时间（ADDSET）为1个HCLK 1/72M=13.8ns
	FSMC_ReadWriteTim.AddressHoldTime=0x00;			//地址保持时间（ADDHLD）模式A未用到
	FSMC_ReadWriteTim.DataSetupTime=0x01;			//数据保存时间为3个HCLK	=4*13.8=55ns
	FSMC_ReadWriteTim.BusTurnAroundDuration=0X00;
	FSMC_ReadWriteTim.AccessMode=FSMC_ACCESS_MODE_A;//模式A
	HAL_SRAM_Init(&SRAM_Handler,&FSMC_ReadWriteTim,&FSMC_ReadWriteTim);	
}

//在指定地址(WriteAddr+Bank1_SRAM3_ADDR)开始,连续写入n个字节.
//pBuffer:字节指针
//WriteAddr:要写入的地址
//n:要写入的字节数
void FSMC_SRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*(vu8*)(SRAM1_BANK3_ADDR + WriteAddr) = *pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}

//在指定地址((WriteAddr+Bank1_SRAM3_ADDR))开始,连续读出n个字节.
//pBuffer:字节指针
//ReadAddr:要读出的起始地址
//n:要写入的字节数
void FSMC_SRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*pBuffer++ = *(vu8*)(SRAM1_BANK3_ADDR + ReadAddr);
		ReadAddr++;
	}
}

#endif


#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4LCD)) && ((STM32F103xG) || (STM32F103xE))

SRAM_HandleTypeDef TFTSRAM_Handler;    //SRAM句柄(用于控制LCD)


//SRAM底层驱动，时钟使能，引脚分配
//此函数会被HAL_SRAM_Init()调用
//hsram:SRAM句柄
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{	
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_FSMC_CLK_ENABLE();			//使能FSMC时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();			//使能GPIOD时钟
	__HAL_RCC_GPIOE_CLK_ENABLE();			//使能GPIOE时钟
	__HAL_RCC_GPIOG_CLK_ENABLE();			//使能GPIOG时钟
	
	//初始化PD0,1,4,5,8,9,10,14,15
	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|\
					 GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_Initure.Mode=GPIO_MODE_AF_PP; 		//推挽复用
	GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化
	
	//初始化PE7,8,9,10,11,12,13,14,15
	GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
	//初始化PG0,12
	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_12;
	HAL_GPIO_Init(GPIOG,&GPIO_Initure);
}

void LCD_with_FSMC_init_FSMC(void)
{
	FSMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
	FSMC_NORSRAM_TimingTypeDef FSMC_WriteTim;
	
	TFTSRAM_Handler.Instance=FSMC_NORSRAM_DEVICE;                
	TFTSRAM_Handler.Extended=FSMC_NORSRAM_EXTENDED_DEVICE;    
    
	TFTSRAM_Handler.Init.NSBank=FSMC_NORSRAM_BANK4;     				//使用NE4
	TFTSRAM_Handler.Init.DataAddressMux=FSMC_DATA_ADDRESS_MUX_DISABLE; 	//地址/数据线不复用
	TFTSRAM_Handler.Init.MemoryType=FSMC_MEMORY_TYPE_SRAM;   			//SRAM
	TFTSRAM_Handler.Init.MemoryDataWidth=FSMC_NORSRAM_MEM_BUS_WIDTH_16; //16位数据宽度
	TFTSRAM_Handler.Init.BurstAccessMode=FSMC_BURST_ACCESS_MODE_DISABLE; //是否使能突发访问,仅对同步突发存储器有效,此处未用到
	TFTSRAM_Handler.Init.WaitSignalPolarity=FSMC_WAIT_SIGNAL_POLARITY_LOW;//等待信号的极性,仅在突发模式访问下有用
	TFTSRAM_Handler.Init.WaitSignalActive=FSMC_WAIT_TIMING_BEFORE_WS;   //存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
	TFTSRAM_Handler.Init.WriteOperation=FSMC_WRITE_OPERATION_ENABLE;    //存储器写使能
	TFTSRAM_Handler.Init.WaitSignal=FSMC_WAIT_SIGNAL_DISABLE;           //等待使能位,此处未用到
	TFTSRAM_Handler.Init.ExtendedMode=FSMC_EXTENDED_MODE_ENABLE;        //读写使用不同的时序
	TFTSRAM_Handler.Init.AsynchronousWait=FSMC_ASYNCHRONOUS_WAIT_DISABLE;//是否使能同步传输模式下的等待信号,此处未用到
	TFTSRAM_Handler.Init.WriteBurst=FSMC_WRITE_BURST_DISABLE;           //禁止突发写
    
	//FMC读时序控制寄存器
	FSMC_ReadWriteTim.AddressSetupTime=0x06;       	//地址建立时间（ADDSET）为7个HCLK 13.8ns*7=96.6ns
	FSMC_ReadWriteTim.AddressHoldTime=0;
	FSMC_ReadWriteTim.DataSetupTime=26;				//数据保存时间为27个HCLK	=13.8*27=372.6ns
	FSMC_ReadWriteTim.AccessMode=FSMC_ACCESS_MODE_A;//模式A
	//FMC写时序控制寄存器
	FSMC_WriteTim.BusTurnAroundDuration=0;			//总线周转阶段持续时间为0，此变量不赋值的话会莫名其妙的自动修改为4。导致程序运行正常
	FSMC_WriteTim.AddressSetupTime=3;          		//地址建立时间（ADDSET）为4个HCLK =55.2ns 
	FSMC_WriteTim.AddressHoldTime=0;
	FSMC_WriteTim.DataSetupTime=0x06;              	//数据保存时间为13.8ns*7个HCLK=96.6ns
	FSMC_WriteTim.AccessMode=FSMC_ACCESS_MODE_A;    //模式A
	HAL_SRAM_Init(&TFTSRAM_Handler,&FSMC_ReadWriteTim,&FSMC_WriteTim);	
	
	delay_ms(50); // delay 50 ms 
}

#endif

