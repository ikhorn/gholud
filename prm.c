/* Module		: prm
 * Created on	: 27 сент. 2011
 * Author		: Ivan Khoronzhook
 * Firstly used	: PATIMA
 */

#include "prm.h"


//------------------------------------------------------------------------------
//external functions
//------------------------------------------------------------------------------

void prm_Rd_Data(void *rd_val, uint8_t par_size, const void *prm_par)
//---read data types from programm memory
{
	uint8_t* rd_byte;
	rd_byte = (uint8_t*)rd_val;
	uint8_pr_t* par_byte;
	par_byte = (uint8_pr_t*)prm_par;
	for (uint8_t i=0; i < par_size; i++) {
		*rd_byte++ = prm_Rd_B(par_byte++);
	}
}

void prm_Rd_Line(uint8_t *ram_line, const uint8_pr_t* prm_line)
{
	do
	{
		*ram_line = prm_Rd_B(prm_line++);
	} while (*ram_line++);
}

