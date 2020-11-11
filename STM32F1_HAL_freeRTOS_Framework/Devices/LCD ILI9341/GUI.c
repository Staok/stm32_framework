#include "lcd.h"
//#include "string.h"
#include "font.h" 
//#include "delay.h"
#include "GUI.h"
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
//////////////////////////////////////////////////////////////////////////////////
//******************************************************************
//函数名：  GUI_DrawPoint
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    GUI描绘一个点
//输入参数：x:光标位置x坐标
//        	y:光标位置y坐标
//			color:要填充的颜色
//返回值：  无
//修改记录：无
//******************************************************************
void GUI_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_SetCursor(x,y);//设置光标位置 
	LCD_DrawPoint_16Bit(color); 
}

//******************************************************************
//函数名：  LCD_Fill
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    在指定区域内填充颜色
//输入参数：sx:指定区域开始点x坐标
//        	sy:指定区域开始点y坐标
//			ex:指定区域结束点x坐标
//			ey:指定区域结束点y坐标
//        	color:要填充的颜色
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{  	

	u16 i,j;			
	u16 width=ex-sx+1; 		//得到填充的宽度
	u16 height=ey-sy+1;		//高度
	LCD_SetWindows(sx,sy,ex-1,ey-1);//设置显示窗口

#if LCD_USE8BIT_MODEL==1
	LCD_RS_SET;//写数据 
	LCD_CS_CLR;	   
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_PBh8dataOut(color);
			//DATAOUT(((GPIOB->IDR)&0x00ff)|(color&0xff00));                                        //这里改了
			LCD_WR_CLR;
			LCD_WR_SET;	
			
			LCD_PBh8dataOut(color<<8);
			//DATAOUT(((GPIOB->IDR)&0x00ff)|((color<<8)&0xff00));
			LCD_WR_CLR;
			LCD_WR_SET;	
		}   
	}
	LCD_CS_SET;	
#else //16位模式
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		LCD_WR_DATA(color);	//写入数据 	 
	}
#endif
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口设置为全屏
}

//******************************************************************
//函数名：  LCD_DrawLine
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    GUI画线
//输入参数：x1,y1:起点坐标
//        	x2,y2:终点坐标 
//返回值：  无
//修改记录：无
//****************************************************************** 
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

//******************************************************************
//函数名：  LCD_DrawRectangle
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    GUI画矩形(非填充)
//输入参数：(x1,y1),(x2,y2):矩形的对角坐标
//返回值：  无
//修改记录：无
//******************************************************************  
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}  

//******************************************************************
//函数名：  LCD_DrawFillRectangle
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    GUI画矩形(填充)
//输入参数：(x1,y1),(x2,y2):矩形的对角坐标
//返回值：  无
//修改记录：无
//******************************************************************   
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_Fill(x1,y1,x2,y2,POINT_COLOR);

}
 
//******************************************************************
//函数名：  _draw_circle_8
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    8对称性画圆算法(内部调用)
//输入参数：(xc,yc) :圆中心坐标
// 			(x,y):光标相对于圆心的坐标
//         	c:填充的颜色
//返回值：  无
//修改记录：无
//******************************************************************  
void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
{
	GUI_DrawPoint(xc + x, yc + y, c);

	GUI_DrawPoint(xc - x, yc + y, c);

	GUI_DrawPoint(xc + x, yc - y, c);

	GUI_DrawPoint(xc - x, yc - y, c);

	GUI_DrawPoint(xc + y, yc + x, c);

	GUI_DrawPoint(xc - y, yc + x, c);

	GUI_DrawPoint(xc + y, yc - x, c);

	GUI_DrawPoint(xc - y, yc - x, c);
}

