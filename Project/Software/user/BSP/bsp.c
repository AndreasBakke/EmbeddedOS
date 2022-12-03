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
*                                       BOARD SUPPORT PACKAGE (BSP)
*
*                                            NXP LPC1768
*  
* Filename      :bsp.c
* Version       :V1.00
* Programmer(s) :FT
*                DC
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#ifndef BSP_MODULE
#define  BSP_MODULE
#include <bsp.h>
#include "LPC17xx.h"                    // Device header
#include <csp.h>
#include <os_cpu.h>

/*
*********************************************************************************************************
*                                             REGISTER & BIT DEFINES
*********************************************************************************************************
*/

                                                                /* - SYSTEM CONTROL AND STATUS REGISTER & BIT DEFINES  -*/
#define  BSP_REG_SCS                             (*(CPU_REG32 *)(0x400FC1A0))
#define  BSP_BIT_SCS_OSCRANGE                    DEF_BIT_04
#define  BSP_BIT_SCS_OSCEN                       DEF_BIT_05
#define  BSP_BIT_SCS_OSCSTAT                     DEF_BIT_06

                                                                /* ------- CLOCK DIVIDERS REGISTER & BIT DEFINES ------ */
#define  BSP_REG_CCLKCFG                         (*(CPU_REG32 *)(0x400FC104))
#define  BSP_REG_CLKSRCSEL                       (*(CPU_REG32 *)(0x400FC10C))
#define  BSP_REG_PCLKSEL0                        (*(CPU_REG32 *)(0x400FC1A8))
#define  BSP_REG_PCLKSEL1                        (*(CPU_REG32 *)(0x400FC1AC))

                                                                /* ------------ PLL REGISTER & BIT DEFINES ------------ */
#define  BSP_ADDR_PLL                            (CPU_INT32U )0x400FC080u

#define  BSP_REG_PLLCTRL(pll_id)                 (*(CPU_REG32 *)(BSP_ADDR_PLL + (0x20u * (pll_id)) + 0x00u))
#define  BSP_REG_PLLCFG(pll_id)                  (*(CPU_REG32 *)(BSP_ADDR_PLL + (0x20u * (pll_id)) + 0x04u))
#define  BSP_REG_PLLSTAT(pll_id)                 (*(CPU_REG32 *)(BSP_ADDR_PLL + (0x20u * (pll_id)) + 0x08u))
#define  BSP_REG_PLLFEED(pll_id)                 (*(CPU_REG32 *)(BSP_ADDR_PLL + (0x20u * (pll_id)) + 0x0Cu))

#define  BSP_MSK_PLLCFG0_MSEL                    0x00007FFFu
#define  BSP_MSK_PLLCFG0_NSEL                    0x00FF0000u

#define  BSP_MSK_PLLCFG1_MSEL                    0x0000001Fu
#define  BSP_MSK_PLLCFG1_NSEL                    0x00000060u

#define  BSP_BIT_PLLCTRL_PLLE                    DEF_BIT_00     /* PLL enable                                           */
#define  BSP_BIT_PLLCTRL_PLLC                    DEF_BIT_01     /* PLL connect                                          */

#define  BSP_BIT_PLLSTAT_PLLE0_STAT              DEF_BIT_24     /* Read-back for the PLL enable bit                     */
#define  BSP_BIT_PLLSTAT_PLLC0_STAT              DEF_BIT_25     /* Read-back for the PLL connect bit                    */
#define  BSP_BIT_PLLSTAT_PLOCK0                  DEF_BIT_26     /* Refkect the PLL lock status                          */

#define  BSP_BIT_PLLSTAT_PLLE1_STAT              DEF_BIT_08     /* Read-back for the PLL enable bit                     */
#define  BSP_BIT_PLLSTAT_PLLC1_STAT              DEF_BIT_09     /* Read-back for the PLL connect bit                    */
#define  BSP_BIT_PLLSTAT_PLOCK1                  DEF_BIT_10     /* Refkect the PLL lock status                          */

