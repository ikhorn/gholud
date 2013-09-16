#include "rmd.h"
#include "rtc.h"
#include "menucore/usr.h"
#include "msg.h"
#include "sgn.h"
#include "dmem.h"
#include "def.h"
#include "res.h"
#include "note.h"

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

no_init_ grmd_t gRmd[RMD_NUM];
bool rmd_Check_Trigger(ubase_t rnum);											//---проверяет или не сработал будильник
void rmd_Handle(ubase_t rnum);													//---обрабатывает будильник
bool rmd_Set_Ring(ubase_t rnum);													//---настраивает будильник на статус звонка
bool rmd_Reset(ubase_t rnum);													//---перезапускает будильник
bool rmd_Is_Date(ubase_t rnum, date_t* date);										//---проверяет или сейчас тот день
void rmd_Check_AlarmTime(ubase_t rnum);											//---проверить необходимось в сигнале

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------

void rmd_Ini(void)
{
	for (ubase_t i=0; i<RMD_NUM; i++)
	{
		gRmd[i].state = 0;														//обновить статусы
		uint8_t state = DEF_RMD_STATE;
		dmem_Rd_Rmd_State(i, &state);
		dmem_Rd_Rmd_Melody(i, &gRmd[i].melody);
		dmem_Rd_Rmd_DateOn(i, &gRmd[i].dateon);
		dmem_Rd_Rmd_Filtr(i, &gRmd[i].filtr);
		dmem_Rd_Rmd_Repnum(i, &gRmd[i].repnum);
		if (!state) continue;
		SETB(gRmd[i].state, RMD_STATE_B);
//		SETB(gRmd[i].state, RMD_STOP_B);
		rmd_Resignal(i);
	}
}

bool rmd_State(ubase_t rnum)
//---узнать статус напоминалки, 1 - в сработанном состоянии, 2 - в несработанном состоянии
{
	if (CHKB(gRmd[rnum].state, RMD_STATE_B) && !CHKB(gRmd[rnum].state, RMD_STOP_B) &&
		(CHKB(gRmd[rnum].state, RMD_RING_B) || CHKB(gRmd[rnum].state, RMD_WAITE_B))) 
	return 1;
	return 0;
} 
void rmd_Resignal(ubase_t rnum)
//---пересигналить
{
	if (!CHKB(gRmd[rnum].state, RMD_STATE_B)) return;
	gRmd[rnum].state = 0;														//обновить статусы
	SETB(gRmd[rnum].state, RMD_STATE_B);
	SETB(gRmd[rnum].state, RMD_STOP_B);

	dmem_Rd_Rmd_Repnum(rnum, &gRmd[rnum].repnum);
	dmem_Rd_Rmd_TimeOn(rnum, &gRmd[rnum].time);

	/*проверить или не надо звонить*/
	if (rmd_Is_Date(rnum, &gRmd[rnum].dateon))
	{
		if (!tdf_Time_More(&gRtc.time, &gRmd[rnum].time)) return;
		rmd_Check_AlarmTime(rnum);
	}
	else
	{
		date_t last_date;
		if (!tdf_Date_Minus_Day(&gRtc.date, 1, &last_date)) return;
		if (!rmd_Is_Date(rnum, &last_date)) return;
		if (tdf_Time_More(&gRtc.time, &gRmd[rnum].time)) return;
		rmd_Check_AlarmTime(rnum);
	}
}

void rmd(void)
{
	for (ubase_t rnum=0; rnum<RMD_NUM; rnum++)
	{
		rmd_Handle(rnum);
	}
}


bool rmd_On(ubase_t rnum)
//---включает напоминалку
{
	if (!rmd_Reset(rnum)) return 0;
	if (!dmem_Wr_Rmd_State(rnum, 1)) return 0;
	SETB(gRmd[rnum].state, RMD_STATE_B);
	return 1;
}


bool rmd_Off(ubase_t rnum)
//---отключает напоминалку
{
	sgn_Off();
	CLRB(gRmd[rnum].state, RMD_STATE_B);
	if (!dmem_Wr_Rmd_State(rnum, 0)) return 0;
	return 1;
}


bool rmd_Reset(ubase_t rnum)
//---перезапускает будильник
{
	sgn_Off();
	CLRB(gRmd[rnum].state, RMD_NEAR_B);
	SETB(gRmd[rnum].state, RMD_STOP_B);
	CLRB(gRmd[rnum].state, RMD_RING_B);
	CLRB(gRmd[rnum].state, RMD_WAITE_B);

	time_t time;																	//считать данные чисто для проверки
	uint8_t str[31];
	if (!dmem_Rd_Rmd_TimeDur(rnum, &time)) return 0;
	if (!dmem_Rd_Rmd_DateOn(rnum, &gRmd[rnum].dateon)) return 0;
	if (!dmem_Rd_Rmd_TimeRep(rnum, &time)) return 0;
	if (!dmem_Rd_Rmd_Repnum(rnum, &gRmd[rnum].repnum)) return 0;
	if (!dmem_Rd_Rmd_Filtr(rnum, &gRmd[rnum].filtr)) return 0;
	if (!dmem_Rd_Rmd_Melody(rnum, &gRmd[rnum].melody)) return 0;
	if (!dmem_Rd_Rmd_String(rnum, str)) return 0;
	if (!dmem_Rd_Rmd_TimeOn(rnum, &gRmd[rnum].time)) return 0;					//считать необходимые при старте данные
	uint8_t state;
	if (!dmem_Rd_Rmd_State(rnum, &state)) return 0;
    if (state) SETB(gRmd[rnum].state, RMD_STATE_B);
    else CLRB(gRmd[rnum].state, RMD_STATE_B);
	return 1;
}


