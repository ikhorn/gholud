#include "sta.h"

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

gsta_t gSta;

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

void sta_Ini(void)
{
	STA_N(STA_IIC);
	STA_N(STA_MEM);
	STA_N(STA_TMP_1);
	STA_N(STA_TMP_2);
	STA_N(STA_RTC);
}

