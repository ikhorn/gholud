/* Прямой екундомер
 * обновлен.....................................................................07.12.2010 14:34:21
*/

#include "rtc.h"
#include "tdf.h"
#include "pdef.h"

//------------------------------------------------------------------------------
//состояния таймера
//------------------------------------------------------------------------------

#define FSTW_IS_OFF							1
#define FSTW_IS_STOP						2
#define FSTW_IS_RUN							3

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

typedef struct
{
	ubase_t state;
	time_t time;
} gFSTW_;

extern gFSTW_ gFstw;

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

no_init_ gFSTW_ gFstw;
void fstw_Off(void);															//---сбрасывает будильник

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------

void fstw_Ini(void)
{
	fstw_Off();
}

void fstw_Start(void)
//---запускает будильник
{
	gFstw.state = FSTW_IS_RUN;
	gFstw.time = gRtc.time;
}

void fstw_Stop(void)
//---останавливает будильник
{
	for (;;)
	{
		uint32_t sec;
		if (!tdf_Minus_Time(&gFstw.time, &gRtc.time, &sec)) break;
		if (!tdf_Sec_to_Time(sec, &gFstw.time)) break;
		gFstw.state = FSTW_IS_STOP;
		return;
	}
	fstw_Off();
}

void fstw_Continue(void)
{
	if (gFstw.state != FSTW_IS_STOP) return;
	for (;;)
	{
		uint32_t sec;
		if (!tdf_Minus_Time(&gFstw.time, &gRtc.time, &sec)) break;
		if (!tdf_Sec_to_Time(sec, &gFstw.time)) break;
		gFstw.state = FSTW_IS_RUN;
		return;
	}
	fstw_Off();
}

void fstw_Off(void)
//---сбрасывает будильник
{
	gFstw.state = FSTW_IS_OFF;
}

void fstw_Rd_Time(time_t* time)
//---читает время секундомера
{
	switch (gFstw.state)
	{
		case FSTW_IS_RUN:
		{
			uint32_t sec;
			if (!tdf_Minus_Time(&gFstw.time, &gRtc.time, &sec)) break;
			if (!tdf_Sec_to_Time(sec, time)) break;	
			return;
		}
		case FSTW_IS_STOP:
		{
			*time = gFstw.time;
			return;
		}
		default: break;
	}
	time->sec = 0x00;
	time->min = 0x00;
	time->hour = 0x00;
}
