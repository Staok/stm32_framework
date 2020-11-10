/*
参考“傻孩子的菜单框架”

整体框架：
中断负责监视和获取外部输入的信息，当有外部输入激活中断，中断产生一个信号告诉“菜单函数”有输入进来，
并且告诉这是什么输入信号，接着，当“菜单函数”接到有输入进来的信号，就根据当前菜单指针的位置，
进行刷屏和执行功能，执行功能也都必须是设置标志位和改变参数，在其他任务或者函数里面具体处理这些变化，
菜单函数要写的简洁！
*/
#include "sys_menu.h"

struct input_struct inputKey = 
{
	.keyValue = none,
	.keyMode = notRdy
};

//基本思想就是，所有的系统功能均用一个变量来控制是否开启，即标志位控制
//对于裸机，在大循环中用控制变量（标志）决定某些功能是否执行，对于RTOS，每一个功能就是一个任务，同样用标志位判断是否执行，具体就变成了等待信号量
/*_______________________STEP0: 画出菜单结构________________________________________*/
/*
例子：（有符号*表示有下一级菜单；进入下一级和退回上一级用enter和exit键，同级间切换用上下键，调整参数用左右键）
最多三级，再多没意义，改多的话下面程序Locate改的也多
			运行*									调参*
			  |										  |
			模式1									模式1*
			模式2									  |-参数1（-+）
			灯开关（-+）	   					  	  |-参数2（-+）
			联网开关（-+）							模式2*
													  |-参数1（-+）
													  |-参数2（-+）
													选项*
													  |-是否开机开灯（-+）
													  |-是否开机联网（-+）
*/
/*___________________________STEP1: 结构体声明_____________________________________*/
//0级菜单-数量和结构体定义
#define m0_mainNum 2
struct MenuItem m0_main[m0_mainNum]; 
//1级菜单
#define m1_runNum 4
#define m1_adjustNum 3
struct MenuItem m1_run[m1_runNum]; 
struct MenuItem m1_adjust[m1_adjustNum]; 
//2级菜单
#define m2_mode1Num 2
#define m2_mode2Num 2
#define m2_optionNum 2
struct MenuItem m2_mode1[m2_mode1Num]; 
struct MenuItem m2_mode2[m2_mode2Num]; 
struct MenuItem m2_option[m2_optionNum];

/*___________________________STEP2: 菜单结构体实现_____________________________________*/
/*当前条目显示的信息的字符串可用中文，但长度尽量控制短*/
struct MenuItem m0_main[m0_mainNum]= 
{//  当前层条数     当前条目显示的信息  执行的函数的指针      指向子节点的指针  指向父节点的指针
	{m0_mainNum,		"run",				Nop,					m1_run,			NULL}, 
	{m0_mainNum,		"adjust",			Nop,					m1_adjust,		NULL}
};

struct MenuItem m1_run[m1_runNum]= 
{ 
	{m1_runNum,			"RunMode1",			runFunctions,			NULL,			m0_main}, 
	{m1_runNum,			"RunMode2",			runFunctions,			NULL,			m0_main}, 
	{m1_runNum,			"lightSwt",			runFunctions,			NULL,			m0_main}, 
	{m1_runNum,			"netSwt",			runFunctions,			NULL,			m0_main}
};
struct MenuItem m1_adjust[m1_adjustNum]=
{
	{m1_adjustNum,		"mode1",			Nop,					m2_mode1,		m0_main}, 
	{m1_adjustNum,		"mode2",			Nop,					m2_mode2,		m0_main}, 
	{m1_adjustNum,		"option",			Nop,					m2_option,		m0_main}	
};

