#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"

/****************************************************************************************************
//=======================================液晶屏数据线接线==========================================//
STM32 PB组接液晶屏DB0~DB16,举例依次为DB0接PB0,..DB15接PB15.   
//=======================================液晶屏控制线接线==========================================//
//LCD_RST	接PB7	//复位信号
//LCD_CS	接PB6	//片选信号
//LCD_RS	接PB5	//寄存器/数据选择信号
//LCD_WR	接PB4	//写信号
//LCD_RD	接PB3	//读信号
//=========================================触摸屏触接线=========================================//
//不使用触摸或者模块本身不带触摸，则可不连接
//MO(MISO)	接PC2	//SPI总线输出
//MI(MOSI)	接PC3	//SPI总线输入
//PEN		接PC1	//触摸屏中断信号
//TCS		接PC13	//触摸IC片选
//CLK		接PC0	//SPI总线时钟
**************************************************************************************************/	

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////	 
//支持横竖屏快速定义切换，支持8/16位模式切换
#define USE_HORIZONTAL  	0	//定义是否使用横屏 		0,不使用.1,使用.
#define LCD_USE8BIT_MODEL   1    //定义数据总线是否使用8位模式 0,使用16位模式.1,使用8位模式
//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#if USE_HORIZONTAL==1	//使用横屏
#define LCD_W 320
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 320
#endif

//TFTLCD部分外要调用的函数		   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 
//QDtech全系列模块采用了三极管控制背光亮灭，用户也可以接PWM调节背光亮度
//#define	LCD_LED PCout(10) //LCD背光    		 PC10 
//如果使用官方库函数定义下列底层，速度将会下降到14帧每秒，建议采用我司推荐方法
//以下IO定义直接操作寄存器，快速IO操作，刷屏速率可以达到28帧每秒！ 

#define	LCD_CS_SET  GPIOB->BSRR=1<<6    //片选端口  	PB6
#define	LCD_RS_SET	GPIOB->BSRR=1<<5    //数据/命令   	PB5	   
#define	LCD_WR_SET	GPIOB->BSRR=1<<4    //写数据		PB4
#define	LCD_RD_SET	GPIOB->BSRR=1<<3    //读数据		PB3
#define	LCD_RST_SET	GPIOB->BSRR=1<<7    //复位			PB7
								    
#define	LCD_CS_CLR  GPIOB->BRR=1<<6     //片选端口  	PB6
#define	LCD_RS_CLR	GPIOB->BRR=1<<5     //数据/命令  	PB5	   
#define	LCD_WR_CLR	GPIOB->BRR=1<<4     //写数据		PB4
#define	LCD_RD_CLR	GPIOB->BRR=1<<3     //读数据		PB3
#define	LCD_RST_CLR	GPIOB->BRR=1<<7    //复位			PB7	

//#define	LCD_CS_SET  GPIO_SetBits(GPIOB,GPIO_Pin_6)    //片选端口  	PB6
//#define	LCD_RS_SET	GPIO_SetBits(GPIOB,GPIO_Pin_5)    //数据/命令   PB5	   
//#define	LCD_WR_SET	GPIO_SetBits(GPIOB,GPIO_Pin_4)    //写数据		PB4
//#define	LCD_RD_SET	GPIO_SetBits(GPIOB,GPIO_Pin_3)    //读数据		PB3
//#define	LCD_RST_SET	GPIO_SetBits(GPIOB,GPIO_Pin_7)    //复位		PB7
//								    
//#define	LCD_CS_CLR  GPIO_ResetBits(GPIOB,GPIO_Pin_6)     //片选端口  	PB6
//#define	LCD_RS_CLR	GPIO_ResetBits(GPIOB,GPIO_Pin_5)     //数据/命令  	PB5	   
//#define	LCD_WR_CLR	GPIO_ResetBits(GPIOB,GPIO_Pin_4)     //写数据		PB4
//#define	LCD_RD_CLR	GPIO_ResetBits(GPIOB,GPIO_Pin_3)     //读数据		PB3
//#define	LCD_RST_CLR	GPIO_ResetBits(GPIOB,GPIO_Pin_7)    //复位			PB7

#define DATAOUT(x) GPIOB->ODR=x; //数据输出
#define DATAIN     GPIOB->IDR;   //数据输入

