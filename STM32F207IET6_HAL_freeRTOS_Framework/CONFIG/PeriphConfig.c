#include "PeriphConfig.h"



/*_____________________________________\\\                               ///____________________________________________*
 *_________________________________________________用户GPIO配置_________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/
/*STEP 1:去.h文件里定义都有什么器件*/

/*STEP 2:定义一共有多少个器件*/
#define devicesNum	3

/*STEP 3:定义每个器件所用到的IO和其配置（只是定义，但可以分别选择初始化或者不初始化）*/
/*参数说明：参数必须在以下列举中选
	PIN：	GPIO_PIN_0~GPIO_PIN_15
	MODE：	输入		GPIO_MODE_INPUT、GPIO_MODE_ANALOG							
			输出		GPIO_MODE_OUTPUT_PP、GPIO_MODE_OUTPUT_OD、GPIO_MODE_AF_PP、GPIO_MODE_AF_OD		（AF不要用，外设IO交给外设初始化函数解决）
			外部中断	GPIO_MODE_IT_RISING、GPIO_MODE_IT_FALLING、GPIO_MODE_IT_RISING_FALLING  		选用IT表示启用EXTI，最多16线EXTI，如PA0和PB0共用EXTI0
	上下拉：	GPIO_NOPULL、GPIO_PULLUP、GPIO_PULLDOWN
	翻转速度：	GPIO_SPEED_FREQ_LOW、GPIO_SPEED_FREQ_MEDIUM、GPIO_SPEED_FREQ_HIGH、GPIO_SPEED_FREQ_VERY_HIGH
	GPIOx：		GPIOA~GPIOE、GPIOF、GPIOG、GPIOH、GPIOI
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
	{{GPIO_PIN_3, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_6, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	
	{{GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_11, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_14, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
};

/*STEP 3.5:在这里和去.h文件里填写用于外部中断的IO的中断标志位*/
//当IO使用外部中断时，用一个标志位记录中断是否发生，把长时间的处理函数放到中断外面！
u8 key_Up_Interrupted 		= 	FALSE; 
u8 key_Down_Interrupted 	= 	FALSE;


/*STEP 3.75:去.h文件里定义IO控制的宏和改变入出模式的宏*/


/*STEP4:按照器件的数量，按照"STEP 1:定义都有什么器件"里面的顺序！完善每个器件的信息*/
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


/*_____________________________________\\\                               ///____________________________________________*
 *_________________________________________________以下不用动___________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/
/*器件IO配置函数*/
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

/*_________________________________________以下外设的声明代码均放在了PeriphConfigCore.h的最下面_________________________________________________________*/

/*___________________________________________用户SPI1、2配置_______________________________________________*/
#if ((SYSTEM_SPI1_ENABLE)||(SYSTEM_SPI2_ENABLE))


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
		//__HAL_RCC_AFIO_CLK_ENABLE();
		
		//PB13,14,15
		GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出，MISO配置为复用推挽没问题
		GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;        //快速   
		GPIO_Initure.Alternate=GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	}else if(hspi->Instance == SPI2)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();       //使能GPIOB时钟
		__HAL_RCC_SPI2_CLK_ENABLE();        //使能SPI2时钟
		//__HAL_RCC_AFIO_CLK_ENABLE();
		
		//PB13,14,15
		GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出，MISO配置为复用推挽没问题
		GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;        //快速 
		GPIO_Initure.Alternate=GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	}

}


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
	
    //SPI2_ReadWriteByte(0Xff);                           //启动传输
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

#endif

