/* Модуль обработки ошибок
 * обновлен.....................................................................29.12.2010 20:09:16
*/

#include "pdef.h"

//------------------------------------------------------------------------------
//оды ошибок
//------------------------------------------------------------------------------

#define ERR_CODE_IIC							1
#define ERR_CODE_MEM_LINK						2
#define ERR_CODE_RTC_LINK						3
#define ERR_CODE_TEMPSENS1_LINK					4
#define ERR_CODE_TEMPSENS2_LINK					5

#define ERR_CODE_ALM_LOST_TIMEON				10
#define ERR_CODE_ALM_LOST_TIMEDUR				11
#define ERR_CODE_ALM_LOST_TIMEREP				12
#define ERR_CODE_ALM_LOST_REPNUM				13
#define ERR_CODE_ALM_LOST_DAY					14
#define ERR_CODE_ALM_LOST_STATE					15
#define ERR_CODE_ALM_LOST_MELODY				16
#define ERR_CODE_ALM_LOST_MESSAGE				17
#define ERR_CODE_ALM_LOST_TIMES 				18

#define ERR_CODE_TMR_LOST_TIMEON				18
#define ERR_CODE_TMR_LOST_TIMEOFF				19
#define ERR_CODE_TMR_LOST_DAY					20
#define ERR_CODE_TMR_LOST_STATE					21
#define ERR_CODE_TMR_LOST_TYPE					22

#define ERR_CODE_TRG_LOST_TBOTTOM				23
#define ERR_CODE_TRG_LOST_TTOP					24
#define ERR_CODE_TRG_LOST_STATE					25
#define ERR_CODE_TRG_LOST_TYPE					26

#define ERR_CODE_RMD_LOST_TIMEON				27
#define ERR_CODE_RMD_LOST_DATEON				28
#define ERR_CODE_RMD_LOST_FILTR					29
#define ERR_CODE_RMD_LOST_STR					30
#define ERR_CODE_RMD_LOST_STATE					31
#define ERR_CODE_RMD_LOST_TIMEDUR				32
#define ERR_CODE_RMD_LOST_TIMEREP				33
#define ERR_CODE_RMD_LOST_REPNUM				34
#define ERR_CODE_RMD_LOST_MELODY				35

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//MEM
//------------------------------------------------------------------------------
void err_FM24C64(void);
//------------------------------------------------------------------------------
//RTC
//------------------------------------------------------------------------------
void err_Rtc(ubase_t err);
void err_Rtc_Rd_AvtoShift(void);
void err_Rtc_Rd_Pore(void);
void err_Rtc_Rd_HourSgn(void);
//------------------------------------------------------------------------------
//LIGHT
//------------------------------------------------------------------------------
void err_LightState_Rd(void);
//------------------------------------------------------------------------------
//PSW
//------------------------------------------------------------------------------
void err_Psw_Rd_Usage(void);
void err_Psw_Rd_Psw(void);
//------------------------------------------------------------------------------
//ALM
//------------------------------------------------------------------------------
void err_Alm_Rd_TimeOn(ubase_t anum);
void err_Alm_Rd_TimeDur(ubase_t anum);
void err_Alm_Rd_TimeRep(ubase_t anum);
void err_Alm_Rd_Repnum(ubase_t anum);
void err_Alm_Rd_Day(ubase_t anum);
void err_Alm_Rd_State(ubase_t anum);
void err_Alm_Rd_Melody(ubase_t anum);
void err_Alm_Rd_Message_Times(ubase_t anum);
//------------------------------------------------------------------------------
//TMR
//------------------------------------------------------------------------------
void err_Tmr_Rd_TimeOn(ubase_t tnum);
void err_Tmr_Rd_TimeOff(ubase_t tnum);
void err_Tmr_Rd_Day(ubase_t tnum);
void err_Tmr_Rd_State(ubase_t tnum);
void err_Tmr_Rd_Type(ubase_t tnum);
//------------------------------------------------------------------------------
//TRG
//------------------------------------------------------------------------------
void err_Trg_Rd_Tbottom(ubase_t tnum);
void err_Trg_Rd_Ttop(ubase_t tnum);
void err_Trg_Rd_State(ubase_t tnum);
void err_Trg_Rd_Type(ubase_t tnum);
//------------------------------------------------------------------------------
//RMD
//------------------------------------------------------------------------------
void err_Rmd_Rd_TimeOn(ubase_t rnum);
void err_Rmd_Rd_DateOn(ubase_t rnum);
void err_Rmd_Rd_Filtr(ubase_t rnum);
void err_Rmd_Rd_String(ubase_t rnum);
void err_Rmd_Rd_State(ubase_t rnum);
void err_Rmd_Rd_TimeDur(ubase_t rnum);
void err_Rmd_Rd_TimeRep(ubase_t rnum);
void err_Rmd_Rd_Repnum(ubase_t rnum);
void err_Rmd_Rd_Melody(ubase_t rnum);
//------------------------------------------------------------------------------
//TMP
//------------------------------------------------------------------------------
void err_Ds1631(void);
void err_Ds18b20(void);
void err_Tmp_Rd_Usage(ubase_t snum);
