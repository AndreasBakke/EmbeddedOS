/*
*********************************************************************************************************
*                                              uC/CSP
*                                        Chip Support Package
*
*                            (c) Copyright 2010; Micrium, Inc.; Weston, FL
*                         (c) Copyright 2003-2010; Micrium, Inc.; Weston, FL
*
*               All rights reserved. Protected by international copyright laws.
*
*               uC/CSP is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       POWER & CLOCK MANAGEMENT
*                                            NXP  LPC17XX
*
* Filename      : csp_pm.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
* Note(s)       : 
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    CSP_PM_MODULE
#include   <csp.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

                                                                /* ---------- PCONP REGISTERS RESERVED VALUES -------- */
#define  CSP_PM_PER_CLK_NBR_PCONP_RSVD0               0u
#define  CSP_PM_PER_CLK_NBR_PCONP_RSVD1               5u
#define  CSP_PM_PER_CLK_NBR_PCONP_RSVD2              11u
#define  CSP_PM_PER_CLK_NBR_PCONP_RSVD3              20u
#define  CSP_PM_PER_CLK_NBR_PCONP_RSVD4              28u

                                                                /* --------- PCLKSEL REGISTERS RESERVED VALUES -------- */
#define  CSP_PM_PER_CLK_NBR_PCLKSEL_RSVD0             5u  
#define  CSP_PM_PER_CLK_NBR_PCLKSEL_RSVD1             9u  
#define  CSP_PM_PER_CLK_NBR_PCLKSEL_RSVD2            20u  
#define  CSP_PM_PER_CLK_NBR_PCLKSEL_RSVD3            28u  

                                                                /* ---------------- REGISTER DEFINES ------------------ */
#define  CSP_PM_ADDR_PLL0                       ( (CPU_INT32U )0x400FC080)
#define  CSP_PM_ADDR_PLL1                       ( (CPU_INT32U )0x400FC0A0)
  
#define  CSP_PM_REG_PLLCTRL0                    (*(CPU_REG32 *)(CSP_PM_ADDR_PLL0 + 0x00))
#define  CSP_PM_REG_PLLCFG0                     (*(CPU_REG32 *)(CSP_PM_ADDR_PLL0 + 0x04))
#define  CSP_PM_REG_PLLSTAT0                    (*(CPU_REG32 *)(CSP_PM_ADDR_PLL0 + 0x08))
#define  CSP_PM_REG_PLLFEED0                    (*(CPU_REG32 *)(CSP_PM_ADDR_PLL0 + 0x0C))

#define  CSP_PM_REG_PLLCTRL1                    (*(CPU_REG32 *)(CSP_PM_ADDR_PLL1 + 0x00))
#define  CSP_PM_REG_PLLCFG1                     (*(CPU_REG32 *)(CSP_PM_ADDR_PLL1 + 0x04))
#define  CSP_PM_REG_PLLSTAT1                    (*(CPU_REG32 *)(CSP_PM_ADDR_PLL1 + 0x08))
#define  CSP_PM_REG_PLLFEED1                    (*(CPU_REG32 *)(CSP_PM_ADDR_PLL1 + 0x0C))

                                                                /* ---------- POWER CONTROL REGISTER DEFINES ---------- */
#define  CSP_PM_REG_PCON                        (*(CPU_REG32 *)(0x400FC0C0))
#define  CSP_PM_REG_PCONP                       (*(CPU_REG32 *)(0x400FC0C4))

                                                                /* -------- CLOCK SOURCE SELECT REGISTER DEFINE ------- */
#define  CSP_PM_REG_CLKSRCSEL                   (*(CPU_REG32 *)(0x400FC10Cu))

                                                                /* ---------- PLL STATUS REGISTER BIT DEFINES --------- */
#define  CSP_PM_MSK_PLLSTAT0_MSEL               DEF_BIT_FIELD(15,  0)
#define  CSP_PM_MSK_PLLSTAT0_PSEL               DEF_BIT_FIELD(8 , 16)
#define  CSP_PM_MSK_PLLSTAT0_PLLE               DEF_BIT_24      /* Read-back for the PLL enable bit.                    */
#define  CSP_PM_MSK_PLLSTAT0_PLLC               DEF_BIT_25      /* Read-back for the PLL connect bit.                   */
#define  CSP_PM_MSK_PLLSTAT0_PLOCK              DEF_BIT_26      /* Refkect the PLL lock status.                         */

#define  CSP_PM_MSK_PLLSTAT1_MSEL               DEF_BIT_FIELD(5,  0)
#define  CSP_PM_MSK_PLLSTAT1_PSEL               DEF_BIT_FIELD(2 , 5)
#define  CSP_PM_MSK_PLLSTAT1_PLLE               DEF_BIT_08      /* Read-back for the PLL enable bit.                    */
#define  CSP_PM_MSK_PLLSTAT1_PLLC               DEF_BIT_09      /* Read-back for the PLL connect bit.                   */
#define  CSP_PM_MSK_PLLSTAT1_PLOCK              DEF_BIT_10      /* Refkect the PLL lock status.                         */


                                                                /* --------- CLOCK DIVIDERS REGISTER DEFINITION ------- */
