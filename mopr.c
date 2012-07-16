#include "dsp.h"
#include "mopr.h"
#include "mctr.h"
#include "dmem.h"
#include "alm.h"
#include "tmr.h"
#include "rmd.h"
#include "sgn.h"
#include "res.h"
#include "rtc.h"
#include "asci.h"
#include "note.h"

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//строки
//------------------------------------------------------------------------------

void mopr_Disp_Str_Time(const USRC_STR_ *str, time_t* time)
//---отображает строку + время
{
	dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_11, str, time);
}

const USRC_STR_* mopr_DayStr(day_t day)
//---выдает строку соответствующую дню недели
{
	switch (day)
	{
		case DAY_MON: 			return note_monday;
		case DAY_TUE: 			return note_tuesday;
		case DAY_WED: 			return note_wednesday;
		case DAY_THU: 			return note_thursday;
		case DAY_FRI: 			return note_friday;
		case DAY_SAT: 			return note_saturday;
		case DAY_SUN: 			return note_sunday;
		default: 				return note_bla_bla;
	}
}


const USRC_STR_* mopr_MelStr(ubase_t mel)
//---выдает строку соответствующую мелодии
{
	switch (mel)
	{
		case SGN_MEL_BLINK_1:	return note_blink_1;
		case SGN_MEL_BLINK_2:	return note_blink_2;
		case SGN_MEL_BOMB:		return note_bomb;
		case SGN_MEL_CHAOS:		return note_chaos;
		case SGN_MEL_MARCH:		return note_march;
		case SGN_MEL_SILENCE:	return note_silence;
		case SGN_MEL_EMERGE:	return note_emerge;
		case SGN_MEL_PUM:		return note_pum;
		case SGN_MEL_PIM:		return note_pim;
		case SGN_MEL_CLAVA:		return note_clava;
		case SGN_MEL_CONVER:	return note_conver;
		default:				return note_bla_bla;
	}
}


void mopr_Disp_Str_Date(date_t* date, day_t day, ubase_t strtype)
//---отображает строку + дату с днем недели
{
	dsp_Print(usr_CurStrNum(), strtype, note_cmd_28, date, mopr_DayStr(day));
}

void mopr_Disp_RtcDate(void)
//---отображает дату часов
{
	uint8_t str[DSP_VIR_STR_LENTH+1];
	dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_35, str, &gRtc.date, mopr_DayStr(gRtc.day));
	if (STA_IS_E(STA_RTC))
	{
		str[1] = '-';
		str[2] = '-';
		str[4] = '-';
		str[5] = '-';
		str[7] = '-';
		str[8] = '-';
		str[9] = '-';
		str[10] = '-';
		str[12] = '-';
		str[13] = '-';
	}
	dsp_Put_String(usr_CurStrNum(), str);
}

void mopr_Disp_RtcTime(void)
//---тображает время часов
{
	uint8_t str[DSP_VIR_STR_LENTH+1];
	dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_31, str, &gRtc.time);
	if (STA_IS_E(STA_RTC))
	{
		str[3] = '-';
		str[4] = '-';
		str[6] = '-';
		str[7] = '-';
		str[9] = '-';
		str[10] = '-';
	}
	dsp_Put_String(usr_CurStrNum(), str);
}

void mopr_Disp_RtcTimeDate(void)
//---тображает время часов с датой
{
	uint8_t str[DSP_VIR_STR_LENTH+1];
	if (STA_IS_N(STA_RTC))
    {
        str[0] = (gRtc.time.hour>>4) | 0x30;
        str[1] = (gRtc.time.hour&0x0F) | 0x30;
        str[2] = DSPC_TIME_SEPARATOR;
        str[3] = (gRtc.time.min>>4) | 0x30;
        str[4] = (gRtc.time.min&0x0F) | 0x30;
        str[5] = DSPC_TIME_SEPARATOR;
        str[6] = (gRtc.time.sec>>4) | 0x30;
        str[7] = (gRtc.time.sec&0x0F) | 0x30;
        str[8] = DSP_SYM_SPACE;
        str[9] = (gRtc.date.date>>4) | 0x30;
        str[10] = (gRtc.date.date&0x0F) | 0x30;
        str[11] = DSPC_DATE_SEPARATOR;
        str[12] = (gRtc.date.month>>4) | 0x30;
        str[13] = (gRtc.date.month&0x0F) | 0x30;
        str[14] = DSP_SYM_SPACE;
        str[15] = 0;
    }
    else
    {
        str[0] = '-';
        str[1] = '-';
        str[2] = DSPC_TIME_SEPARATOR;
        str[3] = '-';
        str[4] = '-';
        str[5] = DSPC_TIME_SEPARATOR;
        str[6] = '-';
        str[7] = '-';
        str[8] = DSP_SYM_SPACE;
        str[9] = '-';
        str[10] = '-';
        str[11] = DSPC_DATE_SEPARATOR;
        str[12] = '-';
        str[13] = '-';
        str[14] = DSP_SYM_SPACE;
        str[15] = 0;
    }
    dsp_Put_String(usr_CurStrNum(), str);
}
void mopr_Disp_CheckmarkStr(const USRC_STR_ *str, bool state)
//---отображает строку с знаком выбора
{
	if (state) dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_24, DSP_SYM_CHECKMARK, str);
	else usr_Disp_Str(str);
}

