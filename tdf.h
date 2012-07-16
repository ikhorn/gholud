/* Модуль функций для работы с временем и датой
 * обновлен.....................................................................18.01.2011 13:54:01
*/

#include "bcd.h"

#ifndef TDF_H_
#define TDF_H_

typedef struct
{
	bcd_t date;
	bcd_t month;
	bcd_t year;
	bcd_t ctry;
} date_t;

typedef struct
{
	bcd_t hour;
	bcd_t min;
	bcd_t sec;
} time_t;

typedef enum																	//обозначение перехода на зимнее/летнее время
{
	WST_WINTER = 1,
	WST_SUMMER = 2
} wst_t;

typedef enum																	//дни недели
{
	DAY_MON = 0x01,
	DAY_TUE = 0x02,
	DAY_WED = 0x04,
	DAY_THU = 0x08,
	DAY_FRI = 0x10,
	DAY_SAT = 0x20,
	DAY_SUN = 0x40,
	DAY_ALL = 0x7F,
	DAY_NO = 0x00
} day_t;

//------------------------------------------------------------------------------
//тип года
//------------------------------------------------------------------------------

#define TDF_FULL_YEAR							0
#define TDF_SMAL_YEAR							1

//------------------------------------------------------------------------------
//Функции
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//TIME
//------------------------------------------------------------------------------
bool tdf_Is_GoodTime(time_t *time);												//---проверяет корректность времени
bool tdf_Time_to_Sec(time_t *in_time, uint32_t *out_time);						//---переводит время в формате bcd в секунды
bool tdf_Sec_to_Time(uint32_t in_sec, time_t *out_time);						//---переводит время в секундах в эквивалетное время суток
bool tdf_Minus_Time(time_t *time_1, time_t *time_2, uint32_t *sec);				//---вычисляет разницу времен в секундах, time2 - time1
uint32_t tdf_Minus_SecTime(uint32_t* sec_1, uint32_t* sec_2);					//---вычисляет разницу времен в секундах, time1 + time2
bool tdf_Plus_Time(time_t *time_1, time_t *time_2, uint32_t *sec);				//---вычисляет сумму времен в секундах, времена поданы в bcd
bool tdf_Time_More(time_t* time1, time_t* time2);								//---сравнивает два времени, true - time1 >= time2
bool tdf_Time_Equal(time_t* time1, time_t* time2);								//---проверка на равность времен, true - равные
//------------------------------------------------------------------------------
//DATE
//------------------------------------------------------------------------------
bool tdf_Is_GoodDate(date_t *date);												//---проверяет корректность данных
day_t tdf_Date_to_Day(date_t* date);												//---переводит формат даты в расширеный формат даты
bool tdf_Date_Plus_Day(date_t* date, uint8_t day, date_t* new_date);				//---плюсирует к дате количество дней
bool tdf_Date_Minus_Day(date_t* date, uint8_t day, date_t* new_date);				//---отнимает от даты дни
bool tdf_SWT(date_t* date, time_t* time, day_t day, wst_t* wst);					//---корректирует время, дату, пору перехода, есл изменена пора - true
void tdf_Count_Pore(date_t* date, time_t* time, day_t day, wst_t* wst);				//---расчитывает пору перевода времени

bcd_t tdf_Month_Lenth(bcd_t month, ubase_t num_year);								//---определяет количество дней в месяце
ubase_t tdf_NumYear(uint8_t year);												//---переводит год в номер 0 - 4, 0 - высокосный
fl32_t tdf_Lunar_FlDay(date_t* date);												//---вычисляет по дате лунный день
fl32_t tdf_Lunar_FlDay_and_Distance(date_t* date, fl32_t* distance);				//---вычисляет по дате лунный день и растояние от земли в радиусах земли
ubase_t tdf_Lunar_Phase(fl32_t day);												//---вычисляет фазу луны по лунному дню, "NEW", "Waxing crescent", "First quarter", "Waxing gibbous", "FULL", "Waning gibbous", "Last quarter", "Waning crescent"

#endif
