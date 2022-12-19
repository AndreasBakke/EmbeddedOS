

#include "button.h"
#include "lpc17xx.h"

extern int down_KEY1;
extern int down_KEY2;
extern int down_KEY3;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	down_KEY3=1;
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
	NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
	LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */

}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	down_KEY1=1;
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
	NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
	LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */

}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	down_KEY2=1;
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */   
	NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
	LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
	
}


