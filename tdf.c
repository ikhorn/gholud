#include "tdf.h"
#include "mat.h"
#include "math.h"

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

ubase_t tdf_Sunday(ubase_t cur_date, day_t cur_day);								//---находит день соответствующий воскресеню в последней неделе
fl64_t tdf_normalize(fl64_t val);													//---нормализация для функции лунного дня

//------------------------------------------------------------------------------
//время
//------------------------------------------------------------------------------

bool tdf_Is_GoodTime(time_t *time)
//---проверяет корректность времени
{
	uint8_t hour;
	if (!bcd_Bcd_to_Num(time->hour, &hour)) return 0;
	if (hour >= 24) return 0;
	
	uint8_t min;
	if (!bcd_Bcd_to_Num(time->min, &min)) return 0;
	if (min >= 60) return 0;
	
	uint8_t sec;
	if (!bcd_Bcd_to_Num(time->sec, &sec)) return 0;
	if (sec >= 60) return 0;
	return 1;
}


bool tdf_Time_to_Sec(time_t *in_time, uint32_t *out_time)
//---переводит время в формате bcd в секунды
{
	uint8_t sec;
	if (!bcd_Bcd_to_Num(in_time->sec, &sec)) return 0;
	if (sec >= 60) return 0;
	(*out_time) = (uint32_t)sec;
	
	uint8_t min;
	if (!bcd_Bcd_to_Num(in_time->min, &min)) return 0;
	if (min >= 60) return 0;
	(*out_time) = (*out_time) + (uint32_t)min*60;
	
	uint8_t hour;
	if (!bcd_Bcd_to_Num(in_time->hour, &hour)) return 0;
	if (hour >= 24) return 0;
	(*out_time) = (*out_time) + (uint32_t)hour*3600;
	return 1;
}


bool tdf_Sec_to_Time(uint32_t in_sec, time_t *out_time)
//---переводит время в секундах в эквивалетное время суток
{
	for (; in_sec >= 86400;) in_sec = in_sec - 86400;							//свести кол. секнд к одним суткам
 	uint8_t hour = 0;
	for (; in_sec >= 3600; hour++) in_sec = in_sec - 3600;						//вычислить кол часов
	uint8_t min = 0;
	for (; in_sec >= 60; min++) in_sec = in_sec - 60;							//вычислить количество минут
	uint8_t sec = (uint8_t)in_sec;
	
	if (!bcd_Num_to_Bcd(hour, &(out_time->hour))) return 0;
	if (!bcd_Num_to_Bcd(min, &(out_time->min))) return 0;
	if (!bcd_Num_to_Bcd(sec, &(out_time->sec))) return 0;
	return 1;
}


bool tdf_Minus_Time(time_t *time_1, time_t *time_2, uint32_t *sec)
//---вычисляет разницу времен в секундах, time2 - time1
{
	uint32_t sec_time_1;
	uint32_t sec_time_2;
	if (!tdf_Time_to_Sec(time_1, &sec_time_1)) return 0;						//стартовое время в секунды
	if (!tdf_Time_to_Sec(time_2, &sec_time_2)) return 0;						//конечное время в секунды
	///вычисляем разницу
	if (sec_time_2 >= sec_time_1) *sec = sec_time_2 - sec_time_1;
	else *sec = (sec_time_2 + 86400 - sec_time_1);
	return 1;
}


uint32_t tdf_Minus_SecTime(uint32_t* sec_1, uint32_t* sec_2)
//---вычисляет разницу времен в секундах, времена в секундах
{
	if (sec_2 >= sec_1) return (sec_2 - sec_1);
	else return (sec_2 + 86400 - sec_1);
}


