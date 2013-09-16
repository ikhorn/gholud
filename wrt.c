#include "dsp/dsp.h"
#include "menucore/usr.h"
#include "rtc.h"
#include "tmp.h"
#include "alm.h"
#include "tmr.h"
#include "rmd.h"
#include "trg.h"
#include "menucore/prog/stw.h"
#include "mopr.h"
#include "note.h"

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

void wrt_Disp_Str(const USRC_STR_ *str);										//---отображает флеш строку на рабочем столе

//-----------------------------------------------------------------------------
//							КАРТЫ РАБОЧИХ СТОЛОВ
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//								 Фаза луны
//------------------------------------------------------------------------------
void wrt_I_LunarPhase_Day(void);
void wrt_I_LunarPhase_Phase(void);
//массив обработчиков пунктов
usrcw_ func_t* const listLunarPhase[] =
{
 	wrt_I_LunarPhase_Day,
 	wrt_I_LunarPhase_Phase
};
usr_wnd_t wrtW_LunarPhase = {listLunarPhase, list_size(listLunarPhase)};
//рабочие столы для перехода
#define LEFTWRT_LUNARPHASE					stwW
#define RIGHTWRT_LUNARPHASE					wrtW_Inf
//------------------------------------------------------------------------------
//								 Инфо
//------------------------------------------------------------------------------
void wrt_I_Inf_Cholud(void);
void wrt_I_Inf_Version(void);
//массив обработчиков пунктов
usrcw_ func_t* const listInf[] =
{
 	wrt_I_Inf_Cholud,
 	wrt_I_Inf_Version
};
usr_wnd_t wrtW_Inf = {listInf, list_size(listInf)};
//рабочие столы для перехода
#define LEFTWRT_Inf							wrtW_LunarPhase
#define RIGHTWRT_Inf						wrtW_Ini

//------------------------------------------------------------------------------
//								 ВРЕМЯ/ДАТА
//------------------------------------------------------------------------------
void wrt_I_Ini_Time(void);
void wrt_I_Ini_Date(void);
//массив обработчиков пунктов
usrcw_ func_t* const listIni[] =
{
 	wrt_I_Ini_Time,
 	wrt_I_Ini_Date,
};
usr_wnd_t wrtW_Ini = {listIni, list_size(listIni)};
//рабочие столы для перехода
#define LEFTWRT_INI							wrtW_Inf
#define RIGHTWRT_INI						wrtW_TimeTemp

//------------------------------------------------------------------------------
//								 ВРЕМЯ/ТЕМПЕРАТУРА
//------------------------------------------------------------------------------
void wrt_I_TimeTemp_Time(void);
void wrt_I_TimeTemp_Temp(void);
//массив обработчиков пунктов
usrcw_ func_t* const listTimeTemp[] =
{
 	wrt_I_TimeTemp_Time,
 	wrt_I_TimeTemp_Temp,
};
usr_wnd_t wrtW_TimeTemp = {listTimeTemp, list_size(listTimeTemp)};
//рабочие столы для перехода
#define LEFTWRT_TIMETEMP					wrtW_Ini
#define RIGHTWRT_TIMETEMP					wrtW_Bd

//------------------------------------------------------------------------------
//								 БУДИЛЬНИКИ
//------------------------------------------------------------------------------
void wrt_I_Bd_Nums(void);
void wrt_I_Bd_States(void);
//массив обработчиков пунктов
usrcw_ func_t* const listBd[] =
{
 	wrt_I_Bd_Nums,
 	wrt_I_Bd_States,
};
usr_wnd_t wrtW_Bd = {listBd, list_size(listBd)};
//рабочие столы для перехода
#define LEFTWRT_BD							wrtW_TimeTemp
#define RIGHTWRT_BD							stwW
#define DOWNWRT_BD							wrtW_Tm
#define UPWRT_BD							wrtW_Np

//------------------------------------------------------------------------------
//								 ТАЙМЕРА
//------------------------------------------------------------------------------
void wrt_I_Tm_Nums(void);
void wrt_I_Tm_States(void);
//массив обработчиков пунктов
usrcw_ func_t* const listTm[] =
{
 	wrt_I_Tm_Nums,
 	wrt_I_Tm_States,
};
usr_wnd_t wrtW_Tm = {listTm, list_size(listTm)};
//рабочие столы для перехода
#define LEFTWRT_TM							wrtW_TimeTemp
#define RIGHTWRT_TM							stwW
#define DOWNWRT_TM							wrtW_Tr
#define UPWRT_TM							wrtW_Bd

