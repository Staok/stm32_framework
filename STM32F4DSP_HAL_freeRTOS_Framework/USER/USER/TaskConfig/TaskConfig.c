#include "TaskConfig.h"

/*__________创建任务函数，封装，固定的不要动_____________*/
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

//STEP 1:总任务数量（用户任务数量 + 1）
#define task_Num	3 

//STEP 2:任务索引枚举，公共变量，与Define_Task里面同样顺序的向下写
enum taskIndex_enum{
	start_t = 0,	/*固定的不要删*/
	
	led0_t,
	Stdby_t
}taskIndex;

//STEP 3:任务函数的声明
void led0_task(void *pvParameters);
void Stdby_task(void *pvParameters);

//STEP 4:设置任务要素
struct TaskStructure Define_Task[task_Num] = {
	/*任务函数      	任务名称      	分配堆栈      传入参数   任务优先级*/
    {start_task,		"start_task",		128,		NULL,		3	},	//创建用户任务的任务，固定的不要删
    {led0_task,			"led0_task",		50,			NULL,		2	},
    {Stdby_task,		"Stdby_task",		200,		NULL,		3	}
};  
/*注；堆栈实际占用字节数为定义数的四倍，即定义数的单位为字*/

//struct TaskStructure Define_Task0 = {
//    .Task_Function = start_task,
//    .Task_Name = "start_task",
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
		FaultASSERT("AT : Creating start_t failed,sys stoped");
        for(;;){;}
    }
}

/*__________开始初始化任务函数_____________*/
void start_task(void *pvParameters)
{
	uint8_t TaskCount;
    taskENTER_CRITICAL();           //进入临界区
	//STEP 4.5:设置通讯要素
	//进行消息队列和任务通知等相关的初始化，用户按需要修改
	
	
	//进行任务相关的初始化，以下部分固定的不用动
	for(TaskCount = 1;TaskCount < task_Num;TaskCount++)
	{
		xTaskCreate_t(TaskCount);
	}
    
    vTaskDelete(Define_Task[start_t].Task_Handle); 
    //删除开始任务，内存也被释放掉，删除任务这里放任务句柄，不取地址

    taskEXIT_CRITICAL();            //退出临界区
}

/*__________STEP 5:用户应用任务_____________*/
/*如若运行出现问题，先把任务分配的内存往高了整！*/
void led0_task(void *pvParameters)
{
	/*本任务所用的变量的定义，第一次运行需要设置的内容，或者本任务只需要运行一次的内容*/
	
	//float staok = 666.0;
	TickType_t PreviousWakeTime = xTaskGetTickCount(); 
	
	//进入本任务的主循环
    for(;;)
    {
        //staok += 0.1;
        vTaskDelayUntil(&PreviousWakeTime,pdMS_TO_TICKS(300));
		//vTaskDelay(300);
		//HAL_Delay(300);
		TestLED_Ctrl = !TestLED_Ctrl;
		TestLED2_Ctrl = !TestLED2_Ctrl;
		
		vTaskDelay(1000);
		
		#if SYSTEM_RTC_ENABLE
			char RTC_buf[50];
			RTC_TimeTypeDef RTC_RealTime;
			RTC_DateTypeDef	RTC_RealDate;
			HAL_RTC_GetTime(&RTC_Handler, &RTC_RealTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&RTC_Handler, &RTC_RealDate, RTC_FORMAT_BIN);
			sprintf(RTC_buf,"%d-%d-%d-%d   %d-%d-%d\r\n",RTC_RealDate.Year + 1970,RTC_RealDate.Month,RTC_RealDate.Date,RTC_RealDate.WeekDay,RTC_RealTime.Hours,RTC_RealTime.Minutes,RTC_RealTime.Seconds);
			printf_uart(UART1,"%s",RTC_buf);
		#endif
    }
	
	/*不能从任务函数中退出，除非提前删除任务，否则会调用configASSERT()引起错误*/
}   


