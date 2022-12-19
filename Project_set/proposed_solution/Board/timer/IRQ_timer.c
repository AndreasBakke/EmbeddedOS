/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/



#include "lpc17xx.h"
#include "../led/led.h"
#include "timer.h"
#include "../GLCD/GLCD.h"





/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
#define N 10

/* k=1/f'*f/n  k=f/(f'*n) k=25MHz/(f'*45) */

//const int freqs[8]={4240,3779,3367,3175,2834,2525,2249,2120};
/* 
131Hz		k=4240 C3
147Hz		k=3779
165Hz		k=3367
175Hz		k=3175
196Hz		k=2834		
220Hz		k=2525
247Hz		k=2249
262Hz		k=2120 C4
*/

const int freqs[8]={2120,1890,1684,1592,1417,1263,1125,1062};
/*
262Hz	k=2120		c4
294Hz	k=1890		
330Hz	k=1684		
349Hz	k=1592		
392Hz	k=1417		
440Hz	k=1263		
494Hz	k=1125		
523Hz	k=1062		c5

*/

uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

int seq4beats[3][N]={{0,8,7,8,8,8,8,8,8,8},
										 {0,8,3,8,6,8,5,8,7,8},
										 {0,0,0,8,8,0,0,0,8,8}};
/* contains sequence of notes from c4 to C5 */
/* position of c4 = 0 to C5 - 7 in the freq const vect ADC */ 
/* position 8 is an artifact to include a pause of 1/4 beat */	
	
int play;
/* current status of sound (on/off) */

int blink_mask = 0xFF;

extern int check;

void TIMER0_IRQHandler (void)
{
	static int ticks=0;
	/* DAC management */	
	LPC_DAC->DACR = (SinTable[ticks]/3)<<6;
	ticks++;
	if(ticks==45) ticks=0;
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
int song=0;
int note=0;
void TIMER1_IRQHandler (void)
{  	
/*	if(note==N){ 
		note=0;
		song++;
		if(song!=3){
			disable_timer(1);
			disable_timer(0);
		}
		else{
			song--;
		}
	}
	else
		play=seq4beats[song][note++];
	
	if(play!=8){
		disable_timer(0);
		reset_timer(0);
		init_timer(0,freqs[play]);
		enable_timer(0);
	}
	else{
		disable_timer(0);
	}*/
	
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler (void)
{
		
	/*static int j=0;
	if(j%2==0)
		LED_Out(blink_mask);
	else
		LED_Out(0);
	j++;
	
	if(blink_mask==0){
		disable_timer(2);
		LED_Out(0xFF);
	}
	*/
	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER3_IRQHandler (void)
{
	
	/*check=2;
	GUI_Text(10, 280, ">> something went wrong <<", White, Red);
	GUI_Text(10, 300, ">>  reset to try again  <<", White, Red);
	song=2;
	note=0;*/
	//enable_timer(1);	
	LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
