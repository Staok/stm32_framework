#ifndef __FSM_H
#define __FSM_H

#include "PeriphConfigCore.h"
#include "PeriphConfig.h"


#define Max_StateJump_Num   3

/*说明，如果当前状态在 2 ，2状态对应的函数会循环执行，现在 e标志 发生了，但是状态并没有跳转，因为只有d或c发生才会跳转，
    后来如果 d标志 发生了，会跳到状态3，在执行一次3状态对应的函数后，就会跳转到状态1然后停止跳转，请注意。
    发生过的标志虽然不会立马响应，但是不会消失，会至少执行一次然后到下一个状态*/
/*如果不想让途径的状态执行其函数一次，下面的宏 fsm_jump_NOT_runOncePasser 设为 1 即可
    否则设为 0 是途径的每一个状态的函数都执行一次，即见者有份*/
#define fsm_jump_NOT_runOncePasser  1


/*定义名为 ‘XXX1’ 的状态机有哪些状态，各个状态的名字*/
enum fsm_XXX1_StateDef_enum
{
    XXX1_State_1 = 0,    /*首个必须是0，不要改其他数*/
    XXX1_State_2,
    XXX1_State_3,
    XXX1_State_4,
    XXX1_State_5,

    XXX1_State_MAX       /*这个固定在最后的不要动，也表示此状态机包含的状态数量*/
};

/*定义 XXX2 状态机的各个状态要执行的函数*/
__weak void fsm_XXX1_state_1_Fun(void);
__weak void fsm_XXX1_state_2_Fun(void);
__weak void fsm_XXX1_state_3_Fun(void);
__weak void fsm_XXX1_state_4_Fun(void);
__weak void fsm_XXX1_state_5_Fun(void);



/*定义名为 ‘XXX2’ 的状态机有哪些状态，各个状态的名字*/
enum fsm_XXX2_StateDef_enum
{
    XXX2_State_1 = 0,    /*首个必须是0，不要改其他数*/
    XXX2_State_2,
    XXX2_State_3,
    XXX2_State_4,

    XXX2_State_MAX       /*这个固定在最后的不要动，也表示此状态机包含的状态数量*/
};

/*定义 XXX2 状态机的各个状态要执行的函数*/
__weak void fsm_XXX2_state_1_Fun(void);
__weak void fsm_XXX2_state_2_Fun(void);
__weak void fsm_XXX2_state_3_Fun(void);
__weak void fsm_XXX2_state_4_Fun(void);



/*________________以下固定的，不用动________________________*/

struct fsm_states_struct                    //此状态机每一个状态
{
	unsigned int state;                     //此状态的状态索引
	void (*stateActFun)();                  //此状态的执行函数

	unsigned int jumpNum;                   //此状态的跳转条件数量
	struct fsm_jumpTo_struct                //此状态的每个跳转条件的标志位和对应的下一个状态的索引
	{
		unsigned int eventFlag;
		unsigned int nextState;
	}jump[Max_StateJump_Num];               /*大小动态设置麻烦，浪费了点空间*/
};

struct fsm_struct
{
    unsigned int CurState;                  //此状态机当前所在状态
	const unsigned int StateNum;			//此状态机包含的状态数量
	struct fsm_states_struct* state;
};


/*fsm有限状态机的处理函数
 *必须放到一个循环执行的区域，返回值为 *fsm 对应的处理后的当前状态
 *对于有周期执行需要的状态机任务，可以把不同 fsm_process(&fsm_xxx) 放到不同的周期函数里！
*/
unsigned int fsm_process(struct fsm_struct* fsm);
void fsm_clear_all_jumpFlag(struct fsm_struct* fsm);

#endif




/*使用示例：
int main()
{
    char message[2][10] = {"start!","end!"},scanf_buf;
    srand((int)time(0));
    printf("\n%s\n",message[0]);
    //——————————————————————————————开始————————————————————————————
    
	
	
    char buf;
    while(buf != 'q')
    {
        buf = 0;
        unsigned int fsm_XXX1_CurSta,fsm_XXX2_CurSta;
        fsm_XXX1_CurSta = fsm_process(&fsm_XXX1);     //处理状态机 ‘fsm_XXX1’
        fsm_XXX2_CurSta = fsm_process(&fsm_XXX2);     //处理状态机 ‘fsm_XXX2’
        printf("fsm_XXX1 Current:%d\n",(unsigned int)fsm_XXX1_CurSta + 1);
        printf("fsm_XXX2 Current:%d\n",(unsigned int)fsm_XXX2_CurSta + 1);

        buf = getch();
        switch(buf)
        {
            case 'a':
                fsm_XXX1.state[(unsigned int)XXX1_State_1].jump[0].eventFlag = 1;
                fsm_XXX2.state[(unsigned int)XXX2_State_1].jump[0].eventFlag = 1;
                break;
            case 'b':
                fsm_XXX1.state[(unsigned int)XXX1_State_1].jump[1].eventFlag = 1;
                fsm_XXX2.state[(unsigned int)XXX2_State_2].jump[0].eventFlag = 1;
                break;
            
            case 'c':
                fsm_XXX1.state[(unsigned int)XXX1_State_2].jump[0].eventFlag = 1;
                fsm_XXX2.state[(unsigned int)XXX2_State_3].jump[0].eventFlag = 1;
                break;
            case 'd':
                fsm_XXX1.state[(unsigned int)XXX1_State_2].jump[1].eventFlag = 1;
                fsm_XXX2.state[(unsigned int)XXX2_State_2].jump[1].eventFlag = 1;
                break;

            case 'e':
                fsm_XXX1.state[(unsigned int)XXX1_State_3].jump[0].eventFlag = 1;
                fsm_XXX2.state[(unsigned int)XXX2_State_3].jump[1].eventFlag = 1;
                break;

            case 'f':
                fsm_XXX1.state[(unsigned int)XXX1_State_4].jump[0].eventFlag = 1;
                fsm_XXX2.state[(unsigned int)XXX2_State_4].jump[0].eventFlag = 1;
                break;

            case 'g':
                fsm_XXX1.state[(unsigned int)XXX1_State_5].jump[0].eventFlag = 1;
                break;

            case '0':
                fsm_clear_all_jumpFlag(&fsm_XXX1);
                fsm_clear_all_jumpFlag(&fsm_XXX2);
                break;
            default:break;
        }


    }






    //——————————————————————————————结束————————————————————————————
	printf("\n%s\n",message[1]);
    printf("Press Enter to exit");
    scanf("%c",&scanf_buf);
    return 0;
}
*/


