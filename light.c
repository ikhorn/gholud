#include "dsp.h"
#include "def.h"
#include "dmem.h"
#include "light.h"
#include "pdef.h"

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

gLIGHT_ gLight;

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------


void light_Ini(void)
{
	dmem_Rd_Light_DefState(&gLight.def_state);
	if (gLight.def_state)
	{
		dsp_On_Light();
		gLight.state = 1;
	}
	else
	{
		dsp_Off_Light();
		gLight.state = 0;
	}
}

void light_Toggle(void)
//---переключает состояние подсветки
{
	if (gLight.state)
	{
		dsp_Off_Light();
		gLight.state = 0;
	}
	else
	{
		dsp_On_Light();
		gLight.state = 1;
	}
}

bool light_Set_DefState(uint8_t def_state)
//---устанавливает состояние подсветки по умолчанию
{
	if (!dmem_Wr_Light_DefState(def_state)) return 0;
	gLight.def_state = def_state;
	return 1;
}

void light_Default(void)
//---устанавливает значение по умолчанию
{
	dmem_Wr_Light_DefState(DEF_LIGTH_STATE);
}
