/* Модуль будильников
 * обновлен.....................................................................06.12.2010 13:20:14
*/

#include "tdf.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

#define ALM_NUM								8									//количество будильников

typedef struct
{
	uint8_t state_vector;														//отображает состояние вкл/отк
	uint8_t near_vector;														//обозначает, что будильник приблизился к срабатыванию
	uint8_t stop_vector;														//будильник находиться в состоянии ждем начала срабатывания
	uint8_t ring_vector;														//будильник находиться в состоянии звонка
	uint8_t waite_vector;														//будильник находиться в состоянии ожидания повторного срабатывания
	uint8_t message_vector;														//будильник будет выдавать сообщение
	uint8_t times_vector;														//будильник сработает один раз и выключиться
	time_t time[ALM_NUM];														//служебное время для срабатывания
	uint8_t repnum[ALM_NUM];													//количество повторов
	uint8_t melody[ALM_NUM];													//мелодии
	day_t day[ALM_NUM];															//дни недели
} galm_t;

extern galm_t gAlm;

void alm_Ini(void);
void alm(void);
bool alm_On(ubase_t anum);
bool alm_Off(ubase_t anum);
bool alm_Reset(ubase_t anum);
void alm_Reset_Ring(ubase_t anum);
bool alm_Set_Melody(ubase_t anum, uint8_t melody);
bool alm_Set_Day(ubase_t anum, day_t day);
bool alm_Set_TimeOn(ubase_t anum, time_t* time_on);
bool alm_Set_TimeDur(ubase_t anum, time_t* time_dur);
bool alm_Set_TimeRep(ubase_t anum, time_t* time_rep);
bool alm_Set_Repnum(ubase_t anum, uint8_t repnum);
bool alm_Set_Message(ubase_t anum, uint8_t message);
bool alm_Set_Times(ubase_t anum, uint8_t times);
void alm_Default(ubase_t anum);
void alm_ReSignal(ubase_t anum);
bool alm_State(ubase_t anum);

