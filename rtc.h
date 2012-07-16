/* Модуль часов реального времени
 * обновлен.....................................................................08.12.2010 20:56:10
*/

#include "pdef.h"
#include "tdf.h"

//------------------------------------------------------------------------------
//время работы часового сигнала
//------------------------------------------------------------------------------

#define RTC_TIME_HOURSGN					1.0
#define RTC_HOURSGN							SGN_MEL_BLINK_2

//------------------------------------------------------------------------------
//типы
//------------------------------------------------------------------------------

#ifndef RTC
#define RTC
typedef struct
{
	time_t time;
	date_t date;
	day_t day;
	ubase_t avto_shift;
	ubase_t hour_sgn;
	ubase_t timer;
	wst_t pore;
	bcd_t last_hour;
	bcd_t last_date;
	fl32_t lunar_day;
	ubase_t lunar_phase;
	fl32_t lunar_distance;
	ubase_t pip_timer;
}
grtc_t;
#endif

extern grtc_t gRtc;

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

void rtc_Ini(void);
void rtc(void);
void rtc_Default(void);															//---устанавливает значения по умолчанию
bool rtc_Set_AvtoShift(ubase_t avto_shift);										//---устанавливает авто переход на зимнее/летнее время
bool rtc_Set_HourSgn(ubase_t hour_sgn);											//---устанавливает сигнал каждый час
bool rtc_Set_Time(time_t* time);												//---устанавливает время
bool rtc_Rd_Time(void);															//---читает время

bool rtc_Set_Date(date_t* date);												//---устанавливает дату
bool rtc_Rd_Date(void);															//---читает дату
bool rtc_Count_Pore(date_t date, time_t time, day_t day, wst_t* wst);			//---выдает текущее зимнее/летнее

#define RTC_CHECK_TIMERS()														\
{																				\
	STM_BEGIN_ACT(gRtc.timer)													\
	STM_END_ACT																	\
	STM_BEGIN_ACT(gRtc.pip_timer)												\
		sgn_Off();																\
	STM_END_ACT																	\
}
