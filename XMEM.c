

#include <avr/io.h>
#include "bit_macros.h"
//Enable external memory interface, and remove the 4 most significant bits so it doesn't interfere with JTAG
void XMEM_init(void){

	set_bit(MCUCR,SRE);		
	set_bit(SFIOR,XMM2);	


}
