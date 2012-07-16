/* Обратный секундомер
 * обновлен.....................................................................07.12.2010 15:55:45
*/

#include "tdf.h"
#include "rtc.h"

//------------------------------------------------------------------------------
//состояния
//------------------------------------------------------------------------------

#define BSTW_IS_OFF							1
#define BSTW_IS_STOP						2
#define BSTW_IS_RUN							3
#define BSTW_IS_RING						4


//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

extern void stw_Signal(void);
extern void stw_Signal_Off(void);
typedef struct
{
	ubase_t state;
	ubase_t close;
	time_t time;
} gBSTW_;

no_init_ gBSTW_ gBstw;
void bstw_Off(void);															//---выключает секундомер

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------

void bstw_Ini(void)
{
	bstw_Off();
}


void bstw(void)
{
	if (gBstw.state == BSTW_IS_RUN)
	{
		uint32_t sec;
		if (!tdf_Minus_Time(&gRtc.time, &gBstw.time, &sec)) return;
		if (sec <= 2) gBstw.close = 1;
		else if (!sec || gBstw.close)
		{
#if STWC_USE_SIGNAL == 1
			uint32_t sec;
			if (!tdf_Time_to_Sec(&gRtc.time, &sec)) return;
			sec += STWC_RING_TIME;
			tdf_Sec_to_Time(sec, &gBstw.time);
			
			gBstw.close = 0;
			gBstw.state = BSTW_IS_RING;
			stw_Signal();
#else
			bstw_Off();
#endif
		}
	}
#if STWC_USE_SIGNAL == 1	
	else if (gBstw.state == BSTW_IS_RING)
	{
		stw_Signal();
		uint32_t sec;
		if (!tdf_Minus_Time(&gRtc.time, &gBstw.time, &sec)) return;
		if (sec <= 2) gBstw.close = 1;
		else if (!sec || gBstw.close)
		{
			bstw_Off();
		}
#endif
	}
}


void bstw_Start(time_t* time)
//---запускает обратный таймер
{
	uint32_t sec;
	if (!tdf_Plus_Time(&gRtc.time, time, &sec)) return;
	tdf_Sec_to_Time(sec, &gBstw.time);
	gBstw.state = BSTW_IS_RUN;
	gBstw.close = 0;
}

void bstw_Stop(void)
//---останавливает таймер
{
	gBstw.state = BSTW_IS_STOP;
	uint32_t sec;
	if (!tdf_Minus_Time(&gRtc.time, &gBstw.time, &sec)) return;
	tdf_Sec_to_Time(sec, &gBstw.time);
}

void bstw_Continue(void)
//---продолжает таймер
{
	if (gBstw.state != BSTW_IS_STOP) return;
	bstw_Start(&gBstw.time);
}

void bstw_Off(void)
//---выключает секундомер
{
#if STWC_USE_SIGNAL == 1
	stw_Signal_Off();
#endif
	gBstw.state = BSTW_IS_OFF;
	gBstw.close = 0;
}


void bstw_Rd_Time(time_t* time)
//---считывает время с таймера
{
	for (;;)
	{
		if (gBstw.state == BSTW_IS_STOP)
		{
			*time = gBstw.time;
			return;
		}
		if (gBstw.state != BSTW_IS_RUN) break;
		uint32_t sec;
		if (!tdf_Minus_Time(&gRtc.time, &gBstw.time, &sec)) break;
		tdf_Sec_to_Time(sec, time);
		return;
	}
	time->sec = 0x00;
	time->min = 0x00;
	time->hour = 0x00;
}

