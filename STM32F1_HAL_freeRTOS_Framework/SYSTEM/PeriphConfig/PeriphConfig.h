#ifndef __PERIPHCONFIG_H
#define __PERIPHCONFIG_H

#include "sys.h"
#include "sys_menu.h"


/*STEP1:定义都有什么器件*/
enum devicesIndex_enum
{
	TestLED_Index = 0,
	KEY_Index,
	LCD_Index,
	
	ALL_Index		//最后这个固定的不要删
};

/*STEP3.5:填写用于外部中断的IO的中断标志位*/
extern u8 key_Up_Interrupted;
extern u8 key_Down_Interrupted;


/*STEP5:定义IO控制的宏和改变入出模式的宏*/
#define key_Up_ReadIn	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)
#define key_Down_ReadIn	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)

#define	SPI1_CS PAout(4)  		//SPI1的片选信号
#define	SPI2_CS PBout(12)  		//SPI2的片选信号

#define TestLED_Ctrl	PAout(7)



/*___________________________________以下不用动_________________________________________________*/
/*单个IO口初始化结构体*/
typedef struct
{
	GPIO_InitTypeDef GPIO_Initure;		//IO初始化结构体
	GPIO_TypeDef* GPIOx;				//选择GPIO端
	GPIO_PinState defaultState;			//初始化完毕后的默认状态
										//如果pinMode选择IT则启用EXTI，最多16线EXTI，如PA0和PB0公用EXTI0
	uint32_t PreemptPriority;			//外部中断抢占优先级（不使用外部中断时此项无效）
	u8 isEnableExit;					//是否使能外部中断（不使用外部中断时此项无效）
	
}GPIO_Init_Struct;


/*单个器件信息的初始化结构体*/
typedef struct
{
	GPIO_Init_Struct* deviceIO_Struct;		//本器件的IO结构体
	
	enum devicesIndex_enum deviceIndex;		//本器件的索引
	char* deviceName;						//本器件的名称
	const u8 device_IOnum;					//本器件所用的IO口数量
}Devices_Init_Struct;

void Devices_Init(Devices_Init_Struct* Devices , enum devicesIndex_enum device2Init);
void deviceIO_Init(Devices_Init_Struct* Devices , enum devicesIndex_enum device2Init);
extern Devices_Init_Struct UserDevices[];

/*____________________IO操作：单点和位带读写、单点改变输入输出___________________________________*/
/*Author:	正点原子@ALIENTEK*/
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
/*____________________________________________________________用户使用____________*/
//IO口操作,只对单一的IO口!确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入



/*位带操作，8位或者16位一同读入读出*/
/*注：x为16位数据类型：
用法：	u16 data = 0xaaaa;
		data = PBdataIn; 		//16位读入
		PAdataOut(data);		//16位输出
*//*____________________________________________________________用户使用____________*/
#define PAdataOut(x) 	GPIOA->ODR=x; //数据输出
#define PAdataIn     	GPIOA->IDR;   //数据输入

/*____________________________________________________________用户使用____________*/
#define PAh8dataOut(x)	PAdataOut(((GPIOA->IDR)&0x00ff)|((x<<8)&0xff00));	//PA高八位输出
#define PAl8dataOut(x)	PAdataOut(((GPIOA->IDR)&0xff00)|(x&0x00ff));		//PA低八位输出
#define PAh8dataIn		(((GPIOA->IDR)>>8)&0x00ff);							//PA高八位读取
#define PAl8dataIn		((GPIOA->IDR)&0x00ff);								//PA低八位读取

/*____________________________________________________________用户使用____________*/
#define PBdataOut(x) 	GPIOB->ODR=x; //数据输出
#define PBdataIn     	GPIOB->IDR;   //数据输入