bool tdf_Plus_Time(time_t *time_1, time_t *time_2, uint32_t *sec)
//---вычисляет сумму времен в секундах, time1 + time2
{
	uint32_t sec_time_1;
	uint32_t sec_time_2;
	if (!tdf_Time_to_Sec(time_1, &sec_time_1)) return 0;						//стартовое время в секунды
	if (!tdf_Time_to_Sec(time_2, &sec_time_2)) return 0;						//конечное время в секунды
	///вычисляем сумму
	*sec = sec_time_2 + sec_time_1;
	return 1;
}


bool tdf_Time_More(time_t* time1, time_t* time2)
//---сравнивает два времени, true - time1 >= time2
{
	if (time1->hour > time2->hour) return 1;
	else if (time1->hour < time2->hour) return 0;
	
	if (time1->min > time2->min) return 1;
	else if (time1->min < time2->min) return 0;
		
	if (time1->sec > time2->sec) return 1;
	else if (time1->sec < time2->sec) return 0;
	return 1;
}


bool tdf_Time_Equal(time_t* time1, time_t* time2)
//---проверка на равность времен
{
	if (time1->hour != time2->hour) return 0;
	if (time1->min != time2->min) return 0;
	if (time1->sec != time2->sec) return 0;
	return 1;
}

bool tdf_SWT(date_t* date, time_t* time, day_t day, wst_t* wst)
//---корректирует время, дату, пору перехода, есл изменена пора - true
{
	wst_t old_pore = *wst;
	tdf_Count_Pore(date, time, day, wst);
	if (old_pore == *wst) return 0;
		
	if (*wst == WST_SUMMER)	
	{
		time_t thd = {0x23, 0x00, 0x00};
		if (!tdf_Time_More(time, &thd)) bcd_Plus_Bcd(time->hour, 0x01, &time->hour);
		else
		{
			time->hour = 0x00;
			tdf_Date_Plus_Day(date, 1, date);
		}
	}
	else
	{
		time_t thd = {0x01, 0x00, 0x00};
		if (tdf_Time_More(time, &thd)) bcd_Minus_Bcd(time->hour, 0x01, &time->hour);
		else
		{
			time->hour = 0x23;
			tdf_Date_Minus_Day(date, 1, date);
		}
	}
	return 1;
}

void tdf_Count_Pore(date_t* date, time_t* time, day_t day, wst_t* wst)
//---расчитывает пору перевода времени
{
	if (date->month >= 0x10 || date->month < 0x03)								//период зитмнего времени
	{
		if (date->month == 0x10)												//октябрь
		{
			if (date->date > 0x24)												//последняя неделя
			{
				if (day == DAY_SUN)												//сейчас воскресенье
				{
					time_t thd1 = {0x04, 0x00, 0x00};
					if (tdf_Time_More(time, &thd1)) *wst = WST_WINTER;			// 4:00
					else
					{
						time_t thd1 = {0x03, 0x00, 0x00};
						if (tdf_Time_More(time, &thd1)) return;					// >= 3:00
						else *wst = WST_SUMMER;
					}
				}
				else if (date->date < tdf_Sunday(date->date, day)) *wst = WST_SUMMER;//воскресенье не прошло
				else *wst = WST_WINTER;
			}
			else *wst = WST_SUMMER;
		}
		else *wst = WST_WINTER;
	}
	else																		//период летнего времени
	{
		if (date->month == 0x03)												//март
		{
			if (date->date > 0x24)												//последняя неделя
			{
				if (day == DAY_SUN)												//сейчас воскресенье
				{
					time_t thd = {0x03, 0x00, 0x00};
					if (tdf_Time_More(time, &thd)) *wst = WST_SUMMER;			// 3:00
					else *wst = WST_WINTER;
				}
				else if (date->date < tdf_Sunday(date->date, day)) *wst = WST_WINTER;//воскресенье не прошло
				else *wst = WST_SUMMER;
			}
			else *wst = WST_WINTER;
		}
		else *wst = WST_SUMMER;
	}
}

//------------------------------------------------------------------------------
//дата
//------------------------------------------------------------------------------

