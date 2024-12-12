#include "LPC17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void JOYSTICK_init(void) {
	/* joystick up, left, right, down functionality */
	// PIN mode GPIO (00b value per P1.26 .. P1.29
  LPC_PINCON->PINSEL3 &= ~(3<<20);
	LPC_PINCON->PINSEL3 &= ~(3<<22);
	LPC_PINCON->PINSEL3 &= ~(3<<24);
	LPC_PINCON->PINSEL3 &= ~(3<<26);
	LPC_GPIO1->FIODIR   &= ~(1<<26);
	LPC_GPIO1->FIODIR   &= ~(1<<27);
	LPC_GPIO1->FIODIR   &= ~(1<<28);
	LPC_GPIO1->FIODIR   &= ~(1<<29);
}
