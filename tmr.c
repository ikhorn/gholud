#include "tmr.h"
#include "tdf.h"
#include "rtc.h"
#include "def.h"
#include "dmem.h"


//------------------------------------------------------------------------------
//работа с портами
//------------------------------------------------------------------------------

#define SIGNAL_ON(TMR)															\
{																				\
	SETB(TMR_SIGNAL_DDR, TMR);													\
	SETB(TMR_SIGNAL_PORT, TMR);													\
}

#define SIGNAL_OFF(TMR)															\
{																				\
	SETB(TMR_SIGNAL_DDR, TMR);													\
	CLRB(TMR_SIGNAL_PORT, TMR);													\
}

#define SIGNAL_TO_Z(TMR)														\
{																				\
	CLRB(TMR_SIGNAL_DDR, TMR);													\
	CLRB(TMR_SIGNAL_PORT, TMR);													\
}


#define SIGNAL_STATE(TMR)						CHKB(TMR_SIGNAL_PORT, TMR)		//проверяет состояние выхода таймера

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

gtmr_t gTmr;
//bool tmr_Set_Signal(ubase_t tnum);												//---настраивает таймер на срабатывание
bool tmr_On_Signal(ubase_t tnum);												//---включает сигнал таймера
bool tmr_Off_Signal(ubase_t tnum);												//---отключает сигнал таймера
void tmr_Handle(ubase_t tnum);													//---обрабатывает таймер
bool tmr_Restart(ubase_t tnum);													//---перезапускает таймер

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------

void tmr_Ini(void)
{
	gTmr.state_vector = 0;
	gTmr.type_vector = 0;
	for (ubase_t tnum=0; tnum<TMR_NUM; tnum++)
	{
		uint8_t state;
		if (!dmem_Rd_Tmr_State(tnum, &state)) state = DEF_TMR_STATE;
		if (state) SETB(gTmr.state_vector, tnum);

		dmem_Rd_Tmr_TimeOn(tnum, &gTmr.timeon[tnum]);
		dmem_Rd_Tmr_TimeOff(tnum, &gTmr.timeoff[tnum]);
		dmem_Rd_Tmr_Day(tnum, &gTmr.day[tnum]);
		uint8_t type;
		if (!dmem_Rd_Tmr_Type(tnum, &type)) type = DEF_TMR_TYPE;
		if (type) SETB(gTmr.type_vector, tnum);
		//tmr_Off_Signal(tnum);
		SIGNAL_TO_Z(tnum)
	}
}


void tmr(void)
{
	for (ubase_t tnum=0; tnum<TMR_NUM; tnum++)
	{
		tmr_Handle(tnum);
	}
}


bool tmr_On(ubase_t tnum)
//---включает таймер
{
	if (!tmr_Restart(tnum)) return 0;
	if (!dmem_Wr_Tmr_State(tnum, 1)) return 0;
	SETB(gTmr.state_vector, tnum);
	return 1;
}


bool tmr_Off(ubase_t tnum)
//---отключает таймер
{
	SIGNAL_TO_Z(tnum);
	CLRB(gTmr.state_vector, tnum);
	if (dmem_Wr_Tmr_State(tnum, 0)) return 1;
	return 0;
}


bool tmr_Set_TimeOn(ubase_t tnum, time_t* timeon)
//---задать время включения
{
	if (!dmem_Wr_Tmr_TimeOn(tnum, timeon)) return 0;
	gTmr.timeon[tnum] = *timeon;
	return 1;
}

bool tmr_Set_TimeOff(ubase_t tnum, time_t* timeoff)
//---задать время тключения
{
	if (!dmem_Wr_Tmr_TimeOff(tnum, timeoff)) return 0;
	gTmr.timeoff[tnum] = *timeoff;
	return 1;
}

bool tmr_Set_Day(ubase_t tnum, day_t day)
//---устанавливает дни недели
{
	if (!dmem_Wr_Tmr_Day(tnum, day)) return 0;
	gTmr.day[tnum] = day;
	return 1;
}


