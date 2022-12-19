/*
*********************************************************************************************************
*                                     MICRIUM BOARD SUPPORT SUPPORT
*
*                          (c) Copyright 2003-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       BOARD SUPPORT PACKAGE
*                                          uCOS-III LAYER
*
* Filename      : bsp_os.c
* Version       : V1.00
* Programmer(s) : DC
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#ifndef BSP_OS_MODULE
#define  BSP_OS_MODULE
#include "bsp_os.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

CPU_FNCT_VOID  VectorTbl_RAM[INT_VTOR_TBL_SIZE] __attribute__(( aligned (INT_VTOR_TBL_ALIGNMENT) ));

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                     BSP OS LOCKS FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      BSP_OS_SemCreate()
*
* Description : Creates a sempahore to lock/unlock
*
* Argument(s) : p_sem        Pointer to a BSP_OS_SEM structure
*
*               sem_val      Initial value of the semaphore.
*
*               p_sem_name   Pointer to the semaphore name.
*
* Return(s)   : DEF_OK       if the semaphore was created.
*               DEF_FAIL     if the sempahore could not be created.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemCreate (BSP_OS_SEM       *p_sem,
                               BSP_OS_SEM_VAL    sem_val,
                               CPU_CHAR         *p_sem_name)
{
    OS_ERR     err;


    OSSemCreate((OS_SEM    *)p_sem,
                (CPU_CHAR  *)p_sem_name,
                (OS_SEM_CTR )sem_val,
                (OS_ERR    *)&err);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                     BSP_OS_SemWait()
*
* Description : Wait on a semaphore to become available
*
* Argument(s) : sem          sempahore handler
*
*               dly_ms       delay in miliseconds to wait on the semaphore
*
* Return(s)   : DEF_OK       if the semaphore was acquire
*               DEF_FAIL     if the sempahore could not be acquire
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemWait (BSP_OS_SEM  *p_sem,
                             CPU_INT32U   dly_ms)
{
    OS_ERR      err;
    CPU_INT32U  dly_ticks;


    dly_ticks  = ((dly_ms * DEF_TIME_NBR_mS_PER_SEC) / OSCfg_TickRate_Hz);

    OSSemPend((OS_SEM *)p_sem,
              (OS_TICK )dly_ticks,
              (OS_OPT  )OS_OPT_PEND_BLOCKING,
              (CPU_TS  )0,
              (OS_ERR *)&err);

    if (err != OS_ERR_NONE) {
       return (DEF_FAIL);
    }

    return (DEF_OK);
}

/*
*********************************************************************************************************
*                                      BSP_OS_SemPost()
*
* Description : Post a semaphore
*
* Argument(s) : sem          Semaphore handler
*
* Return(s)   : DEF_OK     if the semaphore was posted.
*               DEF_FAIL      if the sempahore could not be posted.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemPost (BSP_OS_SEM *p_sem)
{
    OS_ERR  err;


    OSSemPost((OS_SEM *)p_sem,
              (OS_OPT  )OS_OPT_POST_1,
              (OS_ERR *)&err);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*********************************************************************************************************
**                                     uC/OS-III TIMER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    BSP_TimeDlyMs()
*
* Description : This function delay the exceution for specifi amount of miliseconds
*
* Argument(s) : dly_ms       Delay in miliseconds
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void   BSP_OS_TimeDlyMs (CPU_INT32U  dly_ms)
{
    CPU_INT16U  ms;
    CPU_INT16U  sec;
    OS_ERR      err;


    if (dly_ms > 10000u) {                                       /* Limit delays to 10 seconds.                        */
        dly_ms = 10000u;
    }

    if (dly_ms >= 1000u) {
        ms  = dly_ms / 1000u;
        sec = dly_ms % 1000u;
    } else {
        ms  = dly_ms;
        sec = 0u;
    }


    OSTimeDlyHMSM((CPU_INT16U) 0u,
                  (CPU_INT16U) 0u,
                  (CPU_INT16U) sec,
                  (CPU_INT32U) ms,
                  (OS_OPT    ) OS_OPT_TIME_HMSM_STRICT,
                  (OS_ERR   *)&err);
}