#define  BSP_BIT_PLLFEED_VAL0                    0x000000AAu
#define  BSP_BIT_PLLFEED_VAL1                    0x00000055u

#define  BSP_BIT_CLKSRCSEL_RC                    DEF_BIT_NONE
#define  BSP_BIT_CLKSRCSEL_MAIN                  DEF_BIT_00
#define  BSP_BIT_CLKSRCSEL_RTC                   DEF_BIT_01

                                                                /* --- FLASH ACCELERATOR CFG REGISTER & BIT DEFINES --- */
#define  BSP_REG_FLASHCFG                        (*(CPU_REG32 *)(0x400FC000u))

#define  BSP_MSK_FLASHCFG_CLK_1                  DEF_BIT_MASK(1u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_2                  DEF_BIT_MASK(2u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_3                  DEF_BIT_MASK(3u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_4                  DEF_BIT_MASK(4u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_5                  DEF_BIT_MASK(5u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_6                  DEF_BIT_MASK(6u, 12u)
#define  BSP_MSK_FLASHCFG_RST_VAL                0x0000003Au


#define  BSP_REG_DEMCR             (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR            (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT        (*(CPU_REG32 *)0xE0001004)


/*
*********************************************************************************************************
*                                             BSP CONSTANS VALUES
*********************************************************************************************************
*/
                                                               /* Maximum register timeout                            */
#define  BSP_VAL_MAX_TO                         (CPU_INT16U)(0xFFFF)




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


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/

#define  BSP_PLL_FEED_SEQ(pll_nbr)              {    CPU_CRITICAL_ENTER();                             \
                                                                                                       \
                                                     BSP_REG_PLLFEED(pll_nbr) = BSP_BIT_PLLFEED_VAL0;  \
                                                     BSP_REG_PLLFEED(pll_nbr) = BSP_BIT_PLLFEED_VAL1;  \
                                                                                                       \
                                                     CPU_CRITICAL_EXIT();                              \
                                                }


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
*                                              BSP_Start()
*
* Description : Initialize all the peripherals that required OS services (OS initialized)
*
* Argument(s) : tick_rate : The kernel tick rate in Hz.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_Start (void)
{
    CPU_INT32U  cnts;
    CPU_INT32U  cpu_freq;


    cpu_freq = CSP_PM_CPU_ClkFreqGet();                         /* Get the CPU clock                                    */
#if (OS_VERSION >= 30000u)
    cnts     = (cpu_freq / OSCfg_TickRate_Hz);                  /* Calculate the number of SysTick counts               */
#else
    cnts     = (cpu_freq / OS_TICKS_PER_SEC);
#endif
    OS_CPU_SysTickInit(cnts);                                   /* Call the Generic OS Systick initialization           */

}