/*________________________________________用户定时器2配置_________________________________________________________*/
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
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	TIM2_Handler.Instance = 		TIM2;                          	//通用定时器2
    TIM2_Handler.Init.Prescaler=	(60-1);                 	   	//分频系数
    TIM2_Handler.Init.CounterMode=	TIM_COUNTERMODE_UP;    			//向上计数器
    TIM2_Handler.Init.Period=		tim2arr;                       	//自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;			//时钟分频因子
	TIM2_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&TIM2_Handler);
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&TIM2_Handler, &sClockSourceConfig);
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM2_Handler, &sMasterConfig);
	
	#if (STSTEM_TIM2_asPWMorCap == 0)
		/*作为PWM*/
		HAL_TIM_PWM_Init(&TIM2_Handler);       //初始化PWM
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_0001)
		{
			TIM2_CH1Handler.OCMode=TIM_OCMODE_PWM1; 		//模式选择PWM1
			TIM2_CH1Handler.Pulse=tim2arr/2;            	//设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
			TIM2_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; 	//输出比较极性为低 
			HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH1Handler,TIM_CHANNEL_1);//配置TIM2通道1
			__HAL_TIM_ENABLE_OCxPRELOAD(&TIM2_Handler, TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_1);	//开启PWM通道1
		}
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_0010)
		{
			TIM2_CH2Handler.OCMode=TIM_OCMODE_PWM1;
			TIM2_CH2Handler.Pulse=tim2arr/2;
			TIM2_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW;
			HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH2Handler,TIM_CHANNEL_2);
			__HAL_TIM_ENABLE_OCxPRELOAD(&TIM2_Handler, TIM_CHANNEL_2);
			HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_2);
		}
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_0100)
		{
			TIM2_CH3Handler.OCMode=TIM_OCMODE_PWM1;
			TIM2_CH3Handler.Pulse=tim2arr/2;
			TIM2_CH3Handler.OCPolarity=TIM_OCPOLARITY_LOW;
			HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH3Handler,TIM_CHANNEL_3);
			__HAL_TIM_ENABLE_OCxPRELOAD(&TIM2_Handler, TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_3);
		}
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_1000)
		{
			TIM2_CH4Handler.OCMode=TIM_OCMODE_PWM1;
			TIM2_CH4Handler.Pulse=tim2arr/2;
			TIM2_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW;
			HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH4Handler,TIM_CHANNEL_4);
			__HAL_TIM_ENABLE_OCxPRELOAD(&TIM2_Handler, TIM_CHANNEL_4);
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
	
	#if (STSTEM_TIM2_asPWMorCap == 2)
		#if (!STSTEM_TIM2_TI_ENABLE)
			HAL_TIM_Base_Start(&TIM2_Handler);	//不带IT的开启定时器
		#else
			HAL_TIM_Base_Start_IT(&TIM2_Handler);//带IT的开启定时器
		#endif
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
	__HAL_RCC_TIM2_CLK_ENABLE();
    //__HAL_AFIO_REMAP_TIM2_ENABLE();
	  
	/*PA15 PB3*/
	  
    /* 定时器通道1功能引脚IO初始化 */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* 定时器通道2功能引脚IO初始化 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
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
	int32_t CaptureNumber = 0;     // 输入捕获数
	
	/* 速度定时器编码器接口捕获值 */
	CaptureNumber = ( int32_t )__HAL_TIM_GET_COUNTER(&TIM2_Handler) + EncoderOverflowCount*65536;
	
	Speed = ((float)CaptureNumber / (float)PPR);
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
		//__HAL_AFIO_REMAP_TIM2_ENABLE();			/*TIM2通道引脚完全重映射使能 (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11)*/
		
		if(STSTEM_TIM2_Cap_trigV == 1)				//上升沿捕获
			GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
		else if(STSTEM_TIM2_Cap_trigV == 0)			//下降沿捕获
			GPIO_Initure.Pull=GPIO_PULLUP;        	//上拉
		else if(STSTEM_TIM2_Cap_trigV == 2)			//双边沿捕获
			GPIO_Initure.Pull=GPIO_NOPULL;        	//无拉
		
		GPIO_Initure.Mode=GPIO_MODE_AF_PP; 			//复用输入
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;	//高速
		GPIO_Initure.Alternate = GPIO_AF1_TIM2;
		
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
		
		HAL_NVIC_SetPriority(TIM2_IRQn,2,0);    //设置中断优先级，抢占优先级2，子优先级0
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断通道  
	}
}

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数
u8  TIM2CHx_CAPTURE_STA=0;							//输入捕获状态		    				
u16	TIM2CHx_CAPTURE_VAL;							//输入捕获值

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
		compare = ((float)tim2arr) * percent;
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


/*________________________________________用户定时器6配置_________________________________________________________*/

#if STSTEM_TIM6_ENABLE

TIM_HandleTypeDef TIM6_Handler;
void sys_TIM6_ENABLE(void)
{
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	TIM6_Handler.Instance=TIM6;                          		//基本定时器6
    TIM6_Handler.Init.Prescaler = (60-1);                     	//分频系数
    TIM6_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    		//向上计数器
    TIM6_Handler.Init.Period = tim6arr;                        	//自动装载值
    TIM6_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;		//时钟分频因子
	TIM6_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&TIM6_Handler);
	
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM6_Handler, &sMasterConfig);
	
    HAL_TIM_Base_Start_IT(&TIM6_Handler); //使能定时器6和定时器6更新中断：TIM_IT_UPDATE 
}