bool tdf_Is_GoodDate(date_t *date)
//---проверяет корректность данных
{
	uint8_t hi_bcd;
	uint8_t lo_bcd;
	lo_bcd = date->ctry & 0x0F;
	hi_bcd = date->ctry >> 4;
	if ((lo_bcd > 9) || (hi_bcd > 9)) return 0;

	uint8_t year;
	if (!bcd_Bcd_to_Num(date->year, &year)) return 0;
	if (!year) return 0;
	year %= 4;																	//вычислить номер года
	
	uint8_t month;
	if (!bcd_Bcd_to_Num(date->month, &month)) return 0;
	if (month > 12 || !month) return 0;
	
	uint8_t temp_date;
	if (!bcd_Bcd_to_Num(date->date, &temp_date)) return 0;
	if (!temp_date) return 0;
	if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		if (temp_date > 30) return 0;
	}
	else if (month == 2)														//если февраль
	{
		if (year)
		{
			if (temp_date > 28) return 0;
		}
		else
		{
			if (temp_date > 29) return 0;
		}
	}
	else
	{
		if (temp_date > 31) return 0;
	}
	return 1;
}


bcd_t tdf_Month_Lenth(bcd_t month, ubase_t num_year)
//---определяет количество дней в месяце
{
	if (month == 0x04 || month == 0x06 || month == 0x09 || month == 0x11) return 0x30;
	else if (month == 0x02)
	{
		if (num_year) return 0x28;
		else return 0x29;
	}
	else return 0x31;
}

ubase_t tdf_NumYear(uint8_t year)
//---переводит год в номер 0 - 4, 0 - высокосный
{
	return (year % 4);	
}

/*
DAY_ tdf_Date_to_Day(DATE_* date)
//---переводит формат даты в расширеный формат даты
{
	INT08U_ month;
	if (!bcd_Bcd_to_Num(date->month, &month)) return DAY_NO;
	
	INT08U_ tdate;
	if (!bcd_Bcd_to_Num(date->date, &tdate)) return DAY_NO;
		
	INT08U_ year;
	if (!bcd_Bcd_to_Num(date->year, &year)) return DAY_NO;
	
	INT08U_ ctry;
	if (!bcd_Bcd_to_Num(date->ctry, &ctry)) return DAY_NO;
		
	if (month < 3)
	{
		month += 10;
		if (year) --year;
		else
		{
			if (ctry)
			{
				--ctry;
				year = 99;
			}
			else return DAY_NO;
		}
	}
	else month -= 2;
	
	INT16S_ temp = (INT16S_)((FL64_)2.6*month - 0.2);
	temp += tdate;
	temp += (year >> 2);	
	temp += year;
	temp += (ctry >> 2);
	temp -= (ctry << 1);
	BASEU_ day = 0;
	day = mat_Mod_INT16S(temp)%7;
	
	if (!day) return DAY_SUN;
	return (DAY_)B(day-1);
}*/


day_t tdf_Date_to_Day(date_t* date)
//---переводит формат даты в расширеный формат даты
{
	uint8_t month;
	if (!bcd_Bcd_to_Num(date->month, &month)) return DAY_NO;
	
	uint8_t tdate;
	if (!bcd_Bcd_to_Num(date->date, &tdate)) return DAY_NO;
		
	uint8_t year;
	if (!bcd_Bcd_to_Num(date->year, &year)) return DAY_NO;
	
	uint8_t ctry;
	if (!bcd_Bcd_to_Num(date->ctry, &ctry)) return DAY_NO;
		
	uint16_t fyear = ctry*100+year;
	if (month < 3)
	{
		--fyear;
		tdate += 3;
	}
	int16_t temp;
	temp = tdate + fyear;
	temp += fyear>>2;
	temp -= ctry;
	temp += ctry>>2;
	temp += (31*month+10)/12;
	temp %= 7;
	return (day_t)B(temp);
}


