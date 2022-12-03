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
*                                  GENERAL PURPOSE I/O CONTROLLER
*                                            NXP  LPC17XX
* Filename      : csp_gio.c
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

#define    CSP_GPIO_MODULE
#include   <csp.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  CSP_GPIO_PORT_MAX_NBR                            5u    /* LPC17xx maximum number of ports                    */

                                                                /* ------ LPC17xx GPIO REGISTER ADDRESS DEFINES ----- */
#define  CSP_GPIO_ADDR_FIO                      (CPU_ADDR  )(0x2009C000)
#define  CSP_GPIO_ADDR_PINSEL                   (CPU_ADDR  )(0x4002C000)
#define  CSP_GPIO_ADDR_PINMODE                  (CPU_ADDR  )(0x4002C040)
#define  CSP_GPIO_ADDR_INT                      (CPU_ADDR  )(0x40028080)

#define  CSP_GPIO_REG_EXT_INT                   (*(CPU_REG32 *)(0x400FC140))
#define  CSP_GPIO_REG_EXT_MODE                  (*(CPU_REG32 *)(0x400FC148))
#define  CSP_GPIO_REG_EXT_POLAR                 (*(CPU_REG32 *)(0x400FC14C))

#define  CSP_GPIO_REG_FIODIRx(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_FIO     + (     (port_nbr) * 32u) + 0x00))
#define  CSP_GPIO_REG_FIOMASKx(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_FIO     + (     (port_nbr) * 32u) + 0x10))
#define  CSP_GPIO_REG_FIOPINx(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_FIO     + (     (port_nbr) * 32u) + 0x14))
#define  CSP_GPIO_REG_FIOSETx(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_FIO     + (     (port_nbr) * 32u) + 0x18))
#define  CSP_GPIO_REG_FIOCLRx(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_FIO     + (     (port_nbr) * 32u) + 0x1C))

#define  CSP_GPIO_REG_PINSELLx(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PINSEL  + (2u * (port_nbr) *  4u) + 0x00))
#define  CSP_GPIO_REG_PINSELHx(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_PINSEL  + (2u * (port_nbr) *  4u) + 0x04))

#define  CSP_GPIO_REG_PINMODELx(port_nbr)       (*(CPU_REG32 *)(CSP_GPIO_ADDR_PINMODE + (2u * (port_nbr) *  4u) + 0x00))
#define  CSP_GPIO_REG_PINMODEHx(port_nbr)       (*(CPU_REG32 *)(CSP_GPIO_ADDR_PINMODE + (2u * (port_nbr) *  4u) + 0x04))

#define  CSP_GPIO_REG_PINMODE_ODx(port_nbr)     (*(CPU_REG32 *)(CSP_GPIO_ADDR_PINMODE + (     (port_nbr) *  4u) + 0x14))


#define  CSP_GPIO_REG_INT_STAT                  (*(CPU_REG32 *)(CSP_GPIO_ADDR_INT))
#define  CSP_GPIO_REG_INT_STAT_R(port_nbr)      (*(CPU_REG32 *)(CSP_GPIO_ADDR_INT     + (     (port_nbr) * 16u) + 0x04))
#define  CSP_GPIO_REG_INT_STAT_F(port_nbr)      (*(CPU_REG32 *)(CSP_GPIO_ADDR_INT     + (     (port_nbr) * 16u) + 0x08))
#define  CSP_GPIO_REG_INT_CLR(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_INT     + (     (port_nbr) * 16u) + 0x0C))
#define  CSP_GPIO_REG_INT_EN_R(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_INT     + (     (port_nbr) * 16u) + 0x10))
#define  CSP_GPIO_REG_INT_EN_F(port_nbr)        (*(CPU_REG32 *)(CSP_GPIO_ADDR_INT     + (     (port_nbr) * 16u) + 0x14))
#define  CSP_GPIO_REG_INT_CLR(port_nbr)         (*(CPU_REG32 *)(CSP_GPIO_ADDR_INT     + (     (port_nbr) * 16u) + 0x0C))


#define  CSP_GPIO_PORT02_INT_PINS               (DEF_BIT_10 | \
                                                 DEF_BIT_11 | \
                                                 DEF_BIT_12 | \
                                                 DEF_BIT_13)


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
*                                          CSP_GPIO_Init()
*
* Description : Initialize general purpose I/O module.
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

void  CSP_GPIO_Init (void) 
{

}   