//******************************************************************
//函数名：  gui_circle
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    在指定位置画一个指定大小的圆(填充)
//输入参数：(xc,yc) :圆中心坐标
//         	c:填充的颜色
//		 	r:圆半径
//		 	fill:填充判断标志，1-填充，0-不填充
//返回值：  无
//修改记录：无
//******************************************************************  
void gui_circle(int xc, int yc,u16 c,int r, int fill)
{
	int x = 0, y = r, yi, d;

	d = 3 - 2 * r;


	if (fill) 
	{
		// 如果填充（画实心圆）
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				_draw_circle_8(xc, yc, x, yi, c);

			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	} else 
	{
		// 如果不填充（画空心圆）
		while (x <= y) {
			_draw_circle_8(xc, yc, x, y, c);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

//******************************************************************
//函数名：  LCD_ShowChar
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    显示单个英文字符
//输入参数：(x,y):字符显示位置起始坐标
//        	fc:前置画笔颜色
//			bc:背景颜色
//			num:数值（0-94）
//			size:字体大小
//			mode:模式  0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************  
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
{  
    u8 temp;
    u8 pos,t;
	u16 colortemp=POINT_COLOR;      
		   
	num=num-' ';//得到偏移后的值
	LCD_SetWindows(x,y,x+size/2-1,y+size-1);//设置单个文字显示窗口
	if(!mode) //非叠加方式
	{
		
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//调用1206字体
			else temp=asc2_1608[num][pos];		 //调用1608字体
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint_16Bit(fc); 
				else LCD_DrawPoint_16Bit(bc); 
				temp>>=1; 
				
		    }
			
		}	
	}else//叠加方式
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//调用1206字体
			else temp=asc2_1608[num][pos];		 //调用1608字体
			for(t=0;t<size/2;t++)
		    {   
				POINT_COLOR=fc;              
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点    
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏    	   	 	  
} 
//******************************************************************
//函数名：  LCD_ShowString
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    显示英文字符串
//输入参数：x,y :起点坐标	 
//			size:字体大小
//			*p:字符串起始地址
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************  	  
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
{         
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {   
		if(x>(lcddev.width-1)||y>(lcddev.height-1)) 
		return;     
        LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR,*p,size,mode);
        x+=size/2;
        p++;
    }  
} 

//******************************************************************
//函数名：  mypow
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    求m的n次方(gui内部调用)
//输入参数：m:乘数
//	        n:幂
//返回值：  m的n次方
//修改记录：无
//******************************************************************  
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

//******************************************************************
//函数名：  LCD_ShowNum
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    显示单个数字变量值
//输入参数：x,y :起点坐标	 
//			len :指定显示数字的位数
//			size:字体大小(12,16)
//			color:颜色
//			num:数值(0~4294967295)
//返回值：  无
//修改记录：无
//******************************************************************  			 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,temp+'0',size,0); 
	}
} 

