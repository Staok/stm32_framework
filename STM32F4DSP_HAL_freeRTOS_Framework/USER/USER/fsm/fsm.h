#ifndef __FSM_H
#define __FSM_H

/*定义状态机有哪些状态，各个状态的名字*/
enum fsm_state_list
{
    state_1 = 0,    /*首个必须是0，不要改其他数*/
    state_2,
    state_3,
    state_4,
    state_5,

    state_Max       /*这个固定在最后的不要动*/
};

/*说明，如果当前状态在 2 ，2状态对应的函数会循环执行，现在 e标志 发生了，但是状态并没有跳转，因为只有d或c发生才会跳转，
    后来如果 d标志 发生了，会跳到状态3，在执行一次3状态对应的函数后，就会跳转到状态1然后停止跳转，请注意。
    发生过的标志虽然不会立马响应，但是不会消失，会至少执行一次然后到下一个状态*/
/*如果不想让途径的状态执行其函数一次，下面的宏 fsm_jump_NOT_runOncePasser 设为 1 即可
    否则设为 0 是途径的每一个状态的函数都执行一次，即见者有份*/
#define fsm_jump_NOT_runOncePasser  1

/*设置启动时的状态*/
enum fsm_state_list CurrentState = state_1;


/*下面都是固定的，不要动*/

struct fsm_states_struct
{
    enum fsm_state_list state;
    void (*stateActFun)();

    unsigned int jumpConditionNum;
    struct fsm_jumpTo_struct
    {
        unsigned int eventFlag;
        enum fsm_state_list nextState;
    }jumpCondition[state_Max - 1];  /*大小动态设置麻烦，浪费了点空间*/
};

/*根据标志位更新当前状态，必须放到一个大循环里*/
void fsm_state_process(void)
{
    #if fsm_jump_NOT_runOncePasser
        int j;
        char All_cleared = 0;
        while(!All_cleared)
        {
            for(j = 0;j < fsm_states[(unsigned int)CurrentState].jumpConditionNum;j++)
            {
                /*遍历CurrentState状态的所有条件是否成立*/
                if(fsm_states[(unsigned int)CurrentState].jumpCondition[j].eventFlag)
                {
                    /*如果CurrentState状态的第j个条件成立，清标志，并把其下一个状态给当前状态*/
                    fsm_states[(unsigned int)CurrentState].jumpCondition[j].eventFlag = 0;
                    CurrentState = fsm_states[(unsigned int)CurrentState].jumpCondition[j].nextState;
                    break;
                }else
                {
                    if(j == (fsm_states[(unsigned int)CurrentState].jumpConditionNum - 1)) All_cleared = 1;
                }
                
                
            }
        }
    #else
        int j;
        for(j = 0;j < fsm_states[(unsigned int)CurrentState].jumpConditionNum;j++)
        {
            /*遍历CurrentState状态的所有条件是否成立*/
            if(fsm_states[(unsigned int)CurrentState].jumpCondition[j].eventFlag)
            {
                /*如果CurrentState状态的第j个条件成立，清标志，并把其下一个状态给当前状态*/
                fsm_states[(unsigned int)CurrentState].jumpCondition[j].eventFlag = 0;
                CurrentState = fsm_states[(unsigned int)CurrentState].jumpCondition[j].nextState;
                return ;
            }
        }
    #endif
}

/*根据当前状态执行相应函数，必须放到一个大循环里*/
void fsm_stateFun_process(void)
{
    (*(fsm_states[(unsigned int)CurrentState].stateActFun))();
}

#endif




/*使用示例：
    while(...)	//某一个大循环
    {
        char buf = 0;
        fsm_state_process();	//FSM 有限状态机，处理函数	
		fsm_stateFun_process();
		
		//打印当前所处的状态
        printf("CurrentState:%d\n",(unsigned int)CurrentState + 1);
        
		//根据相应的事件，标记相应状态的标志位
        buf = getch();
        switch(buf)
        {
            case 'a':
                fsm_states[(unsigned int)state_1].jumpCondition[0].eventFlag = 1;
                break;
            case 'b':
                fsm_states[(unsigned int)state_1].jumpCondition[1].eventFlag = 1;
                break;
            
            case 'c':
                fsm_states[(unsigned int)state_2].jumpCondition[0].eventFlag = 1;
                break;
            case 'd':
                fsm_states[(unsigned int)state_2].jumpCondition[1].eventFlag = 1;
                break;

            case 'e':
                fsm_states[(unsigned int)state_3].jumpCondition[0].eventFlag = 1;
                break;

            case 'f':
                fsm_states[(unsigned int)state_4].jumpCondition[0].eventFlag = 1;
                break;

            case 'g':
                fsm_states[(unsigned int)state_5].jumpCondition[0].eventFlag = 1;
                break;
            default:break;
        }


    }



*/