void Stdby_task(void *pvParameters)
{
    for(;;)
    {
		//vTaskDelay(300);
		
		#if SYSTEM_StdbyWKUP_ENABLE
			sys_CheckWKUP_4RTOS();  //对于PA0，按下为高电平，按下3s后松开，延时一秒后进入待机状态（关机），再次按下开机
		#endif
		
		/*串口回显*/
//		u16 num4uart1;
//		char* buf4uart1 = mymalloc(InrRAM,(USART1_RX_BUF_MaxNum > USART1_RX_FIFO_MaxNum ? USART1_RX_BUF_MaxNum : USART1_RX_FIFO_MaxNum));
//		if(buf4uart1 != NULL)
//		{
//			if(sys_USART1_RX_Fetch(FALSE, buf4uart1,&num4uart1) == HAL_OK)
//			{
//				printf_uart(UART1,"%s-%d",buf4uart1,num4uart1);
//			}
//		}
//		myfree(InrRAM,buf4uart1);

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
			VTaskDelayUntil(&PreviousWakeTime,Timelncrement);
	

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
			创建：	QueueHandle_t qMsg = xQueueCreate(length,sizeof( uint32_t )); //参数：队列项目数量；每一项的字节数
			发送：	xQueueSend(qMsg,&Value,233); 				//参数：队列句柄；传入变量（拷贝）；超时时间（入队阻塞时间），
																返回值：pdPASS成功；errQUEUE_FULL队列满，发送失败
					xQueueSendFromISR(qMsg,&Value,BaseType_t Y) //中断版，参数：前两个一样，最后一个用于判断是否进行任务切换，中断函数没有超时时间，返回值同上
						用法：BaseType_t YieldRequired;
							  u8 err;
							err = xQueueSendFromISR(qMsg,&Value,&YieldRequired);
							if(err == pdPASS){;}//成功发送或者成功接收
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
			
		线程安全的意思：
			比如有两个任务中会对同一个FIFO队列写或者读，要写成：
			// 4 threads below, 2 for write, 2 for read
			void thread_write_1(void* arg) {
				// Use write mutex
				while (1) {
					mutex_get(&m_w);		//在写之前等其他任务给可写的信号
					lwrb_write(&rb, ...);
					mutex_give(&m_w);		//在写完之后给可写的信号
				}
			}

			void thread_write_2(void* arg) {
				// Use write mutex
				while (1) {
					mutex_get(&m_w);
					lwrb_write(&rb, ...);
					mutex_give(&m_w);
				}
			}

			void thread_read_1(void* arg) {
				// Use read mutex
				while (1) {
					mutex_get(&m_r);		//在读之前等其他任务给可读的信号
					lwrb_read(&rb, ...);
					mutex_give(&m_r);		//在读完之后给可读的信号
				}
			}

			void thread_read_2(void* arg) {
				// Use read mutex
				while (1) {
					mutex_get(&m_r);
					lwrb_read(&rb, ...);
					mutex_give(&m_r);
				}
			}

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
		
		内存申请：（FreeRTOS提供的内存申请释放函数都用uint8_t ucHeap[configTOTAL_HEAP_SIZE]内的，所以这两个函数只用于FreeRTOS任务内部使用，裸机和其他部分就用原子提供的即可）
			自己申请内存的一个好处是可以用外部SRAM
			函数：pvPortMalloc()和vPortFree()
			heap_4：提供内存碎片合并机制，不产生内存碎片，适合任务和队列的堆栈不同的应用，时间不确定
					在MCU内部RAM和外部SRAM之间只能二选一
			heap_5：内存碎片合并算法和heap_4一致，允许多段内存合用作为内存堆，
					内存堆初始化函数：393页开始，初始化内存堆之前不许搞任何内存申请活动
			获取剩余内存大小函数：
			内存申请和释放要成对出现，在一段内存没有释放之前绝不能再申请内存，否则造成内存泄漏！

			
*/


