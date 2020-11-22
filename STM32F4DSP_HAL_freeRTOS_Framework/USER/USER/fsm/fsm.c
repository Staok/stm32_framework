#include "fsm.h"


/*STEP 1: 去.h文件定义有几个状态*/
/*STEP 2: 在下面定义每个状态要执行的函数，如果已经有则可以忽略此步*/
/*STEP 3: 在下下面的 fsm_states 结构体内 定义状态图结构*/

/*以下默认定义了一个状态图实例，为了防止忘记怎么定义，新开项目中最好注释保留之，复制下来改数即可*/

/*定义各个状态要执行的函数*/
void state_1Fun(void){printf("state_1Fun\r\n");}
void state_2Fun(void){printf("state_2Fun\r\n");}
void state_3Fun(void){printf("state_3Fun\r\n");}
void state_4Fun(void){printf("state_4Fun\r\n");}
void state_5Fun(void){printf("state_5Fun\r\n");}


/*状态图示例：（@ http://asciiflow.com/）	下面 fsm_states 定义的结构对应此图，
+-------------------+							这两个东西最好不要删，新开项目注释保留之
|                   |e
v  b        d       +
1+------->2+------->3
+         +         ^
|         |c        |g
|a        v  f      +
|         4+------->5
|                   ^
|                   |
+-------------------+   */
/*定义状态图，按顺序定义各个状态的执行函数、有几个跳转条件和各个条件对应跳转后的状态*/
struct fsm_states_struct fsm_states[(unsigned int)state_Max] = 
{                                                 /*跳转条件都初始化为0*/
    /*状态      执行函数         跳转条件数量       各个条件跳转后的状态（注：根据跳转条件的优先级从高到低往下写）*/
    {state_1,   state_1Fun,     2,{                 {0,state_5},
                                                    {0,state_2},    }},

    {state_2,   state_2Fun,     2,{                 {0,state_4},
                                                    {0,state_3},    }},

    {state_3,   state_3Fun,     1,{                 {0,state_1},    }},

    {state_4,   state_4Fun,     1,{                 {0,state_5},    }},

    {state_5,   state_5Fun,     1,{                 {0,state_3},    }},
};





