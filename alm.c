#include "tdf.h"
#include "alm.h"
#include "rtc.h"
#include "sgn.h"
#include "def.h"
#include "res.h"
#include "usr.h"
#include "msg.h"
#include "dmem.h"

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

galm_t gAlm;
bool alm_Is_Trigger(ubase_t anum);											    //---проверяет или не сработал будильник
void alm_Handle(ubase_t anum);													//---обрабатывает будильник
bool alm_Set_Ring(ubase_t anum);												//---настраивает будильник на статус звонка
bool alm_Reset(ubase_t anum);													//---перезапускает будильник
bool alm_Restart(ubase_t anum);                                                 //---загружает начальные данные будильника заново

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------

void alm_Ini(void)
{
	gAlm.state_vector = 0;														//обновить статусы
	gAlm.near_vector = 0;
	gAlm.stop_vector = 0;
	gAlm.ring_vector = 0;
	gAlm.waite_vector = 0;
	gAlm.message_vector = 0;
	gAlm.times_vector = 0;
	for (ubase_t i=0; i<ALM_NUM; i++)
	{
		dmem_Rd_Alm_Repnum(i, &gAlm.repnum[i]);
		dmem_Rd_Alm_Melody(i, &gAlm.melody[i]);
		dmem_Rd_Alm_Day(i, &gAlm.day[i]);
		uint8_t message = DEF_ALM_MESSAGE;
		dmem_Rd_Alm_Message(i, &message);
		if (message) SETB(gAlm.message_vector, i);
		else CLRB(gAlm.message_vector, i);
		uint8_t times = DEF_ALM_TIMES;
		dmem_Rd_Alm_Times(i, &times);
		if (times) SETB(gAlm.times_vector, i);
		else CLRB(gAlm.times_vector, i);
		uint8_t state = DEF_ALM_STATE;
		dmem_Rd_Alm_State(i, &state);
		if (!state) continue;
		SETB(gAlm.state_vector, i);
		SETB(gAlm.stop_vector, i);
		alm_ReSignal(i);
	}
}

bool alm_State(ubase_t anum)
//---узнать статус будильника, 1 - в сработанном состоянии, 2 - в несработанном состоянии
{
	if (CHKB(gAlm.state_vector, anum) && !CHKB(gAlm.stop_vector, anum) &&
		(CHKB(gAlm.ring_vector, anum) || CHKB(gAlm.waite_vector, anum)))
	return 1;
	return 0;
}

void alm_ReSignal(ubase_t anum)
//---пересигналить
{
	if (!CHKB(gAlm.state_vector, anum)) return;
	CLRB(gAlm.near_vector, anum);
	SETB(gAlm.stop_vector, anum);
	CLRB(gAlm.ring_vector, anum);
	CLRB(gAlm.waite_vector, anum);
	dmem_Rd_Alm_Repnum(anum, &gAlm.repnum[anum]);
	dmem_Rd_Alm_TimeOn(anum, &gAlm.time[anum]);
	/*проверить или не надо звонить*/
	if (!(gRtc.day & gAlm.day[anum])) return;
	uint32_t past_sec;															//разница между текущим и временем срабатывания таймера
	if (!tdf_Minus_Time(&gAlm.time[anum], &gRtc.time, &past_sec)) return;

	uint32_t ring_sec = 0;														//общая продолжительность сигнала
	uint32_t dur_sec;															//продолжительность самого сигнала
	uint32_t rep_sec;															//продолжительность молчания
	time_t timedur;
	if (!dmem_Rd_Alm_TimeDur(anum, &timedur)) return;
	if (!tdf_Time_to_Sec(&timedur, &dur_sec)) return;

	ring_sec += dur_sec*gAlm.repnum[anum];
	time_t timerep;
	if (!dmem_Rd_Alm_TimeRep(anum, &timerep)) return;
	if (!tdf_Time_to_Sec(&timerep, &rep_sec)) return;
	ring_sec += rep_sec*(gAlm.repnum[anum]-1);

	if (past_sec < ring_sec)
	{
		if (CHKB(gAlm.message_vector, anum))
		{
			usr_temp_t var;
			var.uint8_t_a = anum;
			msg_Post(&msgW_AlmWas, MSG_KEY_TYPE, &var);
		}
		if (!alm_Set_Ring(anum)) return;
	}
}

void alm(void)
{
	for (ubase_t anum=0; anum<ALM_NUM; anum++)
	{
		alm_Handle(anum);
	}
}


