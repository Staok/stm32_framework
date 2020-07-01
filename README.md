# stm32_framework
一个计划实现STM32F1和F4的工程模板，包含高级封装的HAL库外设驱动、Menu、FreeRTOS、STemWin、LWIP、USB、FATFS、DSP等，以及包含加密、BPNN、最小二乘等诸多便利的组件和算法，并具有良好的易用性。

已经实现：

1、必备基础运行框架，包含：（具体看sys.h里面的注释）
    可选使用裸机还是OS；
    SYSTEM_MCO_PA8_OUT；
    SYSTEM_RTC_ENABLE；
    SYSTEM_CRC_ENABLE；
    STSTEM_TIM4_ENABLE；
    STSTEM_TIM3PWM_ENABLE；
    SYSTEM_IWDG_ENABLE；
    SYSTEM_UART123_ENABLE；
    SYSTEM_SUPPORT_sprintf；
    SYSTEM_SUPPORT_pid；
    ringbuf；

2、 嵌入FreeRTOS，可选是否启用（SYSTEM_SUPPORT_OS）
    OS相关的模板规范写在TaskConfig.c里面
    重要两点：  关于FreeRTOS的信号量和事件标志组，均用任务通知替代！
               关于FreeRTOS的任务与任务、任务与中断间的通讯只用消息队列，功能最全！