//------------------------------------------------------------------------------
//								 ТЕРМОРЕГУЛЯТОРЫ
//------------------------------------------------------------------------------
void wrt_I_Tr_Nums(void);
void wrt_I_Tr_States(void);
//массив обработчиков пунктов
usrcw_ func_t* const listTr[] =
{
 	wrt_I_Tr_Nums,
 	wrt_I_Tr_States,
};
usr_wnd_t wrtW_Tr = {listTr, list_size(listTr)};
//рабочие столы для перехода
#define LEFTWRT_TR							wrtW_TimeTemp
#define RIGHTWRT_TR							stwW
#define DOWNWRT_TR							wrtW_Np
#define UPWRT_TR							wrtW_Tm

//------------------------------------------------------------------------------
//								 НАПОМИНАЛКИ
//------------------------------------------------------------------------------
void wrt_I_Np_Nums(void);
void wrt_I_Np_States(void);
//массив обработчиков пунктов
usrcw_ func_t* const listNp[] =
{
 	wrt_I_Np_Nums,
 	wrt_I_Np_States,
};
usr_wnd_t wrtW_Np = {listNp, list_size(listNp)};
//рабочие столы для перехода
#define LEFTWRT_NP							wrtW_TimeTemp
#define RIGHTWRT_NP							stwW
#define DOWNWRT_NP							wrtW_Bd
#define UPWRT_NP							wrtW_Tr



//------------------------------------------------------------------------------
//пункты
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//								 Инфо
//------------------------------------------------------------------------------
void wrt_I_Inf_Cholud(void)
{
	wrt_Disp_Str(note_devname);
}

void wrt_I_Inf_Version(void)
{
	wrt_Disp_Str(note_version);
 	wrt_Scan_Key(&LEFTWRT_Inf, &RIGHTWRT_Inf);
}

//------------------------------------------------------------------------------
//								 ВРЕМЯ/ДАТА
//------------------------------------------------------------------------------

void wrt_I_Ini_Time(void)
{
	mopr_Disp_RtcTime();
}

void wrt_I_Ini_Date(void)
{
	mopr_Disp_RtcDate();
    const usr_wnd_t* right_wnd;
    if (CHKB(gTmp.state_vector, 0) || CHKB(gTmp.state_vector, 1)) right_wnd = &RIGHTWRT_INI;
    else right_wnd = &RIGHTWRT_TIMETEMP;
 	wrt_Scan_Key(&LEFTWRT_INI, right_wnd);
}

//------------------------------------------------------------------------------
//								 ВРЕМЯ/ТЕМПЕРАТУРА
//------------------------------------------------------------------------------

void wrt_I_TimeTemp_Time(void)
{
	mopr_Disp_RtcTimeDate();
}

void wrt_I_TimeTemp_Temp(void)
{
	fl32_t t1;
	fl32_t t2;
	uint8_t str[DSP_STR_LENTH];
	TMP_1_CODE_TO_TEMP(gTmp.code[0], t1);
	TMP_2_CODE_TO_TEMP(gTmp.code[1], t2);
	dsp_Print(usr_CurStrNum(), DSP_STR_SPACE_PUTTED, note_cmd_32, str, t1, t2);
	if (STA_IS_E(STA_TMP_1) || !CHKB(gTmp.state_vector, 0)) for (ubase_t i=3; i<3+4; i++) str[i] = '-';
	if (STA_IS_E(STA_TMP_2) || !CHKB(gTmp.state_vector, 1)) for (ubase_t i=11; i<11+4; i++) str[i] = '-';
	dsp_Put_String(usr_CurStrNum(), str);
 	wrt_Scan_Key(&LEFTWRT_TIMETEMP, &RIGHTWRT_TIMETEMP);
}

//------------------------------------------------------------------------------
//								 БУДИЛЬНИКИ
//------------------------------------------------------------------------------

void wrt_I_Bd_Nums(void)
{
	dsp_Print(gUsr.strnum, DSP_STR_LEFT_PUTTED, note_cmd_3, note_Bd, note_12345678);
}

