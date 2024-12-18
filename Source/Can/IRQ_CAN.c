#include <LPC17xx.h>                  /* LPC17xx definitions */
#include "CAN.h"                      /* LPC17xx CAN adaption layer */
#include "../GLCD/GLCD.h"

extern uint8_t icr ; 										//icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;
extern CAN_msg       CAN_TxMsg;    /* CAN message for sending */
extern CAN_msg       CAN_RxMsg;    /* CAN message for receiving */                                
extern game_t 			 game;				 /* game values to update */

/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler (void)  {

  /* check CAN controller 1 */
	icr = 0;
  icr = (LPC_CAN1->ICR | icr) & 0xFF;               /* clear interrupts */
	
  if (icr & (1 << 0)) {                          		/* CAN Controller #1 meassage is received */
		CAN_rdMsg (1, &CAN_RxMsg);	                		/* Read the message */
    LPC_CAN1->CMR = (1 << 2);                    		/* Release receive buffer */
		// Should never arrive here
		__ASM("NOP"); /* Debugging purposes */
  }
		
	/* check CAN controller 2 */
	icr = 0;
	icr = (LPC_CAN2->ICR | icr) & 0xFF;             /* clear interrupts */

	if (icr & (1 << 0)) {                          	/* CAN Controller #2 meassage is received */
		CAN_rdMsg (2, &CAN_RxMsg);	                		/* Read the message */
    LPC_CAN2->CMR = (1 << 2);                    		/* Release receive buffer */
		/* CAN2 should receive the update value message and update game's values */
		/* first two bytes are score,
		 * one byte for remaining lives,
		 * one byte for remaining time
		 */
		game.stats.score = (CAN_RxMsg.data[1] << 8);
		game.stats.score = game.stats.score | CAN_RxMsg.data[0];
		game.stats.lifes = CAN_RxMsg.data[2];
		game.stats.time_left = CAN_RxMsg.data[3];
	}
}
