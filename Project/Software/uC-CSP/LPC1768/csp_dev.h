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
*                                     FAMILY MEMBER DEFINITION 
*                                         NXP  LPC1768 
*
* Filename      : csp_dev.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
* Note(s)       :
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  CSP_DEV_PRESENT
#define  CSP_DEV_PRESENT


/*$PAGE*/
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <cpu_core.h>

#include  <lib_def.h>


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   CSP_DEV_MODULE
#define  CSP_DEV_EXT
#else
#define  CSP_DEV_EXT  extern
#endif


/*
*********************************************************************************************************
*                                        DEFAULT CONFIGURATION
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                   LPC17xx FAMILY DEVICE MEMBER DEFINITION
*********************************************************************************************************
*/

#define  CSP_DEV_LPC1768
#define  CSP_DEV_NAME                           "LPC1768"

/*
*********************************************************************************************************
*                                       LPC1768 PERIPHERALS ENABLES
*********************************************************************************************************
*/

#define  CSP_DEV_USB_DEV_EN                     DEF_ENABLED
#define  CSP_DEV_USB_OTG_EN                     DEF_ENABLED
#define  CSP_DEV_ETHER_EN                       DEF_ENABLED
#define  CSP_DEV_CAN_EN                         DEF_ENABLED


/*
*********************************************************************************************************
*                                       LPC1768 MEMORIES SIZE DEFINES
*********************************************************************************************************
*/

#define  CSP_DEV_FLASH_SIZE                     (512u * 1024u)
#define  CSP_DEV_SRAM_SIZE                      ( 64u * 1024u)


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of CSP_DEV module include.                      */