bool tmr_Set_Type(ubase_t tnum, uint8_t type)
//---устанавливает тип выходного сигнала
{
	if (!dmem_Wr_Tmr_Type(tnum, type)) return 0;
	if (type) SETB(gTmr.type_vector, tnum);
	else CLRB(gTmr.type_vector, tnum);
	return 1;
}

void tmr_Default(ubase_t tnum)
//---по умолчанию значения таймера
{
	time_t timeon = DEF_TMR_TIMEON;
	dmem_Wr_Tmr_TimeOn(tnum, &timeon);
	time_t timeoff = DEF_TMR_TIMEOFF;
	dmem_Wr_Tmr_TimeOff(tnum, &timeoff);
	dmem_Wr_Tmr_Day(tnum, DEF_TMR_DAY);
	dmem_Wr_Tmr_State(tnum, DEF_TMR_STATE);
	dmem_Wr_Tmr_Type(tnum, DEF_TMR_TYPE);
}

bool tmr_OutState(ubase_t tnum)
//---проверяет выхода таймеров
{
	if (SIGNAL_STATE(tnum) && CHKB(gTmr.type_vector, tnum)) return 1;
	else if (SIGNAL_STATE(tnum) && CHKB(gTmr.type_vector, tnum)) return 1;
	else return 0;
}


//------------------------------------------------------------------------------
//внут. функции
//------------------------------------------------------------------------------

void tmr_Handle(ubase_t tnum)
{
	if (!CHKB(gTmr.state_vector, tnum)) return;
	if (!(gRtc.day & gTmr.day[tnum])) return;									//проверь или день подходит

	if (tdf_Time_More(&gTmr.timeoff[tnum], &gTmr.timeon[tnum]))
	{
		if (tdf_Time_More(&gRtc.time, &gTmr.timeon[tnum]))
		{
			if (tdf_Time_More(&gRtc.time, &gTmr.timeoff[tnum]))
			{
				tmr_Off_Signal(tnum);
			}
			else tmr_On_Signal(tnum);
		}
		else tmr_Off_Signal(tnum);
	}
	else
	{
		if (tdf_Time_More(&gRtc.time, &gTmr.timeon[tnum]))
		{
			tmr_On_Signal(tnum);
		}
		else
		{
			if (tdf_Time_More(&gRtc.time, &gTmr.timeoff[tnum]))
			{
				tmr_Off_Signal(tnum);
			}
			else tmr_On_Signal(tnum);
		}
	}
}

bool tmr_On_Signal(ubase_t tnum)
//---включает сигнал таймера
{
	if (CHKB(gTmr.type_vector, tnum))
	{
		SIGNAL_ON(tnum);
	}
	else
	{
		SIGNAL_OFF(tnum);
	}
	return 1;
}


bool tmr_Off_Signal(ubase_t tnum)
//---отключает сигнал таймера
{
	if (CHKB(gTmr.type_vector, tnum))
	{
		SIGNAL_OFF(tnum);
	}
	else
	{
		SIGNAL_ON(tnum);
	}
	return 1;
}


bool tmr_Restart(ubase_t tnum)
//---перезапускает таймер
{
	tmr_Off_Signal(tnum);
	if (!dmem_Rd_Tmr_TimeOff(tnum, &gTmr.timeoff[tnum])) return 0;
	if (!dmem_Rd_Tmr_TimeOn(tnum, &gTmr.timeon[tnum])) return 0;					//считать необходимые при старте данные
	if (!dmem_Rd_Tmr_Day(tnum, &gTmr.day[tnum])) return 0;
	uint8_t type;
	if (!dmem_Rd_Tmr_Type(tnum, &type)) return 0;
	if (type) SETB(gTmr.type_vector, tnum);
	else CLRB(gTmr.type_vector, tnum);
	return 1;
}
