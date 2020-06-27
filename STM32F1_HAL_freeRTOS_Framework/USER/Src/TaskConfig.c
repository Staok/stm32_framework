#include "TaskConfig.h"
#include "sys.h"

/*__________创建任务函数，封装_____________*/
BaseType_t xTaskCreate_t(uint8_t taskIndex)
{
	return (xTaskCreate( 	Define_Task[taskIndex].Task_Function,
                            Define_Task[taskIndex].Task_Name,
                            Define_Task[taskIndex].Task_STK_Size,
                            Define_Task[taskIndex].Task_Para,
                            Define_Task[taskIndex].Task_Prio,
                            &Define_Task[taskIndex].Task_Handle));//任务句柄，创建任务这里取地址
}

/*********************************************************************************************/
/*                      以下为要修改的部分                                                   */
/********************************************************************************************/

//总任务数量
#define task_Num	3 
//任务索引：按照序号给每个任务编号向下写
#define start_t 	0
#define led0_t 		1
#define led1_t 		2

//设置任务要素
struct TaskStructure Define_Task[task_Num] = {
	/*任务函数      任务名称      分配堆栈    传入参数   任务优先级*/
    {start_task,    "start_task",   128,        NULL,       2,},
    {led0_task,     "led0_task",    50,         NULL,       2,},
    {led1_task,     "led1_task",    50,         NULL,       3,},
};  
/*注；堆栈实际占用字节数为定义数的四倍，即定义数的单位为字*/

//struct TaskStructure Define_Task0 = {
//    .Task_Function = start_task,
//    .Task_Name = "ss",
//};

/*__________创建开始任务，这部分固定的不用动_____________*/
void Task_Begin(void)
{
	BaseType_t is_error;
	//创建开始任务	
	is_error = xTaskCreate_t(start_t);
	/*上面函数为对下面一条语句的封装，为了简化*/
//    is_error = xTaskCreate( Define_Task[start_t].Task_Function,
//                            Define_Task[start_t].Task_Name,
//                            Define_Task[start_t].Task_STK_Size,
//                            Define_Task[start_t].Task_Para,
//                            Define_Task[start_t].Task_Prio,
//                            &Define_Task[start_t].Task_Handle);
	if(is_error == pdPASS)
    {
        vTaskStartScheduler();          //开启任务调度
    }else{
		FaultASSERT(0,"AT : Creating start_t failed",((uint8_t*)is_error));
        //for(;;){;}
    }
}

/*__________开始初始化任务函数_____________*/
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	
	//进行队列、信号量等相关的初始化
    
	
	//进行任务相关的初始化
	xTaskCreate_t(led0_t);
    xTaskCreate_t(led1_t);
    
    vTaskDelete(Define_Task[start_t].Task_Handle); 
    //删除开始任务，内存也被释放掉，删除任务这里放任务句柄，不取地址
	
    taskEXIT_CRITICAL();            //退出临界区
}

/*__________用户应用任务_____________*/
void led0_task(void *pvParameters)
{
	/*本任务所用的变量的定义，第一次运行需要设置的内容，或者本任务只需要运行一次的内容*/
	float staok = 666.0;
	//LED0 = ON;
	
	
	//进入本任务的主循环
    for(;;)
    {
        staok += 0.1;
        vTaskDelay(500);
    }
	
	/*不能从任务函数中退出，除非提前删除任务，否则会调用configASSERT()引起错误*/
}   


void led1_task(void *pvParameters)
{
    for(;;)
    {
        ////LED0=~LED0;
        vTaskDelay(500);
    }
}

