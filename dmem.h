/* Хранение параметров прибора
 * проект.......................................................................ЖОЛУДЬ
 * обновлен.....................................................................06.12.2010 17:45:53
*/

#include "tdf.h"
#include "pdef.h"


//------------------------------------------------------------------------------
//число для проверки
//------------------------------------------------------------------------------
#define DMEM_CHECK_NUM					65289
//------------------------------------------------------------------------------
//карта памяти устройтва
//------------------------------------------------------------------------------

#define DMEM_AD_ALM__					0										//будильники
#define DMEM_AD_TMR__					(DMEM_AD_ALM__+			22*8)			//таймера
#define DMEM_AD_TRG__					(DMEM_AD_TMR__+			14*8)			//терморегуляторы
#define DMEM_AD_TMP__					(DMEM_AD_TRG__+			10*2)			//датчики температуры
#define DMEM_AD_RMD__					(DMEM_AD_TMP__+			2*2)			//напоминалки


#define DMEM_AD_FSTART__				8151
#define DMEM_AD_LIGHT__					(DMEM_AD_FSTART__+		3*2)			//подсветка
#define DMEM_AD_RTC__					(DMEM_AD_LIGHT__+		2)				//часы, авто переход
#define DMEM_AD_PSW__					(DMEM_AD_RTC__+			6)				//пароль

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------
bool dmem_Is_First_Start(void);													//---проверяет или прибо запущен первый	раз
void dmem_Clear_FirstStart(void);												//---стирает сигнал первого запуска
bool dmem_Check_Fm24c64(void);													//---проверяет работоспособность памяти FM24C64
//------------------------------------------------------------------------------
//LIGHT
//------------------------------------------------------------------------------
bool dmem_Wr_Light_DefState(uint8_t def_state);									//---записывает состояние по умолчанию
bool dmem_Rd_Light_DefState(uint8_t* def_state);								//---считывает состояние по умолчанию
//------------------------------------------------------------------------------
//RTC
//------------------------------------------------------------------------------
bool dmem_Wr_Rtc_AvtoShift(uint8_t avto_shift);									//---записывает авто переход
bool dmem_Rd_Rtc_AvtoShift(uint8_t* avto_shift);								//---считывает авто переход
bool dmem_Wr_Rtc_Pore(wst_t wst);												//---записывает состояние перевода
bool dmem_Rd_Rtc_Pore(wst_t* wst);												//---считывает состояние перевода
bool dmem_Wr_Rtc_HourSgn(uint8_t hour_sgn);										//---записывает пикать каждый час
bool dmem_Rd_Rtc_HourSgn(uint8_t* hour_sgn);									//---считывает пикать каждый час
//------------------------------------------------------------------------------
//PSW
//------------------------------------------------------------------------------
bool dmem_Wr_Psw_Use(uint8_t psw_use);											//---записывает использование пароля
bool dmem_Rd_Psw_Use(uint8_t* psw_use);											//---считывает использование пароля
bool dmem_Wr_Psw(uint32_t psw);													//---записывает пароль
bool dmem_Rd_Psw(uint32_t* psw);													//---читает пароль
//------------------------------------------------------------------------------
//ALM
//------------------------------------------------------------------------------
bool dmem_Wr_Alm_TimeOn(ubase_t anum, time_t* time);
bool dmem_Rd_Alm_TimeOn(ubase_t anum, time_t* time);
bool dmem_Wr_Alm_TimeDur(ubase_t anum, time_t* time);
bool dmem_Rd_Alm_TimeDur(ubase_t anum, time_t* time);
bool dmem_Wr_Alm_TimeRep(ubase_t anum, time_t* time);
bool dmem_Rd_Alm_TimeRep(ubase_t anum, time_t* time);
bool dmem_Wr_Alm_Repnum(ubase_t anum, uint8_t repnum);
bool dmem_Rd_Alm_Repnum(ubase_t anum, uint8_t* repnum);
bool dmem_Wr_Alm_Day(ubase_t anum, day_t day);
bool dmem_Rd_Alm_Day(ubase_t anum, day_t* day);
bool dmem_Wr_Alm_State(ubase_t anum, uint8_t state);
bool dmem_Rd_Alm_State(ubase_t anum, uint8_t* state);
bool dmem_Wr_Alm_Melody(ubase_t anum, uint8_t melody);
bool dmem_Rd_Alm_Melody(ubase_t anum, uint8_t* melody);
bool dmem_Wr_Alm_Message(ubase_t anum, uint8_t message);
bool dmem_Rd_Alm_Message(ubase_t anum, uint8_t* message);
bool dmem_Wr_Alm_Times(ubase_t anum, uint8_t times);
bool dmem_Rd_Alm_Times(ubase_t anum, uint8_t* times);
//------------------------------------------------------------------------------
//TMR
//------------------------------------------------------------------------------
bool dmem_Wr_Tmr_TimeOn(ubase_t tnum, time_t* time);
bool dmem_Rd_Tmr_TimeOn(ubase_t tnum, time_t* time);
bool dmem_Wr_Tmr_TimeOff(ubase_t tnum, time_t* time);
bool dmem_Rd_Tmr_TimeOff(ubase_t tnum, time_t* time);
bool dmem_Wr_Tmr_Day(ubase_t tnum, day_t day);
bool dmem_Rd_Tmr_Day(ubase_t tnum, day_t* day);
bool dmem_Wr_Tmr_State(ubase_t tnum, uint8_t state);
bool dmem_Rd_Tmr_State(ubase_t tnum, uint8_t* state);
bool dmem_Wr_Tmr_Type(ubase_t tnum, uint8_t type);
bool dmem_Rd_Tmr_Type(ubase_t tnum, uint8_t* type);
//------------------------------------------------------------------------------
//TRG
//------------------------------------------------------------------------------
bool dmem_Wr_Trg_Tbottom(ubase_t tnum, int16_t tbottom);
bool dmem_Rd_Trg_Tbottom(ubase_t tnum, int16_t* tbottom);
bool dmem_Wr_Trg_Ttop(ubase_t tnum, int16_t ttop);
bool dmem_Rd_Trg_Ttop(ubase_t tnum, int16_t* ttop);
bool dmem_Wr_Trg_State(ubase_t tnum, uint8_t state);
bool dmem_Rd_Trg_State(ubase_t tnum, uint8_t* state);
bool dmem_Wr_Trg_Type(ubase_t tnum, uint8_t type);
bool dmem_Rd_Trg_Type(ubase_t tnum, uint8_t* type);
//------------------------------------------------------------------------------
//RMD
//------------------------------------------------------------------------------
typedef struct
{
    time_t time_on;
    date_t date_on;
    uint8_t filtr;
    uint8_t str[31];
    uint8_t state;
    time_t time_dur;
    time_t time_rep;
    uint8_t repnum;
    uint8_t melody;
} DRMD_;