//******************************************************************
//函数名：  GUI_DrawFont16
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    显示单个16X16中文字体
//输入参数：x,y :起点坐标
//			fc:前置画笔颜色
//			bc:背景颜色	 
//			s:字符串地址
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;
	u16 x0=x;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//自动统计汉字数目
	
			
	for (k=0;k<HZnum;k++) 
	{
	  if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
	  { 	LCD_SetWindows(x,y,x+16-1,y+16-1);
		    for(i=0;i<16*2;i++)
		    {
				for(j=0;j<8;j++)
		    	{	
					if(!mode) //非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x80>>j))	LCD_DrawPoint_16Bit(fc);
						else LCD_DrawPoint_16Bit(bc);
					}
					else
					{
						POINT_COLOR=fc;
						if(tfont16[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//画一个点
						x++;
						if((x-x0)==16)
						{
							x=x0;
							y++;
							break;
						}
					}

				}
				
			}
			
			
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}

	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
} 

//******************************************************************
//函数名：  GUI_DrawFont24
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    显示单个24X24中文字体
//输入参数：x,y :起点坐标
//			fc:前置画笔颜色
//			bc:背景颜色	 
//			s:字符串地址
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;
	u16 x0=x;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//自动统计汉字数目
		
			for (k=0;k<HZnum;k++) 
			{
			  if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
			  { 	LCD_SetWindows(x,y,x+24-1,y+24-1);
				    for(i=0;i<24*3;i++)
				    {
							for(j=0;j<8;j++)
							{
								if(!mode) //非叠加方式
								{
									if(tfont24[k].Msk[i]&(0x80>>j))	LCD_DrawPoint_16Bit(fc);
									else LCD_DrawPoint_16Bit(bc);
								}
							else
							{
								POINT_COLOR=fc;
								if(tfont24[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//画一个点
								x++;
								if((x-x0)==24)
								{
									x=x0;
									y++;
									break;
								}
							}
						}
					}
					
					
				}				  	
				continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
			}

	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
}

//******************************************************************
//函数名：  GUI_DrawFont32
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    显示单个32X32中文字体
//输入参数：x,y :起点坐标
//			fc:前置画笔颜色
//			bc:背景颜色	 
//			s:字符串地址
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//****************************************************************** 
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;
	u16 x0=x;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//自动统计汉字数目
	for (k=0;k<HZnum;k++) 
			{
			  if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
			  { 	LCD_SetWindows(x,y,x+32-1,y+32-1);
				    for(i=0;i<32*4;i++)
				    {
						for(j=0;j<8;j++)
				    	{
							if(!mode) //非叠加方式
							{
								if(tfont32[k].Msk[i]&(0x80>>j))	LCD_DrawPoint_16Bit(fc);
								else LCD_DrawPoint_16Bit(bc);
							}
							else
							{
								POINT_COLOR=fc;
								if(tfont32[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//画一个点
								x++;
								if((x-x0)==32)
								{
									x=x0;
									y++;
									break;
								}
							}
						}
					}
					
					
				}				  	
				continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
			}
	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复窗口为全屏  
} 

//******************************************************************
//函数名：  Show_Str
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    显示一个字符串,包含中英文显示
//输入参数：x,y :起点坐标
// 			fc:前置画笔颜色
//			bc:背景颜色
//			str :字符串	 
//			size:字体大小
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************    	   		   
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{					
	u16 x0=x;							  	  
  	u8 bHz=0;     //字符或者中文 
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
			if(x>(lcddev.width-size/2)||y>(lcddev.height-size)) 
			return; 
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {          
		        if(*str==0x0D)//换行符号
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else
				{
					if(size>16)//字库中没有集成12X24 16X32的英文字体,用8X16代替
					{  
					LCD_ShowChar(x,y,fc,bc,*str,16,mode);
					x+=8; //字符,为全字的一半 
					}
					else
					{
					LCD_ShowChar(x,y,fc,bc,*str,size,mode);
					x+=size/2; //字符,为全字的一半 
					}
				} 
				str++; 
		        
	        }
        }else//中文 
        {   
			if(x>(lcddev.width-size)||y>(lcddev.height-size)) 
			return;  
            bHz=0;//有汉字库    
			if(size==32)
			GUI_DrawFont32(x,y,fc,bc,str,mode);	 	
			else if(size==24)
			GUI_DrawFont24(x,y,fc,bc,str,mode);	
			else
			GUI_DrawFont16(x,y,fc,bc,str,mode);
				
	        str+=2; 
	        x+=size;//下一个汉字偏移	    
        }						 
    }   
}

//******************************************************************
//函数名：  Gui_StrCenter
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    居中显示一个字符串,包含中英文显示
//输入参数：x,y :起点坐标
// 			fc:前置画笔颜色
//			bc:背景颜色
//			str :字符串	 
//			size:字体大小
//			mode:模式	0,填充模式;1,叠加模式
//返回值：  无
//修改记录：无
//******************************************************************   
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{
	u16 len=mystrlen((const char *)str);
	u16 x1=(lcddev.width-len*8)/2;
	Show_Str(x+x1,y,fc,bc,str,size,mode);
} 
 
//******************************************************************
//函数名：  Gui_Drawbmp16
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    显示一副16位BMP图像
//输入参数：x,y :起点坐标
// 			*p :图像数组起始地址
//返回值：  无
//修改记录：无
//******************************************************************  
void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p) //显示40*40 QQ图片
{
  	int i; 
	unsigned char picH,picL; 
	LCD_SetWindows(x,y,x+100-1,y+40-1);//窗口设置
    for(i=0;i<100*40;i++)
	{	
	 	picL=*(p+i*2);	//数据低位在前
		picH=*(p+i*2+1);				
		LCD_DrawPoint_16Bit(picH<<8|picL);  						
	}	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//恢复显示窗口为全屏	

}

void DrawPageHead(u8 *str)
{
	//绘制固定栏up
	LCD_Fill(0,0,lcddev.width,20,BLUE);
	//绘制固定栏down
	LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
	//POINT_COLOR=WHITE;
	Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//居中显示
	Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"Staok恒压恒流源",16,1);//居中显示
	//绘制测试区域
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,BLACK);
}
