#include "lcd.h"
//#include "stdlib.h"
//#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103RBT6,主频72M  单片机工作电压3.3V
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtech.net
//淘宝网站：http://qdtech.taobao.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:QDtech2008@gmail.com 
//Skype:QDtech2008
//技术交流QQ群:324828016
//创建日期:2013/5/13
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2009-2019
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//5V接DC 5V电源
//GND接地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为8位并行模式
//8位模式接线：8位模式9341模块接高8位总线，即:
//液晶屏LCD_D0对应单片机PB8
//液晶屏LCD_D1对应单片机PB9
//液晶屏LCD_D2对应单片机PB10
//液晶屏LCD_D3对应单片机PB11
//液晶屏LCD_D4对应单片机PB12
//液晶屏LCD_D5对应单片机PB13
//液晶屏LCD_D6对应单片机PB14
//液晶屏LCD_D7对应单片机PB15
//=======================================液晶屏控制线接线==========================================//
//LCD_RST	接PB7	//复位信号
//LCD_CS	接PB6	//片选信号
//LCD_RS	接PB5	//寄存器/数据选择信号
//LCD_WR	接PB4	//写信号
//LCD_RD	接PB3	//读信号
//=========================================触摸屏触接线=========================================//
//本模块触摸测试需连接外部XPT2046触摸采样芯片，所以本程序不进行触摸测试。
//不使用触摸或者模块本身不带触摸，则可不连接
**************************************************************************************************/	
	   
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//画笔颜色,背景颜色
u16 POINT_COLOR = RED,BACK_COLOR = BLACK;  
u16 DeviceCode;	 


//******************************************************************
//函数名：  LCD_WR_REG
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写16位指令
//输入参数：Reg:待写入的指令值
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_REG(u8 data)
{ 
#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
	LCD_RS_CLR;
	LCD_CS_CLR;
	
	LCD_PBh8dataOut(data<<8);
	//DATAOUT(((GPIOB->IDR)&0x00ff)|((data<<8)&0xff00));
	LCD_WR_CLR;
	LCD_WR_SET;

	LCD_CS_SET;
	
#else//使用16位并行数据总线模式
	LCD_RS_CLR;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
			
#endif	
}

//******************************************************************
//函数名：  LCD_WR_DATA
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写16位数据
//输入参数：Data:待写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_DATA(u16 data)
{
	

#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
	LCD_RS_SET;
	LCD_CS_CLR;
	
	LCD_PBh8dataOut(data<<8);
	//DATAOUT(((GPIOB->IDR)&0x00ff)|((data<<8)&0xff00));
	LCD_WR_CLR;
	LCD_WR_SET;

	LCD_CS_SET;
	
#else//使用16位并行数据总线模式
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
			
#endif

}
//******************************************************************
//函数名：  LCD_DrawPoint_16Bit
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    8位总线下如何写入一个16位数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint_16Bit(u16 color)
{
#if LCD_USE8BIT_MODEL==1
	
	LCD_RD_SET;
	LCD_RS_SET;//写地址
	LCD_CS_CLR;
	
	LCD_PBh8dataOut(color);
	//DATAOUT(((GPIOB->IDR)&0x00ff)|(color&0xff00));                                        //这里改了	
	LCD_WR_CLR;
	LCD_WR_SET;	
	LCD_PBh8dataOut(color<<8);
	//DATAOUT(((GPIOB->IDR)&0x00ff)|((color<<8)&0xff00));	
	LCD_WR_CLR;
	LCD_WR_SET;
	
	LCD_CS_SET;
#else
	LCD_WR_DATA(color); 
#endif


}

//******************************************************************
//函数名：  LCD_WriteReg
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    写寄存器数据
//输入参数：LCD_Reg:寄存器地址
//			LCD_RegValue:要写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   
	 
//******************************************************************
//函数名：  LCD_WriteRAM_Prepare
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    开始写GRAM
//			在给液晶屏传送RGB数据前，应该发送写GRAM指令
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

//******************************************************************
//函数名：  LCD_DrawPoint
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    在指定位置写入一个像素点数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//设置光标位置 
#if LCD_USE8BIT_MODEL==1
	
	LCD_RD_SET;
	LCD_RS_SET;//写地址  
	LCD_CS_CLR;
	
	LCD_PBh8dataOut(POINT_COLOR);
	//DATAOUT(((GPIOB->IDR)&0x00ff)|(POINT_COLOR&0xff00));                                        //这里改了	
	LCD_WR_CLR;
	LCD_WR_SET;	
	
	LCD_PBh8dataOut(POINT_COLOR<<8);
	//DATAOUT(((GPIOB->IDR)&0x00ff)|((POINT_COLOR<<8)&0xff00));
	LCD_WR_CLR;
	LCD_WR_SET;
	
	LCD_CS_SET;