/*
*********************************************************************************************************
*                                          CPU_TS_TmrInit()
*
* Description : Initialize & start CPU timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_Init().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrInit() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                   (c) When applicable, timer period SHOULD be less than the typical measured time
*                       but MUST be less than the maximum measured time; otherwise, timer resolution
*                       inadequate to measure desired times.
*
*                   See also 'CPU_TS_TmrRd()  Note #2'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  fclk_freq;

    fclk_freq = CSP_PM_CPU_ClkFreqGet();

    BSP_REG_DEMCR  |= DEF_BIT_24;
    BSP_REG_DWT_CR |= DEF_BIT_00;

    CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_TmrRd()
*
* Description : Get current CPU timestamp timer count value.
*
* Argument(s) : none.
*
* Return(s)   : Timestamp timer count (see Notes #2a & #2b).
*
* Caller(s)   : CPU_TS_Init(),
*               CPU_TS_Get32(),
*               CPU_TS_Get64(),
*               CPU_IntDisMeasStart(),
*               CPU_IntDisMeasStop().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrRd() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                       (1) If timer is a 'down' counter whose values decrease with each time count,
*                           then the returned timer value MUST be ones-complemented.
*
*                   (c) (1) When applicable, the amount of time measured by CPU timestamps is
*                           calculated by either of the following equations :
*
*                           (A) Time measured  =  Number timer counts  *  Timer period
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer period            Timer's period in some units of
*                                                                   (fractional) seconds
*                                       Time measured           Amount of time measured, in same
*                                                                   units of (fractional) seconds
*                                                                   as the Timer period
*
*                                                  Number timer counts
*                           (B) Time measured  =  ---------------------
*                                                    Timer frequency
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer frequency         Timer's frequency in some units
*                                                                   of counts per second
*                                       Time measured           Amount of time measured, in seconds
*
*                       (2) Timer period SHOULD be less than the typical measured time but MUST be less
*                           than the maximum measured time; otherwise, timer resolution inadequate to
*                           measure desired times.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    CPU_TS_TMR  ts_tmr_cnts;


    ts_tmr_cnts  = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;

    return (ts_tmr_cnts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                         CPU_TSxx_to_uSec()
*
* Description : Convert a 32-/64-bit CPU timestamp from timer counts to microseconds.
*
* Argument(s) : ts_cnts   CPU timestamp (in timestamp timer counts [see Note #2aA]).
*
* Return(s)   : Converted CPU timestamp (in microseconds           [see Note #2aD]).
*
* Caller(s)   : Application.
*
*               This function is an (optional) CPU module application interface (API) function which
*               MAY be implemented by application/BSP function(s) [see Note #1] & MAY be called by
*               application function(s).
*
* Note(s)     : (1) CPU_TS32_to_uSec()/CPU_TS64_to_uSec() are application/BSP functions that MAY be
*                   optionally defined by the developer when either of the following CPU features is
*                   enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) The amount of time measured by CPU timestamps is calculated by either of
*                       the following equations :
*
*                                                                        10^6 microseconds
*                       (1) Time measured  =   Number timer counts   *  -------------------  *  Timer period
*                                                                            1 second
*
*                                              Number timer counts       10^6 microseconds
*                       (2) Time measured  =  ---------------------  *  -------------------
*                                                Timer frequency             1 second
*
*                               where
*
*                                   (A) Number timer counts     Number of timer counts measured
*                                   (B) Timer frequency         Timer's frequency in some units
*                                                                   of counts per second
*                                   (C) Timer period            Timer's period in some units of
*                                                                   (fractional)  seconds
*                                   (D) Time measured           Amount of time measured,
*                                                                   in microseconds
*
*                   (b) Timer period SHOULD be less than the typical measured time but MUST be less
*                       than the maximum measured time; otherwise, timer resolution inadequate to
*                       measure desired times.
*
*                   (c) Specific implementations may convert any number of CPU_TS32 or CPU_TS64 bits
*                       -- up to 32 or 64, respectively -- into microseconds.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;
    CPU_ERR     err;


    fclk_freq = CPU_TS_TmrFreqGet(&err);
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif


#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{

    return (0u);

}
#endif



/*Initialization function of the pwm peripheral 

	Starting value are a duty cycle of 50 % and a frequency of pwm signal of 1 KHz
*/
void PWM_init(void){

// on reset the power is enabled to pwm 
LPC_SC->PCONP|=(1<<6); // enabling power to pwm peripheral 

	LPC_SC->PCLKSEL0|=(3<<12); // clock to pwm peripheral
	
	
	//led 13
	LPC_PINCON->PINSEL3|=(2u<<4);
	LPC_PINCON->PINMODE3 |=(2u <<4);
	
	/*
	// from 2.0 to 2.7 
	LPC_PINCON->PINSEL4|=0x00000555; // enabling the pin mode  to pwm 
LPC_PINCON->PINMODE4=0x00000AAA; // enabling disabling output pull up or pull down resistance
*/
LPC_PWM1->IR=0;// clearing all interrupt flags
	
	LPC_PWM1->TCR|=0x00000002; // reset counter
LPC_PWM1->CTCR=0; // timer mode
	LPC_PWM1->CCR=0;// no capture event 
//	LPC_PWM1->PCR=0x00007E00; // single edge controlled for pwm signal from 2 to 6, also enabling its output
	LPC_PWM1->PCR=(1u<<9);

// ORDER IS IMPORTANT!
	LPC_PWM1->MR0=0x000030D4;  // for gettting  a 1 Khz frequency of the pwm output signal on every channel ( MAXcnt -1 ) 
	LPC_PWM1->MR1=LPC_PWM1->MR2
	=LPC_PWM1->MR3
	=LPC_PWM1->MR4
	=LPC_PWM1->MR5
	=LPC_PWM1->MR6=0x0000186A; // duty cycle of 50 %  default value
	
	LPC_PWM1->MCR=0x00000002;// no stop, no reset neither isr on match of the counter value


}