/*
_____________大部分API罗列_____________


	FreeRTOS提供的延时函数：
		vTaskDelay(ms); 不包括函数运行时间，比如，一个函数A执行时间为2ms，使用vTaskDelay(10)，则再次执行这个函数最少需要等待12ms。
		vTaskDelayUntil(); 使用vTaskDelayUntil(10)，则这2ms包括在内，A以固定10ms周期运行，相当于固定心跳周期，按照一定周期执行。
			具体使用：
			TickType_t PreviousWakeTime; 
			const TickType_t Timelncrement = pdMS_TO_TICKS(10); //把ms数转为节拍数，在1kHz调度频率情况下，或许可以不用，有待实验
			PreviousWakeTime = xTaskGetTickCount();
			//以上三句在任务中只运行一次即可
			VTaskDelayUntil(&PreviousWakeTime,Timelncrement)；
	

	任务挂起：数据不丢失，传入参数：要挂起任务的任务句柄
			void vTaskSuspend(TaskHandle_t xTaskToSuspend);
	任务恢复：传入参数：要恢复任务的任务句柄
			void vTaskResume(TaskHandle_t xTaskToResume);
			中断版本：返回值：pdPASS成功，pdFAIL失败
				BaseType t xTaskResumeFromISR(TaskHandle_t xTaskToResume);
				用法：在中断中
					BaseType_t YieldRequired;
					YieldRequired = xTaskResumeFromISR(Task2Task_Handler);
					//如果函数xTaskResumeFromISRO返回值为pdTRUE，那么说明要恢复的这个任务的任务优先级等于或者高于正在运行的任务（被中断打断的任务），所以在退出中断的时候一定要进行上下文切换！
					if(YieldRequired == pdTRUE) portYIELD_FROM_ISR(YieldRequired);
	
	任务相关API：（以下函数的宏定义均已启用）
			uxTaskPriorityGet()		查询某个任务的优先级。 UBaseType_t uxTaskPriorityGet( const TaskHandle_t xTask );UBaseType_t uxTaskPriorityGetFromISR( const TaskHandle_t xTask )
			vTaskPrioritySet()		改变某个任务的任务优先级。 void vTaskPrioritySet( TaskHandle_t xTask, UBaseType_t uxNewPriority )
		获知：uxTaskGetSystemState()	获取系统中任务状态。 UBaseType_t uxTaskGetSystemState( TaskStatus_t * const pxTaskStatusArray, const UBaseType_t uxArraySize, uint32_t * const pulTotalRunTime )
		获知：vTaskGetInfo()			获取某个任务信息。 void vTaskGetInfo( TaskHandle_t xTask, TaskStatus_t *pxTaskStatus, BaseType_t xGetFreeStackSpace, eTaskState eState )
			xTaskGetCurrentTaskHandle()获取当前正在运行的任务的任务句柄。 TaskHandle_t xTaskGetCurrentTaskHandle( void )
			xTaskGetTickCount()		获取系统时间计数器值。 TickType_t xTaskGetTickCount( void )
			xTaskGetTickCountFromISR()在中断服务函数中获取时间计数器值
			uxTaskGetNumberOfTasks()获取当前系统中存在的任务数量 UBaseType_t uxTaskGetNumberOfTasks( void )
		
		调试：vTaskList() 以一种表格的形式输出当前系统中所有任务的详细信息 void vTaskList( char * pcWriteBuffer )
			调试很有用，会返回所有任务的名字、状态、优先级、任务堆栈历史剩余最少值和任务编号
		调试：vTaskGetRunTimeStats()	获取每个任务的运行时间 void vTaskGetRunTimeStats( char *pcWriteBuffer )
			调试很有用，表格形式统计每个任务的运行总时间和占用CPU的百分比
			
		->要使用以上“获知”和“调试”四个函数的详细用法，看原子F1的FreeRTOS手册172页，还没有完全配置好，不要私自乱用哦<-
		
		关于taskENTER_CRITICAL()和taskEXIT_CRITICAL()用于临界区代码逻辑时序的保护，看本文件对应的.h文件下面注释
		关于低功耗模式、空闲任务钩子函数，看本文件对应的.h文件下面注释
		关于FreeRTOS的内存管理，看本文件对应的.h文件下面注释
			FreeRTOS提供了一套内存分配与释放的方法：
			分配：void *pvPortMalloc(size_t xWantedSize)
			释放：void vPortFree(void *pv)

		
		关于FreeRTOS的信号量和事件标志组，均用任务通知替代！
		关于FreeRTOS的任务与任务、任务与中断间的通讯只用消息队列，功能最全！
		
		消息队列：
			规范：1、FIFO先进先出储存缓存机制，永远队列尾存储据，队列头取数据
				  2、小数据用拷贝方式（实参还可变），大数据用传递指针方式（实参与队列中数据同指针，不可随意变）
				  3、队列为空时读取，有出队阻塞，可选0（不等），x（等x个节拍），portMAX_DELAY（死等），队列为满时写入则为入队阻塞。
			创建：	QueueHandle_t qMsg = xQueueCreate(length,sizeof( uint32_t )); //参数：队列项目；每一项的长度，即项的数据类型
			发送：	xQueueSend(qMsg,&Value,233); 				//参数：队列句柄；传入变量（拷贝）；超时时间（入队阻塞时间），
																返回值：pdPASS成功；errQUEUE_FULL队列满，发送失败
					xQueueSendFromISR(qMsg,&Value,BaseType_t Y) //中断版，参数：前两个一样，最后一个用于判断是否进行任务切换，中断函数没有超时时间，返回值同上
						用法：BaseType_t YieldRequired;
							  u8 err;
							err = xQueueSendFromISR(qMsg,&Value,&YieldRequired);
							if(err = pdPASS){;}//成功发送或者成功接收
							if(YieldRequired == pdTRUE) portYIELD_FROM_ISR(YieldRequired);
					xQueueOverwrite(qMsg,&Value); 				//覆盖写，仅没有超时时间，其他一样，返回值只有成功
					xQueueOverwriteFromISR 						//中断版，覆盖写，参数同xQueueSendFromISR()
			上锁：prvLockQueue(pxQueue); 						//参数：要上锁的队列句柄
			解锁：prvUnlockQueue(&pxQueue);						//参数：要解锁的队列句柄指针
			读取：	xQueueReceive(qMsg, &value, 233) 			//读完删除，拷贝方式；参数：队列句柄；数据缓存区；超时时间（出队阻塞时间），返回值同xQueueSend()
					xQueueReceiveFromISR(qMsg,&Value,BaseType_t Y) //中断版，读完删除，拷贝方式；参数和返回值同xQueueSendFromISR();
					xQueuePeek(qMsg, &value, 233);   			//读完不删；参数和返回值：同xQueueReceive()
					xQueuePeekFromISR(qMsg,&Value);				//中断版，读完不删，拷贝方式；参数：队列句柄；数据缓存区，返回值同xQueueSend()
			剩余： u8 q_remainSize = uxQueueSpacesAvailable(qMsg);
			未读： u8 q_unreadSize =  uxQueueMessagesWaiting(qMsg); //总大小 length 等于 uxQueueMessagesWaiting(qMsg) + uxQueueSpacesAvailable(qMsg)
			
			
		任务通知：
			规范：可模拟二值、计数型信号量和事件标志组；有接收阻塞，没有发送失败的阻塞；不用初始化。
			发送：	xTaskNotify(接收任务的任务句柄，uint32_t类型的通知值，通知方法)			//带通知值，不保留接收任务的原通过值
						通知方法可选：	eSetBits, 					更新指定bit，模拟事件标志组，此时通知值参数就是要set的bit，函数内部作按位与运算
										eIncrement,					通知值加一，模拟计数型信号量
										eSetValueWithOverwrite,		覆盖写入通知值
										eSetValueWithoutOverwrite	不覆盖写（如果接收任务未读，则写入失败，但不会阻塞）
						返回值：当通知方法选为eSetValueWithoutOverwrite时，如果接收任务未读，则返回pdFAIL，否则返回pdPASS
								选择其他通知方法均为pdPASS
					xTaskNotifyFromISR() //前三个参数同上，第四个为BaseType_t Y，用于判断是否调度一次，用法：同xQueueSendFromISR()
					
					xTaskNotifyGive(接收任务的任务句柄)		//不带通知值，不保留接收任务的原通过值
						通知方法：只把被通知任务的通知值加一，模拟计数型信号量
						返回值：pdPASS
					xTaskNotifyGiveFromISR() //第一个参数同上，第二个为BaseType_t Y，用法同xTaskNotifyFromISR()
					
					xTaskNotifyAndQuery(任务句柄，通知值，通知方法，保存上一次通知值的地址)	//带通知值，保留接收任务的原通过值
						通知方法和返回值同xTaskNotify()，不常用
					xTaskNotifyAndQueryFromISR() //多一个BaseType_t Y，用于判断是否调度一次，用法同xTaskNotifyFromISR()
			
			获取：	ulTaskNotifyTake(mode,waitTime) 
					//直接获取；参数：mode为pdFALSE时通知值减一，为pdTRUE时清零；阻塞时间；返回值：减少或清零之前的值
					//建议超时时间设为0
					xTaskNotifyWait(bitsToClearOnEnrty,bitsToClearOnExit,保存通知值的指针，阻塞时间)
					//功能更多的获取；参数：
							bitsToClearOnEnrty：没有收到通知值时讲通知值某些位清零，如设为B0000_0010，就是将第二位清零
							bitsToClearOnExit：在接收到通知值并处理后退出函数之前进行位清零操作，操作同上
							保存通知值的指针：指向在bitsToClearOnExit处理之前的通知值
							返回值：pdTRUE获取到了通知值，pdFALSE通知值获取失败
							tips：1、前两个参数：(uint32_t)0x00 		不清通知值
												 (uint32_t)ULONG_MAX 	清通知值
								  2、任务通知可以模拟长度为1的队列消息，可用于按键任务等，直接传键值或者用位操作判断等发挥


			
*/


