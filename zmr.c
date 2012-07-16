#include "delay.h"
#include "tc3.h"
#include "zmr.h"
#include "pdef.h"


//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------

void zmr_Ini(void)
{
	CLRB(TC3_OCA_PORT, TC3_OCA_B);
	CLRB(TC3_OCA_DDR, TC3_OCA_B);

	TC3_SET_SOURCE(TC3_NO_CLOCK);
	TC3_MODE_CTC_OC();
	TC3_SET_OUT_TOGGLE_OCA();
	TC3_OFF_OUT_OCB();
	TC3_OFF_OUT_OCC();

	TC3_SET_COUNTER(0);
	TC3_DBL_ACOMP_INT();
	TC3_DBL_BCOMP_INT();
	TC3_DBL_CCOMP_INT();
	TC3_DBL_OVER_INT();
	TC3_DBL_INPCAPT_INT();
}


void zmr_On(uint16_t frq)
//---включает зумер на заданной частоте
{
	TC3_SET_SOURCE(TC3_DIV1);
	TC3_SET_TOPA(frq);
	TC3_SET_OUT_TOGGLE_OCA();
}


void zmr_Off(void)
//---отключает зумер
{
	TC3_OFF_OUT_OCA();
	TC3_SET_SOURCE(TC3_NO_CLOCK);
}
