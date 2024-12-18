#include "LPC17xx.h"
#include "adc.h"
#include "../timer/timer.h"

unsigned short AD_current;   
unsigned short AD_last = 0xFF;

//const int freqs[8]={4240,3779,3367,3175,2834,2525,2249,2120};
/* 
131Hz		k=4240 C3
147Hz		k=3779
165Hz		k=3367
175Hz		k=3175
196Hz		k=2834		
220Hz		k=2525
247Hz		k=2249
262Hz		k=2120 C4
*/

const int freqs[8]={2120,1890,1684,1592,1417,1263,1125,1062};
/*
262Hz	k=2120		c4
294Hz	k=1890		
330Hz	k=1684		
349Hz	k=1592		
392Hz	k=1417		
440Hz	k=1263		
494Hz	k=1125		
523Hz	k=1062		c5
*/

void ADC_IRQHandler(void) {
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
  if(AD_current != AD_last){
		disable_timer(2);
		reset_timer(2);
		init_timer(2,freqs[AD_current*7/0xFFF]);
		enable_timer(2);
		
		AD_last = AD_current;
  }
	
}