/*
*********************************************************************************************************
*                                          CSP_GPIO_Cfg()
*
* Description : Configure a set of pins for a specific I/O port
*
* Argument(s) : port_nbr    GPIO port number. (see note #1)
*
*               pins        Bit mask specifying which pins to configure.
*
*               dir         Pins direction :
*                               CSP_GPIO_DIR_IN                  Pins are configured as inputs.
*                               CSP_GPIO_DIR_OUT                 Pins are configured as outputs.
*    
*               drv_mode    Pins Drive mode flags:
*                               CSP_GPIO_FLAG_MODE_NONE          Pins have neither pull-down nor pull-up.
*                               CSP_GPIO_FLAG_MODE_PULLUP        Pins have a pull-up   resistor enabled.
*                               CSP_GPIO_FLAG_MODE_PULLDOWN      Pins have a pull-down resistor enabled.
*                               CSP_GPIO_FLAG_MODE_REPEATER      Pins have repeater mode enabled.
*                               CSP_GPIO_FLAG_MODE_OPEN_DRAIN    Pins have open drain mode enabled.
*
*               int_en      Pin Interrupt enable (not supported.)
*                               DEF_YES                          Pin interrupt is enabled.
*                               DEF_NO                           Pin Interrupt is disabled.
*              
*               int_pol     Pin interrupt polarity:
*                               CSP_INT_POL_LEVEL_HIGH           Pin interrupt is activated on a high level (if int_act = 'CSP_INT_ACT_LEVEL').
*                               CSP_INT_POL_LEVEL_LOW            Pin interrupt is activated on a low level  (if int_act = 'CSP_INT_ACT_LEVEL').
* 
*                               CSP_INT_POL_EDGE_POSITIVE        Pin interrupt is activated on the positive edge. (if int_act = 'CSP_INT_ACT_EDGE').
*                               CSP_INT_POL_EDGE_NEGATIVE        Pin interrupt is activated on the negative edge. (if int_act = 'CSP_INT_ACT_EDGE').
*                               CSP_INT_POL_EDGE_BOTH            Pin interrupt is activated on both edges.        (if int_act = 'CSP_INT_ACT_EDGE').
*
*               fnct         Pins function :
*                               CSP_GPIO_FNCT_00                 Pins are controlled by the GPIO Controller.
*                               CSP_GPIO_FNCT_01                 Pins are controlled by the Peripheral in function 1.
*                               CSP_GPIO_FNCT_02                 Pins are controlled by the Peripheral in function 2.
*                               CSP_GPIO_FNCT_03                 Pins are controlled by the Peripheral in funciton 3.
*
*               opt          Pins extra options:
*                                CSP_GPIO_OPT_RD_WR_DIS          Controlled pin is not affected by writes to the port's.
*                                CSP_GPIO_OPT_RD_WR_EN           Controlled pin is     affected by writes to the port's.
*
* Return(s)   : DEF_OK    If the pins were configured successfully.
*               DEF_FAIL  If the pins could not be configured successfully.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) The 'port_nbr' argument depends on the number of GPIO ports available for this 
*                   family definition.
*
*                   (a) Port numbers identifiers can be found in 'csp.h':
*
*                           CSP_GPIO_PORT_NBR_00
*                           CSP_GPIO_PORT_NBR_01
*                           CSP_GPIO_PORT_NBR_A
*                           CSP_GPIO_PORT_NBR_B
*
*                (2) Some architectures multiplex general purpose pins with peripheral functions the 
*                    'fnct' parameter defines the GPIO function. 
*
*                   (a) GPIO function identifier can be found in csp.h:
*
*                           CSP_GPIO_FNCT_00
*                           CSP_GPIO_FNCT_01
*                           CSP_GPIO_FNCT_02
*                           CSP_GPIO_FNCT_03
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_GPIO_Cfg (CSP_DEV_NBR    port_nbr,
                           CSP_GPIO_MSK   pins,
                           CSP_OPT        dir,                                    
                           CSP_OPT_FLAGS  drv_mode,
                           CPU_BOOLEAN    int_en,
                           CSP_OPT        int_pol,
                           CSP_OPT        fnct)                           
{   
    CPU_INT08U  opt_pinsel;
    CPU_INT08U  opt_pinmode;
    CPU_INT08U  pin_nbr;
    CPU_INT32U  pin_bit_pos;
    CPU_INT32U  pin_mode_high;
    CPU_INT32U  pin_mode_low;
    CPU_INT32U  pin_sel_high;
    CPU_INT32U  pin_sel_low;    
    CPU_SR_ALLOC();

                                                                /* -------------- ARGUMENTS CHECKING ---------------- */
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    switch (port_nbr) {
        case CSP_GPIO_PORT_NBR_00:
        case CSP_GPIO_PORT_NBR_02:

             if ( (int_en  == DEF_ENABLED           ) &&        /* Only level sensitive interrupts can be configured.   */
                 ((int_pol == CSP_INT_POL_LEVEL_HIGH) ||
                  (int_pol == CSP_INT_POL_LEVEL_LOW ))) {                 
                 return (DEF_FAIL);
             }
             break;
        
        case CSP_GPIO_PORT_NBR_01:                              /* Only pins of PORT 0 and PORT 2 can generate int.     */
        case CSP_GPIO_PORT_NBR_03:
        case CSP_GPIO_PORT_NBR_04:             
             if (int_en == DEF_ENABLED) {
                 return (DEF_FAIL);
             }
             break;

        default:
             return (DEF_FAIL);
    }