struct MenuItem m2_mode1[m2_mode1Num]=
{
	{m2_mode1Num,		"para1",			adjustFunctionsMode1,	NULL,			m1_adjust}, 
	{m2_mode1Num,		"para2",			adjustFunctionsMode1,	NULL,			m1_adjust}	
};
struct MenuItem m2_mode2[m2_mode2Num]=
{
	{m2_mode2Num,		"para1",			adjustFunctionsMode2,	NULL,			m1_adjust}, 
	{m2_mode2Num,		"para2",			adjustFunctionsMode2,	NULL,			m1_adjust}	
};
struct MenuItem m2_option[m2_optionNum]=
{
	{m2_optionNum,		"isAutoLight",		adjustFunctionsOption,	NULL,			m1_adjust}, 
	{m2_optionNum,		"isAutoNet",		adjustFunctionsOption,	NULL,			m1_adjust}
};
/*______________________STEP3: 用户功能控制变量和函数实现，参数和函数定义还在sys_menu.h里面_________________________________*/
/*系统参数初始化*/
struct sysPara_struct sysPara = 
{
	.lightSwt = 0,
	.isAutoLight = 0,
	.netSwt = 0,
	.isAutoNet = 0
};
struct runMode1_struct RunMode1 = 
{
	.isRun = 0,
	.Para1 = 5,
	.Para2 = 3
};
struct runMode2_struct RunMode2 = 
{
	.isRun = 0,
	.Para3 = 5,
	.Para4 = 3
};
/*定义以上参数的左右极限，和长按步进*/
#define mode1Para1Min -10
#define mode1Para1Max 20
#define mode1Para1longMove 5

//用户功能-标准形式
//基本思想就是，所有的系统功能均用一个变量来控制是否开启，即标志位控制
//对于裸机，在大循环中用控制变量（标志）决定某些功能是否执行；对于RTOS，每一个功能就是一个任务，同样用标志位判断是否执行，具体就变成了等待信号量
void runFunctions(const struct MenuItem *MenuItemNow,const struct input_struct input)						//run下面的四个功能
{
	switch(input.keyValue)
	{
		case left:
		case right:	
		case enter:
						if(mystrcmp(MenuItemNow->DisplayString,"RunMode1") == 0)
						{
							RunMode1.isRun = !RunMode1.isRun;
							RunMode2.isRun = !RunMode1.isRun;//mode1与mode2的运行状态应该互斥
							
							TestLED_Ctrl = RunMode1.isRun; //点个灯，以示庆祝，不，以示RunMode1成功执行
							
						}else if(mystrcmp(MenuItemNow->DisplayString,"RunMode2") == 0)
						{
							RunMode2.isRun = !RunMode2.isRun;
							RunMode1.isRun = !RunMode2.isRun;//mode1与mode2的运行状态应该互斥
						}
						else if(mystrcmp(MenuItemNow->DisplayString,"lightSwt") == 0)	sysPara.lightSwt = !sysPara.lightSwt;
						else if(mystrcmp(MenuItemNow->DisplayString,"netSwt") == 0)		sysPara.netSwt = !sysPara.netSwt;
						
						break;
		default:break;
	}
}
//调节mode1下面的参数
void adjustFunctionsMode1(const struct MenuItem *MenuItemNow,const struct input_struct input)			//adjust的Mode1下面的2个功能
{
	//把调节参数的步长和极值都用预定义#define代替一下
	switch(input.keyValue)
	{
		case left:
				switch(input.keyMode)
				{
					case once:
							if(mystrcmp(MenuItemNow->DisplayString,"para1") == 0) 
							{
								RunMode1.Para1--;
								if(RunMode1.Para1 <= mode1Para1Min) RunMode1.Para1 = mode1Para1Min;
							}else if(mystrcmp(MenuItemNow->DisplayString,"para2") == 0) 
							{
								RunMode1.Para2--; 
								if(RunMode1.Para2 <= -10) RunMode1.Para2 = -10;
							}
							break;
					case lon:
							if(mystrcmp(MenuItemNow->DisplayString,"para1") == 0) 
							{
								RunMode1.Para1 -= mode1Para1longMove;
								if(RunMode1.Para1 <= mode1Para1Min) RunMode1.Para1 = mode1Para1Min;
							}
							else if(mystrcmp(MenuItemNow->DisplayString,"para2") == 0) 
							{
								RunMode1.Para2 -= mode1Para1longMove; 
								if(RunMode1.Para2 <= -10) RunMode1.Para2 = -10;
							}
							break;
							default:break;
				}
				break;
		case right:
				switch(input.keyMode)
				{
					case once:
							if(mystrcmp(MenuItemNow->DisplayString,"para1") == 0) 
							{
								RunMode1.Para1++;
								if(RunMode1.Para1 >= mode1Para1Max) RunMode1.Para1 = mode1Para1Max;
							}else if(mystrcmp(MenuItemNow->DisplayString,"para2") == 0) 
							{
								RunMode1.Para2++; 
								if(RunMode1.Para2 >= 10) RunMode1.Para2 = 10;
							}
							break;
					case lon:
							if(mystrcmp(MenuItemNow->DisplayString,"para1") == 0) 
							{
								RunMode1.Para1 += mode1Para1longMove;
								if(RunMode1.Para1 >= mode1Para1Max) RunMode1.Para1 = mode1Para1Max;
							}
							else if(mystrcmp(MenuItemNow->DisplayString,"para2") == 0) 
							{
								RunMode1.Para2 += mode1Para1longMove; 
								if(RunMode1.Para2 >= 10) RunMode1.Para2 = 10;
							}
							break;
							default:break;
				}
				break;
		default:break;
	}	
}
void adjustFunctionsMode2(const struct MenuItem *MenuItemNow,const struct input_struct input)			//adjust的Mode2下面的2个功能
{
	//同理
}
void adjustFunctionsOption(const struct MenuItem *MenuItemNow,const struct input_struct input)			//adjust的Option下面的2个功能
{
	//同理
}


