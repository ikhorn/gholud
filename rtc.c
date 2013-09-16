#include "rtc.h"
#include "err.h"
#include "tdf.h"
#include "def.h"
#include "menu/msg.h"
#include "alm.h"
#include "rmd.h"
#include "dmem.h"
#include "drivers/pcf8583.h"
#include "sgn.h"
#include "res.h"


//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

grtc_t gRtc;
//bool rtc_Check_State(void);													//---проверяет настройки часов в нормальном состоянии
void rtc_Restart_TimeDependensis(void);											//---перезапускает ресурсы зависимые от времени

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

void rtc_Ini(void)
{
	pcf8583_Ini();
	rtc_Rd_Time();
	rtc_Rd_Date();
	gRtc.last_hour = gRtc.time.hour;
	gRtc.last_date = gRtc.date.date;
	//gRtc.lunar_day = tdf_Lunar_FlDay(&gRtc.date);
	gRtc.lunar_day = tdf_Lunar_FlDay_and_Distance(&gRtc.date, &gRtc.lunar_distance);
	gRtc.lunar_phase = tdf_Lunar_Phase(gRtc.lunar_day);

	dmem_Rd_Rtc_AvtoShift(&gRtc.avto_shift);
	dmem_Rd_Rtc_HourSgn(&gRtc.hour_sgn);
	dmem_Rd_Rtc_Pore(&gRtc.pore);
	STM_OFF(gRtc.timer);
	STM_OFF(gRtc.pip_timer);
}

void rtc(void)
{
	if (STM_CHECK(gRtc.timer)) return;											//считывать периодично
	STM_SET(gRtc.timer, 0.25);

	for (ubase_t i=0; i<PTRY; i++)
	{
		if (!rtc_Rd_Time()) return;
		if (!rtc_Rd_Date()) return;
		if (!tdf_Is_GoodDate(&gRtc.date) || !tdf_Is_GoodTime(&gRtc.time)) continue;

		if (gRtc.hour_sgn)
		{
			if (gRtc.last_hour != gRtc.time.hour)
			{
				gRtc.last_hour = gRtc.time.hour;
				STM_SET(gRtc.pip_timer, RTC_TIME_HOURSGN);
				sgn_Try_On(RES_IS_RTC, 0, SGN_MEL_BLINK_2);
			}
		}

		if (gRtc.last_date != gRtc.date.date)
		{
			gRtc.last_date = gRtc.date.date;
			//gRtc.lunar_day = tdf_Lunar_FlDay(&gRtc.date);
			gRtc.lunar_day = tdf_Lunar_FlDay_and_Distance(&gRtc.date, &gRtc.lunar_distance);
			gRtc.lunar_phase = tdf_Lunar_Phase(gRtc.lunar_day);
		}

		if (!gRtc.avto_shift) return;

		if (!tdf_SWT(&gRtc.date, &gRtc.time, gRtc.day, &gRtc.pore)) return;
		//usr_temp_t var;
		//var.wst_t_a = gRtc.pore;
		//msg_Post(&msgW_WasTimeShift, MSG_KEY_TYPE, &var);
		STA_N(STA_RTC);
		ubase_t err;
		if ((err = pcf8583_Wr_Date(&gRtc.date)))
		{
			err_Rtc(err);
			return;
		}
		if ((err = pcf8583_Wr_Time(&gRtc.time)))
		{
			err_Rtc(err);
			return;
		}
		rtc_Restart_TimeDependensis();
		dmem_Wr_Rtc_Pore(gRtc.pore);
		gRtc.day = tdf_Date_to_Day(&gRtc.date);
		return;
	}
	err_Rtc(pcf8583_LOST);
}

void rtc_Default(void)
//---устанавливает значения по умолчанию
{
	time_t time = DEF_TIME;
	pcf8583_Wr_Time(&time);
	date_t date = DEF_DATE;
	pcf8583_Wr_Date(&date);
	dmem_Wr_Rtc_AvtoShift(DEF_RTC_AVTOSHIFT);
	dmem_Wr_Rtc_Pore(DEF_RTC_PORE);
	dmem_Wr_Rtc_HourSgn(DEF_RTC_HOURSGN);

}