void wrt_I_Bd_States(void)
{
	uint8_t str[12];
	str[0] = ' ';
	str[1] = ' ';
	str[2] = ' ';
	ubase_t s = 3;
	for (ubase_t i=0; i<ALM_NUM; i++, s++)
	{
		if (CHKB(gAlm.state_vector, i)) str[s] = DSP_SYM_CHECKMARK;
		else str[s] = DSP_SYM_SPACE;
	}
	str[s] = 0;
	dsp_Print(gUsr.strnum, DSP_STR_LEFT_PUTTED, note_cmd_8, str);

    const usr_wnd_t* left_wnd;
    if (CHKB(gTmp.state_vector, 0) || CHKB(gTmp.state_vector, 1)) left_wnd = &LEFTWRT_BD;
    else left_wnd = &LEFTWRT_TIMETEMP;
	
	if (usr_Key == USR_DOWNKEY) usr_Replace(&DOWNWRT_BD, 1, USR_WND_WRT);
	else if (usr_Key == USR_UPKEY)
	{
		usr_Set_Var(uint8_t_a, 1);
		usr_Replace(&UPWRT_BD, 1, USR_WND_WRT);
	}
 	else wrt_Scan_Key(left_wnd, &RIGHTWRT_BD);
}

//------------------------------------------------------------------------------
//								 ТАЙМЕРА
//------------------------------------------------------------------------------

void wrt_I_Tm_Nums(void)
{
	dsp_Print(gUsr.strnum, DSP_STR_LEFT_PUTTED, note_cmd_3, note_Tm, note_12345678);
}

void wrt_I_Tm_States(void)
{
	uint8_t str[12];
	str[0] = ' ';
	str[1] = ' ';
	str[2] = ' ';
	ubase_t s = 3;
	for (ubase_t i=0; i<TMR_NUM; i++, s++)
	{
		if (CHKB(gTmr.state_vector, i))
		{
			if (tmr_OutState(i))
			{
				if (STM_IS_2TICK) str[s] = DSP_SYM_CHECKMARK;
				else str[s] = DSP_SYM_SPACE;
			}
			else str[s] = DSP_SYM_CHECKMARK;
		}
		else str[s] = DSP_SYM_SPACE;
	}
	str[s] = 0;
	dsp_Print(gUsr.strnum, DSP_STR_LEFT_PUTTED, note_cmd_8, str);
	
	const usr_wnd_t* left_wnd;
    if (CHKB(gTmp.state_vector, 0) || CHKB(gTmp.state_vector, 1)) left_wnd = &LEFTWRT_TM;
    else left_wnd = &LEFTWRT_TIMETEMP;

	if (usr_Key == USR_DOWNKEY) usr_Replace(&DOWNWRT_TM, 1, USR_WND_WRT);
	else if (usr_Key == USR_UPKEY) usr_Replace(&UPWRT_TM, 1, USR_WND_WRT);
 	else wrt_Scan_Key(left_wnd, &RIGHTWRT_TM);
}

//------------------------------------------------------------------------------
//								 ТЕРМОСТАТЫ
//------------------------------------------------------------------------------

void wrt_I_Tr_Nums(void)
{
	dsp_Print(gUsr.strnum, DSP_STR_LEFT_PUTTED, note_cmd_3, note_Tr, note_12);
}

void wrt_I_Tr_States(void)
{
	uint8_t str[12];
	str[0] = ' ';
	str[1] = ' ';
	str[2] = ' ';
	ubase_t s = 3;
	for (ubase_t i=0; i<TRG_NUM; i++, s++)
	{
		if (CHKB(gTrg.state_vector, i))
		{
			if (trg_OutState(i))
			{
				if (STM_IS_2TICK) str[s] = DSP_SYM_CHECKMARK;
				else str[s] = DSP_SYM_SPACE;
			}
			else str[s] = DSP_SYM_CHECKMARK;
		}
		else str[s] = DSP_SYM_SPACE;
	}
	str[s] = 0;
	
	dsp_Print(gUsr.strnum, DSP_STR_LEFT_PUTTED, note_cmd_8, str);
	
    const usr_wnd_t* left_wnd;
    if (CHKB(gTmp.state_vector, 0) || CHKB(gTmp.state_vector, 1)) left_wnd = &LEFTWRT_TR;
    else left_wnd = &LEFTWRT_TIMETEMP;

	if (usr_Key == USR_DOWNKEY)
	{
		usr_Set_Var(uint8_t_a, RMD_NUM-9);
		usr_Replace(&DOWNWRT_TR, 1, USR_WND_WRT);
	}
	else if (usr_Key == USR_UPKEY) usr_Replace(&UPWRT_TR, 1, USR_WND_WRT);
 	else wrt_Scan_Key(left_wnd, &RIGHTWRT_TR);
}

