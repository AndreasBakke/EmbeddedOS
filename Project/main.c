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

/* Definition of the Task Control Blocks for all tasks */
static  OS_TCB        APP_TSK_TCB;
static	OS_TCB			  READ_WL_TCB; //Task for reading waterlevel
static	OS_TCB				DRAIN_TCB;
static  OS_TCB				FILL_TCB;
static  OS_TCB        OUTPUT_TCB;
static  OS_TCB				DISPLAY_TCB; //Handles all display on screen

static OS_TMR  WLTmr; //Waterlevel timer
static OS_TMR  OUTPUTTmr; //Random output timer

/* Definition of the Stacks for three tasks */
static  CPU_STK_SIZE  APP_TSK_STACK[APP_CFG_TASK_STK_SIZE];
static	CPU_STK_SIZE	READ_WL_STACK[APP_CFG_TASK_STK_SIZE];
static  CPU_STK_SIZE	DRAIN_STACK[APP_CFG_TASK_STK_SIZE];
static  CPU_STK_SIZE	FILL_STACK[APP_CFG_TASK_STK_SIZE];
static  CPU_STK_SIZE	OUTPUT_STACK[APP_CFG_TASK_STK_SIZE];
static  CPU_STK_SIZE	UPDATE_DISPLAY_TCB[APP_CFG_TASK_STK_SIZE];


/* Global Variables*/
uint16_t position = 0;
uint16_t counter = 0;

static int max_WL = 200; //Define absolute max water level of tank (Liters)
static int alert_WL = 180;  //What WL should give alert.
double WL = 150; //Start water level at 150L - actual water level
int WL_r = 0; //Read water level

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
static void READ_WL ( void * p_args );
static void FILL ( void * p_args );
static void DRAIN ( void * p_args );
static void OUTPUT (void * p_args );

static void WLTmr_callback ( OS_TMR  *p_tmr, void * p_args );
static void OutputTmr_callback ( OS_TMR  *p_tmr, void * p_args );
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
		
	
	
		OSTmrCreate(&WLTmr,         				/* p_tmr          */
									 "Water_level_timer",           	   /* p_name         */
										10,                    /* dly            */
										10,                    /* period =1s       */
										OS_OPT_TMR_PERIODIC,   /* opt            */
										WLTmr_callback,				 /* p_callback     */
										0,                     /* p_callback_arg */
									 &err); 
									 
		OSTmrCreate(&OUTPUTTmr,         				/* p_tmr          */
									 "OutputWater_timer",           	   /* p_name         */
										10,                    /* dly            */
										1,                    /* period =100ms       */
										OS_OPT_TMR_PERIODIC,   /* opt            */
										OutputTmr_callback,				 /* p_callback     */
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
			;;
		}		
		
}


void WLTmr_callback(OS_TMR  *p_tmr,
                     void    *p_arg){
	OS_ERR err;
	OSTaskCreate((OS_TCB     *)&READ_WL_TCB,               /* Create the start task                                */
                 (CPU_CHAR   *)"Read_WaterLevel",
                 (OS_TASK_PTR ) READ_WL,
                 (void       *) 0,
                 (OS_PRIO     ) READ_WL_PRIO, //Definied in app_cfg.h
                 (CPU_STK    *)&READ_WL_STACK[0],
                 (CPU_STK     )(APP_CFG_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
}
										 
void OutputTmr_callback(OS_TMR  *p_tmr,
                     void    *p_arg){
	OS_ERR err;
	OSTaskCreate((OS_TCB     *)&OUTPUT_TCB,               /* Create the start task                                */
                 (CPU_CHAR   *)"OutputWater",
                 (OS_TASK_PTR ) OUTPUT,
                 (void       *) 0,
                 (OS_PRIO     ) OUTPUT_PRIO, //Definied in app_cfg.h
                 (CPU_STK    *)&OUTPUT_STACK[0],
                 (CPU_STK     )(APP_CFG_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
}
										 


void READ_WL (void *p_arg) //Reads water level and creates task
{
	
	(void)p_arg;
	OS_ERR err;
	GUI_Text(50, 50,  "t" ,White, Blue);
	
	WL_r = 0;
	while (WL_r < WL){ //Here we can flip random bit so that deadline is missed!
		WL_r = WL_r +1;
	}
	
	//Here is a good place to insert a bit error. So that we read WL > 180 even though it is not <= empty tank!
	if(WL_r>180){
		//Activate DRAIN and stop FILL
	} else {
		//Fill!
	}
}



double outW, drain, fill = 0; //in L
void OUTPUT (void *p_arg) //Drains random amount between 0 and 1L every 1/th of a second
{
	(void)p_arg;
	OS_ERR err;
	//Find random number between 0 and 1
	//Reduce WaterLevel by random number
	
	WL = WL - outW - drain + fill; 
}


void FILL (void *p_arg){ //Sets level to fill between 0 and 3 - called every 1s when reading new value (as long as WL <180
	drain = 0;
	
	fill = 1+ outW*2-WL_r/100; //Try to inteligently fill (predicting what the water level will be for the next 1s) to stay within 50 to 150L. 
}

void DRAIN (void *p_arg){ //Starts draining!
	fill=0;	
	drain = 3;
}