void mopr_Disp_RadioStr(const USRC_STR_ *str, bool state)
//---отображает строку с знаком переключателя
{
	ubase_t sym;
	if (state) sym = NOTE_SYM_RADIOON;
	else sym = NOTE_SYM_RADIOOFF;
	dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_24, sym, str);
}


void mopr_Disp_MelodyType(ubase_t melody)
//---отображает мелодия тип
{
	usr_Disp_2Str(note_mel, mopr_MelStr(melody));
}

void mopr_Disp_FiltrType(ubase_t filtr)
//---отображает повтор тип
{
	const USRC_STR_ *str;
	switch (filtr)
	{
		case RMD_FILTR_MONTH: str = note_month; break;
		case RMD_FILTR_YEAR: str = note_year; break;
		case RMD_FILTR_OFF: str = note_off; break;
		default: str = note_bla_bla; break;
	}
	usr_Disp_2Str(note_filtr, str);
}

void mopr_Disp_AboutRmd(void)
//---выводит инвформация о напоминалке
{
	ubase_t rnum = usr_Get_Var(uint8_t_a);
	time_t time;
	if (!dmem_Rd_Rmd_TimeOn(rnum, &time))
	{
		time.sec = 0x00;
		time.min = 0x00;
		time.hour = 0x00;
	}
	uint8_t str[13];
	str[0] = ((gRmd[rnum].dateon.date>>4) | 0x30);
	str[1] = ((gRmd[rnum].dateon.date & 0x0F) | 0x30);
	str[2] = '.';
	str[3] = ((gRmd[rnum].dateon.month>>4) | 0x30);
	str[4] = ((gRmd[rnum].dateon.month & 0x0F) | 0x30);
	str[5] = ' ';
	str[6] = ((time.hour>>4) | 0x30);
	str[7] = ((time.hour & 0x0F) | 0x30);
	str[8] = ':';
	str[9] = ((time.min>>4) | 0x30);
	str[10] = ((time.min & 0x0F) | 0x30);
	
	if (CHKB(gRmd[rnum].state, RMD_STATE_B)) str[11] = DSP_SYM_CHECKMARK;
	else str[11] = DSP_SYM_SPACE;
	str[12] = 0;
	
	dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_33, 'H', rnum+1, str);
}

void mopr_Disp_OnDays(day_t day)
//---отображает включенные дни
{
	uint8_t str[8];
	uint8_t* sp = str;
	if (day == DAY_ALL)
	{
		*sp++ = ASCI_ve_;//в
		*sp++ = ASCI_es_;//с
		*sp++ = ASCI_ee_;//е
	}
	else if (day == DAY_NO)
	{
		*sp++ = ASCI_en_;//н
		*sp++ = ASCI_ee_;//е
		*sp++ = ASCI_te_;//т
	}
	else
	{
		if (day & DAY_MON) *sp++ = ASCI_PE_;//П
		else *sp++ = ASCI_pe_;//п
			
		if (day & DAY_TUE) *sp++ = ASCI_VE_;//В
		else *sp++ = ASCI_ve_;//в
					
		if (day & DAY_WED) *sp++ = ASCI_ES_;//С
		else *sp++ = ASCI_es_;//с
			
		if (day & DAY_THU) *sp++ = ASCI_CHE_;//Ч
		else *sp++ = ASCI_che_;//ч
		
		if (day & DAY_FRI) *sp++ = ASCI_PE_;//П
		else *sp++ = ASCI_pe_;//п
				
		if (day & DAY_SAT) *sp++ = ASCI_ES_;//С
		else *sp++ = ASCI_es_;//с
			
		if (day & DAY_SUN) *sp++ = ASCI_EN_;//Н
		else *sp++ = ASCI_en_;//н
	}
	*sp = 0;
	dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_30, note_days, str);
}

//------------------------------------------------------------------------------
//ALM
//------------------------------------------------------------------------------

void mopr_Set_Day(const USRC_STR_ *str_day, day_t wday)
//---устанавливает день недели и отображает
{
	day_t day;
	if (usr_Get_Var(uint8_t_b) == RES_IS_ALM)
	{
		day = gAlm.day[usr_Get_Var(uint8_t_a)];
		if (USR_IS_ENTER)
		{
			if (day & wday) day &= ~wday;
			else day |= wday;
			alm_Set_Day(usr_Get_Var(uint8_t_a), day);
		}
	}
	else if (usr_Get_Var(uint8_t_b) == RES_IS_TMR)
	{
		day = gTmr.day[usr_Get_Var(uint8_t_a)];
		if (USR_IS_ENTER)
		{
			if (day & wday) day &= ~wday;
			else day |= wday;
			tmr_Set_Day(usr_Get_Var(uint8_t_a), day);
		}
	}
	mopr_Disp_CheckmarkStr(str_day, (wday & day) ? 1:0);
	menu_Navigate();
}