bool dmem_Rd_Rmd(ubase_t rnum, DRMD_* rmd);
bool dmem_Wr_Rmd(ubase_t rnum, DRMD_* rmd);
bool dmem_Wr_Rmd_TimeOn(ubase_t rnum, time_t* time);
bool dmem_Rd_Rmd_TimeOn(ubase_t rnum, time_t* time);
bool dmem_Wr_Rmd_DateOn(ubase_t rnum, date_t* date);
bool dmem_Rd_Rmd_DateOn(ubase_t rnum, date_t* date);
bool dmem_Wr_Rmd_Filtr(ubase_t rnum, uint8_t f);
bool dmem_Rd_Rmd_Filtr(ubase_t rnum, uint8_t* f);
bool dmem_Wr_Rmd_String(ubase_t rnum, const uint8_t *str);
bool dmem_Rd_Rmd_String(ubase_t rnum, uint8_t *str);
bool dmem_Wr_Rmd_State(ubase_t rnum, uint8_t state);
bool dmem_Rd_Rmd_State(ubase_t rnum, uint8_t* state);
bool dmem_Wr_Rmd_TimeDur(ubase_t rnum, time_t* time);
bool dmem_Rd_Rmd_TimeDur(ubase_t rnum, time_t* time);
bool dmem_Wr_Rmd_TimeRep(ubase_t rnum, time_t* time);
bool dmem_Rd_Rmd_TimeRep(ubase_t rnum, time_t* time);
bool dmem_Wr_Rmd_Repnum(ubase_t rnum, uint8_t repnum);
bool dmem_Rd_Rmd_Repnum(ubase_t rnum, uint8_t* repnum);
bool dmem_Wr_Rmd_Melody(ubase_t rnum, uint8_t melody);
bool dmem_Rd_Rmd_Melody(ubase_t rnum, uint8_t* melody);
//------------------------------------------------------------------------------
//TMP
//------------------------------------------------------------------------------
bool dmem_Wr_Tmp_Use(ubase_t snum , uint8_t use_sens);
bool dmem_Rd_Tmp_Use(ubase_t snum , uint8_t* use_sens);