void rmd_Reset_Ring(ubase_t rnum)
//---сбрасывает текущий звонок напоминалки
{
	for (;;)
	{
		sgn_Off();
		if (gRmd[rnum].repnum) --gRmd[rnum].repnum;
		if (!gRmd[rnum].repnum) break;

		CLRB(gRmd[rnum].state, RMD_NEAR_B);
		CLRB(gRmd[rnum].state, RMD_STOP_B);
		CLRB(gRmd[rnum].state, RMD_RING_B);
		SETB(gRmd[rnum].state, RMD_WAITE_B);

		uint32_t sec;
		time_t rep_time;
		if (!dmem_Rd_Rmd_TimeRep(rnum, &rep_time)) break;
		if (!tdf_Plus_Time(&gRtc.time, &rep_time, &sec)) break;
		if (!tdf_Sec_to_Time(sec, &gRmd[rnum].time)) break;
		return;
	}
	rmd_Reset(rnum);
}

bool rmd_Set_TimeOn(ubase_t rnum, time_t* time_on)
//---устанавливает время срабатывания
{
	if (!dmem_Wr_Rmd_TimeOn(rnum, time_on)) return 0;
	if (CHKB(gRmd[rnum].state, RMD_STOP_B)) gRmd[rnum].time = *time_on;
	return 1;
}

bool rmd_Set_TimeDur(ubase_t rnum, time_t* time_dur)
//---устанавливает время работы
{
	if (!dmem_Wr_Rmd_TimeDur(rnum, time_dur)) return 0;
	if (CHKB(gRmd[rnum].state, RMD_RING_B))
	{
		gRmd[rnum].time = *time_dur;
	}
	return 1;
}

bool rmd_Set_TimeRep(ubase_t rnum, time_t* time_rep)
//---устанавливает время повтора
{
	if (!dmem_Wr_Rmd_TimeRep(rnum, time_rep)) return 0;
	if (CHKB(gRmd[rnum].state, RMD_WAITE_B))
	{
		gRmd[rnum].time = *time_rep;
	}
	return 1;
}

bool rmd_Set_DateOn(ubase_t rnum, date_t* date)
//---устанавливает дату срабатывания
{
	if (!dmem_Wr_Rmd_DateOn(rnum, date)) return 0;
	gRmd[rnum].dateon = *date;
	return 1;
}

bool rmd_Set_Filtr(ubase_t rnum, uint8_t f)
//---устанавливает повторение
{
	if (!dmem_Wr_Rmd_Filtr(rnum, f)) return 0;
	gRmd[rnum].filtr = f;
	return 1;
}

bool rmd_Set_Melody(ubase_t rnum, uint8_t melody)
//---устанавливает мелодию
{
	if (!dmem_Wr_Rmd_Melody(rnum, melody)) return 0;
	gRmd[rnum].melody = melody;
	return 1;
}

bool rmd_Set_Repnum(ubase_t rnum, uint8_t repnum)
//---устанавливает количество повторов
{
	if (!dmem_Wr_Rmd_Repnum(rnum, repnum)) return 0;
	gRmd[rnum].repnum = repnum;
	return 1;
}

void rmd_Default(ubase_t rnum)
//---по умолчанию значения напоминалки
{
	time_t timeon = DEF_RMD_TIMEON;
	dmem_Wr_Rmd_TimeOn(rnum, &timeon);
	date_t date = DEF_RMD_DATEON;
	dmem_Wr_Rmd_DateOn(rnum, &date);
	dmem_Wr_Rmd_Filtr(rnum, DEF_RMD_FILTR);
	dmem_Wr_Rmd_String(rnum, note_defualt_reminder_string);
	//dmem_Wr_Rmd_String_2(rnum, DEF_RMD_STRING_2);
	dmem_Wr_Rmd_State(rnum, DEF_RMD_STATE);
	time_t timedur = DEF_RMD_TIMEDUR;
	dmem_Wr_Rmd_TimeDur(rnum, &timedur);
	time_t timerep = DEF_RMD_TIMEREP;
	dmem_Wr_Rmd_TimeRep(rnum, &timerep);
	dmem_Wr_Rmd_Repnum(rnum, DEF_RMD_REPNUM);
	dmem_Wr_Rmd_Melody(rnum, DEF_RMD_MELODY);
}

//------------------------------------------------------------------------------
//внут. функции
//------------------------------------------------------------------------------