//定时器6中断服务函数
void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM6_Handler);
}

#endif

/*________________________________________用户定时器7配置_________________________________________________________*/
#if STSTEM_TIM7_ENABLE

TIM_HandleTypeDef TIM7_Handler;
void sys_TIM7_ENABLE(void)
{
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	TIM7_Handler.Instance=TIM7;                          		//基本定时器7
    TIM7_Handler.Init.Prescaler = (60-1);                     	//分频系数
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    		//向上计数器
    TIM7_Handler.Init.Period = tim7arr;                        	//自动装载值
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;		//时钟分频因子
	TIM7_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&TIM7_Handler);
	
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM7_Handler, &sMasterConfig);
    
    HAL_TIM_Base_Start_IT(&TIM7_Handler); //使能定时器6和定时器7更新中断：TIM_IT_UPDATE 
}


//定时器7中断服务函数
void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM7_Handler);
}

#endif




/*_________________________________________________DAC_________________________________________________________*/
#if ((SYSTEM_DAC_OUT1_ENABLE) || (SYSTEM_DAC_OUT2_ENABLE))

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
		DACCH1_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_ENABLE;		//DAC1输出缓冲
		/*通道2配置*/
		DACCH2_Config.DAC_Trigger=DAC_TRIGGER_NONE;             	//不使用触发功能
		DACCH2_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_ENABLE;		//DAC1输出缓冲
		
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


/*________________________________________________FSMC_________________________________________________________*/
#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4SRAM))

SRAM_HandleTypeDef SRAM_Handler;    //SRAM句柄

//SRAM初始化
void sys_FSMC_SRAM_ENABLE(void)
{	
	FSMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
	
	SRAM_Handler.Instance=FSMC_NORSRAM_DEVICE;                
	SRAM_Handler.Extended=FSMC_NORSRAM_EXTENDED_DEVICE;    
    
	SRAM_Handler.Init.NSBank=FSMC_NORSRAM_BANK3;     					//使用NE3
	SRAM_Handler.Init.DataAddressMux=FSMC_DATA_ADDRESS_MUX_DISABLE; 	//地址/数据线不复用
	SRAM_Handler.Init.MemoryType=FSMC_MEMORY_TYPE_SRAM;   				//SRAM
	SRAM_Handler.Init.MemoryDataWidth=FSMC_NORSRAM_MEM_BUS_WIDTH_16; 	//16位数据宽度
	SRAM_Handler.Init.BurstAccessMode=FSMC_BURST_ACCESS_MODE_DISABLE; 	//是否使能突发访问,仅对同步突发存储器有效,此处未用到
	SRAM_Handler.Init.WaitSignalPolarity=FSMC_WAIT_SIGNAL_POLARITY_LOW;	//等待信号的极性,仅在突发模式访问下有用
	SRAM_Handler.Init.WrapMode = FSMC_WRAP_MODE_DISABLE; 
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

#if SYSTEM_FSMC_ENABLE

//SRAM底层驱动，时钟使能，引脚分配
//此函数会被HAL_SRAM_Init()调用
//hsram:SRAM句柄
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{	
	/*如果是用于TFT的FSMC*/
	if(&TFTSRAM_Handler == hsram)
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
		GPIO_Initure.Pull=GPIO_NOPULL;			//
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
		GPIO_Initure.Alternate = GPIO_AF12_FSMC;
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化
		
		//初始化PE7,8,9,10,11,12,13,14,15
		GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
						 GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		
		//初始化PG0,12
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_12;
		HAL_GPIO_Init(GPIOG,&GPIO_Initure);
	}
	
	/*如果是用于SRAM的FSMC*/
	if(&SRAM_Handler == hsram)
	{
		GPIO_InitTypeDef GPIO_Initure;
		
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
		GPIO_Initure.Pull=GPIO_NOPULL;              //
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;    //高速 
		GPIO_Initure.Alternate = GPIO_AF12_FSMC;
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
	}

}

#endif

#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4LCD))

SRAM_HandleTypeDef TFTSRAM_Handler;    //SRAM句柄(用于控制LCD)

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
	TFTSRAM_Handler.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
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
	
	HAL_Delay(50); // delay 50 ms 
}

#endif