/*_____________________以下皆为标准形式，固定的，不用动___________________________________*/

/*外部输入处理函数
为了能够识别按键是单击还是长按，应设置外部中断为“低电平触发”
*/
/*此函数必须一直循环执行，推荐100ms周期运行*/
void keyProcess(void)
{
	static int _100msCountDown = 5; /*0.5s内的双击有效，这里不要动*/
	static int _100msNum = 0,isKeySetOnce = FALSE; 
	char isOnce = 0; /*Magic,Don`t touch!*/
	
	//如果发生了外部中断
	if(key_Up_Interrupted == TRUE)
	{
		//再次判断
		if( key_Up_ReadIn == GPIO_PIN_RESET) //规范：低电平有效，此时按键被按下
		{
			inputKey.keyValue	=	enter;
			
			_100msNum++;_100msCountDown--;
			if(_100msNum > 30) _100msNum = 30;
			if(_100msCountDown < -10) _100msCountDown = -10;
			
			if((_100msCountDown > 0) && (isKeySetOnce == TRUE))//在0.5秒内第二次被按下，则切换按键模式为长按
			{
				isKeySetOnce = FALSE;
				isOnce = 0;
				inputKey.keyMode	=	doub;
				return ;
			}else
			{
				_100msCountDown = 5;
			}
			
			if(_100msNum > 10) //超过了1秒按键仍然时被按下状态，则把按键模式切换为长按，否则识别为单击
			{
				isOnce = 0;
				inputKey.keyMode	=	lon; 
				return ;
			}else
			{
				isOnce = 1;
				return ;
			}
		}else//如果按键被抬起，则清中断标志位等下一次中断到来
		{
			if(inputKey.keyMode	!=	doub)
				isKeySetOnce = TRUE;
			else isKeySetOnce = FALSE,_100msCountDown = 5;
			
			_100msNum = 0;
			key_Up_Interrupted = FALSE;  
			
			if(isOnce)		//对于单击，只有当按键松开，才释放
			{
				inputKey.keyMode	=	once;
				return ;
			}
			
			inputKey.keyValue	= 	none; 
			inputKey.keyMode	=	notRdy;
		}
		
	}else if(key_Down_Interrupted == TRUE)
	{
		if( key_Down_ReadIn == GPIO_PIN_RESET) //规范：低电平有效，此时按键被按下
		{
			inputKey.keyValue	=	down;
			
			_100msNum++;_100msCountDown--;
			if(_100msNum > 30) _100msNum = 30;
			if(_100msCountDown < -10) _100msCountDown = -10;
			
			if((_100msCountDown > 0) && (isKeySetOnce == TRUE))//在0.5秒内第二次被按下，则切换按键模式为长按
			{
				isKeySetOnce = FALSE;
				isOnce = 0;
				inputKey.keyMode	=	doub;
				return ;
			}else
			{
				_100msCountDown = 5;
			}
			
			if(_100msNum > 10) //超过了1秒按键仍然时被按下状态，则把按键模式切换为长按，否则识别为单击
			{
				isOnce = 0;
				inputKey.keyMode	=	lon; 
				return ;
			}else
			{
				isOnce = 1;
				return ;
			}
		}else//如果按键被抬起，则清中断标志位等下一次中断到来
		{
			if(inputKey.keyMode	!=	doub)
				isKeySetOnce = TRUE;
			else isKeySetOnce = FALSE,_100msCountDown = 5;
			
			_100msNum = 0;
			key_Up_Interrupted = FALSE;  
			
			if(isOnce)		//对于单击，只有当按键松开，才释放
			{
				inputKey.keyMode	=	once;
				return ;
			}
			
			inputKey.keyValue	= 	none; 
			inputKey.keyMode	=	notRdy;
		}
	}
}

