#ifndef __SYS_MENU_H
#define __SYS_MENU_H

#include "sys.h"
#include "printf.h"
#include "MyString.h"

/*______________________按键输入信息，基本不动_________________________________*/
//输入可以为独立按键、五项按键、AD Key、触摸按键、无线遥控、旋转编码器等
//枚举，全局，常量，不能取地址
enum inputValue_enum{
	none = 0,
	up,			//对于旋转编码器输入，只用"上、下"标志位
	down,
	left,
	right,
	exit,
	enter
};
enum inputMode_enum{
	once = 1,	//单击
	doub,		//双击
	lon			//长摁
};
//输入信息结构体
struct input_struct
{
	enum inputValue_enum keyValue;
	enum inputMode_enum keyMode;
	
};

extern struct input_struct inputKey;


/*______________________菜单结构，不用动_________________________________*/
struct MenuItem     
{  
	char MenuCount; 					//当前菜单层条目个数
	char *DisplayString;  				//当前菜单条目显示的信息
	void (*Subs)();  					//执行的函数的指针
	struct MenuItem *Childrenms;  		//指向子节点的指针
	struct MenuItem *Parentms;  		//指向父节点的指针
}; 
void keyProcess(void);
void menuProcess(void);
char *Locate(struct MenuItem *MenuItemNow,char menuid[3]);
void Run(struct MenuItem *MenuItemNow,	struct input_struct input);
void Nop(void);


/*______________________定义系统信息，用户修改_________________________________*/

//声明系统所有外设开关状态变量的控制集总
struct sysPara_struct
{
	char lightSwt;
	char isAutoLight;
	char netSwt;
	char isAutoNet;
};
extern struct sysPara_struct sysPara;

//声明关于mode1和mode2的系统参数
//可以用作不同页面、不同运行模式的区分
struct runMode1_struct
{
	char isRun;//控制是否运行mode1
	int Para1;//mode1中的参数1
	int Para2;
};
extern struct runMode1_struct RunMode1;

struct runMode2_struct
{
	char isRun;//控制是否运行mode2
	int Para3;//mode2中的参数1
	int Para4;
};
extern struct runMode2_struct RunMode2;


//用户函数声明
void runFunctions(struct MenuItem *MenuItemNow,	struct input_struct input);//run下面的四个功能
void adjustFunctionsMode1(struct MenuItem *MenuItemNow,	struct input_struct input);//adjust的Mode1下面的2个功能
void adjustFunctionsMode2(struct MenuItem *MenuItemNow,	struct input_struct input);//adjust的Mode2下面的2个功能
void adjustFunctionsOption(struct MenuItem *MenuItemNow,struct input_struct input);//adjust的Option下面的2个功能

#endif