//------------------------------------------------------------------------------
//SGN
//------------------------------------------------------------------------------
/*
void mopr_Set_Melody(const USRC_STR_ *str_melody, ubase_t m)
//---устанавливает мелодию и отображает
{
	uint8_t melody;
	if (usr_Get_Var(uint8_t_b) == RES_IS_ALM) melody = gAlm.melody[usr_Get_Var(uint8_t_a)];
	else if (usr_Get_Var(uint8_t_b) == RES_IS_RMD) melody = gRmd[usr_Get_Var(uint8_t_a)].melody;
	
	if (USR_ITEM_ACTIVE)
	{
		switch (usr_Key)
		{
			case USR_ENTERKEY_1:
			case USR_ENTERKEY_2:
			{
				if (m != melody)
				{
					melody = m;
					if (usr_Get_Var(uint8_t_b) == RES_IS_ALM) alm_Set_Melody(usr_Get_Var(uint8_t_a), melody);
					else if (usr_Get_Var(uint8_t_b) == RES_IS_RMD) rmd_Set_Melody(usr_Get_Var(uint8_t_a), melody);
				}
				else
				{
					sgn_Off();
					usr_Exit();
					return;
				}
				break;
			}
			case USR_UPKEY:
			case USR_DOWNKEY:
			case USR_RETURNKEY:
			case USR_TOGGLEMENU_KEY:
			{
				sgn_Off();
				break;
			}
			default: sgn_Try_On(RES_IS_MENU, 0, m);
		}
	}	
	
	mopr_Disp_RadioStr(str_melody, (m == melody) ? 1:0);
	menu_Navigate();
}
*/
void mopr_Set_Melody(void)
//---устанавливает мелодию и отображает
{
	uint8_t set_melody;
	if (usr_Get_Var(uint8_t_b) == RES_IS_ALM) set_melody = gAlm.melody[usr_Get_Var(uint8_t_a)];
	else if (usr_Get_Var(uint8_t_b) == RES_IS_RMD) set_melody = gRmd[usr_Get_Var(uint8_t_a)].melody;
	
	
	uint8_t menu_melody = usr_Get_Var(uint8_t_c);
	if (USR_ITEM_ACTIVE)
	{
		switch (usr_Key)
		{
			case USR_ENTERKEY_1:
			case USR_ENTERKEY_2:
			{
				if (menu_melody != set_melody)
				{
					set_melody = menu_melody;
					if (usr_Get_Var(uint8_t_b) == RES_IS_ALM) alm_Set_Melody(usr_Get_Var(uint8_t_a), set_melody);
					else if (usr_Get_Var(uint8_t_b) == RES_IS_RMD) rmd_Set_Melody(usr_Get_Var(uint8_t_a), set_melody);
				}
				else
				{
					sgn_Off();
					usr_Exit();
					return;
				}
				break;
			}
			case USR_UPKEY:
			{
				if (menu_melody) --menu_melody;
				else menu_melody = SGN_MEL_MAX;
				usr_Set_Var(uint8_t_c, menu_melody);
				sgn_Off();
				usr_Key = 0;
				break;
			}
			case USR_DOWNKEY:
			{
				if (menu_melody >= SGN_MEL_MAX) menu_melody = 0;
				else menu_melody++;
				usr_Set_Var(uint8_t_c, menu_melody);
				sgn_Off();
				usr_Key = 0;
				break;
			}
			case USR_RETURNKEY:
			case USR_TOGGLEMENU_KEY:
			{
				sgn_Off();
				break;
			}
			default: {
				sgn_Try_On(RES_IS_MENU, 0, menu_melody);
				break;
			}
		}
	}
	
	mopr_Disp_RadioStr(mopr_MelStr(menu_melody), (menu_melody == set_melody) ? 1:0);
	menu_Navigate();
}

//------------------------------------------------------------------------------
//RMD
//------------------------------------------------------------------------------

void mopr_Set_RmdFiltr(const USRC_STR_ *str_r, ubase_t f)
//---устанавливает повторение и отображает
{
	ubase_t filtr;
	filtr = gRmd[usr_Get_Var(uint8_t_a)].filtr;
	if (USR_IS_ENTER)
	{
		if (f != filtr)
		{
			filtr = f;
			rmd_Set_Filtr(usr_Get_Var(uint8_t_a), filtr);
		}
		else usr_Exit();
	}
	mopr_Disp_RadioStr(str_r, (f == filtr) ? 1:0);
	menu_Navigate();
}


	
void mopr_ListUpDown_Num(ubase_t max)
//---листает номер сущности up или down
{
	if (USR_ITEM_ACTIVE)
	{
		uint8_t num;
		num = usr_Get_Var(uint8_t_a);
		if (usr_Key == USR_UPKEY)
		{
			if (num) --num;
			else num = max;
			usr_Set_Var(uint8_t_a, num);
		}
		else if (usr_Key == USR_DOWNKEY)
		{
			if (num >= max) num = 0;
			else num++;
			usr_Set_Var(uint8_t_a, num);
		}
		else if (usr_Key == USR_ENTERKEY_1 || usr_Key == USR_ENTERKEY_2)
		{
			usr_Set_InVar(uint8_t_a, num);
		}
	}
}