/*												
索引格式例子：用于刷屏函数Locate()，写成自动获取菜单结构自适应显示，而不用当菜单结构改变时，索引显示函数也跟着变，不行
（下面三个例子，显示的都是整个的索引，要么都显示，要么都隐藏）
（把当前菜单所在级的所有菜单条目列出来；当前选中菜单条目如果有子级，后面显示一个->，如果没有子级，就是要调东西或者执行东西，就显示-	N	+或者-	go	+）
（对于-	stop	+的，就是执行东西的，当选择执行后，可以隐藏整个索引，直到退出来，或者通过某个按键，弹出一个对话框显示当前索引）
例子1：
索引：/调参/选项->
		*	是否开机开灯	-	N	+
			是否开机联网	-	N	+

例子2：		
索引：/调参->
			模式1->
			模式2->
		*	选项->

例子3：
索引：/运行->
			模式1			-	stop	+
		*	模式2			-	go		+
			灯开关			-	Y		+
			联网开关		-	Y		+
		
*/
//显示索引函数，刷新屏幕和串口，用以显示当前菜单所在的位置
/*先这样，不用写像上面那样高级*/
/*不依赖用户定义的结构，可以简单自适应显示*/
void Locate(const struct MenuItem *MenuItemNow,const char * const menuid,const int deepth) 
{
	/*MenuItemNow是当前菜单的顶地址，(MenuItemNow + menuid[menu_deepth]) 才是当前菜单条目*/
	char buf[50];
	int menu_deepth = deepth; //记录当前在第几层
	int i; //万年不变的i
	POINT_COLOR = RED;
	sprintf_(buf,"Menu View : %d %d %d",menuid[0],menuid[1],menuid[2]);
	LCD_ShowString(50,20,16,(u8*)buf);
	for(i = 0;i < 5;i++)
	{
		if(i < MenuItemNow->MenuCount)
		{
			//遍历当前菜单页中的菜单项
			if((MenuItemNow + menuid[menu_deepth])->Childrenms !=NULL)	//如果本菜单条目下没有子菜单
			{
				if(menuid[menu_deepth] == i)
					 sprintf_(buf,"%-15s * -->",((MenuItemNow + i)->DisplayString));	
				else sprintf_(buf,"%-15s   -->",((MenuItemNow + i)->DisplayString));	//左对齐
			}else
			{
				if(menuid[menu_deepth] == i)
					 sprintf_(buf,"%-15s * Ctl",((MenuItemNow + i)->DisplayString));	
				else sprintf_(buf,"%-15s   Ctl",((MenuItemNow + i)->DisplayString));
			}
			
			LCD_ShowString(10,40 + i*20,16,(u8*)buf);						//显示当前菜单页中每一条菜单项
		}else
		{
			sprintf_(buf,"%-15s      "," ");	
			LCD_ShowString(10,40 + i*20,16,(u8*)buf);	//清空多余区域
		}

	}
	
	
	//这里需要调用屏幕的写字符串，还需再完善怎么现实菜单结构，现实的格式按照"索引格式例子："里面的形式！
//	//menuPointer+menuid[i]
//	char *index_strbuf;		//保存索引的格式显示
//	char *item_strbuf;		//保存罗列当前目录下所有项的格式显示
//	char *tem_strbuf;
//	//printf("Now we at %s\n" ,(MenuItemNow+menuid)->DisplayString); //显示当前的某一个菜单项
//	
//	if(MenuItemNow->Parentms !=NULL)
//	{
//		if(MenuItemNow->Parentms->Parentms !=NULL)
//		{
//					sprintf_(index_strbuf,"/->%s->%s->\n",(MenuItemNow->Parentms->Parentms + menuid[0])->DisplayString,
//																		(MenuItemNow->Parentms + menuid[1])->DisplayString);
//		}else {	sprintf_(index_strbuf,"/->%s->\n",		 MenuItemNow->Parentms + menuid[0])->DisplayString);}
//	}else{		mystrcpy(index_strbuf, "/->\n");}
//	
//	for(i = 0;i < MenuItemNow.MenuCount;i++)
//	{
//		sprintf_(tem_strbuf,"%s\n",MenuItemNow[i]->DisplayString);
//		mystrcat(item_strbuf,tem_strbuf);
//	}
//	mystrcat(index_strbuf,item_strbuf);
//	return index_strbuf;
	
}