bool rmd_Check_Trigger(ubase_t rnum)
//---проверяет или не сработала напоминалка
{
	for (;;)
	{
		uint32_t sec;
		if (!tdf_Minus_Time(&gRtc.time, &gRmd[rnum].time, &sec)) break;
		if (sec == 1) SETB(gRmd[rnum].state, RMD_NEAR_B);
		else if (!sec || CHKB(gRmd[rnum].state, RMD_NEAR_B)) return 1;
		return 0;
	}
	rmd_Reset(rnum);
	return 0;
}


void rmd_Handle(ubase_t rnum)
//---обрабатывает напоминалка
{
	if (!CHKB(gRmd[rnum].state, RMD_STATE_B)) return;
	for (;;)
	{
		if (CHKB(gRmd[rnum].state, RMD_STOP_B))									//остановлен ожидая срабатывания
		{
			if (!rmd_Is_Date(rnum, &gRmd[rnum].dateon)) return;
			if (!rmd_Check_Trigger(rnum)) return;								//проверить время
			usr_temp_t var;
			var.uint8_t_a = rnum;
			var.uint8_t_c = 0;
			msg_Post(&msgW_RmdSignal, MSG_BLINK_TYPE, &var);
			if (!rmd_Set_Ring(rnum)) break;
			if (!dmem_Rd_Rmd_Repnum(rnum, &gRmd[rnum].repnum)) break;
		}
		else if (CHKB(gRmd[rnum].state, RMD_RING_B))							//сработал
		{
			sgn_Try_On(RES_IS_RMD, rnum, gRmd[rnum].melody);
			if (!rmd_Check_Trigger(rnum)) return;
			rmd_Reset_Ring(rnum);
		}
		else if (CHKB(gRmd[rnum].state, RMD_WAITE_B))							//ожидает повтора
		{
			if (!rmd_Check_Trigger(rnum)) return;
			if (!gRmd[rnum].repnum) break;
			if (!rmd_Set_Ring(rnum)) break;
			return;
		}
		else break;
		return;
	}
	rmd_Reset(rnum);
}


bool rmd_Set_Ring(ubase_t rnum)
//---настраивает напоминалку на статус звонка
{
	CLRB(gRmd[rnum].state, RMD_NEAR_B);
	CLRB(gRmd[rnum].state, RMD_STOP_B);
	CLRB(gRmd[rnum].state, RMD_WAITE_B);
	SETB(gRmd[rnum].state, RMD_RING_B);
	uint32_t sec;
	time_t dur_time;
	if (!dmem_Rd_Rmd_TimeDur(rnum, &dur_time)) return 0;
	if (!tdf_Plus_Time(&gRtc.time, &dur_time, &sec)) return 0;
	if (!tdf_Sec_to_Time(sec, &gRmd[rnum].time)) return 0;
	sgn_Try_On(RES_IS_RMD, rnum, gRmd[rnum].melody);
	return 1;
}

bool rmd_Is_Date(ubase_t rnum, date_t* date)
//---проверяет или сейчас тот день
{
	switch (gRmd[rnum].filtr)
	{
		case RMD_FILTR_OFF:
		{
			if ((date->year != gRtc.date.year) ||
				(date->ctry != gRtc.date.ctry)) return 0;
		}
		case RMD_FILTR_YEAR:
		{
			if (date->month != gRtc.date.month) return 0;
		}
		case RMD_FILTR_MONTH:
		{
			if (date->date > 0x27 && gRtc.date.date > 0x27)
			{
				bcd_t month_lenth;
				month_lenth = tdf_Month_Lenth(gRtc.date.month, gRtc.date.year);
				if (month_lenth < date->date)
				{
					if (month_lenth == gRtc.date.date) break;
					return 0;
				}
			}
			if (date->date != gRtc.date.date) return 0;
			break;
		}
		default: return 0;
	}
	return 1;
}

void rmd_Check_AlarmTime(ubase_t rnum)
//---проверить необходимось в сигнале
{
	uint32_t past_sec;
	if (!tdf_Minus_Time(&gRmd[rnum].time, &gRtc.time, &past_sec)) return;

	uint32_t ring_sec = 0;														//общая продолжительность сигнала
	uint32_t dur_sec;															//продолжительность самого сигнала
	uint32_t rep_sec;															//продолжительность молчания

	time_t timedur;
	if (!dmem_Rd_Rmd_TimeDur(rnum, &timedur)) return;
	if (!tdf_Time_to_Sec(&timedur, &dur_sec)) return;

	ring_sec += dur_sec*gRmd[rnum].repnum;
	time_t timerep;
	if (!dmem_Rd_Rmd_TimeRep(rnum, &timerep)) return;
	if (!tdf_Time_to_Sec(&timerep, &rep_sec)) return;
	ring_sec += rep_sec*(gRmd[rnum].repnum-1);

	if (past_sec < ring_sec)
	{
		usr_temp_t var;
		var.uint8_t_a = rnum;
		var.uint8_t_c = 0;
		msg_Post(&msgW_RmdSignal, MSG_BLINK_TYPE, &var);
		if (!rmd_Set_Ring(rnum)) return;
	}
}
