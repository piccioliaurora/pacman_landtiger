/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "Pacman/pacman.h"
#include "button/button.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile char direction;

int pause = 0;

bool gameStarted = true;

void RIT_IRQHandler (void)
{			
	static int up = 0;
	static int down = 0;
	static int left = 0;
	static int right = 0;
	
	if (gameStarted){
		if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0 || (LPC_GPIO1->FIOPIN & (1<<27)) == 0 || (LPC_GPIO1->FIOPIN & (1<<28)) == 0 || (LPC_GPIO1->FIOPIN & (1<<29)) == 0 ) {
			gameStarted = false;
			clearSection(92, 160);
			enable_timer(2);
			initPowerPills();
		}
	}
	
	if (pause == 0) {
		if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) { 
					up++;
					if (up == 1) direction = 'u';
			} else up = 0;

			if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) { 
					down++;
					if (down == 1) direction = 'd';
			} else down = 0;

			if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) { 
					left++;
					if (left == 1) direction = 'l';
			} else left = 0;

			if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) {
					right++;
					if (right == 1) direction = 'r';
			} else right = 0;
	}
		
	if(int0!=0){
		/*************************INT0***************************/
			if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
				switch(int0){
					case 2:
						if(pause == 0)
							pause = displayPause(1);
						else
							pause = displayPause(0);
						break;
					default:
						break;
				}
				int0++;
			} else {
				int0=0;			
				NVIC_EnableIRQ(EINT0_IRQn);							
				LPC_PINCON->PINSEL4    |= (1 << 20);     
			}
	}
	
	reset_RIT();
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}


/******************************************************************************
**                            End Of File
******************************************************************************/
