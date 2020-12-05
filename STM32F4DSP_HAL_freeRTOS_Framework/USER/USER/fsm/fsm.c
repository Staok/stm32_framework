#include "fsm.h"


/*STEP 1: 去.h文件定义有几个状态*/
/*STEP 2: 在下面定义每个状态要执行的函数，如果已经有则可以忽略此步*/
/*STEP 3: 在下下面的 struct fsm_states_struct 结构体内 定义状态图结构*/
/*STEP 4: 在不同的周期执行函数里或者大循环里调用 fsm_process(&fsm_XXX1) 处理和执行状态机*/


/*状态图示例：状态机——fsm_XXX1（@ http://asciiflow.com/）
+-------------------+
|                   |e
v  b        d       +
1+------.2+------.3
+         +         ^
|         |c        |g
|a        v  f      +
|         4+------.5
|                   ^
|                   |
+-------------------+   */
struct fsm_states_struct fsm_XXX1_state_define[5] = 	/*定义描述名为‘fsm_XXX1’的状态机的状态图*/
{                                                 	/*跳转条件都初始化为0*/
	/*状态      执行函数         跳转条件数量       各个条件跳转后的状态（注：根据跳转条件的优先级从高到低往下写）*/
	{(unsigned int)XXX1_State_1,   fsm_XXX1_state_1_Fun,	2,{  	{0,(unsigned int)XXX1_State_5	},
																	{0,(unsigned int)XXX1_State_2	},    }},

	{(unsigned int)XXX1_State_2,   fsm_XXX1_state_2_Fun,	2,{  	{0,(unsigned int)XXX1_State_4	},
																	{0,(unsigned int)XXX1_State_3	},    }},

	{(unsigned int)XXX1_State_3,   fsm_XXX1_state_3_Fun,	1,{  	{0,(unsigned int)XXX1_State_1	},    }},

	{(unsigned int)XXX1_State_4,   fsm_XXX1_state_4_Fun,	1,{  	{0,(unsigned int)XXX1_State_5	},    }},

	{(unsigned int)XXX1_State_5,   fsm_XXX1_state_5_Fun,	1,{  	{0,(unsigned int)XXX1_State_3	},    }},
};

struct fsm_struct fsm_XXX1 =							/*定义名为‘fsm_XXX1’的状态机*/
{
    .CurState   =   (unsigned int)XXX1_State_1,			/*定义初始状态*/
    .state      =   fsm_XXX1_state_define,				/*填入描述名为‘fsm_XXX1’的状态机的状态图的结构体*/
};

/*状态图示例：状态机——fsm_XXX2（@ http://asciiflow.com/）
		a                       b                   c
+--------------------+   +---------------+    +----------------+
+         d          v   |               v    |                v
1<------------------+2+--+               3+---+                4
^                    e                   +                     +
+-------------------------<--------------+                     |
^                                f                             |
+---------------------------------------<----------------------+    */
struct fsm_states_struct fsm_XXX2_state_define[4] = 	/*定义描述名为‘fsm_XXX2’的状态机的状态图*/
{                                                 	/*跳转条件都初始化为0*/
	/*状态      执行函数         跳转条件数量       各个条件跳转后的状态（注：根据跳转条件的优先级从高到低往下写）*/
	{(unsigned int)XXX2_State_1,   fsm_XXX2_state_1_Fun,	1,{     {0,(unsigned int)XXX2_State_2	},   }},

	{(unsigned int)XXX2_State_2,   fsm_XXX2_state_2_Fun,	2,{     {0,(unsigned int)XXX2_State_3	},
																	{0,(unsigned int)XXX2_State_1	},   }},

	{(unsigned int)XXX2_State_3,   fsm_XXX2_state_3_Fun,	2,{     {0,(unsigned int)XXX2_State_4	},
																	{0,(unsigned int)XXX2_State_1	},   }},

	{(unsigned int)XXX2_State_4,   fsm_XXX2_state_4_Fun,	1,{     {0,(unsigned int)XXX2_State_1	},   }},
};
struct fsm_struct fsm_XXX2 =           					/*定义名为‘fsm_XXX2’的状态机*/
{
    .CurState   =   (unsigned int)XXX2_State_1,			/*定义初始状态*/
    .state      =   fsm_XXX2_state_define,				/*填入描述名为‘fsm_XXX2’的状态机的状态图的结构体*/
};

/*________________以下固定的，不用动________________________*/


/*fsm有限状态机的处理函数
 *必须放到一个循环执行的区域，返回值为 *fsm 对应的处理后的当前状态
 *对于有周期执行需要的状态机任务，可以把不同 fsm_process(&fsm_xxx) 放到不同的周期函数里！
*/
unsigned int fsm_process(struct fsm_struct* fsm)
{
    unsigned int CurSta;
    int j;
    CurSta = (unsigned int)fsm->CurState;
    
    #if fsm_jump_NOT_runOncePasser

        char All_cleared = 0;
        while(!All_cleared)
        {
            for(j = 0; j < fsm->state[CurSta].jumpNum; j++)
            {
                /*遍历 fsm->CurState 状态的所有条件是否成立*/
                if(fsm->state[CurSta].jump[j].eventFlag)
                {
                    /*如果 fsm->CurState 状态的第j个条件成立，清标志，并把其下一个状态给当前状态*/
                    fsm->state[CurSta].jump[j].eventFlag = 0;
                    fsm->CurState = fsm->state[CurSta].jump[j].nextState;
                    CurSta = fsm->CurState;
                    break;
                }else
                {
                    if(j == (fsm->state[CurSta].jumpNum - 1)) All_cleared = 1;
                }
            }
        }
        
    #else
        for(j = 0; j < fsm->state[CurSta].jumpNum; j++)
        {
            /*遍历 fsm->CurState 状态的所有条件是否成立*/
            if(fsm->state[CurSta].jump[j].eventFlag)
            {
                /*如果 fsm->CurState 状态的第j个条件成立，清标志，并把其下一个状态给当前状态*/
                fsm->state[CurSta].jump[j].eventFlag = 0;
                fsm->CurState = fsm->state[CurSta].jump[j].nextState;
                CurSta = fsm->CurState;
                break;
            }
        }
    #endif

    (*(fsm->state[CurSta].stateActFun))();
    return CurSta;
}


