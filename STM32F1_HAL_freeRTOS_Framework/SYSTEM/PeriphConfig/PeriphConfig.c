#include "PeriphConfig.h"

/*
______________________________【PIN MAP】__________________________________________
注：外设如串口、PWM等的IO初始化在其初始化函数内，不用单独再初始化
		[IO]											[描述]
外设：	* PA8										MCO输出，默认时钟源为HSE
		* CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1		TIM3默认PWM口
		  CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1		TIM3部分重映射PWM口
		  CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9		TIM3完全重映射PWM口
		* TX/PA9, RX/PA10 - TX/PB6, RX/PB7			USART1默认引脚和重映射引脚
		* TX/PA2, RX/PA3 - TX/PD5, RX/PD6			USART2默认引脚和重映射引脚
		* TX/PB10, RX/PB11 - TX/PD8,  RX/PD9		USART3默认引脚和重映射引脚
		* 											SPI1默认引脚和重映射引脚
		*...

用户：	*
*/

/*___________________________器件IO配置___________________________________________*/

/*STEP1:去.h文件里的"定义都有什么器件"里面写都有哪些器件*/

/*STEP2:定义一共有多少个器件*/
#define devicesNum	2

/*STEP3:定义每个器件所用到的IO和其配置*/
/*参数说明：
	PIN：	GPIO_PIN_0~GPIO_PIN_15，GPIO_PIN_All
	MODE：	GPIO_MODE_INPUT、GPIO_MODE_ANALOG、GPIO_MODE_AF_INPUT							输入
			GPIO_MODE_OUTPUT_PP、GPIO_MODE_OUTPUT_OD、GPIO_MODE_AF_PP、GPIO_MODE_AF_OD		输出
			GPIO_MODE_IT_RISING、GPIO_MODE_IT_FALLING、GPIO_MODE_IT_RISING_FALLING  		选用IT表示启用EXTI，最多16线EXTI，如PA0和PB0共用EXTI0
	上下拉：GPIO_NOPULL、GPIO_PULLUP、GPIO_PULLDOWN
	翻转速度：GPIO_SPEED_FREQ_LOW、GPIO_SPEED_FREQ_MEDIUM、GPIO_SPEED_FREQ_HIGH
	GPIOx：GPIOA~GPIOE
	默认状态:GPIO_PIN_SET（1u）或GPIO_PIN_RESET（0u）
	EXTI优先级：0~15
	启否EXTI：TRUE或者FALSE
*/
GPIO_Init_Struct LCD_IO_Struct[] = 
{	/*	PIN				MODE			  上下拉		翻转速度		  	GPIOx 	  默认状态     EXTI优先级	启否EXTI*/
	{{GPIO_PIN_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH},	GPIOA,  GPIO_PIN_SET,		2,		  TRUE},
	{{GPIO_PIN_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  TRUE}
};

GPIO_Init_Struct BUCK_IO_Struct[] =
{
	{{GPIO_PIN_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH},	GPIOA,  GPIO_PIN_SET,		2,		  TRUE},
	{{GPIO_PIN_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		2,		  TRUE}
};

/*STEP4:按照器件的数量，按照"定义都有什么器件"里面的顺序！完善每个器件的信息*/
Devices_Init_Struct UserDevices[devicesNum] = 
{
	{	
		.deviceIO_Struct 	= 	LCD_IO_Struct	,		//器件IO配置结构体
		.deviceIndex 		= 	LCD				,		//器件enum格式索引
		.deviceName 		= 	"LCD device"	,		//器件名称
		.device_IOnum 		= 	8						//器件有多少个IO口
	},
	{
		.deviceIO_Struct 	= 	BUCK_IO_Struct	,
		.deviceIndex		= 	BUCK			,
		.deviceName 		= 	"BUCK device"	,
		.device_IOnum 		= 	2
	}
};


/*___________________________器件IO配置函数___________________________________________*/
void Devices_Init(Devices_Init_Struct* Devices , enum devicesIndex_enum device2Init)
{
	u8 dIndex;
	if(device2Init == ALL)
	{
		for(dIndex = 0;dIndex < devicesNum;dIndex++)	//遍历所有器件
		{
			deviceIO_Init(Devices,(enum devicesIndex_enum)dIndex);
		}
	}else{
		switch(device2Init)								//特定器件初始化，按需增添		
		{
			case LCD:	deviceIO_Init(Devices,LCD);
				break;
			case BUCK:	deviceIO_Init(Devices,BUCK);
			default:break;
		}
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
		if( (Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Mode == GPIO_MODE_IT_RISING) \
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
			}else if((Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_5) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_6) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_7) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_8) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_9) )
			{
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn); else HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
			}else if((Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_10) \
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
		HAL_GPIO_WritePin(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx, \
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
            break;
		case GPIO_PIN_1:
			break;
        case GPIO_PIN_2:
            break;
        case GPIO_PIN_3:
            break;
        case GPIO_PIN_4:
            break;
    }
}


