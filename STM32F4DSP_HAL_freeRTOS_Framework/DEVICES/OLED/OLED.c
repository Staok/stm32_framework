#include "OLED.h"
#include "oledfont.h"


/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	SimuI2C_Start(&SimuI2C_Handle);
	
	SimuI2C_WriteByte(&SimuI2C_Handle,0x78);
	SimuI2C_WaitAck(&SimuI2C_Handle);	
	
	SimuI2C_WriteByte(&SimuI2C_Handle,0x00);	//write command
	SimuI2C_WaitAck(&SimuI2C_Handle);
	
	SimuI2C_WriteByte(&SimuI2C_Handle,IIC_Command);
	SimuI2C_WaitAck(&SimuI2C_Handle);	
	SimuI2C_Stop(&SimuI2C_Handle);
}

/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	SimuI2C_Start(&SimuI2C_Handle);
	
	SimuI2C_WriteByte(&SimuI2C_Handle,0x78);
	SimuI2C_WaitAck(&SimuI2C_Handle);
	
	SimuI2C_WriteByte(&SimuI2C_Handle,0x40);	//write data
	SimuI2C_WaitAck(&SimuI2C_Handle);	
   
	SimuI2C_WriteByte(&SimuI2C_Handle,IIC_Data);
	SimuI2C_WaitAck(&SimuI2C_Handle);	
	
	SimuI2C_Stop(&SimuI2C_Handle);
}


void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		Write_IIC_Data(dat);
	}else {
		Write_IIC_Command(dat);
	}
}

//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}

void OLED_LightMode(void)  
{  
	u8 i,n;	
		
	for(i=0;i<8;i++)  
	{
		OLED_Set_Pos(0,i);
		for(n=0;n<128;n++)
			OLED_WR_Byte(0xff,OLED_DATA); 
	}
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/8
//OLED扫描方向为：从上到下，从左到右
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 mode)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//得到偏移后的值			
	if(x>Max_Column-1){x=0;y=y+2;}
	if(Char_Size ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		{	if(mode)
				 OLED_WR_Byte(F8X16[c][i],OLED_DATA);
			else OLED_WR_Byte(~F8X16[c][i],OLED_DATA);
		}
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		{	if(mode)
				 OLED_WR_Byte(F8X16[c][i+8],OLED_DATA);
			else OLED_WR_Byte(~F8X16[c][i+8],OLED_DATA);
		}
	}else if(Char_Size == 8){	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		{	
			if(mode)
				 OLED_WR_Byte(F6x8[c][i],OLED_DATA);
			else OLED_WR_Byte(~F6x8[c][i],OLED_DATA);
		}
	}
	 
}				  

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 mode)
{
    while((*chr<='~')&&(*chr>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(Char_Size/2))){x=0;y+=Char_Size;}
        if(y>(64-Char_Size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*chr,Char_Size,mode);	 
        x+=Char_Size/2;
        chr++;
    } 
}

//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
     }	
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
     }					
}

//功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 
/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}

//初始化SSD1306					    
void OLED_Init(void)
{ 	

//	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
//	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
//	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
//	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
//	
//	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
//	
//	OLED_WR_Byte(0x81,OLED_CMD); // contract control
//	OLED_WR_Byte(0x7F,OLED_CMD);//--128   
//	
//	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
//	OLED_WR_Byte(0xA6,OLED_CMD);
//	
//	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
//	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
//	
//	OLED_WR_Byte(0xC0,OLED_CMD);//Com scan direction
////	OLED_WR_Byte(0xC8,OLED_CMD);//
//	
//	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
//	OLED_WR_Byte(0x00,OLED_CMD);
//	
//	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
//	OLED_WR_Byte(0x80,OLED_CMD);
//	
//	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
//	OLED_WR_Byte(0x05,OLED_CMD);//
//	
//	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
//	OLED_WR_Byte(0xF1,OLED_CMD);//
//	
//	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
//	OLED_WR_Byte(0x12,OLED_CMD);//
//	
//	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
//	OLED_WR_Byte(0x30,OLED_CMD);//
//	
//	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
//	OLED_WR_Byte(0x14,OLED_CMD);//
//	
//	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(0x80,OLED_CMD); //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	
	//OLED_WR_Byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xC8,OLED_CMD); //在当前列，C8为从下到上扫描，C0为从上到下；取模时选“顺向”为从下到上
	
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0
	OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.										    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127; //左右扫描顺序
	
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置	 
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0x7F,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	
	HAL_Delay(5);
	OLED_Clear();
}  