#else
	LCD_WR_DATA(POINT_COLOR); 
#endif


}

//******************************************************************
//函数名：  LCD_Clear
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD全屏填充清屏函数
//输入参数：Color:要清屏的填充色
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Clear(u16 Color)
{
	u32 index=0;      
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	
#if LCD_USE8BIT_MODEL==1
	LCD_RS_SET;//写数据 
	LCD_CS_CLR;
	for(index=0;index<lcddev.width*lcddev.height;index++)
	{
		LCD_PBh8dataOut(Color);
		//DATAOUT(((GPIOB->IDR)&0x00ff)|(Color&0xff00));                                        //这里改了
		LCD_WR_CLR;
		LCD_WR_SET;
		
		LCD_PBh8dataOut(Color<<8);
		//DATAOUT(((GPIOB->IDR)&0x00ff)|((Color<<8)&0xff00));
		LCD_WR_CLR;
		LCD_WR_SET;
		
	}
	LCD_CS_SET;
#else //16位模式
	for(index=0;index<lcddev.width*lcddev.height;index++)
	{
		LCD_WR_DATA(Color);		  
	}
#endif
	
} 

//******************************************************************
//函数名：  LCD_GPIOInit
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    液晶屏IO初始化，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_GPIOInit(void)
{
	Devices_Init(UserDevices,LCD);
	LCD_CS_SET;LCD_RS_SET;LCD_WR_SET;LCD_RD_SET;LCD_RST_SET;
}

//******************************************************************
//函数名：  LCD_Reset
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD复位函数，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_RESET(void)
{
	LCD_RST_SET;
	delay_ms(1);
	LCD_RST_CLR;
	delay_ms(30);
	LCD_RST_SET;
	delay_ms(100);
}

//******************************************************************
//函数名：  LCD_Init   ILI9341
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD初始化
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Init(void)
{  
     										 
	LCD_GPIOInit();
 	LCD_RESET();
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xC1); 
	LCD_WR_DATA(0X30);
	
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81);
	
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10);//LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x7A); //LCD_WR_DATA(0x78);
	
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02);
	
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x23);//LCD_WR_DATA(0x1B);   VRH[5:0] 
	
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x10);//LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
	
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x30);//LCD_WR_DATA(0x3E); 	 //3F
	LCD_WR_DATA(0x30);//LCD_WR_DATA(0x28); 	 //3C
	
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB7); //LCD_WR_DATA(0x86);
	
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x48);//
	
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A);//LCD_WR_DATA(0x18);
	
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A);//LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0xA2);//LCD_WR_DATA(0x82);
	LCD_WR_DATA(0x27);
	
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x0E); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x54); 
	LCD_WR_DATA(0XA9); 
	LCD_WR_DATA(0x43); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 		 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x15); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x11); 
	LCD_WR_DATA(0x06); 
	LCD_WR_DATA(0x2B); 
	LCD_WR_DATA(0x56); 
	LCD_WR_DATA(0x3C); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);	 
	LCD_WR_REG(0x11); //Exit Sleep
	delay_ms(120);
	LCD_WR_REG(0x29); //display on	
	
	//delay_ms(500);
	LCD_SetParam();//设置LCD参数	 
	//LCD_LED=1;//点亮背光	 
	//LCD_Clear(WHITE);
}
  		  
/*************************************************
函数名：LCD_SetWindows
功能：设置lcd显示窗口，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);	

	LCD_WriteRAM_Prepare();	//开始写入GRAM				
}   

/*************************************************
函数名：LCD_SetCursor
功能：设置光标位置
入口参数：xy坐标
返回值：无
*************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(0x00FF&Xpos);		

	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(0x00FF&Ypos);		

	LCD_WriteRAM_Prepare();	//开始写入GRAM	
} 

//设置LCD参数
//方便进行横竖屏模式切换
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//使用横屏	  
	lcddev.dir=1;//横屏
	lcddev.width=320;
	lcddev.height=240;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;			 
	LCD_WriteReg(0x36,0x28);//    0x6C

#else//竖屏
	lcddev.dir=0;//竖屏				 	 		
	lcddev.width=240;
	lcddev.height=320;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;	
	LCD_WriteReg(0x36,0x88); // 0xC9    //这里不再修改
#endif
}	


