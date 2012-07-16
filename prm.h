/* Module		: prm
 * Created on	: 27 сент. 2011
 * Author		: Ivan Khoronzhook
 * Firstly used	: PATIMA
 */

#ifndef PRM_H_
#define PRM_H_

#include "pdef.h"
#include <avr/pgmspace.h>

//------------------------------------------------------------------------------
//functions
//------------------------------------------------------------------------------

#define prm_Rd_B(BYTE)							pgm_read_byte(BYTE)
void prm_Rd_Data(void *rd_val, uint8_t par_size, const void *prm_par);			//---read data types from programm memory


#endif /* PRM_H_ */