bool rtc_Set_AvtoShift(ubase_t avto_shift)
//---устанавливает авто переход на зимнее/летнее время
{
	if (!dmem_Wr_Rtc_AvtoShift(avto_shift)) return 0;
	gRtc.avto_shift = avto_shift;
	if (avto_shift)
	{
		tdf_Count_Pore(&gRtc.date, &gRtc.time, gRtc.day, &gRtc.pore);
		if (!dmem_Wr_Rtc_Pore(gRtc.pore)) return 0;
	}
	return 1;
}

bool rtc_Set_HourSgn(ubase_t hour_sgn)
//---устанавливает сигнал каждый час
{
	if (!dmem_Wr_Rtc_HourSgn(hour_sgn)) return 0;
	gRtc.hour_sgn = hour_sgn;
	gRtc.last_hour = gRtc.time.hour;
	return 1;
}

bool rtc_Set_Time(time_t* time)
//---устанавливает время
{
	STA_N(STA_IIC);
	STA_N(STA_RTC);
	ubase_t err;
	if ((err = pcf8583_Wr_Time(time)))
	{
		err_Rtc(err);
		return 0;
	}
	gRtc.time = *time;
	tdf_Count_Pore(&gRtc.date, &gRtc.time, gRtc.day, &gRtc.pore);
	dmem_Wr_Rtc_Pore(gRtc.pore);
	rtc_Restart_TimeDependensis();
	return 1;
}

bool rtc_Rd_Time(void)
//---читает время
{
	ubase_t err;
	for (ubase_t i=0; i<PTRY; i++)
	{
		if ((err = pcf8583_Rd_Time(&gRtc.time)))
			continue;
		STA_N(STA_IIC);
		STA_N(STA_RTC);
		return 1;
	}
	gRtc.time.hour = 0; gRtc.time.min = 0;
	gRtc.time.sec = 0;
	err_Rtc(err);
	return 0;
}

bool rtc_Set_Date(date_t* date)
//---устанавливает дату
{
	STA_N(STA_IIC);
	STA_N(STA_RTC);
	ubase_t err;
	if ((err = pcf8583_Wr_Date(date)))
	{
		err_Rtc(err);
		return 0;
	}
	gRtc.date = *date;
	gRtc.last_date = gRtc.date.date;
	//gRtc.lunar_day = tdf_Lunar_FlDay(&gRtc.date);
	gRtc.lunar_day = tdf_Lunar_FlDay_and_Distance(&gRtc.date, &gRtc.lunar_distance);
	gRtc.lunar_phase = tdf_Lunar_Phase(gRtc.lunar_day);
	gRtc.day = tdf_Date_to_Day(&gRtc.date);
	tdf_Count_Pore(&gRtc.date, &gRtc.time, gRtc.day, &gRtc.pore);
	dmem_Wr_Rtc_Pore(gRtc.pore);
	rtc_Restart_TimeDependensis();
	return 1;
}

bool rtc_Rd_Date(void)
//---читает дату
{
	ubase_t err;
	for (ubase_t i=0; i<PTRY; i++)
	{
		if ((err = pcf8583_Rd_Date(&gRtc.date, &gRtc.day))) continue;
		STA_N(STA_IIC);
		STA_N(STA_RTC);
		return true;
	}
	gRtc.date.date = 0x00; gRtc.date.month = 0x00;
	gRtc.date.year = 0x00; gRtc.date.ctry = 0x20;
	gRtc.day = DAY_NO;
	err_Rtc(err);
	return false;
}

//------------------------------------------------------------------------------
//внут. функции
//------------------------------------------------------------------------------

void rtc_Restart_TimeDependensis(void)
//---перезапускает ресурсы зависимые от времени
{
	sgn_Off();
	for (ubase_t i=0; i<ALM_NUM; i++) alm_ReSignal(i);
	for (ubase_t i=0; i<RMD_NUM; i++) rmd_Resignal(i);
	gRtc.last_hour = gRtc.time.hour;
}
