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
*                                          TIMER MANAGEMENT
*                                            NXP LPC17xx 
*
* Filename      : csp_tmr.c
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

#define  CSP_TMR_MODULE
#include  <csp.h>

/*
*********************************************************************************************************
*                                    REGISTER & BIT-FIELD DEFINITION
*********************************************************************************************************
*/

#define  CSP_TMR_ADDR_TMR0                  (CPU_INT32U)(0x40004000)
#define  CSP_TMR_ADDR_TMR1                  (CPU_INT32U)(0x40008000)
#define  CSP_TMR_ADDR_TMR2                  (CPU_INT32U)(0x40090000)
#define  CSP_TMR_ADDR_TMR3                  (CPU_INT32U)(0x40094000)


#define  CSP_TMR_BIT_CR_START                   DEF_BIT_00
#define  CSP_TMR_BIT_CR_RST                     DEF_BIT_01

#define  CSP_TMR_MASK_TCR_MODE                  DEF_BIT_FIELD(2, 0)
#define  CSP_TMR_BIT_TCR_TMR_MODE               DEF_BIT_NONE


#define  CSP_TMR_MASK_MCR_MR                    DEF_BIT_FIELD(3, 0)

#define  CSP_TMR_MASK_MCR_MRx(match_nbr)        DEF_BIT_MASK(CSP_TMR_MASK_MCR_MR, 3u * (match_nbr))


                                                                /* --------------- MATCH CONTROL REGISTER ------------- */
#define  CSP_TMR_BIT_MCR_MRI                    DEF_BIT_00      /* Interrupt on match.                                  */
#define  CSP_TMR_BIT_MCR_MRR                    DEF_BIT_01      /* Reset on match.                                      */
#define  CSP_TMR_BIT_MCR_MRS                    DEF_BIT_02      /* Stop on match.                                       */

#define  CSP_TMR_BIT_MCR_MRIx(match_nbr)        DEF_BIT_MASK(CSP_TMR_BIT_MCR_MRI, 3u * (match_nbr))
#define  CSP_TMR_BIT_MCR_MRRx(match_nbr)        DEF_BIT_MASK(CSP_TMR_BIT_MCR_MRR, 3u * (match_nbr))
#define  CSP_TMR_BIT_MCR_MRSx(match_nbr)        DEF_BIT_MASK(CSP_TMR_BIT_MCR_MRS, 3u * (match_nbr))


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

typedef  struct  csp_tmr_reg {
    CPU_REG32  IR;
    CPU_REG32  TCR;
    CPU_REG32  TC;
    CPU_REG32  PR;
    CPU_REG32  PC;
    CPU_REG32  MCR;
    CPU_REG32  MRx[4];
    CPU_REG32  CCR;
    CPU_REG32  CCRx[2];
    CPU_REG32  RESERVED0[2];
    CPU_REG32  EMR;
    CPU_REG32  RESERVED1[12];
} CSP_TMR_REG;


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

static  const  CPU_ADDR     CSP_TmrAddrTbl[4] = {
    CSP_TMR_ADDR_TMR0,
    CSP_TMR_ADDR_TMR1,
    CSP_TMR_ADDR_TMR2,
    CSP_TMR_ADDR_TMR3
};

