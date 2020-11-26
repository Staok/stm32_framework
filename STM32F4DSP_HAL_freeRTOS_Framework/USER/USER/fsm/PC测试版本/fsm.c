#include<stdio.h>
#include <conio.h>
#include<stdlib.h>
#include<time.h>

#define Max_StateJump_Num   3

/*定义名为 ‘XXX1’ 的状态机有哪些状态，各个状态的名字*/
enum fsm_XXX1_StateDef_enum
{
    XXX1_State_1 = 0,    /*首个必须是0，不要改其他数*/
    XXX1_State_2,
    XXX1_State_3,
    XXX1_State_4,
    XXX1_State_5,

    XXX1_State_MAX       /*这个固定在最后的不要动*/
};

/*定义各个状态要执行的函数*/
void state_1Fun(void){printf("state_1Fun\n");}
void state_2Fun(void){printf("state_2Fun\n");}
void state_3Fun(void){printf("state_3Fun\n");}
void state_4Fun(void){printf("state_4Fun\n");}
void state_5Fun(void){printf("state_5Fun\n");}



/*定义名为 ‘XXX2’ 的状态机有哪些状态，各个状态的名字*/
enum fsm_XXX2_StateDef_enum
{
    XXX2_State_1 = 0,    /*首个必须是0，不要改其他数*/
    XXX2_State_2,
    XXX2_State_3,
    XXX2_State_4,

    XXX2_State_MAX       /*这个固定在最后的不要动*/
};

/*定义各个状态要执行的函数*/
void state_11Fun(void){printf("state_11Fun\n");}
void state_22Fun(void){printf("state_22Fun\n");}
void state_33Fun(void){printf("state_33Fun\n");}
void state_44Fun(void){printf("state_44Fun\n");}





// struct fsm_states_struct                    //此状态机每一个状态
// {
//     unsigned int state;                     //此状态的状态索引
//     void (*stateActFun)();                  //此状态的执行函数

//     unsigned int jumpNum;                   //此状态的跳转条件数量
//     struct fsm_jumpTo_struct                //此状态的每个跳转条件的标志位和对应的下一个状态的索引
//     {
//         unsigned int eventFlag;
//         unsigned int nextState;
//     }jump[Max_StateJump_Num];  /*大小动态设置麻烦，浪费了点空间*/
// };
// struct fsm_struct
// {
//     unsigned int CurState;                      //此状态机当前所在状态
//     struct fsm_states_struct*   state;

// };


struct fsm_struct 
{
    unsigned int CurState;                      //此状态机当前所在状态
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
    }state[5];
};




/*状态图示例：（@ http://asciiflow.com/）
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
/*定义状态图，按顺序定义各个状态的执行函数、有几个跳转条件和各个条件对应跳转后的状态*/
// struct fsm_states_struct fsm_XXX1_states[(unsigned int)XXX1_State_MAX] = 
// {                                                 /*跳转条件都初始化为0*/
//     /*状态      执行函数         跳转条件数量       各个条件跳转后的状态（注：根据跳转条件的优先级从高到低往下写）*/
//     {(unsigned int)XXX1_State_1,   state_1Fun,     2,{  {0,(unsigned int)XXX1_State_5},
//                                                         {0,(unsigned int)XXX1_State_2},    }},

//     {(unsigned int)XXX1_State_2,   state_2Fun,     2,{  {0,(unsigned int)XXX1_State_4},
//                                                         {0,(unsigned int)XXX1_State_3},    }},

//     {(unsigned int)XXX1_State_3,   state_3Fun,     1,{  {0,(unsigned int)XXX1_State_1},    }},

//     {(unsigned int)XXX1_State_4,   state_4Fun,     1,{  {0,(unsigned int)XXX1_State_5},    }},

//     {(unsigned int)XXX1_State_5,   state_5Fun,     1,{  {0,(unsigned int)XXX1_State_3},    }},
// };

// struct fsm_struct fsm_XXX1          //定义名为‘XXX1’的状态机
// {
//     .CurState   =   (unsigned int)XXX1_State_1,
//     .state      =   &fsm_XXX1_states,
// };