/*____________________________________________________________用户使用____________*/
#define PBh8dataOut(x)	PBdataOut(((GPIOB->IDR)&0x00ff)|((x<<8)&0xff00));	//PB高八位输出
#define PBl8dataOut(x)	PBdataOut(((GPIOB->IDR)&0xff00)|(x&0x00ff));		//PB低八位输出
#define PBh8dataIn		(((GPIOB->IDR)>>8)&0x00ff);							//PB高八位读取
#define PBl8dataIn		((GPIOB->IDR)&0x00ff);								//PB低八位读取

/*输入输出模切换（不提供上下拉切换，应在初始化IO时想好上下拉）*/
/*用法：
	PAmodeIn(15); //设置PA15脚为带上下拉的输入
	PAmodeOut(6); //设置PA6脚为推挽输出
*/
#define calMove(x)	4*(x-8)
/*下面两句是例子，帮助回忆*/
//#define PA15in()  {GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=(u32)8<<28;}
//#define PA15out() {GPIOA->CRH&=0X0FFFFFFF;GPIOA->CRH|=(u32)3<<28;}

//分别为PA15~8脚和PA7~0脚设置为带上下拉的输入
#define PAmodeInH(x)	{GPIOA->CRH &= (~(0x0000000F<<(calMove(x))));GPIOA->CRH|=(u32)8<<(calMove(x));}
#define PAmodeInL(x)	{GPIOA->CRL &= (~(0x0000000F<<(calMove(x))));GPIOA->CRL|=(u32)8<<(calMove(x));}
//分别为PA15~8脚和PA7~0脚设置为50MHz推挽输出
#define PAmodeOutH(x)	{GPIOA->CRH &= (~(0x0000000F<<(calMove(x))));GPIOA->CRH|=(u32)3<<(calMove(x));}
#define PAmodeOutL(x)	{GPIOA->CRL &= (~(0x0000000F<<(calMove(x))));GPIOA->CRL|=(u32)3<<(calMove(x));}

/*____________________________________________________________用户使用____________*/
#define PAmodeIn(x)		{if(x>7){PAmodeInH(x);}else{PAmodeInL(x);}}
#define PAmodeOut(x)	{if(x>7){PAmodeOutH(x);}else{PAmodeOutL(x);}}


/*_______________________________定时器2___________________________________*/
extern TIM_HandleTypeDef TIM2_Handler;
void sys_TIM2_ENABLE(void); //写在预编译外面为了外部文件能够调用
#if STSTEM_TIM2_ENABLE
	#if (STSTEM_TIM2_asPWMorCap == 0)
		#define TIM2PWM_Channel_1	1
		#define TIM2PWM_Channel_2	2
		#define TIM2PWM_Channel_3	3
		#define TIM2PWM_Channel_4	4
		extern TIM_OC_InitTypeDef 	TIM2_CH1Handler,TIM2_CH2Handler,TIM2_CH3Handler,TIM2_CH4Handler;
		void TIM2_set_Channel_Pulse(u8 channel,float percent);
	#elif (STSTEM_TIM2_asPWMorCap == 1)
		extern TIM_IC_InitTypeDef TIM2_CHxConfig;
		void Process_TIM2_IC_CallBack_Channel_1(void);
		void Process_TIM2_IC_CallBack_Channel_2(void);
		void Process_TIM2_IC_CallBack_Channel_3(void);
		void Process_TIM2_IC_CallBack_Channel_4(void);
	#elif (STSTEM_TIM2_asPWMorCap == 3)
		extern TIM_Encoder_InitTypeDef sEncoderConfig;
	#endif
#endif

/*输入捕获私有变量*/
extern u8  	TIM2CHx_CAPTURE_STA;	    				
extern u16	TIM2CHx_CAPTURE_VAL;
float Peek_TIM2_Cap_Val(void);

/*正交解码私有变量*/
extern int32_t EncoderOverflowCount;//定时器溢出次数
float peek_TIM2_Encoder_Speed(void);
int32_t peek_TIM2_Encoder_Value(void);

