#ifndef __TFTLCD_H__
#define __TFTLCD_H__

#include "PeriphConfigCore.h"
#include "PeriphConfig.h"

/*
  说明和API：

默认：
1、统一把LCD的RESET脚连到MCU的RESET脚
2、FSMC目前只支持16位数据（既然已经上FSMC，那么IO口肯定不紧张，没必要用8位数据线了）
3、IO口驱动默认8位数据（为了IO口少的MCU考虑）
4、IO口驱动目前没有ReadReg()和ReadPoint()函数，也没有读出型号，目前只支持ILI9341
5、FSMC驱动独有的API函数：
void LCD_with_FSMC_init_LCD(void);						//初始化
void LCD_SSD_BackLightSet(u8 pwm);						//SSD1963 背光控制
u32  LCD_ReadPoint(u16 x,u16 y); 						//读点 
void LCD_DisplayOn(void);								//开显示
void LCD_DisplayOff(void);								//关显示
void LCD_Scan_Dir(u8 dir);								//设置屏扫描方向
void LCD_Display_Dir(u8 dir);							//设置屏幕显示方向
u16 LCD_ReadReg(u16 LCD_Reg);
u16 LCD_RD_DATA(void);

6、IO驱动独有的API：
void LCD_Init_no_FSMC(void);							//初始化
void LCD_SetParam_4ILI9341(void);						//写入只支持ILI9341的参数
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);	//居中显示一个字符串，包含中英文显示
void DrawPageHead(u8 *str);										//在屏幕顶部画此页面的标题
void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p);			//显示显示一副16位BMP图像
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);

void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
//用于写中英文和中文混合字符串，推荐单独写中文用；中文跟支持16、24和32，需要提前取模加入到FONT.H里面

7、两种驱动共有的API：
void LCD_Clear(u32 Color);	 									//清屏
void LCD_SetCursor(u16 Xpos, u16 Ypos);							//设置光标
void LCD_DrawPoint(u16 x,u16 y);								//画点
void LCD_Fast_DrawPoint(u16 x,u16 y,u32 color);					//快速画点
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);						//画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);				//画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);			//画矩形
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);		   	//填充单色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);	//填充指定颜色

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);			//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  			//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);	//显示 数字

void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p);	
//显示一个字符串，12/16/24/32字体，用于写英文和数字，不能写中文；推荐只写写英文和数字用（TODO：字库已经加入24和32的编码，测试4种大小是否都可以支持）

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);	


8、指定颜色，要避免画笔和背景同色
POINT_COLOR = RED;
BACK_COLOR = BLACK;


	使用说明：

两种驱动，一种16位数据线的FSMC驱动，一种8位数据选的IO口驱动，对应不同的初始化函数：

1、若用FSMC，不用自己初始化IO口，打开宏定义SYSTEM_FSMC_ENABLE和SYSTEM_FSMC_use4LCD，然后调用LCD_Init_with_FSMC()初始化函数即可，
默认在sys_Device_Init_Seq()里面已经调用。默认使用FSMC的块1区3驱动，IO连接如下：
	一般接法：（CE前面加一个N，表示CE低有效，其他同理）
	FSMC			LCD
	NEx			CS（低选中）
	NOE			RD（低读）
	NWE			WR（低写）
				RS（高数据低命令）——可接至FSMC的一个地址脚如A10
	D[15:0]			D[15:0]（数据）
	LCD分辨率参数：
	1、修改函数void LCD_Display_Dir(u8 dir)内定义的LCD分分辨率
	2、修改下面的 //LCD分辨率设置（注：这两个参数只用于SSD1963控制器）
			#define SSD_HOR_RESOLUTION		240		//LCD水平分辨率
			#define SSD_VER_RESOLUTION		320		//LCD垂直分辨率

2、若用IO口驱动，只需调用LCD_Init_no_FSMC()初始化函数即可，默认在sys_Device_Init_Seq()里面已经调用。
LCD分辨率参数：
	如果使用非240x320的屏幕，在LCD_SetParam_4ILI9341()函数里修改分辨率
若需要改IO按照periphconfig.c里面的定义IO改，并相应的改lcd.h里面的IO口宏定义（比较麻烦）；默认IO口如下：
	LCD_RST	接PB7	//复位信号
	LCD_CS	接PB6	//片选信号
	LCD_RS	接PB5	//寄存器/数据选择信号
	LCD_WR	接PB4	//写信号
	LCD_RD	接PB3	//读信号
	D[0:7]	接PB8~15	//数据线
	
	说明：对于0x5510控制器，写寄存器地址是16位的，目前的函数是八位的，还不支持16位的
	
*/

/*声明：以下几乎全部参考和移植于正点原子的LCD章节源码和网络上的部分源码，但都是开源代码，本代码也开源，并不用于商业用途*/

/*_______________________只修改下面的DFT_SCAN_DIR也可以，这里就不用动了___________________________________*/
#define USE_HORIZONTAL  	0	//定义是否使用横屏 		0,竖屏.1,横屏.

/*去FONT.H里面定义字库的编译选项，以缩减FLASH占用*/
	//里面有以下这两个宏选项
	//#define USE_ALL_ASC_FONT	0	//是否使用所有的ASC字体；此选项如果是0则只编译1608的ASC
	//#define USE_Chinese_FONT	1	//是否使用中文显示，如果没用到就置0，则不会编译相关函数和字库

/*_______________________________以下都是固定的，不用动______________________________________*/