void         BSP_OS_TmrTickInit        (CPU_INT32U      tick_rate){





}



/*
*********************************************************************************************************
*                                         BSP_OS_TickEnable()
*
* Description : Enable the OS tick interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none
*********************************************************************************************************
*/

void  BSP_OS_TickEnable (void)
{
    CPU_REG_NVIC_ST_CTRL |= (CPU_REG_NVIC_ST_CTRL_TICKINT |     /* Enables SysTick exception request                    */
                             CPU_REG_NVIC_ST_CTRL_ENABLE);      /* Enables SysTick counter                              */
}

/*
*********************************************************************************************************
*                                              BSP_IntInit()
*
* Description : Initialize interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : (1) The Cortex-M "Vector Table Offset Register" section states the following:
*
*                   You must align the offset to the number of exception entries in the vector table.
*                   The minimum alignment is 32 words, enough for up to 16 interrupts. For more
*                   interrupts, adjust the alignment by rounding up to the next power of two. For example,
*                   if you require 21 interrupts, the alignment must be on a 64-word boundary because the
*                   required table size is 37 words, and the next power of two is 64. SEE YOUR VENDOR
*                   DOCUMENTATION FOR THE ALIGNMENT DEATILS FOR YOUR DEVICE.
*********************************************************************************************************
*/

void  BSP_IntInit (void)
{
    CPU_INT16U      int_id;
    CPU_FNCT_VOID  *p_vect_tbl;
    CPU_SR_ALLOC();


    p_vect_tbl = (CPU_FNCT_VOID *)CPU_REG_NVIC_VTOR;
    for (int_id = 0u; int_id <= (INT_VTOR_TBL_SIZE - 1u); int_id++) {
        VectorTbl_RAM[int_id] = p_vect_tbl[int_id];             /* Copy current table into RAM table                    */
    }

    CPU_CRITICAL_ENTER();
    CPU_REG_NVIC_VTOR = (CPU_INT32U)&VectorTbl_RAM[0u];         /* See note 1.                                          */
    CPU_CRITICAL_EXIT();
}


/*
*********************************************************************************************************
*                                             BSP_IntClear()
*
* Description : Clear interrupt.
*
* Argument(s) : int_id    Interrupt to clear.
*
* Return(s)   : none.
*
* Note(s)     : (1) An interrupt does not need to be cleared within the interrupt controller.
*********************************************************************************************************
*/

void  BSP_IntClear (BSP_INT_ID  int_id)
{

}


/*
*********************************************************************************************************
*                                            BSP_IntDisable()
*
* Description : Disable interrupt.
*
* Argument(s) : int_id    Interrupt to disable.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntDisable (BSP_INT_ID  int_id)
{
    if (int_id < INT_ID_MAX_NBR) {
        CPU_IntSrcDis(int_id + ARMV7M_CORE_EXCS);
    }
}


/*
*********************************************************************************************************
*                                             BSP_IntEnable()
*
* Description : Enable interrupt.
*
* Argument(s) : int_id    Interrupt to enable.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntEnable (BSP_INT_ID  int_id)
{
    if (int_id < INT_ID_MAX_NBR) {
        CPU_IntSrcEn(int_id + ARMV7M_CORE_EXCS);
    }
}


/*
*********************************************************************************************************
*                                            BSP_IntVectSet()
*
* Description : Assign ISR handler.
*
* Argument(s) : int_id         Interrupt for which vector will be set.
*
*               int_prio       Priority of interrupt to be set.
*
*               int_type       Type of interrupt(Depends on CPU Architecture).
*
*               isr_handler    Handler to assign
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntVectSet (BSP_INT_ID     int_id,
                      CPU_INT08U     int_prio,
                      CPU_FNCT_VOID  isr_handler)
{
   (void)int_prio;

    if (int_id < INT_ID_MAX_NBR) {
        VectorTbl_RAM[int_id + ARMV7M_CORE_EXCS] = isr_handler;
    }
}

#endif 