bool tdf_Date_Plus_Day(date_t* date, uint8_t day, date_t* new_date)
//---плюсирует к дате количество дней
{
	uint8_t month;
	if (!bcd_Bcd_to_Num(date->month, &month)) return 0;
	uint8_t tdate;
	if (!bcd_Bcd_to_Num(date->date, &tdate)) return 0;
	uint8_t year;
	if (!bcd_Bcd_to_Num(date->year, &year)) return 0;
	uint8_t ctry;
	if (!bcd_Bcd_to_Num(date->ctry, &ctry)) return 0;

	tdate += day;
	ubase_t nyear;
	nyear = year%4;																//вычислить номер года
	uint16_t i;
	for (i=1; i; i++)
	{
		if (month == 4 || month == 6 || month == 9 || month == 11)
		{
			if (tdate <= 30) break;
			tdate -= 30;
		}
		else if (month == 2)													//если февраль
		{
			if (nyear)
			{				
				if (tdate <= 28) break;
				tdate -= 28;
			}
			else
			{
				if (tdate <= 29) break;
				tdate -= 29;
			}
		}
		else
		{
			if (tdate <= 31) break;
			tdate -= 31;
		}
		
		month++;
		if (month > 12)
		{
			month = 1;
			if (year == 99)
			{
				year = 0;
				if (ctry == 99) return 0;
				ctry++;
			}
			else year++;
			nyear = year%4;
		}
	}
	
	if (!bcd_Num_to_Bcd(tdate, &(new_date->date))) return 0;
	if (i == 1)
	{
		new_date->month = date->month;
		new_date->year = date->year;
		new_date->ctry = date->ctry;
		return 1;
	}
	if (!bcd_Num_to_Bcd(month, &(new_date->month))) return 0;
	if (!bcd_Num_to_Bcd(year, &(new_date->year))) return 0;
	if (!bcd_Num_to_Bcd(ctry, &(new_date->ctry))) return 0;
	return 1;
}


bool tdf_Date_Minus_Day(date_t* date, uint8_t day, date_t* new_date)
//---отнимает от даты дни
{
	uint8_t tdate;
	if (!bcd_Bcd_to_Num(date->date, &tdate)) return 0;
	if (day < tdate)
	{
		tdate -= day;
		if (!bcd_Num_to_Bcd(tdate, &(new_date->date))) return 0;
		new_date->month = date->month;
		new_date->year = date->year;
		new_date->ctry = date->ctry;
		return 1;
	}
	
	uint8_t month;
	if (!bcd_Bcd_to_Num(date->month, &month)) return 0;
	uint8_t year;
	if (!bcd_Bcd_to_Num(date->year, &year)) return 0;
	uint8_t ctry;
	if (!bcd_Bcd_to_Num(date->ctry, &ctry)) return 0;

	ubase_t nyear;
	nyear = year%4;
	for (;day >= tdate;)
	{
		day -= tdate;
		if (month == 1)
		{
			month = 12;
			if (year) --year;
			else
			{
				year = 99;
				if (!ctry) return 0;
				--ctry;
			}
			nyear = year%4;
		}
		else --month;
		if (month == 4 || month == 6 || month == 9 || month == 11) tdate = 30;
		else if (month == 2)													//если февраль
		{
			if (nyear) tdate = 28;
			else tdate = 29;
		}
		else tdate = 31;
	}
	tdate -= day;
	if (!bcd_Num_to_Bcd(tdate, &(new_date->date))) return 0;
	if (!bcd_Num_to_Bcd(month, &(new_date->month))) return 0;
	if (!bcd_Num_to_Bcd(year, &(new_date->year))) return 0;
	if (!bcd_Num_to_Bcd(ctry, &(new_date->ctry))) return 0;
	return 1;
}

