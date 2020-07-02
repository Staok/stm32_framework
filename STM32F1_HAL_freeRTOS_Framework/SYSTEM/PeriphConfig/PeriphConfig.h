#ifndef __PERIPHCONFIG_H
#define __PERIPHCONFIG_H

#include "sys.h"


/*STEP1:定义都有什么器件*/
enum devicesIndex_enum
{
	LCD = 0,
	BUCK,
	
	ALL		//最后这个固定的不要删
};


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
*/
#define PAdataOut(x) 	GPIOA->ODR=x; //数据输出
#define PAdataIn     	GPIOA->IDR;   //数据输入

#define PAh8dataOut(x)	PAdataOut(((GPIOA->IDR)&0x00ff)|((x<<8)&0xff00));	//PA高八位输出
#define PAl8dataOut(x)	PAdataOut(((GPIOA->IDR)&0xff00)|(x&0x00ff));		//PA低八位输出
#define PAh8dataIn		(((GPIOA->IDR)>>8)&0x00ff);							//PA高八位读取
#define PAl8dataIn		((GPIOA->IDR)&0x00ff);								//PA低八位读取

#define PBdataOut(x) 	GPIOB->ODR=x; //数据输出
#define PBdataIn     	GPIOB->IDR;   //数据输入

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

#define PAmodeIn(x)		{if(x>7){PAmodeInH(x);}else{PAmodeInL(x);}}
#define PAmodeOut(x)	{if(x>7){PAmodeOutH(x);}else{PAmodeOutL(x);}}


/*____________________定时器2___________________________________*/
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
	#endif
#endif

extern u8  	TIM2CHx_CAPTURE_STA;	    				
extern u16	TIM2CHx_CAPTURE_VAL;
float Peek_TIM2_Cap_Val(void);

/*____________________ADC1___________________________________*/
extern ADC_HandleTypeDef ADC1_Handler;
void sys_ADC1_ENABLE(void);
void Get_Adc_Average(u32 ch,u8 times,u32* result);
float Get_Temprate(u32 adcx);
#if SYSTEM_ADC1_ENABLE
	void ADC_RegularChannelConfig(ADC_HandleTypeDef *AdcHandle, uint32_t Channel, uint32_t Rank, uint32_t SamplingTime);
	u16 Get_Adc(u32 ch);
	#if SYSTEM_ADC1_useDMA1
		extern DMA_HandleTypeDef  ADC1rxDMA_Handler;
		void ADC_DMA_Cfg(void);
	#endif
#endif

/*____________________用户SPI1配置_________________________*/

extern SPI_HandleTypeDef SPI1_Handler;  //SPI1句柄
extern SPI_HandleTypeDef SPI2_Handler;  //SPI2句柄
void sys_SPI1_ENABLE(void);
void sys_SPI2_ENABLE(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI1_ReadWriteByte(u8 TxData);
u8 SPI2_ReadWriteByte(u8 TxData);

void sys_SPI1_SS_io_Init(void);
#define	SPI1_CS PAout(4)  		//SPI1的片选信号
void sys_SPI2_SS_io_Init(void);
#define	SPI2_CS PBout(12)  		//SPI2的片选信号


#endif