//LCD重要参数集
typedef struct  
{		 	 
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram的指令
	u16 setxcmd;		//设置x坐标指令
	u16 setycmd;		//设置y坐标指令 
}_lcd_dev; 
extern _lcd_dev lcddev;	//管理LCD重要参数


//LCD的画笔颜色和背景色	   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色，默认为白色


//扫描方向定义
#define L2R_U2D  0 		//从左到右,从上到下
#define L2R_D2U  1 		//从左到右,从下到上——对于IO驱动，为竖屏反
#define R2L_U2D  2 		//从右到左,从上到下——对于IO驱动，为竖屏正
#define R2L_D2U  3 		//从右到左,从下到上

#define U2D_L2R  4 		//从上到下,从左到右——对于IO驱动，为横屏朝左
#define U2D_R2L  5 		//从上到下,从右到左
#define D2U_L2R  6 		//从下到上,从左到右
#define D2U_R2L  7		//从下到上,从右到左——对于IO驱动，为横屏朝右

/*_______________________用户修改___________________________________*/
#define DFT_SCAN_DIR  L2R_D2U  //默认的扫描方向

//画笔颜色，都是16位颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


#define	LCD_CS_SET  GPIOB->BSRR=1<<6    //片选端口  	PB6
#define	LCD_RS_SET	GPIOB->BSRR=1<<5    //数据/命令   	PB5	   
#define	LCD_WR_SET	GPIOB->BSRR=1<<4    //写数据		PB4
#define	LCD_RD_SET	GPIOB->BSRR=1<<3    //读数据		PB3
#define	LCD_RST_SET	GPIOB->BSRR=1<<7    //复位			PB7
								    
#define	LCD_CS_CLR  GPIOB->BSRR=1<<(6+16)     //片选端口  	PB6
#define	LCD_RS_CLR	GPIOB->BSRR=1<<(5+16)     //数据/命令  	PB5	   
#define	LCD_WR_CLR	GPIOB->BSRR=1<<(4+16)     //写数据		PB4
#define	LCD_RD_CLR	GPIOB->BSRR=1<<(3+16)     //读数据		PB3
#define	LCD_RST_CLR	GPIOB->BSRR=1<<(7+16)     //复位		PB7	

//#define	LCD_CS_SET  PBout(6) = 1    //片选端口  	PB6
//#define	LCD_RS_SET	PBout(5) = 1    //数据/命令   	PB5	   
//#define	LCD_WR_SET	PBout(4) = 1    //写数据		PB4
//#define	LCD_RD_SET	PBout(3) = 1    //读数据		PB3
//#define	LCD_RST_SET	PBout(7) = 1    //复位			PB7
//								    
//#define	LCD_CS_CLR  PBout(6) = 0     //片选端口  	PB6
//#define	LCD_RS_CLR	PBout(5) = 0     //数据/命令  	PB5	   
//#define	LCD_WR_CLR	PBout(4) = 0     //写数据		PB4
//#define	LCD_RD_CLR	PBout(3) = 0     //读数据		PB3
//#define	LCD_RST_CLR	PBout(7) = 0     //复位			PB7	

#define DATAOUT(x) GPIOB->ODR = x; //数据输出
#define DATAIN     (GPIOB->IDR);   //数据输入

#define LCD_PBh8dataOut(x)	GPIOB->ODR=(((GPIOB->IDR)&0x00ff)|((x << 8)&0xff00))	//PB高八位输出


void LCD_with_FSMC_init_LCD(void);												//初始化
void LCD_Init_no_FSMC(void);
void LCD_DisplayOn(void);													//开显示
void LCD_DisplayOff(void);													//关显示
void LCD_Clear(u32 Color);	 												//清屏
void LCD_SetCursor(u16 Xpos, u16 Ypos);										//设置光标
void LCD_DrawPoint(u16 x,u16 y);											//画点
void LCD_Fast_DrawPoint(u16 x,u16 y,u32 color);								//快速画点
u32  LCD_ReadPoint(u16 x,u16 y); 											//读点 
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);						 			//画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//画矩形
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);		   				//填充单色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//填充指定颜色
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p);								//显示一个字母数据字符串,12/16/24/32字体


void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_Scan_Dir(u8 dir);									//设置屏扫描方向
void LCD_Display_Dir(u8 dir);								//设置屏幕显示方向
void LCD_SSD_BackLightSet(u8 pwm);							//SSD1963 背光控制
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);	//FSMC相关函数没有调用，设置窗口	

void LCD_SetParam_4ILI9341(void);								//写入只支持ILI9341的参数
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);//用于IO口驱动的相关函数调用
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);	//居中显示一个字符串，包含中英文显示
void DrawPageHead(u8 *str);										//在屏幕顶部画此页面的标题
void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p);			//显示显示一副16位BMP图像
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);

void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);	

//LCD分辨率设置（注：这两个参数只用于SSD1963控制器）
#define SSD_HOR_RESOLUTION		240		//LCD水平分辨率
#define SSD_VER_RESOLUTION		320		//LCD垂直分辨率
//LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		//水平脉宽
#define SSD_HOR_BACK_PORCH		46		//水平前廊
#define SSD_HOR_FRONT_PORCH		210		//水平后廊

#define SSD_VER_PULSE_WIDTH		1		//垂直脉宽
#define SSD_VER_BACK_PORCH		23		//垂直前廊
#define SSD_VER_FRONT_PORCH		22		//垂直前廊
//如下几个参数，自动计算
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)




#endif


