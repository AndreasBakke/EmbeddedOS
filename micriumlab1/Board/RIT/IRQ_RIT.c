/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../GLCD/GLCD.h"
#include "../button_EXINT/button.h"
#include "../timer/timer.h"
#include "../TouchPanel/TouchPanel.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

int down_INT0=0;
int down_KEY1=0;
int down_KEY2=0;

extern int blink_mask;
extern int check;

void RIT_IRQHandler (void)
{						
	static int flag=0;
	static int jways=0;
	static char str[2]="0";
	int x,y;
	/* button management */
	if(flag==1){
		/* INT0 */
		if(down_INT0!=0){ 
			if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
				down_INT0++;				
				switch(down_INT0){
					case 2:
						blink_mask &= ~(1<<2);
						GUI_Text(10, 240, "      * INT0 ", White, Blue);
						break;
					default:
						break;
				}
			}
			else {	/* button released */
				down_INT0=0;			
				NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
				LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
			}
		}
		/* KEY1 */
		if(down_KEY1!=0){ 
			if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
				down_KEY1++;				
				switch(down_KEY1){
					case 2:
						blink_mask &= ~(1<<1);
						GUI_Text(105, 240, " * KEY1", White, Blue);
						break;
					default:
						break;
				}
			}
			else {	/* button released */
				down_KEY1=0;			
				NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
				LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
			}
		}
		/* KEY2 */
		if(down_KEY2!=0){ 
			if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
				down_KEY2++;				
				switch(down_KEY2){
					case 2:
						blink_mask &= ~(1<<0);
						GUI_Text(160, 240, " * KEY2 ", White, Blue);
						break;
					default:
						break;
				}
			}
			else {	/* button released */
				down_KEY2=0;			
				NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
				LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
			}
		}
	}	

	if(check==2 && flag==0){
		/* joystick management */
		if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	/* Joytick Select pressed */
			if((blink_mask & (1<<7))!=0){
				GUI_Text(10, 220, "    * Select ", White, Blue);
				jways++;
				str[0]='0'+jways;
				GUI_Text(114, 220, str, White, Blue);
				blink_mask &= ~(1<<7);
			}
		}
		else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	/* Joytick down pulled */
			if((blink_mask & (1<<6))!=0){
				GUI_Text(10, 220, "    * Down   ", White, Blue);
				jways++;
				str[0]='0'+jways;
				GUI_Text(114, 220, str, White, Blue);
				blink_mask &= ~(1<<6);
			}
		}
		else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	/* Joytick left pulled */
			if((blink_mask & (1<<5))!=0){
				GUI_Text(10, 220, "    * Left   ", White, Blue);
				jways++;
				str[0]='0'+jways;
				GUI_Text(114, 220, str, White, Blue);
				blink_mask &= ~(1<<5);
			}		
		}
		else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	/* Joytick right pulled */
			if((blink_mask & (1<<4))!=0){
				GUI_Text(10, 220, "    * Right   ", White, Blue);
				jways++;
				str[0]='0'+jways;
				GUI_Text(114, 220, str, White, Blue);
				blink_mask &= ~(1<<4);
			}					
		}
		else if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	/* Joytick up pulled */
			if((blink_mask & (1<<3))!=0){
				GUI_Text(10, 220, "    * Up   ", White, Blue);
				jways++;
				str[0]='0'+jways;
				GUI_Text(114, 220, str, White, Blue);
				blink_mask &= ~(1<<3);
			}		
		}
	}
	
	if(blink_mask == 0x7){									
			if (flag==0){
				flag=1;				
				GUI_Text(160, 180, "   -> ok                      ", White, Blue);
				GUI_Text(10, 200, "4) Buttons                    ", White, Blue);
				GUI_Text(10, 220, "    -> Press the 3 buttons", White, Blue);				
				BUTTON_init();												/* BUTTON Initialization              */				
			}
	}
	else if(blink_mask == 0x0 && flag==1){
			GUI_Text(160, 200, "   -> ok                      ", White, Blue);			
			GUI_Text(10, 220, "5) Touch Panel:                ", White, Blue);				
			GUI_Text(10, 240, "    * touch the cross          ", White, Blue);
		
			LCD_DrawLine(200, 260, 200, 270, Red);				/*  |  */
			LCD_DrawLine(200, 260, 200, 250, Yellow);		  /*  |  */			
			LCD_DrawLine(210, 260, 200, 260, Green);			/* -   */
			LCD_DrawLine(200, 260, 190, 260, White);			/*   - */
		
			do{
				TP_GetAdXY(&x,&y);
			}while((x<2900 || x>3100)&&(y<2900 || y>3100));
			
			GUI_Text(160, 220, "   -> ok                      ", White, Blue);
			flag=2;	

			GUI_Text(10, 240, "                           ", White, Blue);
			GUI_Text(10, 250, "                           ", White, Blue);
			GUI_Text(10, 260, "                           ", White, Blue);
			GUI_Text(10, 270, "                           ", White, Blue);
			GUI_Text(10, 280, ">>    everything works   <<", Blue, Green);
		  GUI_Text(10, 300, ">> you are ready to work <<", Blue, Green);
		
			enable_timer(1);
			disable_timer(3);
	}
	/* ADC management */
	if(check!=2)
		ADC_start_conversion();		
			
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