//------------------------------------------------------------------------------
//								 НАПОМИНАЛКИ
//------------------------------------------------------------------------------

void wrt_I_Np_Nums(void)
{
	dsp_Print(gUsr.strnum, DSP_STR_LEFT_PUTTED, note_cmd_3, note_Np, note_0123456789);
}

void wrt_I_Np_States(void)
{
	uint8_t str[11];
	ubase_t degree;
	degree = usr_Get_Var(uint8_t_a);
	if (degree%10 || degree > 90)
	{
		usr_Set_Var(uint8_t_a, 1);
		degree = 1;
	}
	ubase_t s = degree-1;
	for (ubase_t i=0; i<10; i++, s++)
	{
		if (CHKB(gRmd[s].state, RMD_STATE_B)) str[i] = DSP_SYM_CHECKMARK;
		else str[i] = DSP_SYM_SPACE;
	}
	str[10] = 0;
	dsp_Print(gUsr.strnum, DSP_STR_LEFT_PUTTED, note_cmd_34, degree, str);
	
    const usr_wnd_t* left_wnd;
    if (CHKB(gTmp.state_vector, 0) || CHKB(gTmp.state_vector, 1)) left_wnd = &LEFTWRT_NP;
    else left_wnd = &LEFTWRT_TIMETEMP;

	if (usr_Key == USR_DOWNKEY)
	{
		if (degree <= 1)
		{
			usr_Replace(&DOWNWRT_NP, 1, USR_WND_WRT);
			return;
		}
		else
		{
			degree -= 10;
			if (degree < 1) degree = 1;
			usr_Set_Var(uint8_t_a, degree);
		}
		usr_Key = 0;
		USR_REFRESH_DISP();
	}
	else if (usr_Key == USR_UPKEY)
	{
		if (degree >= RMD_NUM-9)
		{
			usr_Replace(&UPWRT_NP, 1, USR_WND_WRT);
			return;
		}
		else
		{
			if (degree == 1) --degree;
			degree += 10;
			usr_Set_Var(uint8_t_a, degree);
		}
		usr_Key = 0;
		USR_REFRESH_DISP();
	}
	else wrt_Scan_Key(left_wnd, &RIGHTWRT_NP);
}

//------------------------------------------------------------------------------
//								 Фаза луны
//------------------------------------------------------------------------------
void wrt_I_LunarPhase_Day(void)
{
	dsp_Print(usr_CurStrNum(), DSP_STR_SPACE_PUTTED, note_cmd_37, note_LD, gRtc.lunar_day, note_L, gRtc.lunar_distance);
}

void wrt_I_LunarPhase_Phase(void)
{
	const USRC_STR_ *str;
	switch (gRtc.lunar_phase)
	{
		case 1: str = note_new_luna; break;
		case 2: str = note_waxing_crescent; break;
		case 3: str = note_first_quarter; break;
		case 4: str = note_waxing_gibbous; break;
		case 5: str = note_full_luna; break;
		case 6: str = note_waning_gibbous; break;
		case 7: str = note_last_quarter; break;
		case 8: str = note_waning_crescent; break;
		default: str = note_new_luna; break;
	}
	dsp_Print(usr_CurStrNum(), DSP_STR_SPACE_PUTTED, note_cmd_39, gRtc.lunar_phase, str);
 	wrt_Scan_Key(&LEFTWRT_LUNARPHASE, &RIGHTWRT_LUNARPHASE);
}

//------------------------------------------------------------------------------
//внутренние функции
//------------------------------------------------------------------------------

void wrt_Disp_Str(const USRC_STR_ *str)
//---отображает flash строку на рабочем столе
{
	dsp_Print(usr_CurStrNum(), DSP_STR_SPACE_PUTTED, note_cmd_1, str);
}