#endif

                                                                /* -------------- PIN MODE DECODIFICATION ------------- */
                                                                /* Px.y MODE register defines (Port x Pin y) ...        */
                                                                /* ... '00b' Px.y pin has a pull-up resistor enabled.   */
                                                                /* ... '01b' Px.y pin has repeater mode enabled.        */
                                                                /* ... '10b' Px.y pin has neither pull-up nor pull-down.*/
                                                                /* ... '11b' Px.y has a pull-down resistor enabled.     */
    switch (drv_mode & ~CSP_GPIO_FLAG_MODE_OPEN_DRAIN) {        
        case CSP_GPIO_FLAG_MODE_NONE:
             opt_pinmode = 2u;                                  
             break;
             
        case CSP_GPIO_FLAG_MODE_PULLUP:
             opt_pinmode = 0u;
             break;
             
        case CSP_GPIO_FLAG_MODE_PULLDOWN:
             opt_pinmode = 3u;
             break;
             
        case CSP_GPIO_FLAG_MODE_REPEATER:
             opt_pinmode = 1u;
             break;

        default:
             return (DEF_FAIL);
    }
                                                                /* ----------- PIN DIRECTION DECODIFICATION ----------- */
    switch (fnct) {
        case CSP_GPIO_FNCT_00:
             opt_pinsel = 0u;                                   /* '00b' GPIO Function PINSELx register.                */
         
             CPU_CRITICAL_ENTER();
             if (dir == CSP_GPIO_DIR_IN) {                      /* Pin direction decodification.                        */
                 DEF_BIT_CLR(CSP_GPIO_REG_FIODIRx(port_nbr), pins);
             } else {
                 DEF_BIT_SET(CSP_GPIO_REG_FIODIRx(port_nbr), pins);
             }

             CPU_CRITICAL_EXIT();
             break;
        
        case CSP_GPIO_FNCT_01:
             opt_pinsel = 1u;                                   /* '01b' Function 01 PINSELx register.                  */
             break;
        
        case CSP_GPIO_FNCT_02:
             opt_pinsel = 2u;                                   /* '10b' Function 02 PINSELx register.                  */
             break;
             
        case CSP_GPIO_FNCT_03:
             opt_pinsel = 3u;                                   /* '11b' Function 03 PINSELx register.                  */
             break;
             
        default:
             return (DEF_FAIL);
    }
    
    CPU_CRITICAL_ENTER();
    if (DEF_BIT_IS_SET(drv_mode, CSP_GPIO_FLAG_MODE_OPEN_DRAIN)) {
        DEF_BIT_SET(CSP_GPIO_REG_PINMODE_ODx(port_nbr), pins);
    } else {
        DEF_BIT_CLR(CSP_GPIO_REG_PINMODE_ODx(port_nbr), pins);
    }
    CPU_CRITICAL_EXIT();
     
    pin_mode_low  = CSP_GPIO_REG_PINMODELx(port_nbr);
    pin_mode_high = CSP_GPIO_REG_PINMODEHx(port_nbr);
    pin_sel_low   = CSP_GPIO_REG_PINSELLx(port_nbr);    
    pin_sel_high  = CSP_GPIO_REG_PINSELHx(port_nbr);
        
    for (pin_nbr = 0u; pin_nbr < 16u; pin_nbr++) {
        pin_bit_pos = pin_nbr * 2u;
        if (DEF_BIT_IS_SET(pins, DEF_BIT(pin_nbr))) {           /* Configure the pins 0 .. 15                           */            
            DEF_BIT_CLR(pin_mode_low, DEF_BIT_FIELD(2u, pin_bit_pos));
            DEF_BIT_CLR(pin_sel_low,  DEF_BIT_FIELD(2u, pin_bit_pos));

            DEF_BIT_SET(pin_mode_low, DEF_BIT_MASK(opt_pinmode, pin_bit_pos));
            DEF_BIT_SET(pin_sel_low,  DEF_BIT_MASK(opt_pinsel,  pin_bit_pos));
        }
                                                             
        if (DEF_BIT_IS_SET(pins, DEF_BIT(pin_nbr + 16u))) {     /* Configure the pins 16 .. 32                          */
            DEF_BIT_CLR(pin_mode_high, DEF_BIT_FIELD(2u, pin_bit_pos));
            DEF_BIT_CLR(pin_sel_high,  DEF_BIT_FIELD(2u, pin_bit_pos));

            DEF_BIT_SET(pin_mode_high, DEF_BIT_MASK(opt_pinmode, pin_bit_pos));        
            DEF_BIT_SET(pin_sel_high,  DEF_BIT_MASK(opt_pinsel,  pin_bit_pos));                    
        }    
    }
    
    CPU_CRITICAL_ENTER();

    CSP_GPIO_REG_PINMODELx(port_nbr) = pin_mode_low;
    CSP_GPIO_REG_PINMODEHx(port_nbr) = pin_mode_high;
    CSP_GPIO_REG_PINSELLx(port_nbr)  = pin_sel_low;
    CSP_GPIO_REG_PINSELHx(port_nbr)  = pin_sel_high;

    CPU_CRITICAL_EXIT();

    if ((port_nbr == CSP_GPIO_PORT_NBR_00) ||
        (port_nbr == CSP_GPIO_PORT_NBR_02)) {
        
        CPU_CRITICAL_ENTER();
        
        if (int_en == DEF_YES) {
            switch (int_pol) {
                case CSP_INT_POL_LEVEL_HIGH:
                     DEF_BIT_CLR(CSP_GPIO_REG_EXT_MODE, (pins >> 10u) & DEF_BIT_FIELD(4u, 0u));
                     DEF_BIT_SET(CSP_GPIO_REG_EXT_POLAR, (pins >> 10u) & DEF_BIT_FIELD(4u, 0u)); 
                     break;

                case CSP_INT_POL_LEVEL_LOW:
                     DEF_BIT_CLR(CSP_GPIO_REG_EXT_MODE, (pins >> 10u) & DEF_BIT_FIELD(4u, 0u));
                     DEF_BIT_CLR(CSP_GPIO_REG_EXT_POLAR, (pins >> 10u) & DEF_BIT_FIELD(4u, 0u));
                     break;
                
                case CSP_INT_POL_EDGE_POSITIVE:
                     DEF_BIT_SET(CSP_GPIO_REG_INT_EN_R(port_nbr), pins);
                     DEF_BIT_SET(CSP_GPIO_REG_EXT_MODE, (pins >> 10u) & DEF_BIT_FIELD(4u, 0u));
                     DEF_BIT_SET(CSP_GPIO_REG_EXT_POLAR, (pins >> 10u) & DEF_BIT_FIELD(4u, 0u)); 
                     break;

                case CSP_INT_POL_EDGE_NEGATIVE:
                     DEF_BIT_SET(CSP_GPIO_REG_INT_EN_F(port_nbr), pins);
                     DEF_BIT_SET(CSP_GPIO_REG_EXT_MODE, (pins >> 10u) & DEF_BIT_FIELD(4u, 0u));
                     DEF_BIT_CLR(CSP_GPIO_REG_EXT_POLAR, (pins >> 10u) & DEF_BIT_FIELD(4u, 0u));
                     break;

                case CSP_INT_POL_EDGE_BOTH:
                     DEF_BIT_SET(CSP_GPIO_REG_INT_EN_R(port_nbr), pins);
                     DEF_BIT_SET(CSP_GPIO_REG_INT_EN_F(port_nbr), pins);
                     DEF_BIT_SET(CSP_GPIO_REG_EXT_MODE, (pins >> 10u) & DEF_BIT_FIELD(4u, 0u));
                     break;

                default:
                     break;
            }
        } else {
            DEF_BIT_CLR(CSP_GPIO_REG_INT_EN_R(port_nbr), pins);
            DEF_BIT_CLR(CSP_GPIO_REG_INT_EN_F(port_nbr), pins);
        }

        CPU_CRITICAL_EXIT();
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                         CSP_GPIO_BitClr()
*
* Description : Clear a group of pins for a specific I/O port.
*
* Argument(s) : port_nbr    GPIO port number. (See 'CSP_GPIO_Cfg()' note #1).
*
*               pins        Bit mask specifying which pins to clear.
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.                   
*********************************************************************************************************
*/

void  CSP_GPIO_BitClr (CSP_DEV_NBR   port_nbr,
                       CSP_GPIO_MSK  pins)
{
    CPU_SR_ALLOC();


#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)     
    if (port_nbr > CSP_GPIO_PORT_NBR_04) {
        return;
    }
#endif
    
    CPU_CRITICAL_ENTER();
    CSP_GPIO_REG_FIOCLRx(port_nbr) = pins;            
    CPU_CRITICAL_EXIT();
}


/*
*********************************************************************************************************
*                                         CSP_GPIO_BitSet()
*
* Description : Set a group of pins for a specific I/O port
*
* Argument(s) : port_nbr    GPIO port number. (See 'CSP_GPIO_Cfg()' note #1).
*
*               pins        Bit mask specifying which pins to set.
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.                   
*********************************************************************************************************
*/

void  CSP_GPIO_BitSet (CSP_DEV_NBR    port_nbr,
                       CSP_GPIO_MSK  pins)
{
    CPU_SR_ALLOC();

#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)         
    if (port_nbr > CSP_GPIO_PORT_NBR_04) {
        return;
    }
#endif
    
    CPU_CRITICAL_ENTER();
    CSP_GPIO_REG_FIOSETx(port_nbr) = pins;            
    CPU_CRITICAL_EXIT();
}


/*
*********************************************************************************************************
*                                         CSP_GPIO_BitToggle()
*
* Description : Toggle a group of pins for a specific I/O port.
*
* Argument(s) : port_nbr    GPIO port number. (See 'CSP_GPIO_Cfg()' note #1).
*
*               pins        Bit mask specifying which pins to Toggle.
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.                   
*********************************************************************************************************
*/

void  CSP_GPIO_BitToggle (CSP_DEV_NBR    port_nbr,
                          CSP_GPIO_MSK  pins)                          
{
    CSP_GPIO_MSK  pins_set;
    CSP_GPIO_MSK  pins_clr;
    CPU_SR_ALLOC();
    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)    
    if (port_nbr > CSP_GPIO_PORT_NBR_04) {
        return;
    }
#endif
    
    pins_clr                       =  (CSP_GPIO_REG_FIOPINx(port_nbr)) & pins;    
    pins_set                       = ~(CSP_GPIO_REG_FIOPINx(port_nbr)) & pins;
    
    CPU_CRITICAL_ENTER();
    CSP_GPIO_REG_FIOCLRx(port_nbr) = pins_clr;
    CSP_GPIO_REG_FIOSETx(port_nbr) = pins_set;    
    CPU_CRITICAL_EXIT();
}


/*
*********************************************************************************************************
*                                         CSP_GPIO_Rd()
*
* Description : Read the current value of the port.
*
* Argument(s) : port_nbr    GPIO port number. (See 'CSP_GPIO_Cfg()' note #1).
*
* Return(s)   : The current value of the port.
*
* Caller(s)   : Application.
*
* Note(s)     : none.                   
*********************************************************************************************************
*/

CSP_GPIO_MSK  CSP_GPIO_Rd (CSP_DEV_NBR  port_nbr)
{
    CSP_GPIO_MSK  port_val;


#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)     
    if (port_nbr > CSP_GPIO_PORT_NBR_04) {
        return (CSP_GPIO_MSK)(0);
    }
#endif

    port_val = CSP_GPIO_REG_FIOPINx(port_nbr);
    
    return (port_val);
}


/*
*********************************************************************************************************
*                                         CSP_GPIO_Wr()
*
* Description : Write a value to a specific port.
*
* Argument(s) : port_nbr    GPIO port number. (See 'CSP_GPIO_Cfg()' note #1).
*
*               val         Value to be written in the port.
*
* Return(s)   : The current value of the port.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_GPIO_Wr (CSP_DEV_NBR   port_nbr,
                   CSP_GPIO_MSK  val)
{
    CPU_INT32U  pins_clr;
    CPU_INT32U  pins_set;
    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)     
    if (port_nbr > CSP_GPIO_PORT_NBR_04) {
        return;
    }
#endif
    
    pins_set =  val;
    pins_clr = ~val;
    CSP_GPIO_REG_FIOSETx(port_nbr) = pins_set;
    CSP_GPIO_REG_FIOCLRx(port_nbr) = pins_clr;
}


/*
*********************************************************************************************************
*                                         CSP_GPIO_IntClr()
*
* Description : Clear pending interrupts for a specific port.
*
* Argument(s) : port_nbr    GPIO port number. (See 'CSP_GPIO_Cfg()' note #1).
*
* Return(s)   : Interrupt status.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CSP_GPIO_MSK  CSP_GPIO_IntClr (CSP_DEV_NBR  port_nbr)
{
    CPU_INT32U   int_stat;
    

    int_stat = DEF_BIT_NONE;
    
    switch (port_nbr) {
        case CSP_GPIO_PORT_NBR_01:
        case CSP_GPIO_PORT_NBR_03:
        case CSP_GPIO_PORT_NBR_04:
             break;

        case CSP_GPIO_PORT_NBR_00:
             if (DEF_BIT_IS_CLR(CSP_GPIO_REG_INT_STAT, DEF_BIT_00)) {
                 break;
             }
             int_stat  = CSP_GPIO_REG_INT_STAT_R(CSP_GPIO_PORT_NBR_00);
             int_stat |= CSP_GPIO_REG_INT_STAT_F(CSP_GPIO_PORT_NBR_00);             
             break;

        case CSP_GPIO_PORT_NBR_02:
             if (DEF_BIT_IS_SET(CSP_GPIO_REG_INT_STAT, DEF_BIT_00)) {
                 int_stat  = CSP_GPIO_REG_INT_STAT_R(CSP_GPIO_PORT_NBR_02);
                 int_stat |= CSP_GPIO_REG_INT_STAT_F(CSP_GPIO_PORT_NBR_02);
             }
             
             if ((((CSP_GPIO_REG_PINSELLx(CSP_GPIO_PORT_NBR_02) >> 20u) & DEF_BIT_FIELD(2u, 0u)) == 1u     ) &&  
                   (DEF_BIT_IS_SET(CSP_GPIO_REG_EXT_MODE, DEF_BIT_00)                            == DEF_YES) &&
                   (DEF_BIT_IS_SET(CSP_GPIO_REG_EXT_INT , DEF_BIT_00)                            == DEF_YES)) {
                 CSP_GPIO_REG_EXT_INT = DEF_BIT_00;
             }
             if ((((CSP_GPIO_REG_PINSELLx(CSP_GPIO_PORT_NBR_02) >> 22u) & DEF_BIT_FIELD(2u, 0u)) == 1u     ) &&  
                   (DEF_BIT_IS_SET(CSP_GPIO_REG_EXT_MODE, DEF_BIT_01)                            == DEF_YES) &&
                   (DEF_BIT_IS_SET(CSP_GPIO_REG_EXT_INT , DEF_BIT_01)                            == DEF_YES)) {
                 CSP_GPIO_REG_EXT_INT = DEF_BIT_01;
             }
             if ((((CSP_GPIO_REG_PINSELLx(CSP_GPIO_PORT_NBR_02) >> 24u) & DEF_BIT_FIELD(2u, 0u)) == 1u     ) &&  
                   (DEF_BIT_IS_SET(CSP_GPIO_REG_EXT_MODE, DEF_BIT_02)                            == DEF_YES) &&
                   (DEF_BIT_IS_SET(CSP_GPIO_REG_EXT_INT , DEF_BIT_02)                            == DEF_YES)) {
                 CSP_GPIO_REG_EXT_INT = DEF_BIT_02;
             }
             if ((((CSP_GPIO_REG_PINSELLx(CSP_GPIO_PORT_NBR_02) >> 27u) & DEF_BIT_FIELD(2u, 0u)) == 1u     ) &&  
                   (DEF_BIT_IS_SET(CSP_GPIO_REG_EXT_MODE, DEF_BIT_03)                            == DEF_YES) &&
                   (DEF_BIT_IS_SET(CSP_GPIO_REG_EXT_INT , DEF_BIT_03)                            == DEF_YES)) {
                 CSP_GPIO_REG_EXT_INT = DEF_BIT_03;
             }             
             break;
        
        default:
             break;
        
    }

    if (int_stat != DEF_BIT_NONE) {
        CSP_GPIO_REG_INT_CLR(port_nbr) = int_stat;
    }

    return (int_stat);
}
