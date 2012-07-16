#include "sgn.h"
#include "res.h"
#include "dsp.h"
#include "alm.h"
#include "note.h"
#include "asci.h"

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------

void stp_Handler(void)
{
	uint8_t sym_1;
	uint8_t sym_2;
	if (gSgn.state)
	{
		if (gSgn.host_name == RES_IS_ALM)
		{
			if (STM_IS_2TICK) sym_1 = NOTE_SYM_BELL;
			else sym_1 = DSP_SYM_SPACE;
			sym_2 = (gSgn.host_num|0x30)+1;
		}
		else if (gSgn.host_name == RES_IS_RMD)
		{
			if (STM_IS_2TICK) sym_1 = ASCI_en_;
			else sym_1 = DSP_SYM_SPACE;
			sym_2 = (gSgn.host_num|0x30)+1;
		}
		else
		{
			sym_1 = DSP_SYM_SPACE;
			sym_2 = DSP_SYM_SPACE;
		}
	}
	else
	{
		sym_2 = DSP_SYM_SPACE;
		if (gAlm.waite_vector) sym_1 = NOTE_SYM_BELL;
		else sym_1 = DSP_SYM_SPACE;
	}
	
	dsp_Set_Sym(DSP_STR_START_ADR_0+DSP_VIR_STR_LENTH, sym_1);
	dsp_Set_Sym(DSP_STR_START_ADR_1+DSP_VIR_STR_LENTH, sym_2);
}
