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
*                                         INTERRUPT MANAGEMENT
*                                            NXP  LPC17XX
*
* Filename      : csp_int.c
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

#define    CSP_INT_MODULE
#include   <csp.h>


/*
*********************************************************************************************************
*                                          DEFAULT CONFIGURATION
*********************************************************************************************************
*/

#ifndef  CSP_DEV_USB_OTG_EN
#define  CSP_DEV_USB_OTG_EN                  DEF_DISABLED
#endif


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
 
                                                                /* ------------ USB STATUS REGISTER DEFINES ----------- */
#define  CSP_INT_REG_USBINTST                   ( *(CPU_REG32 *)(0x400FC1C0))

#define  CSP_INT_BIT_USBINTST_INT_REQ_LP        DEF_BIT_00      /* USB Low priority request  (Device)                   */
#define  CSP_INT_BIT_USBINTST_INT_REQ_HP        DEF_BIT_01      /* USB High priority request (Device)                   */
#define  CSP_INT_BIT_USBINTST_INT_REQ_DMA       DEF_BIT_02      /* USB DMA Request (Device)                             */
#define  CSP_INT_BIT_USBINTST_INT_HOST          DEF_BIT_03      /* USB Host interrupt                                   */
#define  CSP_INT_BIT_USBINTST_INT_ATX           DEF_BIT_04      /* USB ATX interrupt                                    */
#define  CSP_INT_BIT_USBINTST_INT_OTG           DEF_BIT_05      /* USB OTG interrupt                                    */
#define  CSP_INT_BIT_USBINTST_INT_I2C           DEF_BIT_06      /* USB I2C interrupt                                    */
#define  CSP_INT_BIT_USBINTST_INT_NEED_CLK      DEF_BIT_08      /* USB Need clock interrupt                             */

                                                                /* USB Device interrupt bit-field                       */
#define  CSP_INT_BIT_USBINTST_INT_DEV          (CSP_INT_BIT_USBINTST_INT_REQ_LP  | \
                                                CSP_INT_BIT_USBINTST_INT_REQ_HP  | \
                                                CSP_INT_BIT_USBINTST_INT_REQ_DMA)

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                           LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                     INTERRUPT VECTOR TABLE
*
* Note(s) (1) 'CSP_MainVectTbl[]' store all the interrupts handlers for the external interrupts
*             on the LPC17xx devices.
*
*         (2) 'CSP_USB_VectTbl[]' store all the interrupts handlers for the shared USB global interrupt 
*              handler.
*
*              (a) USB shared handler is only enable for parts that have an OTG controller. 
*********************************************************************************************************
*/
                                                                /* NVIC external interrupts vector table               */
static  CSP_INT_VECT  CSP_MainVectTbl[CSP_INT_SRC_NBR_MAX]; 

#if (CSP_DEV_USB_OTG_EN == DEF_ENABLED)
                                                                /* USB Interrupt vector table                         */
static  CSP_INT_VECT  CSP_USB_VectTbl[CSP_INT_USB_SRC_MAX_NBR]; 
#endif

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

#if (CSP_DEV_USB_OTG_EN == DEF_ENABLED)
#define  CSP_INT_USB_DISPATCH(p_int_vect,      \
                              usb_stat,        \
                              usb_stat_msk,    \
                              usb_src_nbr)              {    if (DEF_BIT_IS_SET_ANY((usb_stat), (usb_stat_msk))) {        \
                                                                  (p_int_vect) = &CSP_USB_VectTbl[(usb_src_nbr)];         \
                                                                  CSP_IntVectDeref((p_int_vect));                         \
                                                             }                                                            \
                                                         }
#else
#define  CSP_INT_USB_DISPATCH(p_int_vect,      \
                              usb_stat,        \
                              usb_stat_msk,    \
                              usb_src_nbr)  
#endif

 


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if (CSP_DEV_USB_OTG_EN == DEF_ENABLED)
static  void  CSP_IntUSB_Handler    (void *p_arg);
#endif

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
*                                            CSP_IntClr()
*
* Description : Clear an interrupt source on a specific interrupt controller.
*
* Argument(s) : int_ctrl   Interrupt controller number (see note #1).
*
*               src_nbr    Interrupt source number (see note #2). 
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Interrupt controllers numbers identifiers can be found in 'csp_grp.h' 
* 
*                       CSP_INT_CTRL_NBR_XX'  where 'XX" is the name of the interrupt controller.
*
*               (2) Interrupt source identifiers can be found in 'csp_grp.h' as CSP_INT
*
*                       CSP_INT_SRC_NBR_XX      where XX is the peripheral or interrupt source name.
*                                               Main interrupt controller only.
* 
*                       CSP_INT_XX_SRC_NBR_YY   where xx is the name of the interrupt controller or shared 
*                                               handler and YY is the peripheral or source name.
* 
*               (3) The pending status can be only cleared  for the NVIC's external sources.
*
*                   (a) The pending status cannot be cleared in a source from a a shared interrupt 
*                       handler.
*********************************************************************************************************
*/

void  CSP_IntClr (CSP_DEV_NBR  int_ctrl,
                  CSP_DEV_NBR  src_nbr)                  
{
    if ((int_ctrl == CSP_INT_CTRL_NBR_MAIN) &&
        (src_nbr   < CSP_INT_SRC_NBR_MAX  )) {
        CPU_IntSrcPendClr(src_nbr + CPU_INT_EXT0);
    }
}


/*
*********************************************************************************************************
*                                          CSP_IntDis()
*
* Description : Disable an interrupt source from a specific interrupt controller.
*
* Argument(s) : int_ctrl   Interrupt controller number. (see 'CSP_IntClr()' note #1).
*
*               src_nbr    Interrupt source number. (see 'CSP_IntClr()' note #2).
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Only the NVIC's external sources can be disabled.
*********************************************************************************************************
*/

void  CSP_IntDis (CSP_DEV_NBR  int_ctrl,
                  CSP_DEV_NBR  src_nbr)                   
{   
    if ((int_ctrl == CSP_INT_CTRL_NBR_MAIN) &&
        (src_nbr   < CSP_INT_SRC_NBR_MAX  )) {
        CPU_IntSrcDis(src_nbr + CPU_INT_EXT0);
    }
}


/*
*********************************************************************************************************
*                                          CSP_IntDisAll()
*
* Description : Disable all interrupts on a specific interrupt controller.
*
* Argument(s) : int_ctrl   Interrupt controller number. (see 'CSP_IntClr()' note #1).
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_IntDisAll (CSP_DEV_NBR  int_ctrl) 
{
    CSP_DEV_NBR  int_src;
    CSP_DEV_NBR  int_src_grp;
   
    if (int_ctrl == CSP_INT_CTRL_NBR_MAIN) {
        int_src     = 0u;
        int_src_grp = 0u;
        while (int_src < CSP_INT_SRC_NBR_MAX) {
            CPU_REG_NVIC_CLREN(int_src_grp) = DEF_BIT_FIELD(32u, 0u);
            int_src_grp++;
            int_src += 32u;
        }
    }    
}


/*
*********************************************************************************************************
*                                          CSP_IntEn()
*
* Description : Enable an interrupt source on a specific interrupt controller.
*
* Argument(s) : int_ctrl   Interrupt controller number. (see 'CSP_IntClr()' note #1).
*
*               src_nbr    Interrupt source number. (see 'CSP_IntClr()' note #2).
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_IntEn (CSP_DEV_NBR  int_ctrl,
                 CSP_DEV_NBR  src_nbr)                 
{
    if ((int_ctrl == CSP_INT_CTRL_NBR_MAIN) &&
        (src_nbr   < CSP_INT_SRC_NBR_MAX  )) {
        CPU_IntSrcEn(src_nbr + CPU_INT_EXT0);
    }
}


/*
*********************************************************************************************************
*                                          CSP_IntInit()
*
* Description : Initiailze all interrupt controllers.
*                   (1) Disable all interrupts.
*                   (2) Clear all pending interrupts.
*                   (3) Initialize all vector interrupt tables.
*                   (4) Install all shared interrupt handlers. 
*
* Argument(s) : None.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_IntInit  (void)
{
    CPU_INT08U     src_nbr;
    CSP_INT_VECT  *p_int_vect;
    CPU_SR_ALLOC();
    
    
    p_int_vect = &CSP_MainVectTbl[0];

    for (src_nbr = 0u; src_nbr < CSP_INT_SRC_NBR_MAX; src_nbr++) {
        CPU_IntSrcDis(src_nbr + CPU_INT_EXT0);                           /* Disable external interrupts.                         */
        CPU_IntSrcPendClr(src_nbr + CPU_INT_EXT0);
        p_int_vect = &CSP_MainVectTbl[src_nbr];

        CPU_CRITICAL_ENTER();     
        CSP_IntVectClr(p_int_vect);                             /* Initialize main vector table entry.                  */
        CPU_CRITICAL_EXIT();        
    }

#if (CSP_DEV_USB_OTG_EN == DEF_ENABLED)    
    for (src_nbr = 0u; src_nbr < CSP_INT_USB_SRC_MAX_NBR; src_nbr++ ) {
        
        p_int_vect = &CSP_USB_VectTbl[src_nbr];

        CPU_CRITICAL_ENTER();                                   /* Initialize USB shared handler the vector table.      */
        CSP_IntVectClr(p_int_vect);
        CPU_CRITICAL_EXIT();
    }
#endif    
                                                                /* Install the USB shared handler.                      */
    src_nbr    = CSP_INT_SRC_NBR_USB_00;
    p_int_vect = &CSP_MainVectTbl[src_nbr];

    CPU_CRITICAL_ENTER();    
    CSP_IntVectSet((CSP_INT_VECT *)p_int_vect,
                   (CPU_FNCT_PTR  )CSP_IntUSB_Handler,
                   (void *        )0);    
    CPU_CRITICAL_EXIT();       
}


/*
*********************************************************************************************************
*                                          CSP_IntUSB_Handler()
*
* Description : USB ISR handler.            
*
* Argument(s) : p_arg    Interrupt ISR argument (not used)
*
* Return(s)   : None.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : (1) This funtion dispatch a Interrupt service routine for everyinterrupt line of 
*                   the NVIC's USB channel.
*********************************************************************************************************
*/

#if (CSP_DEV_USB_OTG_EN == DEF_ENABLED)
static  void  CSP_IntUSB_Handler (void  *p_arg)
{
    CSP_INT_VECT   *p_int_vect;
    CPU_INT32U      usb_int_stat;

    
    p_arg        = p_arg;    
    usb_int_stat = CSP_INT_REG_USBINTST;

    CSP_INT_USB_DISPATCH(p_int_vect,
                         usb_int_stat,
                         CSP_INT_BIT_USBINTST_INT_DEV, 
                         CSP_INT_USB_SRC_NBR_DEV);

    CSP_INT_USB_DISPATCH(p_int_vect,
                         usb_int_stat,
                         CSP_INT_BIT_USBINTST_INT_HOST, 
                         CSP_INT_USB_SRC_NBR_HOST);

    CSP_INT_USB_DISPATCH(p_int_vect,
                         usb_int_stat,
                         CSP_INT_BIT_USBINTST_INT_ATX, 
                         CSP_INT_USB_SRC_NBR_ATX);

    CSP_INT_USB_DISPATCH(p_int_vect,
                         usb_int_stat,
                         CSP_INT_BIT_USBINTST_INT_OTG, 
                         CSP_INT_USB_SRC_NBR_OTG);

    CSP_INT_USB_DISPATCH(p_int_vect,
                         usb_int_stat,
                         CSP_INT_BIT_USBINTST_INT_I2C, 
                         CSP_INT_USB_SRC_NBR_I2C);
}
#endif


/*
*********************************************************************************************************
*                                          CSP_IntSrcCfg()
*
* Description : Configure an interrupt source.
*
* Argument(s) : int_ctrl   Interrupt controller number. (see 'CSP_IntClr()' note #1).
*
*               src_nbr    Interrupt source number. (see 'CSP_IntClr()' note #2).
*
*               src_prio   Interrupt source priority.
*
*               src_pol    Interrupt source polarity:
*                              CSP_INT_POL_LEVEL_HIGH           Interrupt is activated on a high level.
*                              CSP_INT_POL_LEVEL_LOW            Interrupt is activated on a low level.
* 
*                              CSP_INT_POL_EDGE_RISING          Interrupt is activated on the rising edge.
*                              CSP_INT_POL_EDGE_FALLING         Interrupt is activated on the falling edge.
*                              CSP_INT_POL_EDGE_BOTH            Interrupt is activated on both edges.       
*
*
* Return(s)   : DEF_OK,    if the interrupt source was configured
*               DEF_FAIL,  otherwise.
*
* Note(s)     : (1) Interrupt source priority is determined by the interrupt controller implementation.
*                   The maximum and minimum values are defined in the family definition file 'csp_grp.h'
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_IntSrcCfg (CSP_DEV_NBR  int_ctrl,
                            CSP_DEV_NBR  src_nbr,
                            CSP_OPT      src_prio,
                            CSP_OPT      src_pol)

{
    CPU_BOOLEAN  err;
    
    err = DEF_FAIL;
    
    if ((int_ctrl == CSP_INT_CTRL_NBR_MAIN) &&
        (src_nbr   < CSP_INT_SRC_NBR_MAX  ) && 
        (src_prio  < 0xFF                 )) {
        CPU_IntSrcPrioSet(src_nbr + 16u, CPU_INT_EXT0);
        err = DEF_OK;
    }
    
    return (err);
}


/*
*********************************************************************************************************
*                                          CSP_IntVectReg()
*
* Description : Register an interrupt service routine handler for a specific interrupt controller.
*
* Argument(s) : int_ctrl    Interrupt controller number. (see 'CSP_IntClr()' note #1).
*
*               src_nbr     Interrupt source number. (see 'CSP_IntClr()' note #2).
*
*               isr_fnct    Pointer to the interrupt service routine (ISR).
*
*               p_arg       Pointer to the argument of the interrupt service routine (ISR).
*
* Return(s)   : DEF_OK,    If the vector was registered successfully.
*               DEF_FAIL,  otherwise.
*               
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_IntVectReg (CSP_DEV_NBR    int_ctrl,
                             CSP_DEV_NBR    src_nbr,
                             CPU_FNCT_PTR   isr_fnct,
                             void          *p_arg)
{
    CSP_INT_VECT  *p_int_vect;
    CPU_SR_ALLOC();

#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)    
    if (isr_fnct == (CPU_FNCT_PTR)0) {
        return (DEF_FAIL);
    }
    
    if ((int_ctrl == CSP_INT_CTRL_NBR_MAIN) &&
        (src_nbr  >= CSP_INT_SRC_NBR_MAX  )) {
        return (DEF_FAIL);
    }

#if (CSP_DEV_USB_OTG_EN == DEF_ENABLED)
    if ((int_ctrl == CSP_INT_CTRL_NBR_USB) &&
        (src_nbr  >= CSP_INT_SRC_NBR_MAX  )) {
        return (DEF_FAIL);
    }
#endif
#endif    
    
    switch (int_ctrl) {
        case CSP_INT_CTRL_NBR_MAIN:                             /* Install interrupt ISR in the NVIC's vector table.    */
             p_int_vect = &CSP_MainVectTbl[src_nbr];
             break;

