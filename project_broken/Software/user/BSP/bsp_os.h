/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2012; Micrium, Inc.; Weston, FL
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
* Filename      : bsp_os.h
* Version       : V1.00
* Programmer(s) : DC
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_OS_PRESENT
#define  BSP_OS_PRESENT


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <os.h>


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_OS_MODULE
#define  BSP_OS_EXT
#else
#define  BSP_OS_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef   OS_SEM       BSP_OS_SEM;
typedef   OS_SEM_CTR   BSP_OS_SEM_VAL;


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               DEFINES
*
* Note(s) : (1) The Cortex-M "Vector Table Offset Register" section states the following:
*
*               You must align the offset to the number of exception entries in the vector table. The
*               minimum alignment is 32 words, enough for up to 16 interrupts. For more interrupts,
*               adjust the alignment by rounding up to the next power of two. For example, if you require
*               21 interrupts, the alignment must be on a 64-word boundary because the required table
*               size is 37 words, and the next power of two is 64. See your vendor documentation for the
*               alignment details for your device.
*********************************************************************************************************
*/

#define  ARMV7M_CORE_EXCS             16u

#define  INT_ID_MAX_NBR              32u                       /* Maximum number of external interrupt sources.        */

                                                                /* 191 VTOR entries; next power of 2 is 256             */
#define  INT_VTOR_TBL_SIZE         (INT_ID_MAX_NBR + ARMV7M_CORE_EXCS)
#define  INT_VTOR_TBL_ALIGNMENT    (0x400uL)                    /* 64 * 4 = 256 words (see note 1)                    */


/*
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*/

                                                    /* External Interrupt IDs.                                          */
typedef  enum  bsp_int_id {
	INT_ID_WDT                 =   0u,             /* watchdog interrupt */
	INT_ID_TIM0 = 1u , /*timer 0 interrupt */
	INT_ID_TIM1 = 2u , /*timer 1 interrupt */
	INT_ID_TIM2 = 3u , /*timer 2 interrupt */
	INT_ID_TIM3 = 4u , /*timer 3 interrupt */
	INT_ID_UART0 = 5u , /* uart 0 interrupt */
	INT_ID_UART1 = 6u , /* uart1 interrupt */
	INT_ID_UART2 = 7u , /* uart 2 interrupt */
	INT_ID_UART3 = 8u , /* uart 3 interrupt */
	INT_ID_PWM = 9u , /*pwm interrupt */
	INT_ID_I2C0=10u , /*i2c0 interrupt */
	INT_ID_I2C1=11u , /*i2c1 interrupt */
	INT_ID_I2C2=12u , /*i2c2 interrupt */
	INT_ID_SPI=13u, /*spi interrupt */
	INT_ID_SSP0=14u , /*ssp 0 interrupt*/
	INT_ID_SSP1=15u , /*ssp 1 interrupt*/
	INT_ID_PLL0=16u, /*PLL0 locl ( main pll) */
	INT_ID_RTC=17u, /*real time clock interrupt */
	INT_ID_EINT0=18u , /*external interrupt 0 */
	INT_ID_EINT1=19u , /*external interrupt 1 */
	INT_ID_EINT2=20u , /*external interrupt 2 */
	INT_ID_EINT3=21u , /*external interrupt 3 */
		INT_ID_ADC=22u, /*A/d converter interrupt*/
		INT_ID_BOD=23u, /* brown-out detect interrupt*/
		INT_ID_USB=24u, /*usb interrupt */
		INT_ID_CAN=25u, /*can interrupt */
	INT_ID_GPDMA=26u, /*geberal purpose dma interrupt*/
INT_ID_I2S=27u, /*i2s interrupt */
INT_ID_ENET=28u, /*ethernet interrupt */
INT_ID_RIT=29u, /*repetitive interrupt timer interrupt*/
INT_ID_MCPWM=30u, /*motor control pwm interrupt*/
INT_ID_QEI=31u, /*quadrature encoder interface interrupt*/
INT_ID_PLL1=32u, /*pll1 lock  ( usb pll ) interrupt */
INT_ID_USBAct=33u, /*usb activity interrupt to wakeup*/
INT_ID_CANAct=34u /*can activity interrupt to wakeup*/
} BSP_INT_ID;


/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemCreate          (BSP_OS_SEM     *p_sem,
                                        BSP_OS_SEM_VAL  sem_val,
                                        CPU_CHAR       *p_sem_name);

CPU_BOOLEAN  BSP_OS_SemWait            (BSP_OS_SEM     *p_sem,
                                        CPU_INT32U      dly_ms);

CPU_BOOLEAN  BSP_OS_SemPost            (BSP_OS_SEM     *p_sem);

void  BSP_IntInit   (void);

void  BSP_IntEnable (BSP_INT_ID     int_id);

void  BSP_IntDisable(BSP_INT_ID     int_id);

void  BSP_IntClear  (BSP_INT_ID     int_id);

void  BSP_IntVectSet(BSP_INT_ID     int_id,
                     CPU_INT08U     int_prio,
                     CPU_FNCT_VOID  isr_handler);


void         BSP_OS_TmrTickInit        (CPU_INT32U      tick_rate);
void         BSP_OS_TimeDlyMs          (CPU_INT32U      dly_ms);
void  BSP_OS_TickEnable (void);

/*
*********************************************************************************************************
*                                          CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */
