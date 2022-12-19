
/*
*********************************************************************************************************
*
*                                         BOARD SUPPORT PACKAGE
*
*                                      
*                                            NXP LPC1768
*                                      LPC1768- LandTiger Board V2
* Filename      : bsp.h
* Version       : V1.00
* Programmer(s) : FT
*                 DC
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

#ifndef  BSP_PRESENT
#define  BSP_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*
* Note(s) : (1) The following common software files are located in the following directories :
*
*               (a) \<Custom Library Directory>\lib*.*
*
*               (b) (1) \<CPU-Compiler Directory>\cpu_def.h
*
*                   (2) \<CPU-Compiler Directory>\<cpu>\<compiler>\cpu*.*
*
*                           where
*                                   <Custom Library Directory>      directory path for custom   library      software
*                                   <CPU-Compiler Directory>        directory path for common   CPU-compiler software
*                                   <cpu>                           directory name for specific processor (CPU)
*                                   <compiler>                      directory name for specific compiler
*
*           (2) Compiler MUST be configured to include the '\<Custom Library Directory>\uC-LIB\',
*               '\<CPU-Compiler Directory>\' directory, & the specific CPU-compiler directory as
*               additional include path directories.
*
*           (3) NO compiler-supplied standard library functions SHOULD be used.
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <bsp_os.h>


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/


#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void         BSP_Init               (void);
void         BSP_Start              (void);




																/*		PWM FUNCTIONS					*/
typedef  enum channels{CH1,CH2,CH3,CH4,CH5,CH6,ALL}PWM_channel ; 

/*Channel correspondance:
	ch1->led 11
	ch2->led 10
ch3->led 9
ch4->led 8
ch5->led 7
ch6->led 6
*/


void PWM_init(void);
void PWM_enable(void);

/*service functions */
void update_dutycycle(float value, PWM_channel channel );
void change_frequency ( int value ) ;
int get_dutycycle(PWM_channel channel );
int get_frequency(PWM_channel channel);

		/* END PWM FUNCTIONS  */

/*
*********************************************************************************************************
*                                          CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              MODULE END
*********************************************************************************************************
*/

#endif