#define  CSP_PM_REG_CCLKCFG                     (*(CPU_REG32 *)(0x400FC104))
#define  CSP_PM_REG_USBCLKCFG                   (*(CPU_REG32 *)(0x400FC108))
#define  CSP_PM_REG_IRCTRIM                     (*(CPU_REG32 *)(0x400FC1A4))
#define  CSP_PM_REG_PCLKSEL0                    (*(CPU_REG32 *)(0x400FC1A8))
#define  CSP_PM_REG_PCLKSEL1                    (*(CPU_REG32 *)(0x400FC1AC))

                                                                /* ------ INTERNAL OSCILLATOR/CLOCK FREQUENCY ------- */
#define  CSP_PM_INT_RC_OSC_FREQ_HZ               4000000L       /* IRC frequency is 4Mhz.                             */
                                                                /* RTC oscillator frequency                           */
#define  CSP_PM_INT_RTC_OSC_FREQ_HZ              CSP_CFG_PM_SLOW_XTAL_FREQ_HZ


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
*                                            LOCAL MACRO's
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
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            CSP_PM_Init()
*
* Description : Initialize power management module.
*
* Argument(s) : none.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_PM_Init (void) 
{

} 


/*
*********************************************************************************************************
*                                         CSP_PM_CPU_ClkFreqGet()
*
* Description : Return the CPU clock frequency.
*                   
* Argument(s) : none
*  
* Return(s)   : The CPU frequency in Hertz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.              
*********************************************************************************************************
*/

CPU_INT32U  CSP_PM_CPU_ClkFreqGet (void)
{
    CPU_INT32U  cpu_freq;
    CPU_INT08U  cpu_div;
    CPU_INT16U  pll_mul;
    CPU_INT08U  pll_div;
    CPU_INT32U  reg_val;


    reg_val = CSP_PM_REG_CLKSRCSEL & DEF_BIT_FIELD(2u, 0u);
 
    switch (reg_val) {
        case 0u:
             cpu_freq = CSP_PM_INT_RC_OSC_FREQ_HZ;
             break;
    
        case 1u:
             cpu_freq = CSP_CFG_PM_MAIN_XTAL_FREQ_HZ;
             break;

        case 2u:
             cpu_freq = CSP_PM_INT_RTC_OSC_FREQ_HZ;
             break;
                  
        default:
             return ((CPU_INT32U)(0u));
    }
 
    pll_mul =   ((CSP_PM_REG_PLLSTAT0 & CSP_PM_MSK_PLLSTAT0_MSEL)          + 1u);
    pll_div = ((((CSP_PM_REG_PLLSTAT0 & CSP_PM_MSK_PLLSTAT0_PSEL)) >> 16u) + 1u);

    if (DEF_BIT_IS_SET(CSP_PM_REG_PLLSTAT0, (CSP_PM_MSK_PLLSTAT0_PLLE | 
                                             CSP_PM_MSK_PLLSTAT0_PLLC |
                                             CSP_PM_MSK_PLLSTAT0_PLOCK ))) {
        cpu_freq = ((cpu_freq * 2u * pll_mul) / pll_div);
    }
        
    cpu_div = CSP_PM_REG_CCLKCFG & DEF_BIT_FIELD(8u, 0u); 
    cpu_div++;
    
    cpu_freq /= cpu_div;
        
    return (cpu_freq);

}

/*
*********************************************************************************************************
*                                       CSP_PM_PerClkDivCfg()
*
* Description : Configure peripheral clock divider.
*                   
* Argument(s) : clk_nbr     Peripheral clock id (see note #1).
*  
*               clk_div     Peripheral clock divider.
*
* Return(s)   : DEF_OK,   If the clock was configured correctly.
*               DEF_FAIL, Otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Peripheral clk number are defined in 'csp_grp.h'.
*
*                       CSP_PER_CLK_NBR_XX  where 'XX' is the peripheral name.
*
*               (2) A divider of value '6' divider can be only used with the following clks:
*                             CSP_PM_PER_CLK_NBR_CAN1          
*                             CSP_PM_PER_CLK_NBR_CAN2          
*                             CSP_PM_PER_CLK_NBR_ACF              
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_PM_PerClkDivCfg (CSP_DEV_NBR    clk_nbr,
                                  CPU_SIZE_T     clk_div)

{
    CPU_INT08U  pclk_sel;
    CPU_SR_ALLOC();


                                                                /* ------------------- ARGUMENTS CHECKING ------------- */
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)         
    if (clk_nbr >= CSP_PM_PER_CLK_NBR_MAX) {
        return (DEF_FAIL);    
    }

    switch (clk_nbr) {
        case CSP_PM_PER_CLK_NBR_PCLKSEL_RSVD0:
        case CSP_PM_PER_CLK_NBR_PCLKSEL_RSVD1:
        case CSP_PM_PER_CLK_NBR_PCLKSEL_RSVD2:
        case CSP_PM_PER_CLK_NBR_PCLKSEL_RSVD3:
             return (DEF_FAIL);
       
        default:
             break;    
    }