#define LCD_PBh8dataOut(x)	GPIOB->ODR=(((GPIOB->IDR)&0x00ff)|(x&0xff00));	//PB高八位输出
//////////////////////////////////////////////////////////////////////


//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左

#define DFT_SCAN_DIR  0  //默认的扫描方向


//画笔颜色
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //棕色
#define BRRED 			0XFC07 //棕红色
#define GRAY  			0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0X841F //浅绿色
//#define LIGHTGRAY     0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
extern u16 BACK_COLOR, POINT_COLOR ;  

void LCD_Init(void);
//void LCD_DisplayOn(void);
//void LCD_DisplayOff(void);
void LCD_Clear(u16 Color);	 
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x,u16 y);//画点
//u16  LCD_ReadPoint(u16 x,u16 y); //读点
//void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
//void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);
void LCD_DrawPoint_16Bit(u16 color);
//u16 LCD_RD_DATA(void);//读取LCD数据									    
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_REG(u8 data);
void LCD_WR_DATA(u16 data);
//u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_GPIOInit(void);
void LCD_RESET(void);
//void LCD_WriteRAM(u16 RGB_Code);
//u16 LCD_ReadRAM(void);		   
//u16 LCD_BGR2RGB(u16 c);
void LCD_SetParam(void);



//如果仍然觉得速度不够快，可以使用下面的宏定义,提高速度.
//注意要去掉lcd.c中void LCD_WR_DATA(u16 data)函数定义哦
/*
#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	DATAOUT(data<<8);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	}
	#else//使用16位并行数据总线模式
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	} 	
#endif
*/

//9320/9325 LCD寄存器  
//#define R0             0x00
//#define R1             0x01
//#define R2             0x02
//#define R3             0x03
//#define R4             0x04
//#define R5             0x05
//#define R6             0x06
//#define R7             0x07
//#define R8             0x08
//#define R9             0x09
//#define R10            0x0A
//#define R12            0x0C
//#define R13            0x0D
//#define R14            0x0E
//#define R15            0x0F
//#define R16            0x10
//#define R17            0x11
//#define R18            0x12
//#define R19            0x13
//#define R20            0x14
//#define R21            0x15
//#define R22            0x16
//#define R23            0x17
//#define R24            0x18
//#define R25            0x19
//#define R26            0x1A
//#define R27            0x1B
//#define R28            0x1C
//#define R29            0x1D
//#define R30            0x1E
//#define R31            0x1F
//#define R32            0x20
//#define R33            0x21
//#define R34            0x22
//#define R36            0x24
//#define R37            0x25
//#define R40            0x28
//#define R41            0x29
//#define R43            0x2B
//#define R45            0x2D
//#define R48            0x30
//#define R49            0x31
//#define R50            0x32
//#define R51            0x33
//#define R52            0x34
//#define R53            0x35
//#define R54            0x36
//#define R55            0x37
//#define R56            0x38
//#define R57            0x39
//#define R59            0x3B
//#define R60            0x3C
//#define R61            0x3D
//#define R62            0x3E
//#define R63            0x3F
//#define R64            0x40
//#define R65            0x41
//#define R66            0x42
//#define R67            0x43
//#define R68            0x44
//#define R69            0x45
//#define R70            0x46
//#define R71            0x47
//#define R72            0x48
//#define R73            0x49
//#define R74            0x4A
//#define R75            0x4B
//#define R76            0x4C
//#define R77            0x4D
//#define R78            0x4E
//#define R79            0x4F
//#define R80            0x50
//#define R81            0x51
//#define R82            0x52
//#define R83            0x53
//#define R96            0x60
//#define R97            0x61
//#define R106           0x6A
//#define R118           0x76
//#define R128           0x80
//#define R129           0x81
//#define R130           0x82
//#define R131           0x83
//#define R132           0x84
//#define R133           0x85
//#define R134           0x86
//#define R135           0x87
//#define R136           0x88
//#define R137           0x89
//#define R139           0x8B
//#define R140           0x8C
//#define R141           0x8D
//#define R143           0x8F
//#define R144           0x90
//#define R145           0x91
//#define R146           0x92
//#define R147           0x93
//#define R148           0x94
//#define R149           0x95
//#define R150           0x96
//#define R151           0x97
//#define R152           0x98
//#define R153           0x99
//#define R154           0x9A
//#define R157           0x9D
//#define R192           0xC0
//#define R193           0xC1
//#define R229           0xE5							  		 
#endif  
	 
	 