static  const  CSP_DEV_NBR  CSP_TmrPerTbl[4] = {
    CSP_PM_PER_CLK_NBR_TMR_00,
    CSP_PM_PER_CLK_NBR_TMR_01,
    CSP_PM_PER_CLK_NBR_TMR_02,
    CSP_PM_PER_CLK_NBR_TMR_03
};


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
*                                        CSP_TmrInit()
*
* Description : Initialize & disable all the timers.
*                   
* Argument(s) : none.
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_TmrInit (void)
{
    CSP_DEV_NBR  per_nbr;
    CSP_DEV_NBR  tmr_nbr;
    CSP_TMR_REG  *p_tmr_reg;
    CPU_SR_ALLOC();


    for (tmr_nbr = CSP_TMR_NBR_00; tmr_nbr <= CSP_TMR_NBR_03; tmr_nbr++) {
        p_tmr_reg = (CSP_TMR_REG *)CSP_TmrAddrTbl[tmr_nbr];
        per_nbr   = (CSP_DEV_NBR  )CSP_TmrPerTbl[tmr_nbr];
        
        CPU_CRITICAL_ENTER();
        CSP_PM_PerClkEn(per_nbr);
        p_tmr_reg->MCR    = DEF_BIT_NONE;
        p_tmr_reg->MRx[0] = 0u;
        p_tmr_reg->MRx[1] = 0u;
        p_tmr_reg->MRx[2] = 0u;
        p_tmr_reg->IR     = DEF_BIT_FIELD(5u, 0u);
        p_tmr_reg->TCR    = DEF_BIT_NONE;        
        p_tmr_reg->MCR    = DEF_BIT_NONE;
        p_tmr_reg->EMR    = DEF_BIT_NONE;
        CSP_PM_PerClkDis(per_nbr);        
        CPU_CRITICAL_EXIT();
    }
}

/*
*********************************************************************************************************
*                                        CSP_TmrCfg()
*
* Description : Configure a specific periodic/free running timer 
*                   
* Argument(s) : tmr_nbr    Timer number identifier (see note #1).
*
*               freq       Periodic timer frequency. (see note #2)
*                          
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : (1) If the timer is configured to be used in periodic mode the interrupt handler needs
*                   to be installed first in the interrupt controller and the interrupt needs to be enabled
*                   before calling this function.
*
*               (2) If 'freq' == 0 Timer is configured as free-running timer.
*                   If 'freq' > 0  Timer will be configured to generate a interrupt event every
*                   (1/freq) seconds.
*
*               (3) MATCH 0 register is always used to generate periodic events.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_TmrCfg (CSP_DEV_NBR   tmr_nbr,
                         CPU_INT32U    freq)
{
    CSP_TMR_REG  *p_tmr_reg;
    CPU_INT32U    match_val;
    CPU_INT32U    per_freq;
    CSP_DEV_NBR   per_nbr;    
    CPU_SR_ALLOC();
    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)     
    if (tmr_nbr > CSP_TMR_NBR_03) {
        return (DEF_FAIL);
    }
#endif

    
    p_tmr_reg = (CSP_TMR_REG *)CSP_TmrAddrTbl[tmr_nbr];
    per_nbr   = (CSP_DEV_NBR  )CSP_TmrPerTbl[tmr_nbr];    
    
    if (freq == 0u) {                                           /* -------- FREE RUNNING TIMER CONFIGURATION --------- */
        CSP_PM_PerClkEn(per_nbr);                               /* Enable peripheral clock                             */                                                               
        
        CPU_CRITICAL_ENTER();        
        p_tmr_reg->MCR = DEF_BIT_NONE;                          /* Disable theInterrupt, Reset and Stop feauture on    */
                                                                /* ... Match channels.                                 */
        
        DEF_BIT_CLR(p_tmr_reg->TCR, CSP_TMR_MASK_TCR_MODE);     /* Clear the Mode.                                     */
        DEF_BIT_SET(p_tmr_reg->TCR, CSP_TMR_BIT_TCR_TMR_MODE);  /* Set the Timer Mode.                                 */
        
        p_tmr_reg->PR  = 0u;                                    /* Set the prescaler to 0.                             */
        p_tmr_reg->TCR = CSP_TMR_BIT_CR_RST;                    /* Reset the timer.                                    */
        CPU_CRITICAL_EXIT();

    } else {                                                    /* ---------- PERIODIC TIMER CONFIGURATION ----------- */
        per_freq = CSP_PM_PerClkFreqGet(per_nbr);               /* Get the peripheral clock.                           */

        if (freq > per_freq) {
            return (DEF_FAIL);
        }

        match_val = ((2u * per_freq / freq) + 1u) / 2u;
        if (match_val == 0u) {
            return (DEF_FAIL);
        }
        
        
        CSP_PM_PerClkEn(per_nbr);                               /* Enable peripheral clock.                           */                                                               
        CPU_CRITICAL_ENTER();
        DEF_BIT_CLR(p_tmr_reg->EMR, DEF_BIT_MASK(3u, (2u * 0u) + 4u));

                                                                /* Disable the Interrupt, Reset and Stop features on   */        
                                                                /* ... Match channel 0, 1, 2 or 3.                     */
        DEF_BIT_CLR(p_tmr_reg->MCR, CSP_TMR_MASK_MCR_MRx(0u));
                                    
        DEF_BIT_CLR(p_tmr_reg->TCR, CSP_TMR_MASK_TCR_MODE);     /* Clear the Mode                                      */
        DEF_BIT_SET(p_tmr_reg->TCR, CSP_TMR_BIT_TCR_TMR_MODE);  /* Set the Timer Mode                                  */

        p_tmr_reg->PR     = 0u;                                 /* Set the prescaler to 0                              */
        p_tmr_reg->MRx[0] = match_val;
        p_tmr_reg->TCR    = CSP_TMR_BIT_CR_RST;                 /* Reset the timer                                     */
                                                                /* Enable Interrupt and Reset when TC matches MR0, ... */
                                                                /* MR1, MR2, etc                                       */        
        DEF_BIT_SET(p_tmr_reg->MCR, CSP_TMR_BIT_MCR_MRIx(0u) | 
                                    CSP_TMR_BIT_MCR_MRRx(0u));
        CPU_CRITICAL_EXIT();
    }

    return (DEF_OK);
}



