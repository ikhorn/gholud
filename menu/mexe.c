#include "../rtc.h"
#include "../menucore/usr.h"
#include "../dsp/dsp.h"
#include "../alm.h"
#include "../tmr.h"
#include "../trg.h"
#include "../rmd.h"
#include "../wdt.h"
#include "../res.h"
#include "../def.h"
#include "../tmp.h"
#include "../dmem.h"
#include "mexe.h"
#include "mctr.h"
#include "../delay.h"
#include "../light.h"

//------------------------------------------------------------------------------
//TMR
//------------------------------------------------------------------------------

void mexe_Set_ByDef(void)
//---устанавливает параметры по умолчанию
{
	enum {MAX = 10};
	if (usr_Get_ExeVar(uint8_t_b) == RES_IS_ALM)
	{
		uint8_t i = usr_Get_ExeVar(uint8_t_a);
		for (ubase_t th=0; i < ALM_NUM && th < MAX; i++, th++)					//будильники
		{
			alm_Default(i);
			usr_Get_ExeVar(uint8_t_c)++;
		}
		if (i < ALM_NUM)
		{
			usr_Set_ExeVar(uint8_t_a, i);
			return;
		}
		else
		{
			usr_Set_ExeVar(uint8_t_a, 0);
			usr_Set_ExeVar(uint8_t_b, RES_IS_TMR);
			return;
		}
	}
	else if (usr_Get_ExeVar(uint8_t_b) == RES_IS_TMR)
	{
		uint8_t i = usr_Get_ExeVar(uint8_t_a);
		for (ubase_t th=0; i < TMR_NUM && th < MAX; i++, th++)					//таймера
		{
			tmr_Default(i);
			usr_Get_ExeVar(uint8_t_c)++;
		}
		
		if (i < TMR_NUM)
		{
			usr_Set_ExeVar(uint8_t_a, i);
			return;
		}
		else
		{
			usr_Set_ExeVar(uint8_t_a, 0);
			usr_Set_ExeVar(uint8_t_b, RES_IS_TRG);
			return;
		}
	}
	else if (usr_Get_ExeVar(uint8_t_b) == RES_IS_TRG)
	{
		uint8_t i = usr_Get_ExeVar(uint8_t_a);
		for (ubase_t th=0; i < TRG_NUM && th < MAX; i++, th++)					//терморегуляторы
		{
			trg_Default(i);
			usr_Get_ExeVar(uint8_t_c)++;
		}
		
		if (i < TRG_NUM)
		{
			usr_Set_ExeVar(uint8_t_a, i);
			return;
		}
		else
		{
			usr_Set_ExeVar(uint8_t_a, 0);
			usr_Set_ExeVar(uint8_t_b, RES_IS_RMD);
			return;
		}
	}
	else if (usr_Get_ExeVar(uint8_t_b) == RES_IS_RMD)
	{
		uint8_t i = usr_Get_ExeVar(uint8_t_a);
		for (ubase_t th=0; i < RMD_NUM && th < MAX; i++, th++)					//напоминалки
		{
			rmd_Default(i);
			usr_Get_ExeVar(uint8_t_c)++;
		}
		
		if (i < RMD_NUM)
		{
			usr_Set_ExeVar(uint8_t_a, i);
			return;
		}
		else
		{
			usr_Set_ExeVar(uint8_t_a, 0);
			usr_Set_ExeVar(uint8_t_b, RES_IS_TMP);
			return;
		}
	}
	else if (usr_Get_ExeVar(uint8_t_b) == RES_IS_TMP)
	{
		uint8_t i = usr_Get_ExeVar(uint8_t_a);
		for (ubase_t th=0; i < TMP_NUM && th < MAX; i++, th++)					//датчики температуры
		{
			tmp_Default(i);
			usr_Get_ExeVar(uint8_t_c)++;
		}
		if (i < TMP_NUM)
		{
			usr_Set_ExeVar(uint8_t_a, i);
			return;
		}
		else
		{
			usr_Set_ExeVar(uint8_t_a, 0);
			usr_Set_ExeVar(uint8_t_b, RES_IS_REST);
			return;
		}
	}
	else
	{
		light_Default();														//подсветка
		rtc_Default();															//часы
		dmem_Wr_Psw(DEF_PSW);													//пароль
		dmem_Wr_Psw_Usage(DEF_PSW_USAGE);											//использование пароля
		usr_Off_Executing();
		wdt_Reset_Dev_by_Wdt();
	}
}

