/*----------------------------------------------------------------------------
 * Name:    main.c
 * Purpose:  Proposed solution to lab exercise on Real time safety and fault-injections
 * Note(s):  Two solutions working together has been implemented. First, a "watchdog" timing test, checking for a deadline miss.
 *           Secondly a dedicated monitoring task, monitoring values of importance, and looking for change.
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 * Proposed Solution
 *----------------------------------------------------------------------------*/
                  
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "core_cm3.h"
#include "RTE_Device.h"                 // Keil::Device:Startup
#include "includes.h"
#include "BSP.h"
#include <stdlib.h>
#include <time.h>

short down_KEY1;
short down_KEY2;
short down_KEY3;
     /* ----------------- APPLICATION GLOBALS ------------------ */

/* Definition of the Task Control Blocks for all tasks */
static  OS_TCB        APP_TSK_TCB;
static	OS_TCB			  READ_WL_TCB; //Task for reading waterlevel
static  OS_TCB        OUTPUT_TCB;  //"Analog" task
static  OS_TCB				TT_TCB; // Timing test TCB
static  OS_TCB        MONITOR_TCB;

static OS_TMR  TTTmr; //Timing test timer
static OS_TMR  WLTmr; //Waterlevel timer
static OS_TMR  OUTPUTTmr; //Random output timer
static OS_TMR  MONITORTmr; //Timer for monitor task

/* Definition of the Stacks for tasks */
static  CPU_STK_SIZE  APP_TSK_STACK[APP_CFG_TASK_STK_SIZE];
static	CPU_STK_SIZE	READ_WL_STACK[APP_CFG_TASK_STK_SIZE];
static  CPU_STK_SIZE	OUTPUT_STACK[APP_CFG_TASK_STK_SIZE];
static  CPU_STK_SIZE  TT_STACK[APP_CFG_TASK_STK_SIZE];
static  CPU_STK_SIZE 	MONITOR_STACK[APP_CFG_TASK_STK_SIZE];


/* Global Variables*/
uint16_t position = 0;
uint16_t counter = 0;
static int s = 200;
static short max_WL = 200; //Define absolute max water level of tank (Liters)
static short alert_WL = 180;  //What WL should give alert.
double WL = 150; //Start water level at 150L - actual water level
unsigned char WL_r = 0; //Read water level
int outW, fill = 0; //How to make theese double or something without going over memory limit?

static int caught = 0; //Added a variable to track how many errors have been caught
uint8_t * val1[16];

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/* Generic function */
static int tostring( uint8_t * str, int num );
static void App_ObjCreate  (void);
static void App_TaskCreate(void);


/* Task functions */
static void APP_TSK ( void   *p_arg );
static void READ_WL ( void * p_args );
static void OUTPUT (void * p_args );
static void TIMING_TEST (void * p_args );
static void MONITOR (void * p_args );

static void WLTmr_callback ( OS_TMR  *p_tmr, void * p_args );
static void OutputTmr_callback ( OS_TMR  *p_tmr, void * p_args );
static void TTTmr_callback( OS_TMR  *p_tmr, void * p_args );
static void MONITOR_callback (OS_TMR *p_tmr, void * p_args );
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
		
		LCD_Clear(White);
	
	
		/*
	
	
			Here you can create timers!
		
	
		*/
	
		/* Timing test / watchdog implemented to keep track of deadline miss - manually reset by sensor-read */ 
		OSTmrCreate( &TTTmr,         				/* p_tmr          */
										"Timing_test_timer",           	   /* p_name         */
										0,                    /* dly            */
										2,                    /* period =200ms       */
										OS_OPT_TMR_ONE_SHOT,   /* opt            */
										TTTmr_callback,				 /* p_callback     */
										0,                     /* p_callback_arg */
									 &err); 
		/* Timer for monitoring task */
		OSTmrCreate( &MONITORTmr,         				/* p_tmr          */
										"Monitor_timer",           	   /* p_name         */
									  11,                    /* dly =1.1s       */
										1,                    /* period =100ms       */
										OS_OPT_TMR_PERIODIC,   /* opt            */
										MONITOR_callback,				 /* p_callback     */
										0,                     /* p_callback_arg */
									 &err); 
	
	
		//Create timer to read waterlevel which fires every 1s
		OSTmrCreate(&WLTmr,         				/* p_tmr          */
									 "Water_level_timer",           	   /* p_name         */
										10,                    /* dly            */
										10,                    /* period =1s       */
										OS_OPT_TMR_PERIODIC,   /* opt            */
										WLTmr_callback,				 /* p_callback     */
										0,                     /* p_callback_arg */
									 &err); 
		
		
		//Create timer to simulate analog system every 100ms						 
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