fl32_t tdf_Lunar_FlDay(date_t* date)
//---вычисляет по дате лунный день
{
	uint8_t month;
	bcd_Bcd_to_Num(date->month, &month);
	uint8_t tdate;
	bcd_Bcd_to_Num(date->date, &tdate);
	uint8_t year;
	bcd_Bcd_to_Num(date->year, &year);
	uint8_t ctry;
	bcd_Bcd_to_Num(date->ctry, &ctry);
		
	uint16_t fyear = year + ctry*100;
	uint32_t yy;
	uint32_t mm;
	yy = (uint32_t)(fyear - floor((12-month)/10.0));
	mm = month + 9;
	
	if (mm >= 12) mm -= 12;
	
	uint32_t k1 = (uint32_t)floor(365.25*yy + 1721058.0);
	uint32_t k2 = (uint32_t)floor(30.6*mm + 0.5);
	uint32_t k3 = (uint32_t)floor(floor((yy/100.0) + 49)*0.75) - 38;
	
	uint32_t jd = k1 + k2 + tdate + 59 - k3;
	fl32_t ip = tdf_normalize((jd - 2451550.1)/29.530588853);
	return (29.53*ip);
}

fl32_t tdf_Lunar_FlDay_and_Distance(date_t* date, fl32_t* distance)
//---вычисляет по дате лунный день и растояние от земли в радиусах земли
{
	uint8_t month;
	bcd_Bcd_to_Num(date->month, &month);
	uint8_t tdate;
	bcd_Bcd_to_Num(date->date, &tdate);
	uint8_t year;
	bcd_Bcd_to_Num(date->year, &year);
	uint8_t ctry;
	bcd_Bcd_to_Num(date->ctry, &ctry);
		
	uint16_t fyear = year + ctry*100;
	uint32_t yy;
	uint32_t mm;
	yy = (uint32_t)(fyear - floor((12-month)/10.0));
	mm = month + 9;
	
	if (mm >= 12) mm -= 12;
	
	uint32_t k1 = (uint32_t)floor(365.25*yy + 1721058.0);
	uint32_t k2 = (uint32_t)floor(30.6*mm + 0.5);
	uint32_t k3 = (uint32_t)floor(floor((yy/100.0) + 49)*0.75) - 38;
	
	uint32_t jd = (k1 + k2 + tdate + 59 - k3);
	fl32_t ip = tdf_normalize((jd - 2451550.1)/(29.530588853));

	fl32_t dp = 2.0*MAT_PI*tdf_normalize((jd - 2451562.2)/27.55454988);

	fl32_t ip2;
	ip2 = 2*ip;	
	*distance = 60.4 - 3.3*cos(dp) - 0.6*cos(ip2 - dp) - 0.5*cos(ip2);
	return (29.53*ip);
}


ubase_t tdf_Lunar_Phase(fl32_t day)
//---вычисляет фазу луны по лунному дню, "NEW", "Waxing crescent", "First quarter", "Waxing gibbous", "FULL", "Waning gibbous", "Last quarter", "Waning crescent"
{
	if (day < 1.84566) return 1;
	else if (day < 5.53699) return 2;
	else if (day < 9.22831) return 3;
	else if (day < 12.91963) return 4;
	else if (day < 16.61096) return 5;
	else if (day < 20.30228) return 6;
	else if (day < 23.99361) return 7;
	else if (day < 27.68493) return 8;
	else return 1;
}

//------------------------------------------------------------------------------
//внут. функции
//------------------------------------------------------------------------------

ubase_t tdf_Sunday(ubase_t cur_date, day_t cur_day)
//---находит день соответствующий воскресеню в последней неделе
{
	uint8_t date;
	bcd_Bcd_to_Num(cur_date, &date);
	for (ubase_t i=0; i<7; i++)
	{
		if (cur_day >= DAY_SUN) break;
		date++;
		cur_day <<= 1;
	}
	if (date > 31) date -= 7;
	
	bcd_Num_to_Bcd(date, &cur_date);
	return cur_date;
}

fl64_t tdf_normalize(fl64_t val)
//---нормализация для функции лунного дня
{
	val = val - floor(val);
	if (val < 0) val++;
	return val;
}