bool alm_On(ubase_t anum)
//---включает будильник
{
	if (!alm_Restart(anum)) return 0;
	if (!dmem_Wr_Alm_State(anum, 1)) return 0;
	SETB(gAlm.state_vector, anum);
	return 1;
}


bool alm_Off(ubase_t anum)
//---отключает будильник
{
	sgn_Off();
	CLRB(gAlm.state_vector, anum);
	if (dmem_Wr_Alm_State(anum, 0)) return 1;
	return 0;
}


bool alm_Reset(ubase_t anum)
//---сбрасывает будильник
{
    if (CHKB(gAlm.times_vector, anum))
    {
        if (!alm_Off(anum)) return 0;
    }
    return alm_Restart(anum);
}


void alm_Reset_Ring(ubase_t anum)
//---сбрасывает текущий звонок будильника
{
	for (;;)
	{
		sgn_Off();
		if (gAlm.repnum[anum]) --gAlm.repnum[anum];
		if (!gAlm.repnum[anum])
        {
            alm_Reset(anum);
            return;
        }

		CLRB(gAlm.near_vector, anum);
		CLRB(gAlm.stop_vector, anum);
		CLRB(gAlm.ring_vector, anum);
		SETB(gAlm.waite_vector, anum);

		uint32_t sec;
		time_t rep_time;
		if (!dmem_Rd_Alm_TimeRep(anum, &rep_time)) break;
		if (!tdf_Plus_Time(&gRtc.time, &rep_time, &sec)) break;
		if (!tdf_Sec_to_Time(sec, &gAlm.time[anum])) break;
		return;
	}
	alm_Restart(anum);
}

bool alm_Set_Melody(ubase_t anum, uint8_t melody)
//---устанавливает мелодию
{
	if (!dmem_Wr_Alm_Melody(anum, melody)) return 0;
	gAlm.melody[anum] = melody;
	return 1;
}


bool alm_Set_TimeOn(ubase_t anum, time_t* time_on)
//---устанавливает время срабатывания
{
	if (!dmem_Wr_Alm_TimeOn(anum, time_on)) return 0;
	if (CHKB(gAlm.stop_vector, anum))
	{
		gAlm.time[anum] = *time_on;
	}
	return 1;
}

bool alm_Set_TimeDur(ubase_t anum, time_t* time_dur)
//---устанавливает время работы
{
	if (!dmem_Wr_Alm_TimeDur(anum, time_dur)) return 0;
	if (CHKB(gAlm.ring_vector, anum))
	{
		gAlm.time[anum] = *time_dur;
	}
	return 1;
}

bool alm_Set_TimeRep(ubase_t anum, time_t* time_rep)
//---устанавливает время повтора
{
	if (!dmem_Wr_Alm_TimeRep(anum, time_rep)) return 0;
	if (CHKB(gAlm.waite_vector, anum))
	{
		gAlm.time[anum] = *time_rep;
	}
	return 1;
}

bool alm_Set_Repnum(ubase_t anum, uint8_t repnum)
//---устанавливает количество повторов
{
	if (!dmem_Wr_Alm_Repnum(anum, repnum)) return 0;
	gAlm.repnum[anum] = repnum;
	return 1;
}

bool alm_Set_Day(ubase_t anum, day_t day)
//---устанавливает дни недели
{
	if (!dmem_Wr_Alm_Day(anum, day)) return 0;
	gAlm.day[anum] = day;
	return 1;
}

bool alm_Set_Message(ubase_t anum, uint8_t message)
//---устанавливает будет ли выводиться сообщение
{
	if (message) SETB(gAlm.message_vector, anum);
	else CLRB(gAlm.message_vector, anum);
	if (!dmem_Wr_Alm_Message(anum, message)) return 0;
	return 1;
}

bool alm_Set_Times(ubase_t anum, uint8_t times)
//---устанавливает разовое срабатывание будильника
{
	if (times) SETB(gAlm.times_vector, anum);
	else CLRB(gAlm.times_vector, anum);
	if (!dmem_Wr_Alm_Times(anum, times)) return 0;
	return 1;
}

void alm_Default(ubase_t anum)
//---по умолчанию значения будильника
{
	time_t timeon = DEF_ALM_TIMEON;
	dmem_Wr_Alm_TimeOn(anum, &timeon);
	time_t timedur = DEF_ALM_TIMEDUR;
	dmem_Wr_Alm_TimeDur(anum, &timedur);
	time_t timerep = DEF_ALM_TIMEREP;
	dmem_Wr_Alm_TimeRep(anum, &timerep);
	dmem_Wr_Alm_Repnum(anum, DEF_ALM_REPNUM);
	dmem_Wr_Alm_Day(anum, DEF_ALM_DAY);
	dmem_Wr_Alm_State(anum, DEF_ALM_STATE);
	dmem_Wr_Alm_Melody(anum, DEF_ALM_MELODY);
	dmem_Wr_Alm_Message(anum, DEF_ALM_MESSAGE);
	dmem_Wr_Alm_Times(anum, DEF_ALM_TIMES);
}

