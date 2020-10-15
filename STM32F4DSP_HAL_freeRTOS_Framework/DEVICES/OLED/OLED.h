#ifndef __OLED_H
#define __OLED_H			  	 
#include "PeriphConfigCore.h" 



/*_________________以下都不用动_________________*/

#define Max_Column	128
#define Max_Row		64
		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_LightMode(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 mode);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);
#endif  


/*
API用例：
		OLED_Init();
		void OLED_Display_Off(void);
		void OLED_Display_On(void);
		void OLED_LightMode(void);
		OLED_Clear(); 


	TODO：显示中文有待修改更方便的格式
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
	TODO：实现反白显示		
		OLED_ShowString(0,6,"ASCII:",16);
		
		
		
		OLED_DrawBMP(0,0,128,8,BMP1);
		
		
*/
	 