#if (CSP_DEV_USB_OTG_EN == DEF_ENABLED)
        case CSP_INT_CTRL_NBR_USB:                              /* Install interrupt ISR in the USB's vector table.     */
             p_int_vect = &CSP_USB_VectTbl[src_nbr];
             break;
#endif
 
        default:
             return (DEF_FAIL);
    }

    CPU_CRITICAL_ENTER();
    
    CSP_IntVectSet((CSP_INT_VECT *)p_int_vect,
                   (CPU_FNCT_PTR  )isr_fnct,
                   (void *        )p_arg);

    CPU_CRITICAL_EXIT();
    
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                          CSP_IntVectUnreg()
*
* Description : Un-register an interrupt vector.
*
* Argument(s) : int_ctrl   Interrupt controller number. (see 'CSP_IntClr()' note #1).
*
*               src_nbr    Interrupt source number. (see 'CSP_IntClr()' note #2).
*
* Return(s)   : DEF_OK,     If the vector was unregistered successfully.
*               DEF_FAIL,   otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_IntVectUnreg (CSP_DEV_NBR  int_ctrl,
                               CSP_DEV_NBR  src_nbr)                         
{
    CSP_INT_VECT  *p_int_vect;
    CPU_SR_ALLOC();


#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)        
    if ((int_ctrl == CSP_INT_CTRL_NBR_MAIN) &&
        (src_nbr  >= CSP_INT_SRC_NBR_MAX  )) {
        return (DEF_FAIL);
    }

#if (CSP_DEV_USB_OTG_EN == DEF_ENABLED)
    if ((int_ctrl == CSP_INT_CTRL_NBR_USB) &&
        (src_nbr  >= CSP_INT_SRC_NBR_MAX  )) {
        return (DEF_FAIL);
    }
#endif
#endif    
    
    switch (int_ctrl) {
        case CSP_INT_CTRL_NBR_MAIN:                             /* Install interrupt ISR in the NVIC's vector table.    */
             p_int_vect = &CSP_MainVectTbl[src_nbr];
             break;

#if (CSP_DEV_USB_OTG_EN == DEF_ENABLED)
        case CSP_INT_CTRL_NBR_USB:                              /* Install interrupt ISR in the USB's vector table.     */
             p_int_vect = &CSP_USB_VectTbl[src_nbr];
             break;
#endif
 
        default:
             return (DEF_FAIL);
    }

    CPU_CRITICAL_ENTER();
    
    CSP_IntVectClr(p_int_vect);                  
                  
    CPU_CRITICAL_EXIT();
    
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                          CSP_IntHandler()
*
* Description : Global interrupt handler.
*
* Argument(s) : None.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_IntHandler (void)
{
    CSP_INT_VECT  *p_int_vect;
    CSP_DEV_NBR    src_nbr;        
    CPU_INT32U     reg_val;

                                                                /* Query the NVIC interrupt controller                 */
    reg_val  = (CPU_REG_NVIC_ICSR & CPU_MSK_NVIC_ICSR_VECT_ACTIVE);
    src_nbr  = (CSP_DEV_NBR)reg_val -  CPU_INT_EXT0;
    
    if (src_nbr < CSP_INT_SRC_NBR_MAX) {
        p_int_vect = &CSP_MainVectTbl[src_nbr];

        CSP_IntVectDeref(p_int_vect);                           /* Call Dereferencing funtion                          */
    }
}


/*
*********************************************************************************************************
*                                       CSP_IntHandlerSrc()
*
* Description : Global interrupt handler.
*
* Argument(s) : src_nbr      Interrupt source number.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_IntHandlerSrc (CSP_DEV_NBR  src_nbr)
{
    CSP_INT_VECT  *p_int_vect;

    
    if (src_nbr < CSP_INT_SRC_NBR_MAX) {
        p_int_vect = &CSP_MainVectTbl[src_nbr];

        CSP_IntVectDeref(p_int_vect);                           /* Call Dereferencing funtion                          */
    }
}
