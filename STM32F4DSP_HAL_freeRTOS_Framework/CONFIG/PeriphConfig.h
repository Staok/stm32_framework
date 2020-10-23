#ifndef __PERIPHCONFIG_H
#define __PERIPHCONFIG_H

#include "PeriphConfigCore.h"
#include "simui2c.h"
#include "simuspi.h"


/*_____________________________________\\\                               ///____________________________________________*
 *_________________________________________________用户GPIO配置_________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/


/*STEP 1:定义都有什么器件*/
enum devicesIndex_enum
{
	TestLED_Index = 0,
	KEY_Index,
	LCD_Index,
//	simuI2C_Index,
	
	ALL_Index		//最后这个固定的不要删
};

/*STEP 3.5:填写用于外部中断的IO的中断标志位*/
extern u8 key_Up_Interrupted; 
extern u8 key_Down_Interrupted;

/*STEP 3.75:定义IO控制的宏和改变入出模式的宏*/
#define key_Up_ReadIn	PAin(1)
#define key_Down_ReadIn	PAin(2)
//继续：再去外部中断回调函数HAL_GPIO_EXTI_Callback()中填写IO的外部中断标志为置位

#define	SPI1_CS PAout(4)  		//SPI1的片选信号
#define	SPI2_CS PBout(12)  		//SPI2的片选信号

#define TestLED_Ctrl	PFout(9)
#define TestLED2_Ctrl	PFout(10)

//#define	simuI2C_SCLout 	PEout(2)
//#define	simuI2C_SDAout 	PEout(3)
//#define	simuI2C_SDAin	PEin(3)
//#define simuI2C_SDAinMode	PEinMode(3)
//#define simuI2C_SDAoutMode	PEoutMode(3)


/*_____________________________________\\\                               ///____________________________________________*
 *_________________________________________________以下不用动___________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/
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
//void deviceIO_Init(Devices_Init_Struct* Devices , enum devicesIndex_enum device2Init);
extern Devices_Init_Struct UserDevices[];

/*____________________IO操作：单点和位带读写、单点改变输入输出___________________________________*/
/*Author:	正点原子@ALIENTEK*/
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+20)
#define GPIOB_ODR_Addr    (GPIOB_BASE+20)
#define GPIOC_ODR_Addr    (GPIOC_BASE+20)
#define GPIOD_ODR_Addr    (GPIOD_BASE+20)
#define GPIOE_ODR_Addr    (GPIOE_BASE+20)
#define GPIOF_ODR_Addr    (GPIOF_BASE+20)  
#define GPIOG_ODR_Addr    (GPIOG_BASE+20)   

#define GPIOA_IDR_Addr    (GPIOA_BASE+16)
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16)
#define GPIOD_IDR_Addr    (GPIOD_BASE+16)
#define GPIOE_IDR_Addr    (GPIOE_BASE+16)
#define GPIOF_IDR_Addr    (GPIOF_BASE+16)
#define GPIOG_IDR_Addr    (GPIOG_BASE+16)
 
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

#define PEinMode(x)		{GPIOE->MODER &= ~(0x00000003 << (x * 2));GPIOE->MODER |= 0x00000000 << (x * 2);}
#define PEoutMode(x)	{GPIOE->MODER &= ~(0x00000003 << (x * 2));GPIOE->MODER |= 0x00000001 << (x * 2);}
//默认输出设置是推挽，非开漏

#define DS18B20_IO_IN()  {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=0<<(9*2);}	//PG9输入模式
#define DS18B20_IO_OUT() {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=1<<(9*2);} 	//PG9输出模式


#endif