struct fsm_struct fsm_XXX1 =           //定义名为‘XXX1’的状态机
{
    .CurState   =   (unsigned int)XXX1_State_1,
    .state      =   
    {                                                 /*跳转条件都初始化为0*/
        /*状态      执行函数         跳转条件数量       各个条件跳转后的状态（注：根据跳转条件的优先级从高到低往下写）*/
        {(unsigned int)XXX1_State_1,   state_1Fun,     2,{  {0,(unsigned int)XXX1_State_5},
                                                            {0,(unsigned int)XXX1_State_2},    }},

        {(unsigned int)XXX1_State_2,   state_2Fun,     2,{  {0,(unsigned int)XXX1_State_4},
                                                            {0,(unsigned int)XXX1_State_3},    }},

        {(unsigned int)XXX1_State_3,   state_3Fun,     1,{  {0,(unsigned int)XXX1_State_1},    }},

        {(unsigned int)XXX1_State_4,   state_4Fun,     1,{  {0,(unsigned int)XXX1_State_5},    }},

        {(unsigned int)XXX1_State_5,   state_5Fun,     1,{  {0,(unsigned int)XXX1_State_3},    }},
    }
};


/*      a                       b                   c
+--------------------+   +---------------+    +----------------+
+         d          v   |               v    |                v
1<------------------+2+--+               3+---+                4
^                    e                   +                     +
+-------------------------<--------------+                     |
^                                f                             |
+---------------------------------------<----------------------+    */
// struct fsm_states_struct fsm_XXX2_states[(unsigned int)XXX2_State_MAX] = 
// {                                                 /*跳转条件都初始化为0*/
//     /*状态      执行函数         跳转条件数量       各个条件跳转后的状态（注：根据跳转条件的优先级从高到低往下写）*/
//     {(unsigned int)XXX2_State_1,   state_11Fun,     1,{     {0,(unsigned int)XXX2_State_2},   }},

//     {(unsigned int)XXX2_State_2,   state_22Fun,     2,{     {0,(unsigned int)XXX2_State_3},
//                                                             {0,(unsigned int)XXX2_State_1},   }},

//     {(unsigned int)XXX2_State_3,   state_33Fun,     2,{     {0,(unsigned int)XXX2_State_4},
//                                                             {0,(unsigned int)XXX2_State_1},   }},

//     {(unsigned int)XXX2_State_4,   state_44Fun,     1,{     {0,(unsigned int)XXX2_State_1},   }},
// };

// struct fsm_struct fsm_XXX2          //定义名为‘XXX2’的状态机
// {
//     .CurState   =   (unsigned int)XXX2_State_1,
//     .state      =   &fsm_XXX2_states,
// };


struct fsm_struct fsm_XXX2 =           //定义名为‘XXX2’的状态机
{
    .CurState   =   (unsigned int)XXX2_State_1,
    .state      =   
    {                                                 /*跳转条件都初始化为0*/
        /*状态      执行函数         跳转条件数量       各个条件跳转后的状态（注：根据跳转条件的优先级从高到低往下写）*/
        {(unsigned int)XXX2_State_1,   state_11Fun,     1,{     {0,(unsigned int)XXX2_State_2},   }},

        {(unsigned int)XXX2_State_2,   state_22Fun,     2,{     {0,(unsigned int)XXX2_State_3},
                                                                {0,(unsigned int)XXX2_State_1},   }},

        {(unsigned int)XXX2_State_3,   state_33Fun,     2,{     {0,(unsigned int)XXX2_State_4},
                                                                {0,(unsigned int)XXX2_State_1},   }},

        {(unsigned int)XXX2_State_4,   state_44Fun,     1,{     {0,(unsigned int)XXX2_State_1},   }},
    }
};


/*说明，如果当前状态在 2 ，2状态对应的函数会循环执行，现在 e标志 发生了，但是状态并没有跳转，因为只有d或c发生才会跳转，
    后来如果 d标志 发生了，会跳到状态3，在执行一次3状态对应的函数后，就会跳转到状态1然后停止跳转，请注意。
    发生过的标志虽然不会立马响应，但是不会消失，会至少执行一次然后到下一个状态*/
/*如果不想让途径的状态执行其函数一次，下面的宏 fsm_jump_NOT_runOncePasser 设为 1 即可
    否则设为 0 是途径的每一个状态的函数都执行一次，即见者有份*/
#define fsm_jump_NOT_runOncePasser  1



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


int main()
{
    char message[2][10] = {"start!","end!"},scanf_buf;
    srand((int)time(0));
    printf("\n%s\n",message[0]);
    /*——————————————————————————————开始————————————————————————————*/
    
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
            default:break;
        }


    }






    /*——————————————————————————————结束————————————————————————————*/
	printf("\n%s\n",message[1]);
    printf("Press Enter to exit");
    scanf("%c",&scanf_buf);
    return 0;
}


