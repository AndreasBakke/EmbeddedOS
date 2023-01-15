/*
*********************************************************************************************************
*                                              uC/OS-III
*                                        The Real-Time Kernel
*
*                          (c) Copyright 2004-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/OS-III is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/OS-III in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to
*               help you experience uC/OS-III.  The fact that the source code is provided
*               does NOT mean that you can use it without paying a licensing fee.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       APPLICATION CONFIGURATION
*
*                                             NXP LPC1768
*                                                on the
*                                     IAR LPC1768-SK Kickstart Kit
*
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : FT
*                 DC
*********************************************************************************************************
*/

#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT


/*
*********************************************************************************************************
*                                       ADDITIONAL uC/MODULE ENABLES
*********************************************************************************************************
*/

#define OS_CFG_TASK_DEL_EN 1
#define OS_CFG_TMR_EN 1
#define	 OS_TMR_EN 1
/*
*********************************************************************************************************
*                                            TASK PRIORITIES (Low prio number = high priority ref. manual)
*********************************************************************************************************
*/
#define  OS_LOWEST_PRIO 													7u
#define  APP_CFG_TASK_PRIO                  			14u
#define  READ_WL_PRIO                      				13u
#define  OUTPUT_PRIO															12u
#define  MONITOR_PRIO															11u
#define  TIMING_TEST_PRIO													10u

#define  OS_TASK_TMR_PRIO                         (OS_LOWEST_PRIO - 2)


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_STK_SIZE                     256u


/*
*********************************************************************************************************
*                                             uC/LIB CONFIGURATION
*********************************************************************************************************
*/

#define  LIB_MEM_CFG_OPTIMIZE_ASM_EN            DEF_ENABLED
#define  LIB_MEM_CFG_ARG_CHK_EXT_EN             DEF_ENABLED
#define  LIB_MEM_CFG_ALLOC_EN                   DEF_ENABLED
#define  LIB_MEM_CFG_HEAP_SIZE                     15*1024u



/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#define  TRACE_LEVEL_OFF                                  0u
#define  TRACE_LEVEL_INFO                                 1u
#define  TRACE_LEVEL_DBG                                  2u

#define  APP_CFG_TRACE_LEVEL                    TRACE_LEVEL_DBG
#define  APP_CFG_TRACE                          printf

#define  APP_TRACE_INFO(x)                     ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_CFG_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                      ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_CFG_TRACE x) : (void)0)

#endif