//执行用户定义的功能函数，传入当前菜单项的结构体指针和当前的按键值信息，用于判断应该改变哪个具体的值以及怎么变
void Run(const struct MenuItem *MenuItemNow,const struct input_struct input)
{ 
	(*(MenuItemNow->Subs))(MenuItemNow,input); //传入参数应和DoSomething函数的一致
}
void Nop(void) 
{}

/*______________________菜单处理函数_________________________________*/
/*______________________负责：执行用户函数和执行刷屏函数_________________*/
/*______________________位置：裸机：主循环，定时器中断给信号；RTOS：一个任务*/
/*固定的，几乎不用动*/
/*刷屏函数Locate是直接调用，刷屏需要的时间可能比较长，这里按需修改成标志位控制*/
/*用户功能函数的标准形式都是只改变数值，不加入复杂的具体功能，所以这个菜单处理函数不会占用太多时间，
只要保证用户功能函数按照标准去写*/
/*此函数必须一直循环执行，推荐100ms周期运行*/
void menuProcess(void)
{
	
	static char menuid[3]={0,0,0}; //记录某一级的菜单当前在第几个项目上 
	static char i=0; //上面数组的下标
	static struct MenuItem *menuPointer = &m0_main[0]; //菜单的漫游指针，并指定开始的位置
	
	if((inputKey.keyValue != none)&&(inputKey.keyMode != notRdy))
	{
		switch(inputKey.keyValue)
		{
			case up:
										//在当前菜单级别跳到上一个菜单项
										if (menuid[i]==0) menuid[i] = (menuPointer->MenuCount-1);
										else menuid[i]--;
										
										//Locate(menuPointer,menuid);
										break;
			
			case down:
										//在当前菜单级别跳到下一个菜单项
										menuid[i]++;
										if (menuid[i] > (menuPointer->MenuCount-1)) menuid[i]=0;
										
										//Locate(menuPointer,menuid);
										break;
			
			case left:
										Run(menuPointer+menuid[i],inputKey);
										break;
			
			case right:
											//到下一级菜单,无下级时执行某功能
											if ((menuPointer+menuid[i])->Childrenms !=NULL) 
											{ 
												menuPointer = (menuPointer+menuid[i])->Childrenms; 
												i++; 
												menuid[i]=0; 
												
												//Locate(menuPointer,menuid);
											} 
											else 
											{ 
												Run(menuPointer+menuid[i],inputKey);
											}
											break;
			
			case back:
										//到上一级菜单，没有上一级时不用动
										if ((menuPointer+menuid[i])->Parentms !=NULL) 
										{ 
											menuPointer = (menuPointer+menuid[i])->Parentms; 
											i--; 
											
											//Locate(menuPointer,menuid);
										} 
										else 
										{ 
											//printf("You are at the top of menu"); 
										}
										break;
			
			case enter:
										//到下一级菜单,无下级时执行某功能
										if ((menuPointer+menuid[i])->Childrenms !=NULL) 
										{
											menuPointer = (menuPointer+menuid[i])->Childrenms; 
											i++; 
											menuid[i]=0; 
											
											//Locate(menuPointer,menuid);
										} 
										else 
										{ 
											Run(menuPointer+menuid[i],inputKey);
										}				
										break;
			
			default:break;
		}
		inputKey.keyValue = none;
		inputKey.keyMode = notRdy;
	}
	
	Locate(menuPointer,menuid,i);	//刷新显示 注意：menuPointer + menuid[i]才是当前菜单条目
	
}


