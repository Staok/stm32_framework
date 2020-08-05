# stm32_framework
一个计划实现STM32F1和F4的工程模板，包含高级封装的HAL库外设驱动、Menu、FreeRTOS、STemWin、LWIP、USB、FATFS、DSP等，以及包含加密、BPNN、最小二乘等诸多便利的组件和算法，并具有良好的易用性。

目前只有F1，当F1完成后，F4就快了...吧。

**具体组件：**（加下划线的为暂未实现的，以后会加上- -，工作量好大的！ ）

-   大部分HAL库外设驱动高级封装（ MCO \ RTC \ CRC \ TIM \ ADC \ <u>DAC</u> \ IWDG \ USART \ SPI \ WFI \ FLASH \ <u>IAP</u> \ IO \ <u>LTDC LCD</u> \ <u>DCMI</u> \ <u>FSMC</u> \ <u>DMA</u> \ <u>DSP</u> \ <u>FPU</u> \ <u>USB</u> \ <u>CAN</u>）
-   FreeRTOS
-   <u>LittlevGL或者STemWin</u>
-   <u>LWIP</u>
-   <u>FATFS</u>
-   PID算法
-   ringbuf
-   Menu框架
-   无依赖的string库和sprintf库
-   软件模拟I2C\SPI

**具体说明：**

-   所有基础外设均在sys.h里面通过宏定义配置，每一个宏定义旁边均有使用说明的注释；所有用户配置外设均在PeriphConfig.c里面，有详细注释，不看不会用
-   MCU外设配置的统一函数为void sys_MCU_Init_Seq(void)，一般无需改动；外接器件初始化统一函数为void sys_Device_Init_Seq(void)，需要自行按需修改
-   所有头文件均放在sys.h里面，其他库文件想互相调用时只需调sys.h即可，不乱
-   本框架是正经的框架，有命名规范和应用规范说明，在sys.h的上面

1.  裸跑或者FreeRTOS；

    -   SYSTEM_SUPPORT_OS：是否开启FreeRTOS，不开为裸跑；具体应用逻辑编写和任务配置分别在TaskConfig.c和BareConfig.c里面

        FreeRTOS说明：所有API均罗列在TaskConfig.h里！关于FreeRTOS的信号量和事件标志组，均用任务通知替代！关于FreeRTOS的任务与任务、任务与中断间的通讯只用消息队列，功能最全！

2.  框架常用组件

    -   SYSTEM_SUPPORT_Menu：提供一个实现菜单功能的模板；推荐作为标准常用，应用逻辑围绕这个菜单框架展开编写；具体文件在sys_menu.c里面；按照里面的注释，void keyProcess(void)函数负责扫描输入，void menuProcess(void)函数负责执行菜单功能，这两个函数必须放到一个循环里一直执行
    -   SYSTEM_SUPPORT_MyString：提供一个实现了string.h大部分字符操作函数的库；具体作用看MyString.c文件里的注释
    -   SYSTEM_SUPPORT_sprintf：提供一个无依赖的独立实现的sprint库，github开源https://github.com/mpaland/printf；经过修改，可以实现对指定串口发送printf格式化的字符串
    -   SYSTEM_SUPPORT_pid：提供一个pid算法实现库，集成了积分分离和变限积分，以及可选的不完全微分和微分先行，具体用法看pid.h里面
    -   ringbuf：提供一个软件实现的唤醒缓冲区，依据FIFO规则；默认已经用在串口接受的地方，具体看串口宏定义处旁边的注释
    -   <u>LittlevGL或者STemWin</u>：缺省
    -   <u>LWIP</u>：缺省
    -   <u>FATFS</u>：缺省
    -   <u>DSP\FPU</u>：缺省

3.  框架基础外设（再次说明，具体实用方法看相应宏定义旁的注释，均在sys.h里）

    -   SYSTEM_MCO_PA8_OUT：设置PA8为MCO输出，默认时钟源为HSE
    -   SYSTEM_RTC_ENABLE：配置使用RTC
-   SYSTEM_CRC_ENABLE：配置使用CRC循环冗余校验
    -   STSTEM_TIM4_ENABLE：模板基本功能，必须开启！使用通用定时器4，提供时基，默认10ms周期中断
    -   STSTEM_TIM3PWM_ENABLE：模板基本功能，必须开启！使用通用定时器3，默认不开启中断，提供四路PWM输出；引脚重映射、中断、频率、占空比均提供修改
    -   STSTEM_TIM2_ENABLE：通用定时器2，功能自定，默认分频系数为72，初始化函数在PeriphCconfig.c里面定义；提供定时器中断、PWM、输入捕获和正交解码功能，用户按需自定
    -   SYSTEM_ADC1_ENABLE：启否ADC1；提供添加规则组通道、是否连续扫描、是否使用DMA等可选功能；有详细注释
    -   SYSTEM_IWDG_ENABLE：开启独立看门狗，默认1S的喂狗周期，默认在TIM4定时中断里喂狗，用IWDG必开TIM4
    -   SYSTEM_UARTx_ENABLE：开启串口，x = 1、2和3；引脚重映射和波特率可选
    -   SYSTEM_SPIx_ENABLE：开启SPI，x = 1和2；尽量只用其中一个，多个器件用多个SS使能端，不提供引脚重映射
    -   SYSTEM_StdbyWKUP_ENABLE：使用待机-低功耗模式
    -   SYSTEM_FLASH_IAP_ENABLE：启用对内部FLASH储存空间编程
    -   GPIO相关的初始化、输入出和位带操作以及改变入出模式的调用口均在PeriphCconfig.c和PeriphCconfig.h里，有详细注释
    

