
    出处：公众号： 最后一个bug；文章：单片机常用程序框架之分时轮询(详注代码)
    思路与定时器分频一样，就是用起来和RTOS很像很方便
    以下修改修改命名规范，整理整理，修改的地方大一些，别一眼看出一样


TaskManage.h 文件
#ifndef __TASKMANAGE__
#define __TASKMANAGE__

/************************************************************
 * Fuction :数据类型定义区 
 * Author  :(公众号:最后一个bug) 
 ***********************************************************/
#define TRUE                      (1)
#define FALSE                     (0)

#ifndef NULL
#define NULL                      ((void*)(0))
#endif

typedef unsigned int   u32;             //检查一下我的模板里面有没有这个
typedef unsigned short u16;
typedef unsigned char  u8;
typedef signed long    s32;
typedef signed short   s16;
typedef signed char    s8;

typedef volatile unsigned int   vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned int  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */


/************************************************************
 * Fuction :任务类型定义区 
 * Author  :(公众号:最后一个bug) 
 ***********************************************************/
#define TASK_NUM_MAX 20

//运行模式
#define TASK_STOP (0) 
#define TASK_RUN  (1)

/************************************************************
 * Fuction :类型定义区 
 * Author  :(公众号:最后一个bug) 
 ***********************************************************/
#pragma pack(1)
typedef struct _tag_taskdata
{
    u8  statue;         //运行状态 
    u32 time;           //运行周期
    u32 count_time;     //运行计数变量 
    void (*fuc)(void);  //运行函数指针
} stTaskData;

typedef struct _tag_taskmanage
{
    stTaskData task[TASK_NUM_MAX]; //最大任务数管理 
    u8 registerTaskNum;            //已经注册的任务         
}stTaskManage;
#pragma pack()


/************************************************************
 * Fuction :功能接口区域 
 * Author  :(公众号:最后一个bug) 
 ***********************************************************/
extern void InitialTaskManager(void); 
extern u8  RegisterTask(u32 time, void * taskFuc); 
extern void Task_Process(void);
extern void Task_RunCheck(void);

#endif


TaskManage.c 文件内容
#include "TaskManage.h"

/************************************************************
 * descri  : 变量定义区 
 * Author  :(公众号:最后一个bug) 
 ***********************************************************/
stTaskManage sTaskManage; 

/************************************************************
 * Fuction : InitialTaskManager 
 * descri  : 初始化任务管理 
 * Author  :(公众号:最后一个bug) 
 ***********************************************************/
 void InitialTaskManager(void)
 {
    u8 i = 0;
    for(i = 0;i< TASK_NUM_MAX;i++) 
    {
        sTaskManage.task[i].statue     = TASK_STOP;   //运行标识
        sTaskManage.task[i].time       = 0;           //运行周期
        sTaskManage.task[i].count_time = 0;           //运行计数变量 
        sTaskManage.task[i].fuc        = NULL;        //运行函数指针
    }
    
    sTaskManage.registerTaskNum = 0;    //已经注册的任务计数清零 
 } 
 
/************************************************************
 * Fuction : RegisterTask
 * descri  : 注册任务 
 * Author  :(公众号:最后一个bug) 
 ***********************************************************/
 u8  RegisterTask(u32 time, void * taskFuc)
 {
    if(sTaskManage.registerTaskNum >= TASK_NUM_MAX)return FALSE;
    if(taskFuc == NULL)return FALSE;
    
    if(sTaskManage.task[sTaskManage.registerTaskNum].fuc == NULL) //找到没有使用的任务数据 
    {
        sTaskManage.task[sTaskManage.registerTaskNum].statue     = TASK_STOP;   //运行状态 
        sTaskManage.task[sTaskManage.registerTaskNum].time       = time;        //运行周期
        sTaskManage.task[sTaskManage.registerTaskNum].count_time = 0;           //运行计数变量 
        sTaskManage.task[sTaskManage.registerTaskNum].fuc        = taskFuc;     //运行函数指针
        
        sTaskManage.registerTaskNum++;//已经注册的任务计数 
        return TRUE;//注册成功 
    }

    return FALSE; //全部注册完毕 
     
 } 
 
/************************************************************
 * Fuction : Task_Process
 * descri  : 任务处理过程 
 * Author  :(公众号:最后一个bug) 
 ***********************************************************/
void Task_Process(void)
{
    u8 taskcount= 0;
    //遍历已经注册的任务 
    for(taskcount = 0; taskcount < sTaskManage.registerTaskNum;taskcount++) 
    {
        if(sTaskManage.task[taskcount].statue == TASK_RUN)//任务可以运行 
        {
        
            (*sTaskManage.task[taskcount].fuc)(); 
            sTaskManage.task[taskcount].statue = TASK_STOP;
        } 

    } 
    
}

/************************************************************
 * Fuction : Task_RunCheck
 * descri  : 任务运行条件核对 
 * Author  :(公众号:最后一个bug) 
 ***********************************************************/
void Task_RunCheck(void)
{
    u8 taskcount= 0;
    //遍历已经注册的任务 
    for(taskcount = 0; taskcount < sTaskManage.registerTaskNum;taskcount++) 
    {
        if((++sTaskManage.task[taskcount].count_time) >= sTaskManage.task[taskcount].time) //任务时间到来 
        {
            sTaskManage.task[taskcount].count_time = 0; 
            sTaskManage.task[taskcount].statue     = TASK_RUN; 
        
        } 

    } 
    
}



主函数 内容：
//使用例程

#include <stdio.h> 
#include "TaskManage.h"

/************************************************************
 * Fuction : Task1 
 * descri  : 任务1 
 * Author  :(公众号:最后一个bug) 
 ***********************************************************/
void Task1(void)
{ 
    printf("Run Task_1\n");
} 
/************************************************************
 * Fuction : Task2 
 * descri  : 任务2 
 * Author  :(公众号:最后一个bug) 
 ***********************************************************/
void Task2(void)
{ 
    printf("Run Task_2\n");
} 
/************************************************************
 * Fuction : Task3 
 * descri  : 任务3 
 * Author  :(公众号:最后一个bug) 
 ***********************************************************/
void Task3(void)
{ 
    printf("Run Task_3\n");
} 



int main(int argc, char *argv[]) 
{
    u16 SimuTime = 0; 
     
    InitialTaskManager();
    RegisterTask(10,Task1);
    RegisterTask(20,Task2);
    RegisterTask(50,Task3);
    
    while(1)
    {
        Task_Process();
        
        //模拟定时器中断中调用该函数 
        if((++SimuTime) <= 100)
        {
            Task_RunCheck();
            
        }
        else 
        {
            break; 
        } 
        
    }
    
    printf("最后一个bug");
    return 0;
    
}
