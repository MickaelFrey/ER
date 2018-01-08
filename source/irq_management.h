/*
 * irq_management.h
 *
 * Manage interruptions of the game.
 *
 *  Created on: Dec 08, 2018
 *      Author: Anthony Cavin and Mickaël Frey
 *       Place: EPFL
 *        Game: Escape room
 */

#ifndef IRQ_MANAGEMENT_H_
#define IRQ_MANAGEMENT_H_

#include <nds.h>
#include <stdio.h>

/*
 * Declaration of min and sec needed by the timer 1 call-back function.
 */
extern int min, sec;

/*
 * IRQ management for Room 1
 */
void configure_room1_irq();

#endif /* IRQ_MANAGEMENT_H_ */
