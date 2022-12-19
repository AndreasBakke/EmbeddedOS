/*----------------------------------------------------------------------------
 * Name:    main.c
 * Purpose: 
 * Note(s): Original provided program. Kept unedited here for safe keeping.
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "core_cm3.h"
#include "RTE_Device.h"                 // Keil::Device:Startup
#include "includes.h"
#include "BSP.h"
int down_KEY1;
int down_KEY2;
int down_KEY3;
unsigned short AD_current; 
     /* ----------------- APPLICATION GLOBALS ------------------ */

/* Definition of the Task Control Blocks for three tasks */
static  OS_TCB        APP_TSK_TCB;
static	OS_TCB			  GUI_TSK_TCB;
static	OS_TCB				INPUT_TSK_TCB;

/* Definition of the Stacks for three tasks */
static  CPU_STK_SIZE  APP_TSK_STACK[APP_CFG_TASK_STK_SIZE];
static	CPU_STK_SIZE	GUI_TSK_STACK[APP_CFG_TASK_STK_SIZE];
static  CPU_STK_SIZE	INPUT_TSK_STACK[APP_CFG_TASK_STK_SIZE];

/* Global Variables*/
uint16_t position = 0;
uint16_t counter = 0;


OS_SEM sem_counter;
OS_SEM sem_position;

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/* Generic function */
static void App_ObjCreate  (void);
static int tostring( uint8_t * str, int num );
static void App_TaskCreate(void);

/* Task functions */
static void APP_TSK ( void   *p_arg );
//static void GUI_TSK ( void * p_args );
//static void INPUT_TSK ( void * p_args );
/**********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This the main standard entry point.
*
* Note(s)     : none.
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;
	
		SystemInit();	 					/*initialize the clock */
		BSP_IntInit();					/*initiialize interrupt*/
		BSP_Start(); 						/*initialize kernel system tick timer */
		CPU_IntDis(); 					/*disable interrupt*/
    CPU_Init();   					/*init cpu*/
    Mem_Init();							/*Memory initialization*/

    OSInit(&err);						/* Initialize "uC/OS-III, The Real-Time Kernel"         */
	
	
    OSTaskCreate((OS_TCB     *)&APP_TSK_TCB,               /* Create the start task                                */
                 (CPU_CHAR   *)"APP_TASK",
                 (OS_TASK_PTR ) APP_TSK,
                 (void       *) 0,
                 (OS_PRIO     ) APP_CFG_TASK_PRIO,
                 (CPU_STK    *)&APP_TSK_STACK[0],
                 (CPU_STK     )(APP_CFG_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

		OSStart(&err);      /* Start multitasking (i.e. give control to uC/OS-III). */

    while(DEF_ON){			/* Should Never Get Here	*/
    };
}

static int   tostring( uint8_t * str, int num ){
    int i, rem, len = 0, n;
		// handling the case 0
		if(num==0){
		str[0]='0';
			str[1]='\0';
			return 1;
		}
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {   rem = num % 10;
        num = num / 10;
        str[len-(i+1)] = rem + '0';
    }
		str[len]='\0';
		return len-1;
    }

static  void  APP_TSK (void *p_arg)
{
    (void)p_arg;        /* See Note #1                                          */
		OS_ERR  os_err;

	
		BSP_OS_TickEnable(); /* Enable the tick timer and interrupt                  */
		LED_init(); 
		LCD_Initialization();
		
		ADC_init();
		joystick_init();
		
	
		BUTTON_init();
	
		//PWM_init();
		//PWM_enable();

    //Test led functions
		

	
		//Clear the display in ble
		LCD_Clear(Blue);
		//Draw a line on screen
		LCD_DrawLine( 10, 10, 100, 100 , White );
		//Write a char on screen
		PutChar( 50, 50, 'a', Green, White );
		//Write a string on screen
		GUI_Text(10, 200, (uint8_t *) "Pippo",White, Green);
    //Test ADC conversion. When conversion is complete an interrupt is dispatched
		ADC_start_conversion ();
		OSTimeDlyHMSM (0,0,0,5,OS_OPT_TIME_TIMEOUT ,&os_err);
		uint8_t * val1[16];
		tostring((uint8_t*) val1, AD_current);
		GUI_Text(10, 200, (uint8_t*)  val1 ,White, Green);

	  //Led out set led status based on a bitmask
	  //LED_Out(0);
	  //Led on turns on a specific led
		//LED_On(7);
	  //Led off turns off a specific led
		//LED_Off(0);
	
	  int i=0;
		while (DEF_TRUE) {   
													 
					//Joystick has no interrupt it can be polled
					if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){
						 GUI_Text(10, 250, (uint8_t*)  "JOY UP" ,White, Red);
					}		
					if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	/* Joytick Select pressed */
						 GUI_Text(10, 250, (uint8_t*)  "JOY PRESSED" ,White, Red);
					} 
					else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	/* Joytick down pulled */
						 GUI_Text(10, 250, (uint8_t*)  "JOY DOWN" ,White, Red);
					}
					else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	/* Joytick right pulled */
						 GUI_Text(10, 250, (uint8_t*)  "JOY LEFT" ,White, Red);
					}
					else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	/* Joytick right pulled */
						 GUI_Text(10, 250, (uint8_t*)  "JOY RIGHT" ,White, Red);
					}
					
					
					if (down_KEY1 == 1) {
						  i++;
							GUI_Text(10, 240, (uint8_t*)  "BUTTON1" ,White, Red);
							tostring((uint8_t*) val1, i);
						  GUI_Text(100, 240, (uint8_t*)  val1 ,White, Red);
							down_KEY1 = 0;
					}
					
					if (down_KEY2 == 1) {
						  i++;
							GUI_Text(10, 260, (uint8_t*)  "BUTTON2" ,White, Red);
							tostring((uint8_t*) val1, i);
						  GUI_Text(100, 260, (uint8_t*)  val1 ,White, Red);
							down_KEY2 = 0;
					}
					
					if (down_KEY3 == 1) {
						  i++;
							GUI_Text(10, 280, (uint8_t*)  "BUTTON3" ,White, Red);
							tostring((uint8_t*) val1, i);
						  GUI_Text(100, 280, (uint8_t*)  val1 ,White, Red);
							down_KEY3 = 0;
					}
		}
		
}
