
#include "irq_management.h"


/*
 * Timer 1 handler.
 * This handler is called each 1 ms. It updates the value of the variables
 * 'min', 'sec' and 'msec'.
 */
void ISR_Timer1()
{
	if(msec == 999)
	{
		msec = 0;
		if(sec == 59)
		{
			sec = 0;
			min = (min + 1) % 59;
		}
		else
			sec++;
	}
	else
		msec++;
}

void configure_room1_irq(){
	/*
	 * Timers initialization
	 */
	TIMER1_CR = TIMER_DIV_1024  | TIMER_IRQ_REQ | TIMER_ENABLE;
	TIMER1_DATA = TIMER_FREQ_1024(1000);

	/*
	 * Irq Handlers set up
	 */
	irqSet(IRQ_TIMER1, &ISR_Timer1);

	irqEnable(IRQ_TIMER1);
}