/*_______________________________ADC1___________________________________*/
extern ADC_HandleTypeDef ADC1_Handler;
void sys_ADC1_ENABLE(void);
u32 Get_Adc_Average(u32 ch,u8 times);
float Get_Temprate(u32 adcx);
#if SYSTEM_ADC1_ENABLE
	void ADC_RegularChannelConfig(ADC_HandleTypeDef *AdcHandle, uint32_t Channel, uint32_t Rank, uint32_t SamplingTime);
	u16 Get_Adc(u32 ch);
	#if SYSTEM_ADC1_useScan
		extern DMA_HandleTypeDef  ADC1rxDMA_Handler;
		void ADC_DMA_Cfg(void);
	#endif
#endif

/*_______________________________SPI1、2________________________________*/

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

/*_________________________________低功耗StandbyMode________________________________*/

void sys_StdbyWKUP_ENABLE(void);
void sys_CheckWKUP_4RTOS(void);
#if SYSTEM_StdbyWKUP_ENABLE
	#define WKUP_KD HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //PA0 检测是否外部WK_UP按键按下
	u8 Check_WKUP(void);  			//检测WKUP脚的信号
	void Sys_Enter_Standby(void);	//系统进入待机模式
#endif

/*____________________________FLASH编程___________________________________*/
void STMFLASH_Read(		u32 ReadAddr,	u16 *pBuffer,	u16 NumToRead);		//可用API
void STMFLASH_Write(	u32 WriteAddr,	u16 *pBuffer,	u16 NumToWrite);	//可用API
#if SYSTEM_FLASH_IAP_ENABLE
	
	#if STM32_FLASH_WREN	//如果使能了写   
		extern void FLASH_PageErase(uint32_t PageAddress);
		void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);
	#endif

	u16 STMFLASH_ReadHalfWord(u32 faddr);
#endif

/*_____________________________________DAC________________________________________*/

#if ((SYSTEM_DAC_OUT1_ENABLE) || (SYSTEM_DAC_OUT2_ENABLE)) && ((STM32F103xG) || (STM32F103xE))
extern DAC_HandleTypeDef DAC1_Handler;//DAC句柄
void sys_DAC_ENABLE(void);
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac);
void DAC_Set_Ch1_Vol(float vol);
void DAC_Set_Ch2_Vol(float vol);

#endif



/*____________________SDIO SD_____________________________________________*/

u8 SD_Init(void);
u8 SD_ReadDisk(u8* buf,u32 sector,u32 cnt);
u8 SD_WriteDisk(u8 *buf,u32 sector,u32 cnt);
void show_sdcard_info(void);						//通过串口1打印SD卡相关信息
#if (SYSTEM_SDIO_SD_ENABLE) && ((STM32F103xG) || (STM32F103xE))

extern HAL_SD_CardInfoTypeDef  	SDCardInfo;              	//SD卡信息
extern HAL_SD_CardCIDTypeDef	SDCard_CID;					//SD卡CID信息
extern SD_HandleTypeDef        	SDCARD_Handler;     		//SD卡句柄
#define SD_TIMEOUT 			((uint32_t)100000000)  			//超时时间

#endif

/*____________________________FSMC for SARM_____________________________________________*/
void sys_FSMC_SRAM_ENABLE(void);
void FSMC_SRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n);
void FSMC_SRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n);
#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4SRAM)) && ((STM32F103xG) || (STM32F103xE))

extern SRAM_HandleTypeDef SRAM_Handler;    //SRAM句柄
//使用NOR/SRAM的 Bank1.sector3,地址位HADDR[27,26]=10 
//对IS61LV25616/IS62WV25616,地址线范围为A0~A17 
//对IS61LV51216/IS62WV51216,地址线范围为A0~A18
#define SRAM1_BANK3_ADDR    ((u32)(0x68000000))	//计算： (0x6000 0000 | ((u32)64*1024*1024*(3 - 1)))

#endif


/*____________________________FSMC for LCD_____________________________________________*/
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
#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4LCD)) && ((STM32F103xG) || (STM32F103xE))

extern SRAM_HandleTypeDef TFTSRAM_Handler;    //SRAM句柄(用于控制LCD)

#endif


#endif