void PWM_enable(void){
LPC_PWM1->TCR=0x00000001; //counter enable 
}


void change_frequency ( int value ) {
/*
	in this configuration  the frequency is global for all pwm signals
	*/
		
	float old_mr0=LPC_PWM1->MR0;
	LPC_PWM1->TCR|=0x00000002; // reset counter and stop
	LPC_PWM1->MR0=((float)12500000)/((1+LPC_PWM1->PC)*(float)value);  // adjusting the max counter
	
	// scaling the duty cycle to all channels
	update_dutycycle( (((float)LPC_PWM1->MR1)/((float)old_mr0)),CH1);
	
	// restart the count 
LPC_PWM1->TCR=0x00000001;

}



void update_dutycycle( float value, PWM_channel channel ){
	
	
int com_value=(((LPC_PWM1->MR0+1)*value));  // convesion for scaling to the global cnt

	switch ( channel ){
	case CH1:
		LPC_PWM1->MR1=com_value;
		break;
	case CH2:
		LPC_PWM1->MR2=com_value;
		break;
case CH3:
	LPC_PWM1->MR3=com_value;
		break;
case CH4:
		LPC_PWM1->MR4=com_value;
		break;
case CH5:
		LPC_PWM1->MR5=com_value;
		break;
	case CH6:
		LPC_PWM1->MR6=com_value;
		break;
	case ALL:
			LPC_PWM1->MR1=LPC_PWM1->MR2
	=LPC_PWM1->MR3
	=LPC_PWM1->MR4
	=LPC_PWM1->MR5
	=LPC_PWM1->MR6=com_value;
	break;
	default:
		break;

}


}

int get_dutycycle(PWM_channel channel ){
/*  duty cycle=mr_i/MR0  i goes from 1 to 6*/
	int  value=0;
	switch ( channel) {
		case CH1:
			value=100*LPC_PWM1->MR1/(float)LPC_PWM1->MR0;
			break;
		case CH2:
						value=100*LPC_PWM1->MR2/(float)LPC_PWM1->MR0;

			break;
		case CH3:
						value=100*LPC_PWM1->MR3/(float)LPC_PWM1->MR0;
			break;
		case CH4:
									value=100*LPC_PWM1->MR4/(float)LPC_PWM1->MR0;
			break;
		case CH5:
						value=100*LPC_PWM1->MR5/(float)LPC_PWM1->MR0;
		break;
		case CH6:
									value=100*LPC_PWM1->MR6/(float)LPC_PWM1->MR0;
			break;
		default:
			break;
	}

	
	return 100-value;
}
int get_frequency(PWM_channel channel){
/*
	fpwm=f_clk/(divider+1)(maxcnt+1)
	*/
int value=((float)12500000)/((LPC_PWM1->PC+1)*LPC_PWM1->MR0);
	
	
	return value;


}

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */

