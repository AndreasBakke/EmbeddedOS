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
#include <stdlib.h>
#include <time.h>

short down_KEY1;
short down_KEY2;
short down_KEY3;
     /* ----------------- APPLICATION GLOBALS ------------------ */

/* Definition of the Task Control Blocks for all tasks */
static  OS_TCB        APP_TSK_TCB;
static	OS_TCB			  READ_WL_TCB; //Task for reading waterlevel
//static	OS_TCB				DRAIN_TCB;
//static  OS_TCB				FILL_TCB;
static  OS_TCB        OUTPUT_TCB;

static OS_TMR  WLTmr; //Waterlevel timer
static OS_TMR  OUTPUTTmr; //Random output timer

/* Definition of the Stacks for three tasks */
static  CPU_STK_SIZE  APP_TSK_STACK[APP_CFG_TASK_STK_SIZE];
static	CPU_STK_SIZE	READ_WL_STACK[APP_CFG_TASK_STK_SIZE];
//static  CPU_STK_SIZE	DRAIN_STACK[APP_CFG_TASK_STK_SIZE];
//static  CPU_STK_SIZE	FILL_STACK[APP_CFG_TASK_STK_SIZE];
static  CPU_STK_SIZE	OUTPUT_STACK[APP_CFG_TASK_STK_SIZE];


/* Global Variables*/
uint16_t position = 0;
uint16_t counter = 0;
static short s = 0;

static short max_WL = 200; //Define absolute max water level of tank (Liters)
static short alert_WL = 180;  //What WL should give alert.
double WL = 200; //Start water level at 150L - actual water level
unsigned char WL_r = 0; //Read water level
int outW, drain, fill = 0; //How to make theese double or something without going over memory limit?
uint8_t * val1[16];
OS_SEM sem_counter;
OS_SEM sem_position;

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
		
		LCD_Clear(White);
	
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

static  void  APP_TSK (void *p_arg)
{
    (void)p_arg;        /* See Note #1                                          */
		OS_ERR  os_err;
		OS_ERR err;
		CPU_BOOLEAN  status;
		BSP_OS_TickEnable(); /* Enable the tick timer and interrupt                  */
		LCD_Initialization();
		joystick_init();
		BUTTON_init();
		LCD_Clear(White);
		
		//Draw initial watertank and static elements
	  LCD_DrawLine(0, 119, 100, 119, Black);
	  LCD_DrawLine(100, 120, 100, 320, Black);
		GUI_Text( 10, 100, &s, Black, White);
		GUI_Text( 50, 20, "Read value:", Black, White);
		GUI_Text(50, 50, "Act", White, Blue);
		int i=0;
		while (i<WL){
			int level = 320-i;
			LCD_DrawLine(0, level, 99, level, Blue);
			//LCD_DrawLine(140, level, 240, level, Blue);
			i=i+1;
		}
		
		OSTmrStart(&WLTmr, &err);
		OSTmrStart(&OUTPUTTmr, &err);
	  
		while (DEF_TRUE) {
			if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){ //Joystick up
				int bit_pos = rand() % 16;
				s ^= (1 << bit_pos); //Flips bit in sensor read (May introduce timing fault)
				tostring((uint8_t*) val1, s);
				GUI_Text(0,0,val1, Black, White);
			}
			else
				if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	/* Joystick down  */
				//GUI_Text(0,0,"D", Black, White);
				//unsigned char bit_pos = rand() % 8;
				//WL_r ^= (1 << bit_pos); //Flips bit in read value
			}

			else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	/* Joystick Left */
				//GUI_Text(0,0,"L", Black, White);
				unsigned char bit_pos = rand() %4;
				drain ^=(1 << bit_pos);
				//Flip Drain
			}

			else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	/* Joytick right  */
				unsigned char bit_pos = rand() %4;
				fill ^=(1 << bit_pos);
				//int bit_pos = rand() %32;
				//int *addr = (int *)rand();
				//*addr ^= 1 << bit_pos;
				//GUI_Text(0,0,"R", Black, White);
				//unsigned char bit_pos = rand() %8;
				//drain ^=(1 << bit_pos);
				//flip drain
			}
			/*if (down_KEY2 == 1) {
					unsigned char bit_pos = rand() % 8;
					WL_r ^= (1 << bit_pos); //Flips bit in read value
					down_KEY2 = 0;
			}
			if (down_KEY3 == 1) { //Flips random memory adress ? (Made by ChatGTP)
					int bit_pos = rand() %8;
					int *addr = (int *)rand();
					*addr ^= 1 << bit_pos;
				
				
				//Memory address 1 start: 0x10000000
				// Size: 0x8000
				
				//Mem 2: 0x2007C000
				//Size 0x8000
					down_KEY3 = 0;
			}*/
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
	while(s < WL){ //Simulate reading sensor
		WL_r = s;
		s= s+1;
	}
	s=0;
	tostring((uint8_t*) val1, WL_r);
	
	GUI_Text( 150, 20, (uint8_t *) val1, Black, White);
	
	
	if(WL_r>180){
		fill=0;	
    drain = 3;
	} else {
		drain = 0;
		fill = 1+ outW*2-WL_r/100;
	}
}




void OUTPUT (void *p_arg) //Drains random amount between 0 and 1L every 1/th of a second
{

	(void)p_arg;
	OS_ERR err;

  // Store the current value of WL in a variable
  int WL_old = WL;

  // Generate a random value between 0 and 1.5 and assign it to outW
  outW = (double)(rand() % 1500) / 1000;

  // Update the value of WL based on the values of outW, drain, and fill
  WL = WL - outW - drain + fill;
	tostring(&val1, WL);
  // Display the updated value of WL on the screen
  GUI_Text(150, 50, (uint8_t*)val1, White, Blue);
  

  // Ensure that WL never goes below 0
  if (WL < 0) {
    WL = 0;
		GUI_Text(110, 200, "TANK EMPTIED", White, Red);
  }
	
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