#endif
    
    switch (clk_div) {
        case 1u:
             pclk_sel = 1u;
             break;
             
        case 2u:
             pclk_sel = 2u;
             break;
             
        case 4u:
             pclk_sel = 0u;
             break;
             
        case 6u:
             switch (clk_nbr) {                                 /* See note #2.                                         */
                 case CSP_PM_PER_CLK_NBR_CAN_01:
                 case CSP_PM_PER_CLK_NBR_CAN_02:
                 case CSP_PM_PER_CLK_NBR_ACF_00:
                      pclk_sel = 3u;
                      break;
                 
                 default:
                      return (DEF_FAIL);     
             }
             break;
             
        case 8u:                                                /* See note #2.                                         */
             switch (clk_nbr) {
                 case CSP_PM_PER_CLK_NBR_CAN_01:
                 case CSP_PM_PER_CLK_NBR_CAN_02:
                 case CSP_PM_PER_CLK_NBR_ACF_00:
                      return (DEF_FAIL);

                 default:
                      pclk_sel = 3u;
                      break;
              }

        default:
             return (DEF_FAIL);
    }
                  
    CPU_CRITICAL_ENTER();
    
    if (clk_nbr < 16u) {
        DEF_BIT_CLR(CSP_PM_REG_PCLKSEL0, DEF_BIT_FIELD(2u,       clk_nbr       * 2u));
        DEF_BIT_SET(CSP_PM_REG_PCLKSEL0, DEF_BIT_MASK(pclk_sel,  clk_nbr       * 2u));                
    } else {
        DEF_BIT_CLR(CSP_PM_REG_PCLKSEL1, DEF_BIT_FIELD(2u,      (clk_nbr - 16u) * 2u));
        DEF_BIT_SET(CSP_PM_REG_PCLKSEL1, DEF_BIT_MASK(pclk_sel, (clk_nbr - 16u) * 2u));                        
    }
    
    CPU_CRITICAL_EXIT();    
    
    return (DEF_OK);
}    


/*
*********************************************************************************************************
*                                          CSP_PM_PerClkEn()
*
* Description : Enable a peripheral clock.
*                   
* Argument(s) : clk_nbr     Peripheral clock number (see 'CSP_PM_PerClkDivCfg()' note #1).
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.               
*********************************************************************************************************
*/

void  CSP_PM_PerClkEn (CSP_DEV_NBR clk_nbr)
{
    CPU_SR_ALLOC();


    if (clk_nbr >= CSP_PM_PER_CLK_NBR_MAX) {
        return;    
    }
    
    switch (clk_nbr) {
        case CSP_PM_PER_CLK_NBR_PCONP_RSVD0:
        case CSP_PM_PER_CLK_NBR_PCONP_RSVD1:
        case CSP_PM_PER_CLK_NBR_PCONP_RSVD2:
        case CSP_PM_PER_CLK_NBR_PCONP_RSVD3:
        case CSP_PM_PER_CLK_NBR_PCONP_RSVD4:
             return;
       
        default:
             break;    
    }
    
    CPU_CRITICAL_ENTER();
    DEF_BIT_SET(CSP_PM_REG_PCONP, DEF_BIT(clk_nbr));
    CPU_CRITICAL_EXIT();    
}


