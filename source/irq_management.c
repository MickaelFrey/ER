/*
 * irq_managagement.c
 *
 * Description in irq_managagement.h
 *
 *  Created on: Dec 08, 2018
 *      Author: Anthony Cavin and Mickaël Frey
 *       Place: EPFL
 *        Game: Escape room
 */

#include "irq_management.h"

/*
 * Timer 1 handler.
 * This handler is called each second. It updates the value of the variables
 * 'sec' and 'min'.
 */
void ISR_Timer1()
{
	if(sec == 59)
	{
		sec = 0;
		min = (min + 1) % 59;
	}
	else
		sec++;
}

void configure_room1_irq(){
	/*
	 * Timers initialization. The larger divider is choosen (1024) because
	 * only one interruption every second is needed (1 Hz).
	 * More accurate time measure is not needed for the game.
	 */
	TIMER1_CR = TIMER_DIV_1024  | TIMER_IRQ_REQ | TIMER_ENABLE;
	TIMER1_DATA = TIMER_FREQ_1024(1);

	/*
	 * Irq Handlers set up
	 */
	irqSet(IRQ_TIMER1, &ISR_Timer1);

	irqEnable(IRQ_TIMER1);
}
