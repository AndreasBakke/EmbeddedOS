/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/
#ifndef lpc17xx_H 
	#define lpc17xx_H 
	#include "lpc17xx.h"
#endif

#ifndef ADC_H 
	#define ADC_H 
	#include "../adc/adc.h"	
#endif

#ifndef LED_H 
	#define LED_H 
	#include "../led/led.h"
#endif

#ifndef TIMER_H 
	#define TIMER_H 
	#include "../timer/timer.h"
#endif

#ifndef GLCD_H 
	#define GLCD_H 
	#include "../GLCD/GLCD.h"
#endif


/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */

int check=0;

void ADC_IRQHandler(void) {
  	
	
	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */  
		
	
	if(check!=2){
		if(AD_current != AD_last){
			LED_Off(AD_last*7/0xFFF);	  // ad_last : AD_max = x : 7 		LED_Off((AD_last*7/0xFFF));	
			LED_On(AD_current*7/0xFFF);	// ad_current : AD_max = x : 7 		LED_On((AD_current*7/0xFFF));	
		}
	}
	if(check == 0 && AD_current == 0) 
		check++;
	if(check == 1 && AD_current == 0xFFF)
		check++;
	if(check == 2){
		GUI_Text(160, 140, "   -> ok ", White, Blue);
		GUI_Text(10, 160, "   ? all leds flashing ? ", White, Blue);
		GUI_Text(10, 180, "3) Joystick                ", White, Blue);
		GUI_Text(10, 200, "   -> Toggle 5-ways switch ", White, Blue);
		init_timer(2,0x002625A0);		/* used to blink leds fast 20HZ */
		enable_timer(2);						/* 25MHz/10Hz = 0x002625A0	*/
	}
	
	AD_last = AD_current;
  
	
}