/*
*********************************************************************************************************
*                                         CSP_PM_PerClkDis()
*
* Description : Disable a peripheral clock.
*                   
* Argument(s) : clk_nbr     Peripheral clock number (see 'CSP_PM_PerClkDivCfg()' note #1).
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_PM_PerClkDis (CSP_DEV_NBR clk_nbr)
{
    CPU_SR_ALLOC();


    if (clk_nbr >= CSP_PM_PER_CLK_NBR_MAX) {
        return;
    }
    
    switch (clk_nbr) {
        case CSP_PM_PER_CLK_NBR_PCONP_RSVD0:
        case CSP_PM_PER_CLK_NBR_PCONP_RSVD1:
        case CSP_PM_PER_CLK_NBR_PCONP_RSVD2:
        case CSP_PM_PER_CLK_NBR_PCONP_RSVD3:
        case CSP_PM_PER_CLK_NBR_PCONP_RSVD4:
             return;
       
        default:
             break;    
    }
    
    CPU_CRITICAL_ENTER();
    DEF_BIT_CLR(CSP_PM_REG_PCONP, DEF_BIT(clk_nbr));
    CPU_CRITICAL_EXIT();    
}


/*
*********************************************************************************************************
*                                        CSP_PM_PerClkFreqGet()
*
* Description : Return the Peripheral clock
*                   
* Argument(s) : clk_nbr     Peripheral clock number. (see 'CSP_PM_PerClkDivCfg()' note #1).
*
* Return(s)   : The peripheral clock frequency.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_INT32U  CSP_PM_PerClkFreqGet  (CSP_DEV_NBR  clk_nbr)
{
    CPU_INT32U  clk_freq;    
    CPU_INT08U  clk_div;
  

    if (clk_nbr >= CSP_PM_PER_CLK_NBR_MAX) {
        return (DEF_FAIL);    
    }

    switch (clk_nbr) {
        case CSP_PM_PER_CLK_NBR_PCLKSEL_RSVD0:
        case CSP_PM_PER_CLK_NBR_PCLKSEL_RSVD1:
        case CSP_PM_PER_CLK_NBR_PCLKSEL_RSVD2:
        case CSP_PM_PER_CLK_NBR_PCLKSEL_RSVD3:
             return (DEF_FAIL);
       
        default:
             break;    
    }
    
    if (clk_nbr < 16u) {
        clk_div = (CPU_INT08U)((CSP_PM_REG_PCLKSEL0 >> ( clk_nbr       * 2)) & DEF_BIT_FIELD(2, 0));
    } else {
        clk_div = (CPU_INT08U)((CSP_PM_REG_PCLKSEL0 >> ((clk_nbr - 16) * 2)) & DEF_BIT_FIELD(2, 0));    
    }

    switch (clk_div) {
        case 0u:
             clk_div = 4u;
             break;
             
        case 1u:
             clk_div = 1u;
             break;
             
        case 2u:
             clk_div = 2u;
             break;
             
        case 3u:
             switch (clk_nbr) {
                 case CSP_PM_PER_CLK_NBR_CAN_01:
                 case CSP_PM_PER_CLK_NBR_CAN_02:
                 case CSP_PM_PER_CLK_NBR_ACF_00:
                      clk_div = 6u;
                      break;
                      
                 default:
                      clk_div = 8u;
                      break;
             }
             break;
             
        default:
             return ((CPU_INT32U)0);
        
    }
    
    clk_freq  = CSP_PM_CPU_ClkFreqGet();
    clk_freq /= clk_div;
    
    return (clk_freq);
}


/*
*********************************************************************************************************
*                                        CSP_PM_SysClkEn()
*
* Description : Enable a System Clock.
*                   
* Argument(s) : clk_nbr     System clock number (see note #1)                            
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Sytem clk number are defined in 'csp_grp.h'.
*
*                       CSP_SYS_CLK_NBR_XX  where 'XX' is the system clk name.
*
*                (2) The USB clock is the only system system clock found on the LPC17xx family. This clock is 
*                   enabled and configured trougth the PLL1 or PLL 0 and the USB clock divider.
*                   Since these is part of the startup code, System clock functionality is not implemented for
*                   this family.
*********************************************************************************************************
*/

void  CSP_PM_SysClkEn (CSP_DEV_NBR  clk_nbr)
{
    (void)clk_nbr;
}


/*
*********************************************************************************************************
*                                        CSP_PM_SysClkDis()
*
* Description : Disable a System Clock
*                   
* Argument(s) : clk_nbr     System clock number (see 'CSP_PM_SysClkEn()' note #1)
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_PM_SysClkDis (CSP_DEV_NBR clk_nbr)
{
    (void)clk_nbr;
}


/*
*********************************************************************************************************
*                                         CSP_PM_SysClkDivCfg()
*
* Description : Configure peripheral clock divider.
*                   
* Argument(s) : clk_nbr     System clock number (see 'CSP_PM_SysClkEn()' note #1)
*  
*               clk_div     System clock divider.
*
* Return(s)   : DEF_OK,    If the clock was configured,
*               DEF_FAIL,  otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_PM_SysClkDivCfg (CSP_DEV_NBR  clk_nbr,
                                  CPU_SIZE_T   clk_div)
{
    (void)&clk_nbr;
    (void)&clk_div;
    
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                        CSP_PM_SysClkFreqGet()
*
* Description : Return system clock frequency.
*                   
* Argument(s) : clk_nbr     System clock number (see 'CSP_PM_SysClkEn()' note #1)
*
* Return(s)   : System clock frequency.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_INT32U  CSP_PM_SysClkFreqGet (CSP_DEV_NBR  clk_nbr)
{
    (void)clk_nbr;
    
    return ((CPU_INT32U)0);
}
