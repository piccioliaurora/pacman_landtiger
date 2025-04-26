#include "button.h"
#include "LPC17xx.h"

int int0;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	int0 = 1;
	NVIC_DisableIRQ(EINT0_IRQn);
	LPC_PINCON->PINSEL4 &= ~(1 << 20);
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}

