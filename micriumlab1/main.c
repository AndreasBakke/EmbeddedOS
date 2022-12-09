/*----------------------------------------------------------------------------
 * Name:    main.c
 * Purpose: 
 * Note(s):
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
int down_KEY4;
unsigned short AD_current; 
     /* ----------------- APPLICATION GLOBALS ------------------ */

/* Definition of the Task Control Blocks for three tasks */
static  OS_TCB        APP_TSK_TCB;
static	OS_TCB			  TASK_1_TCB;
static	OS_TCB				TASK_2_TCB;

static OS_TMR  Task1Tmr;
static OS_TMR  Task2Tmr;	

/* Definition of the Stacks for three tasks */
static  CPU_STK_SIZE  APP_TSK_STACK[APP_CFG_TASK_STK_SIZE];
static	CPU_STK_SIZE	TASK_1_STACK[APP_CFG_TASK_STK_SIZE];
static  CPU_STK_SIZE	TASK_2_STACK[APP_CFG_TASK_STK_SIZE];

/* Global Variables*/
uint16_t position = 0;
uint16_t counter = 0;
int pos_x = 0;
int pos_y = 0;

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
static void TASK_1 ( void * p_args );
static void TASK_2 ( void * p_args );
static void timer1_callback ( OS_TMR  *p_tmr, void * p_args );
static void timer2_callback ( OS_TMR  *p_tmr, void * p_args );
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
		
	
	
		OSTmrCreate(&Task1Tmr,         				/* p_tmr          */
									 "Task1Timer",           	   /* p_name         */
										10,                    /* dly            */
										5,                    /* period         */
										OS_OPT_TMR_PERIODIC,   /* opt            */
										timer1_callback,         				 /* p_callback     */
										0,                     /* p_callback_arg */
									 &err); 

		OSTmrCreate(&Task2Tmr,         				/* p_tmr          */
									 "Task2Timer",           	   /* p_name         */
										10,                    /* dly            */
										5,                    /* period         */
										OS_OPT_TMR_PERIODIC,   /* opt            */
										timer2_callback,         				 /* p_callback     */
										0,                     /* p_callback_arg */
									 &err);  
	
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
		OS_ERR err;
		CPU_BOOLEAN  status;
	
		BSP_OS_TickEnable(); /* Enable the tick timer and interrupt                  */
		LED_init(); 
		LCD_Initialization();
		
		ADC_init();
		joystick_init();
		
	
		BUTTON_init();
	
		LCD_Clear(Blue);
		while (DEF_TRUE) {   
					
					
					if (down_KEY1 == 1) {
						GUI_Text(10, 240, (uint8_t*)  "Tasks Started" ,White, Blue);
						//Activate Task 1
							OSTmrStart(&Task1Tmr, &err);
							OSTmrStart(&Task2Tmr, &err);
							down_KEY1 = 0;
							
					}
					
			
					
					if (down_KEY2 == 1) {
						LCD_Clear(Blue); //Clear LCD
						GUI_Text(10, 260, (uint8_t*)  "Task1 Stopped" ,White, Blue);
						//Stop Task 1
						OSTmrStop(&Task1Tmr,
											OS_OPT_TMR_NONE,
											(void *)0,
											&err
													);
						down_KEY2 = 0;
					}
					
					if (down_KEY3 == 1) {
						GUI_Text(10, 280, (uint8_t*)  "Task2 Stopped" ,White, Blue);
						LED_Out(0x00);//Clear leds
						OSTmrStop(&Task2Tmr,
											OS_OPT_TMR_NONE,
											(void *)0,
											&err
											);
						down_KEY3 =0;
						
					}
					
						if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){
							pos_y = pos_y-1;
							LCD_Clear(Blue);
						 PutChar(pos_x, pos_y, (char) "0" ,White, Blue);

					}		
					else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	/* Joytick down pulled */
						LCD_Clear(Blue);
							pos_y = pos_y +1;
						 PutChar(pos_x, pos_y, (char) "0" ,White, Blue);
					}

					else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	/* Joytick right pulled */
						LCD_Clear(Blue);
							pos_x = pos_x -1;
						 PutChar(pos_x, pos_y, (char) "0" ,White, Blue);
					}

					else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	/* Joytick right pulled */
						LCD_Clear(Blue);
							pos_x = pos_x +1;
						 PutChar(pos_x, pos_y, (char) "0" ,White, Blue);
					}
		}
		
}

int i = 0;


void timer1_callback(OS_TMR  *p_tmr,
                     void    *p_arg){
	OS_ERR err;
	OSTaskCreate((OS_TCB     *)&TASK_1_TCB,               /* Create the start task                                */
                 (CPU_CHAR   *)"TASK_1_Task",
                 (OS_TASK_PTR ) TASK_1,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_1_PRIO,
                 (CPU_STK    *)&TASK_1_STACK[0],
                 (CPU_STK     )(APP_CFG_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
}
										 
void timer2_callback(OS_TMR  *p_tmr,
                     void    *p_arg){
	OS_ERR err;
	OSTaskCreate((OS_TCB     *)&TASK_2_TCB,               /* Create the start task                                */
                 (CPU_CHAR   *)"TASK_2_Task",
                 (OS_TASK_PTR ) TASK_2,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_2_PRIO,
                 (CPU_STK    *)&TASK_2_STACK[0],
                 (CPU_STK     )(APP_CFG_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
					
}

char message[80] = "The Answer to the Ultimate Question of Life, the Universe, and Everything is 42";
uint8_t * val1[16];

void TASK_1 (void *p_arg)
{
	(void)p_arg;
	OS_ERR err;
	i=(i+1)%80;
	char disp=message[i];
	tostring((uint8_t*) val1, message[i]);
	//GUI_Text(50, 50,  "t" ,White, Blue);
	LCD_Clear(Blue);
	PutChar(50,70, (uint8_t *) disp, White, Blue);
	OSTaskDel((OS_TCB *)0, &err);
}
int led_position=0x01;

void TASK_2 (void *p_arg)
{
	(void)p_arg;
	OS_ERR err;
	if (led_position >= 0b10000000) {
		led_position = 0x01;
	} else {
		led_position = led_position <<1;
	}
	LED_Out(led_position);
	OSTaskDel((OS_TCB *)0, &err);
}