//------------------------------------------------------------------------------
//внут. функции
//------------------------------------------------------------------------------

bool alm_Is_Trigger(ubase_t anum)
//---проверяет или не сработал будильник
{
	for (;;)
	{
		uint32_t sec;
		if (!tdf_Minus_Time(&gRtc.time, &gAlm.time[anum], &sec)) break;
		if (sec == 1) SETB(gAlm.near_vector, anum);
		else if (!sec || CHKB(gAlm.near_vector, anum))
		{
			CLRB(gAlm.near_vector, anum);
			return 1;
		}
		return 0;
	}
	alm_Restart(anum);
	return 0;
}


void alm_Handle(ubase_t anum)
//---обрабатывает будильник
{
	if (!CHKB(gAlm.state_vector, anum)) return;
	for (;;)
	{
		if (CHKB(gAlm.stop_vector, anum))										//остановлен ожидая срабатывания
		{
			if (!(gRtc.day & gAlm.day[anum])) return;
			if (!alm_Is_Trigger(anum)) return;
			if (CHKB(gAlm.message_vector, anum))
			{
				usr_temp_t var;
				var.uint8_t_a = anum;
				msg_Post(&msgW_AlmWas, MSG_KEY_TYPE, &var);
			}
			if (!alm_Set_Ring(anum)) break;
		}
		else if (CHKB(gAlm.ring_vector, anum))									//сработал
		{
			sgn_Try_On(RES_IS_ALM, anum, gAlm.melody[anum]);
			if (!alm_Is_Trigger(anum)) return;
			alm_Reset_Ring(anum);
		}
		else if (CHKB(gAlm.waite_vector, anum))									//ожидает повтора
		{
			if (!alm_Is_Trigger(anum)) return;
			if (!alm_Set_Ring(anum)) break;
			return;
		}
		else break;
		return;
	}
	alm_Restart(anum);
}

bool alm_Set_Ring(ubase_t anum)
//---настраивает будильник на статус звонка
{
	CLRB(gAlm.near_vector, anum);
	CLRB(gAlm.stop_vector, anum);
	CLRB(gAlm.waite_vector, anum);
	SETB(gAlm.ring_vector, anum);

	uint32_t sec;
	time_t dur_time;
	if (!dmem_Rd_Alm_TimeDur(anum, &dur_time)) return 0;
	if (!tdf_Plus_Time(&gRtc.time, &dur_time, &sec)) return 0;
	if (!tdf_Sec_to_Time(sec, &gAlm.time[anum])) return 0;
	sgn_Try_On(RES_IS_ALM, anum, gAlm.melody[anum]);
	return 1;
}


bool alm_Restart(ubase_t anum)
//---загружает начальные данные будильника заново
{
	sgn_Off();
	CLRB(gAlm.near_vector, anum);
	SETB(gAlm.stop_vector, anum);
	CLRB(gAlm.ring_vector, anum);
	CLRB(gAlm.waite_vector, anum);

	time_t time;																//считать данные чисто для проверки
	uint8_t message;
	uint8_t times;
	if (!dmem_Rd_Alm_TimeDur(anum, &time)) return 0;
	if (!dmem_Rd_Alm_TimeRep(anum, &time)) return 0;
	if (!dmem_Rd_Alm_Repnum(anum, &gAlm.repnum[anum])) return 0;
	if (!dmem_Rd_Alm_Day(anum, &gAlm.day[anum])) return 0;
	if (!dmem_Rd_Alm_Melody(anum, &gAlm.melody[anum])) return 0;
	if (!dmem_Rd_Alm_Message(anum, &message)) return 0;
	if (message) SETB(gAlm.message_vector, anum);
	else CLRB(gAlm.message_vector, anum);
	if (!dmem_Rd_Alm_Times(anum, &times)) return 0;
	if (times) SETB(gAlm.times_vector, anum);
	else CLRB(gAlm.times_vector, anum);
	if (!dmem_Rd_Alm_TimeOn(anum, &gAlm.time[anum])) return 0;					//считать необходимые при старте данные
	return 1;
}
