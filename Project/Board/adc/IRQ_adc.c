/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "./adc.h"	
#include "../led/led.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

extern unsigned short AD_current;   

int check=0;

void ADC_IRQHandler(void) {
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */  
	GUI_Text(10,100, (uint8_t *) "ADC IRQ HANDLER Executed", Red, Green);
}