static  void  APP_TSK (void *p_arg)
{
    (void)p_arg;        /* See Note #1                                          */
		OS_ERR  os_err;
		OS_ERR err;
		CPU_BOOLEAN  status;
	
		BSP_OS_TickEnable(); /* Enable the tick timer and interrupt                  */
		LCD_Initialization(); //Init LCD OBS: Update GLCD.c if using LAB board.
		joystick_init();
		BUTTON_init();
		
		//Draw initial watertank and static elements
		LCD_Clear(White);	
	  LCD_DrawLine(0, 119, 100, 119, Black);
	  LCD_DrawLine(100, 120, 100, 320, Black);
		//GUI_Text( 10, 100, &s, Black, White);
		GUI_Text( 50, 20, "Read value:", Black, White);
		GUI_Text(50, 50, "Act", White, Blue);
	  GUI_Text( 50, 70, "Caught:", Black, White);
		int i=0;
		while (i<WL){
			int level = 320-i;
			LCD_DrawLine(0, level, 99, level, Blue);
			i=i+1;
		}
		
		OSTmrStart(&WLTmr, &err);
		OSTmrStart(&OUTPUTTmr, &err);
		OSTmrStart(&MONITORTmr, &err); //Remember to start the timer
		while (DEF_TRUE) {
			if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){ //Joystick up -- flips random bit in "sensor initialization"
				if (s<=200){ //Only flip bit once every second. Will be reset to 0 after read => new bit flip
					int bit_pos = rand() % 32;
					s ^= (1 << bit_pos); //Flips bit in sensor initialization (May introduce timing fault)
					tostring((uint8_t*) val1, s);
					GUI_Text(0,0,"s flipped to: ", Black, White);
					GUI_Text(120,0,"               ", White, White); //Clear past value
					GUI_Text(120,0,val1, Black, White);
				}
			}
			else
				if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	/* Joystick down --does nothing  */

			}

			else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	/* Joystick Left - does nothing */
				unsigned char bit_pos = rand() %4;

			}

			else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	/* Joytick right - flips random bit in fill  */
				unsigned char bit_pos = rand() %4;
				fill ^=(1 << bit_pos);
			}
		}		
		
}


/*

    Here you can implement callback functions for your timers!

*/

/*  Callback function for watchdog */
void TTTmr_callback(OS_TMR  *p_tmr,
                     void    *p_arg){
	OS_ERR err;
	OSTaskCreate((OS_TCB     *)&TT_TCB,              
                 (CPU_CHAR   *)"Reacts to timing test timer",
                 (OS_TASK_PTR ) TIMING_TEST,
                 (void       *) 0,
                 (OS_PRIO     ) TIMING_TEST_PRIO, //Definied in app_cfg.h
                 (CPU_STK    *)&TT_STACK[0],
                 (CPU_STK     )(APP_CFG_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
}
	
//Callback function for monitor timer
void MONITOR_callback(OS_TMR  *p_tmr,
                     void    *p_arg){
	OS_ERR err;
	OSTaskCreate((OS_TCB     *)&MONITOR_TCB,              
                 (CPU_CHAR   *)"Monitors important values and looks for errors",
                 (OS_TASK_PTR ) MONITOR,
                 (void       *) 0,
                 (OS_PRIO     ) MONITOR_PRIO, //Definied in app_cfg.h
                 (CPU_STK    *)&MONITOR_STACK[0],
                 (CPU_STK     )(APP_CFG_TASK_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
}



//Callback function for the WL timer
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


//Callback function for the analog-timer
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
										 

void TIMING_TEST (void *p_arg) //If we reach this, the timing_test timer has run out => potential deadline miss!
{
	
	(void)p_arg;
	OS_ERR err;
	caught = caught +1;
	tostring((uint8_t*) val1, caught);
	GUI_Text( 150, 70, (uint8_t *) val1, Black, White);
	
	//Two recovery-options: Gracefull degredation vs Hard recovery
	/*Gracefull degredation: Alter system to reach deadline. 
	Example: set timer a lot lower than deadline, then reset task to "try again".
	Example2: Alter priorities, and temporarily grant task higher priority
	Usefull when the error is "non critical" eg. we still have time to try to fix it
	*/
	
	/* Hard recovery: Switch to a safe state (Do something safe!)
	Example1: Stop task and do the same as last time
	Example2: Stop task and close the valve is the last WL was high, and open if the las WL was low. (Or based of last N readings)
	
	Usefull when we no longer have time to fix the problem. What defines a "safe state" depends on the system functionality and requirements
	Examples of "safe": Same as last time, match some other variable, do nothing.
	*/
	
	//Here we do a hard recovery
	OSTaskDel(&READ_WL, &err);
	fill = 0; //example
	s=200;
	
	//Example of gracefull degredation:
	/*
	OSTaskDel(&READ_WL, &err); //stop task
	OSTaskCreate(....); //Start it again
	
	//OR
	OSTaskChangePrio(...);
	
	
	//Note: Additional logic may be required if both gracefull degredation & hard recovery is implemented to determine which recovery should be used.	
	*/

}

//Monitors important values in our system
void MONITOR (void *p_arg)
{
	(void)p_arg;
	OS_ERR err;
	unsigned char WL_new = WL_r; //Use local variables
  int fill_new = fill; //Use local variables
	/* Variables to store last known values */
	static unsigned char WL_prev = 150; //initialization of static variables to some "insignificant value"
	static int fill_prev = 1; 
	
	/*   We have multiple ways of monitoring, and checking for errors. See "learn by heart list"
	*    Only some options will be implemented here.
	*    Detection implies something has gone wrong (water level read wrong / bit flipped) etc.
	*    Recovery is here "set the same as last time" which may still lead to errors (multiple faults in a row), but highly unlikely.
	*/
	
	//Reasonableness test:
	if((WL_new > 200) || (WL_new <0)) {//Detection
		//recovery
		WL_r = WL_prev; //Set WL_r to last known value
		WL_new = WL_prev; //remember to update WL_new;
		caught = caught +1;
		tostring((uint8_t*) val1, caught);
		GUI_Text( 150, 70, (uint8_t *) val1, Black, White);
	};
	
	if((fill_new > 3) || (fill_new <0)) { //Detection. These implies "illegal values" and we need to recover
		fill = fill_prev;
		fill_new = fill_prev;
		
	  caught = caught +1;
		tostring((uint8_t*) val1, caught);
		GUI_Text( 150, 70, (uint8_t *) val1, Black, White);
	};
	
	
	
	
	//Dynamic reasonableness-test:
	if(abs(WL_new - WL_prev)>10) { //Detection
		WL_r = WL_prev; //update the read-value
		WL_new = WL_prev; //remember to update WL_new;
		
		caught = caught +1;
		tostring((uint8_t*) val1, caught);
		GUI_Text( 150, 70, (uint8_t *) val1, Black, White);
	};
	
	if (abs(fill_new - fill_prev) >2){ //Detection, big leaps from low fills to high fills implies something may be wrong
		fill = fill_prev;
		fill_new = fill_prev;
		
		caught = caught +1;
		tostring((uint8_t*) val1, caught);
		GUI_Text( 150, 70, (uint8_t *) val1, Black, White);

	};
	
	
	// "Contextual reasonableness-test" - here done as last test, so that WL_new & fill_new hopefully are safe
	if (WL_new > 170 && fill_new > 0) {
		//Whenever WL is higher than 170, we should not fill. Based on this context, we can detect errors.
		fill = 0;
		fill_new = 0;
	};
	
	if (WL_new < 10 && fill_new < 2) { //Can also check lower-values for context-correctness
		//Not implemented
	};
	
	
	WL_prev = WL_new; //Save current value for next time
	fill_prev = fill_new; //Save current value
}



void READ_WL (void *p_arg) //Reads water level and creates task
{
	(void)p_arg;
	OS_ERR err;

	OSTmrStart(&TTTmr, &err); //Start the watchdog timer
	
	while(s > WL){ //Simulate reading sensor
		s= s-1;
	}
	WL_r = s;
	s=199;
	
	//Display read Water level
	tostring((uint8_t*) val1, WL_r);
	GUI_Text( 150, 20, "   ", Black, White);
	GUI_Text( 150, 20, (uint8_t *) val1, Black, White);
	
	
	//Decide what to do based on water level
	if(WL_r>170){
		fill=0;	
	} else {
		fill = 1+ outW-WL_r/100;
	}
	
	
	//Stop the timer - indicating that deadline has been met, and system was not delayed significantly 
	OSTmrStop(
				&TTTmr, //timer pointer
				OS_OPT_TMR_NONE,//opts
				0,							//args
				&err					//err
	);
	
}




//Simulates "analog system". Draining water, and displaying what the actual WL is.
//Do not touch, but feel free to read.
void OUTPUT (void *p_arg)
{
	(void)p_arg;
	OS_ERR err;

  // Store the current value of WL in a variable
  int WL_old = WL;

  // Generate a random value between 0 and 1.5 and assign it to outW
  outW = (double)(rand() % 1500) / 1000;

  // Update the value of WL based on the values of outW, drain, and fill
  WL = WL - outW + fill;
	tostring(&val1, WL);
  // Display the updated value of WL on the screen
	GUI_Text(150, 50, "   ", White, Blue);
  GUI_Text(150, 50, (uint8_t*)val1, White, Blue);
  

  // Ensure that WL never goes below 0 and display warning if it does.
  if (WL <= 0) {
    WL = 0;
		GUI_Text(110, 200, "TANK EMPTIED", White, Red);
  }
	//Display warning if WL > 200
	else if (WL > 200) {
		GUI_Text(110, 240, "TANK OVERFLOW", White, Red);
	}
	LCD_DrawLine(0, 119, 100, 119, Black);
	
  // If the value of WL has increased, draw blue lines on the screen
  if (WL_old < WL) {
    for (int i = WL_old; i < WL; i++) {
      int level = 320 - i;
      LCD_DrawLine(0, level, 99, level, Blue);
    }
  }
	
  // If the value of WL has decreased, draw white lines on the screen
  else if (WL_old > WL) {
    for (int i = WL_old; i > WL; i--) {
      int level = 320 - i;
      LCD_DrawLine(0, level, 99, level, White);
    }
  }
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