/*
*********************************************************************************************************
*                                           CSP_TmrOutCmpCfg()
*
* Description : Configure a timer for compare mode.  
*                   
* Argument(s) : tmr_nbr    Timer number identifier.
*                              CSP_TMR_NBR_00
*                              CSP_TMR_NBR_01
*                              CSP_TMR_NBR_02
*                              CSP_TMR_NBR_03
*
*               pin_nbr    Pin number.
*
*               pin_action  Output compare pin action
*
*                              CSP_TMR_OPT_PIN_OUT_NONE      Do nothing.
*                              CSP_TMR_OPT_PIN_OUT_CLR       Clear  the corresponding external pin for output compare.
*                              CSP_TMR_OPT_PIN_OUT_SET       Set    the corresponding external pin for output compare.
*                              CSP_TMR_OPT_PIN_OUT_TOGGLE    Toggle the corresponding external pin for output compare.
* 
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : (1) The LPC17xx series timers implements 4 match register and 4 externals outputs.
*********************************************************************************************************
*/

CPU_BOOLEAN CSP_TmrOutCmpCfg  (CSP_DEV_NBR    tmr_nbr,
                               CSP_DEV_NBR    pin,
                               CSP_OPT        pin_action,
                               CPU_INT32U     freq)

{
    CSP_TMR_REG  *p_tmr_reg;
    CPU_INT08U    match_nbr;
    CPU_INT32U    match_val;
    CPU_INT32U    per_freq;
    CSP_DEV_NBR   per_nbr;
    CPU_SR_ALLOC();
    
                                                                /* ---------------- ARGUMENTS CHECKING -------------- */
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (tmr_nbr > CSP_TMR_NBR_01) {
        return (DEF_FAIL);
    }
    
    if (pin > CSP_TMR_PIN_OUT_NBR_03) {
        return (DEF_FAIL);
    }

    if (freq == 0u) {
        return (DEF_FAIL);
    }
#endif
    
    p_tmr_reg = (CSP_TMR_REG *)CSP_TmrAddrTbl[tmr_nbr];
    per_nbr   = (CSP_DEV_NBR  )CSP_TmrPerTbl[tmr_nbr];    
    
    match_nbr = pin,
    per_freq  = CSP_PM_PerClkFreqGet(per_nbr);                  /* Get the peripheral clock.                           */

    if (freq > per_freq) {
        return (DEF_FAIL);
    }

    match_val = ((2u * per_freq / freq) + 1u) / 2u;
    if (match_val == 0u) {
        return (DEF_FAIL);
    }
    
    
    CSP_PM_PerClkEn(per_nbr);                                   /* Enable Peripherical clock.                          */                                                               
    CPU_CRITICAL_ENTER();

    DEF_BIT_CLR(p_tmr_reg->EMR, DEF_BIT_MASK(3u, (2u * match_nbr) + 4u));
        
    switch (pin_action) {
        case CSP_TMR_OPT_PIN_OUT_NONE:
             break;
                              
        case CSP_TMR_OPT_PIN_OUT_CLR:
             DEF_BIT_SET(p_tmr_reg->EMR, DEF_BIT_MASK(1u, (2u * match_nbr) + 4u));
             break;
                         
        case CSP_TMR_OPT_PIN_OUT_SET:
             DEF_BIT_SET(p_tmr_reg->EMR, DEF_BIT_MASK(2u, (2u * match_nbr) + 4u));
             break;
        
        case CSP_TMR_OPT_PIN_OUT_TOGGLE:
             DEF_BIT_SET(p_tmr_reg->EMR, DEF_BIT_MASK(3u, (2u * match_nbr) + 4u));
             break;
             
        default:
             CPU_CRITICAL_EXIT();
             return (DEF_FAIL);            
    }
                                                            /* Disable the Interrupt, Reset and Stop features on   */        
                                                            /* ... Match channel 0, 1, 2 or 3.                     */
    DEF_BIT_CLR(p_tmr_reg->MCR, CSP_TMR_MASK_MCR_MRx(match_nbr));
                                
    DEF_BIT_CLR(p_tmr_reg->TCR, CSP_TMR_MASK_TCR_MODE);     /* Clear the Mode                                      */
    DEF_BIT_SET(p_tmr_reg->TCR, CSP_TMR_BIT_TCR_TMR_MODE);  /* Set the Timer Mode                                  */

    p_tmr_reg->PR             = 0u;                         /* Set the prescaler to 0                              */
    p_tmr_reg->MRx[match_nbr] = match_val;
    p_tmr_reg->TCR            = CSP_TMR_BIT_CR_RST;         /* Reset the timer                                     */
                                                            /* Enable Interrupt and Reset when TC matches MR0, ... */
                                                            /* MR1, MR2, etc                                       */
    DEF_BIT_SET(p_tmr_reg->MCR, CSP_TMR_BIT_MCR_MRIx(match_nbr) | 
                                CSP_TMR_BIT_MCR_MRRx(match_nbr));
    CPU_CRITICAL_EXIT();

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                           CSP_TmrIntClr()
*
* Description : Clear a periodic timer interrupt.
*                   
* Argument(s) : tmr_nbr    Timer number identifier.
*                              CSP_TMR_NBR_00
*                              CSP_TMR_NBR_01
*                              CSP_TMR_NBR_02
*                              CSP_TMR_NBR_03
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_TmrIntClr (CSP_DEV_NBR tmr_nbr)
{
    CSP_TMR_REG  *p_tmr_reg;
    CSP_DEV_NBR   match_nbr;
    CPU_INT32U    reg_msk;
    CPU_INT32U    reg_stat;
    CPU_INT32U    reg_clr;
    

#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)         
    if (tmr_nbr > CSP_TMR_NBR_03) {
        return;
    }
#endif
    
    p_tmr_reg = (CSP_TMR_REG *)CSP_TmrAddrTbl[tmr_nbr];
    reg_clr   = DEF_BIT_NONE;
    reg_msk   = p_tmr_reg->MCR;
    reg_stat  = p_tmr_reg->IR;
    
    for (match_nbr =  CSP_TMR_MATCH_NBR_00; match_nbr < CSP_TMR_MATCH_NBR_03; match_nbr++) {
        if ((DEF_BIT_IS_SET(reg_msk,  CSP_TMR_BIT_MCR_MRIx(match_nbr)) == DEF_YES) &&
            (DEF_BIT_IS_SET(reg_stat, DEF_BIT(match_nbr))              == DEF_YES)) {
            DEF_BIT_SET(reg_clr, DEF_BIT(match_nbr));
        }
    }
        
    p_tmr_reg->IR = reg_clr;
}

/*
*********************************************************************************************************
*                                        CSP_TmrRst()
*
* Description : Resets a timer
*                   
* Argument(s) : tmr_nbr    Timer number identifier.
*                              CSP_TMR_NBR_00
*                              CSP_TMR_NBR_01
*                              CSP_TMR_NBR_02
*                              CSP_TMR_NBR_03
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_TmrRst (CSP_DEV_NBR  tmr_nbr)
{
    CSP_TMR_REG  *p_tmr_reg;
    CPU_SR_ALLOC();
    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (tmr_nbr > CSP_TMR_NBR_03) {
        return;    
    }
#endif
    
    p_tmr_reg = (CSP_TMR_REG *)CSP_TmrAddrTbl[tmr_nbr];
    
    CPU_CRITICAL_ENTER();
    p_tmr_reg->TCR = CSP_TMR_BIT_CR_RST;
    CPU_CRITICAL_EXIT();       
}


/*
*********************************************************************************************************
*                                          CSP_TmrRd()
*
* Description : Read the current value of a timer.
*                   
* Argument(s) : tmr_nbr    Timer number identifier.
*                              CSP_TMR_NBR_00
*                              CSP_TMR_NBR_01
*                              CSP_TMR_NBR_02
*                              CSP_TMR_NBR_03
*
* Return(s)   : The current value of the timer.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CSP_TMR_VAL  CSP_TmrRd  (CSP_DEV_NBR  tmr_nbr)
{
    CSP_TMR_VAL  tmr_val;
    CSP_TMR_REG *p_tmr_reg;

#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (tmr_nbr > CSP_TMR_NBR_03) {
        return ((CSP_TMR_VAL)(0u));
    }
#endif
    
    p_tmr_reg = (CSP_TMR_REG *)CSP_TmrAddrTbl[tmr_nbr];
          
    tmr_val = p_tmr_reg->TC;
    return (tmr_val);
}

/*
*********************************************************************************************************
*                                     CSP_TmrStart()
*
* Description : Start a timer.
*                   
* Argument(s) : tmr_nbr    Timer number identifier.
*                              CSP_TMR_NBR_00
*                              CSP_TMR_NBR_01
*                              CSP_TMR_NBR_02
*                              CSP_TMR_NBR_03
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_TmrStart (CSP_DEV_NBR  tmr_nbr)
{
    CSP_TMR_REG *p_tmr_reg;    
    CPU_SR_ALLOC();


#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (tmr_nbr > CSP_TMR_NBR_03) {
        return;
    }
#endif
    
    p_tmr_reg = (CSP_TMR_REG *)CSP_TmrAddrTbl[tmr_nbr];
    
    CPU_CRITICAL_ENTER();
    p_tmr_reg->TCR = CSP_TMR_BIT_CR_START;
    CPU_CRITICAL_EXIT();    
}


/*
*********************************************************************************************************
*                                        CSP_TmrStop()
*
* Description : Stop a Timer.
*                   
* Argument(s) : tmr_nbr    Timer number identifier.
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_TmrStop (CSP_DEV_NBR  tmr_nbr)
{
    CSP_TMR_REG *p_tmr_reg;    
    CPU_SR_ALLOC();
    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (tmr_nbr > CSP_TMR_NBR_03) {
        return;
    }
#endif
    
    p_tmr_reg = (CSP_TMR_REG *)CSP_TmrAddrTbl[tmr_nbr];
    
    CPU_CRITICAL_ENTER();
    p_tmr_reg->TCR = 0u;
    CPU_CRITICAL_EXIT();    
}

/*
*********************************************************************************************************
*                                         CSP_TmrWr()
*
* Description : Write a value to the timer.
*                   
* Argument(s) : tmr_nbr    Timer number identifier.
*
*               tmr_val    value to write.
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_TmrWr  (CSP_DEV_NBR  tmr_nbr,
                  CSP_TMR_VAL  tmr_val)
{
    CSP_TMR_REG *p_tmr_reg;    
    CPU_SR_ALLOC();
    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (tmr_nbr > CSP_TMR_NBR_03) {
        return;
    }
#endif
    
    p_tmr_reg = (CSP_TMR_REG *)CSP_TmrAddrTbl[tmr_nbr];
    
    CPU_CRITICAL_ENTER();
    p_tmr_reg->TCR = tmr_val;
    CPU_CRITICAL_EXIT();    
}
