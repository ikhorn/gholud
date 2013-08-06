#include "mem.h"
#include "sta.h"
#include "err.h"
#include "dmem.h"
#include "drivers/fm24c64.h"
#include "usr.h"

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------


//#define mem	0

bool dmem_Is_First_Start(void)
//---проверяет или прибор запущен первый раз
{
	enum {PSIZE = 2};
	uint16_t ch;
	if (!mem_Read_ResParam(DMEM_AD_FSTART__, DMEM_AD_FSTART__+3, &ch, PSIZE))
	{
		if (STA_IS_E(STA_MEM)) return 0;
	}
	else if (ch == DMEM_CHECK_NUM) return 0;
	return 1;
}

void dmem_Clear_FirstStart(void)
//---стирает сигнал первого запуска
{
	enum {PSIZE = 2};
	uint16_t ch;
	ch = DMEM_CHECK_NUM;
	mem_Write_ResParam(DMEM_AD_FSTART__, DMEM_AD_FSTART__+3, &ch, PSIZE);
}

//------------------------------------------------------------------------------
//LIGHT
//------------------------------------------------------------------------------

bool dmem_Wr_Light_DefState(uint8_t def_state)
//---записывает состояние по умолчанию
{
	enum {PSIZE = 1};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_LIGHT__;
	if (mem_Write_Param(adr, &def_state, PSIZE)) return 1;
	return 0;
}

bool dmem_Rd_Light_DefState(uint8_t* def_state)
//---считывает состояние по умолчанию
{
	enum {PSIZE = 1};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_LIGHT__;
	if (mem_Read_Param(adr, def_state, PSIZE)) return 1;
	err_LightState_Rd();
	return 0;
}

//------------------------------------------------------------------------------
//RTC
//------------------------------------------------------------------------------

bool dmem_Wr_Rtc_AvtoShift(uint8_t avto_shift)
//---записывает авто переход
{
	enum {PSIZE = 1, SHIFT = 0};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RTC__  + SHIFT;
	if (mem_Write_Param(adr, &avto_shift, PSIZE)) return 1;
	return 0;
}

bool dmem_Rd_Rtc_AvtoShift(uint8_t* avto_shift)
//---считывает авто переход
{
	enum {PSIZE = 1, SHIFT = 0};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RTC__  + SHIFT;
	if (mem_Read_Param(adr, avto_shift, PSIZE)) return 1;
	err_Rtc_Rd_AvtoShift();
	return 0;
}

bool dmem_Wr_Rtc_Pore(wst_t wst)
//---записывает состояние перевода
{
	enum {PSIZE = 1, SHIFT = 2};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RTC__  + SHIFT;
	if (mem_Write_Param(adr, &wst, PSIZE)) return 1;
	return 0;
}

bool dmem_Rd_Rtc_Pore(wst_t* wst)
//---считывает состояние перевода
{
	enum {PSIZE = 1, SHIFT = 2};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RTC__  + SHIFT;
	if (mem_Read_Param(adr, wst, PSIZE)) return 1;
	err_Rtc_Rd_Pore();
	return 0;
}

bool dmem_Wr_Rtc_HourSgn(uint8_t hour_sgn)
//---записывает пикать каждый час
{
	enum {PSIZE = 1, SHIFT = 4};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RTC__  + SHIFT;
	if (mem_Write_Param(adr, &hour_sgn, PSIZE)) return 1;
	return 0;
}

bool dmem_Rd_Rtc_HourSgn(uint8_t* hour_sgn)
//---считывает пикать каждый час
{
	enum {PSIZE = 1, SHIFT = 4};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RTC__  + SHIFT;
	if (mem_Read_Param(adr, hour_sgn, PSIZE)) return 1;
	err_Rtc_Rd_HourSgn();
	return 0;
}
//------------------------------------------------------------------------------
//PSW
//------------------------------------------------------------------------------

bool dmem_Wr_Psw_Use(uint8_t psw_use)
//---записывает использование пароля
{
	enum {PSIZE = 1};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_PSW__;
	if (mem_Write_Param(adr, &psw_use, PSIZE)) return 1;
	return 0;
}

bool dmem_Rd_Psw_Use(uint8_t* psw_use)
//---считывает использование пароля
{
	enum {PSIZE = 1};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_PSW__;
	if (mem_Read_Param(adr, psw_use, PSIZE)) return 1;
	err_Psw_Rd_Use();
	return 0;
}

bool dmem_Wr_Psw(uint32_t psw)
//---записывает пароль
{
	enum {PSIZE = 4, SHIFT = 2};
	uint32_t adr;
	adr = DMEM_AD_PSW__ + SHIFT;
	if (mem_Write_Param(adr, &psw, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Psw(uint32_t* psw)
//---читает пароль
{
	enum {PSIZE = 4, SHIFT = 2};
	uint32_t adr;
	adr = DMEM_AD_PSW__ + SHIFT;
	if (mem_Read_Param(adr, psw, PSIZE)) return true;
	err_Psw_Rd_Psw();
	return false;
}

bool dmem_Check_Fm24c64(void)
//---проверяет работоспособность памяти FM24C64
{
	gMem.adr = 5;
	if (!mem_Open(DIR_RD)) return 0;
	uint8_t data[5];
	if (!mem_Rx_Data(data, 5)) return 0;
	mem_Close(DIR_RD);
	return 1;
}

//------------------------------------------------------------------------------
//ALM
//------------------------------------------------------------------------------

bool dmem_Wr_Alm_TimeOn(ubase_t anum, time_t* time)
{
	enum {PSIZE = 3, STEP = 22, SHIFT = 0};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Write_Param(adr, time, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Alm_TimeOn(ubase_t anum, time_t* time)
{
	enum {PSIZE = 3, STEP = 22, SHIFT = 0};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Read_Param(adr, time, PSIZE)) return true;
	err_Alm_Rd_TimeOn(anum);
	return false;
}

bool dmem_Wr_Alm_TimeDur(ubase_t anum, time_t* time)
{
	enum {PSIZE = 3, STEP = 22, SHIFT = 4};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Write_Param(adr, time, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Alm_TimeDur(ubase_t anum, time_t* time)
{
	enum {PSIZE = 3, STEP = 22, SHIFT = 4};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Read_Param(adr, time, PSIZE)) return true;
	err_Alm_Rd_TimeDur(anum);
	return false;
}

bool dmem_Wr_Alm_TimeRep(ubase_t anum, time_t* time)
{
	enum {PSIZE = 3, STEP = 22, SHIFT = 8};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Write_Param(adr, time, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Alm_TimeRep(ubase_t anum, time_t* time)
{
	enum {PSIZE = 3, STEP = 22, SHIFT = 8};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Read_Param(adr, time, PSIZE)) return true;
	err_Alm_Rd_TimeRep(anum);
	return false;
}

bool dmem_Wr_Alm_Repnum(ubase_t anum, uint8_t repnum)
{
	enum {PSIZE = 1, STEP = 22, SHIFT = 12};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Write_Param(adr, &repnum, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Alm_Repnum(ubase_t anum, uint8_t* repnum)
{
	enum {PSIZE = 1, STEP = 22, SHIFT = 12};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Read_Param(adr, repnum, PSIZE)) return true;
	err_Alm_Rd_Repnum(anum);
	return false;
}

bool dmem_Wr_Alm_Day(ubase_t anum, day_t day)
{
	enum {PSIZE = 1, STEP = 22, SHIFT = 14};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Write_Param(adr, &day, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Alm_Day(ubase_t anum, day_t* day)
{
	enum {PSIZE = 1, STEP = 22, SHIFT = 14};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Read_Param(adr, day, PSIZE)) return true;
	err_Alm_Rd_Day(anum);
	return false;
}


bool dmem_Wr_Alm_State(ubase_t anum, uint8_t state)
{
	enum {PSIZE = 1, STEP = 22, SHIFT = 16};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Write_Param(adr, &state, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Alm_State(ubase_t anum, uint8_t* state)
{
	enum {PSIZE = 1, STEP = 22, SHIFT = 16};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Read_Param(adr, state, PSIZE)) return true;
	err_Alm_Rd_State(anum);
	return false;
}


bool dmem_Wr_Alm_Melody(ubase_t anum, uint8_t melody)
{
	enum {PSIZE = 1, STEP = 22, SHIFT = 18};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Write_Param(adr, &melody, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Alm_Melody(ubase_t anum, uint8_t* melody)
{
	enum {PSIZE = 1, STEP = 22, SHIFT = 18};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
	if (mem_Read_Param(adr, melody, PSIZE)) return true;
	err_Alm_Rd_Melody(anum);
	return false;
}

bool dmem_Wr_Alm_Message(ubase_t anum, uint8_t message)
{
	enum {PSIZE = 1, STEP = 22, SHIFT = 20};
    enum {MESB = 0};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
    uint8_t rg;
	if (!mem_Read_Param(adr, &rg, PSIZE)) return false;
    if (message)
    {
        SETB(rg, MESB);
    }
    else
    {
        CLRB(rg, MESB);
    }
	if (mem_Write_Param(adr, &rg, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Alm_Message(ubase_t anum, uint8_t* message)
{
	enum {PSIZE = 1, STEP = 22, SHIFT = 20};
    enum {MESB = 0};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
    uint8_t rg;
	if (!mem_Read_Param(adr, &rg, PSIZE))
    {
	    err_Alm_Rd_Message_Times(anum);
	    return 0;
    }
    if (CHKB(rg, MESB)) *message = 1;
    else *message = 0;
    return 1;
}

bool dmem_Wr_Alm_Times(ubase_t anum, uint8_t times)
{
	enum {PSIZE = 1, STEP = 22, SHIFT = 20};
    enum {TIMESB = 1};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
    uint8_t rg;
	if (!mem_Read_Param(adr, &rg, PSIZE)) return false;
    if (times)
    {
        SETB(rg, TIMESB);
    }
    else
    {
        CLRB(rg, TIMESB);
    }
	if (mem_Write_Param(adr, &rg, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Alm_Times(ubase_t anum, uint8_t* times)
{
	enum {PSIZE = 1, STEP = 22, SHIFT = 20};
    enum {TIMESB = 1};
	uint32_t adr;
	adr = DMEM_AD_ALM__ + SHIFT + STEP*(uint32_t)anum;
    uint8_t rg;
	if (!mem_Read_Param(adr, &rg, PSIZE))
    {
		err_Alm_Rd_Message_Times(anum);
	    return 0;
    }
    if (CHKB(rg, TIMESB)) *times = 1;
    else *times = 0;
    return 1;
}
//------------------------------------------------------------------------------
//TMR
//------------------------------------------------------------------------------

bool dmem_Wr_Tmr_TimeOn(ubase_t tnum, time_t* time)
{
	enum {PSIZE = 3, STEP = 14, SHIFT = 0};
	uint32_t adr;
	adr = DMEM_AD_TMR__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Write_Param(adr, time, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Tmr_TimeOn(ubase_t tnum, time_t* time)
{
	enum {PSIZE = 3, STEP = 14, SHIFT = 0};
	uint32_t adr;
	adr = DMEM_AD_TMR__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Read_Param(adr, time, PSIZE)) return true;
	err_Tmr_Rd_TimeOn(tnum);
	return false;
}

bool dmem_Wr_Tmr_TimeOff(ubase_t tnum, time_t* time)
{
	enum {PSIZE = 3, STEP = 14, SHIFT = 4};
	uint32_t adr;
	adr = DMEM_AD_TMR__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Write_Param(adr, time, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Tmr_TimeOff(ubase_t tnum, time_t* time)
{
	enum {PSIZE = 3, STEP = 14, SHIFT = 4};
	uint32_t adr;
	adr = DMEM_AD_TMR__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Read_Param(adr, time, PSIZE)) return true;
	err_Tmr_Rd_TimeOff(tnum);
	return false;
}

bool dmem_Wr_Tmr_Day(ubase_t tnum, day_t day)
{
	enum {PSIZE = 1, STEP = 14, SHIFT = 8};
	uint32_t adr;
	adr = DMEM_AD_TMR__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Write_Param(adr, &day, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Tmr_Day(ubase_t tnum, day_t* day)
{
	enum {PSIZE = 1, STEP = 14, SHIFT = 8};
	uint32_t adr;
	adr = DMEM_AD_TMR__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Read_Param(adr, day, PSIZE)) return true;
	err_Tmr_Rd_Day(tnum);
	return false;
}

bool dmem_Wr_Tmr_State(ubase_t tnum, uint8_t state)
{
	enum {PSIZE = 1, STEP = 14, SHIFT = 10};
	uint32_t adr;
	adr = DMEM_AD_TMR__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Write_Param(adr, &state, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Tmr_State(ubase_t tnum, uint8_t* state)
{
	enum {PSIZE = 1, STEP = 14, SHIFT = 10};
	uint32_t adr;
	adr = DMEM_AD_TMR__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Read_Param(adr, state, PSIZE)) return true;
	err_Tmr_Rd_State(tnum);
	return false;
}

bool dmem_Wr_Tmr_Type(ubase_t tnum, uint8_t type)
{
	enum {PSIZE = 1, STEP = 14, SHIFT = 12};
	uint32_t adr;
	adr = DMEM_AD_TMR__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Write_Param(adr, &type, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Tmr_Type(ubase_t tnum, uint8_t* type)
{
	enum {PSIZE = 1, STEP = 14, SHIFT = 12};
	uint32_t adr;
	adr = DMEM_AD_TMR__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Read_Param(adr, type, PSIZE)) return true;
	err_Tmr_Rd_Type(tnum);
	return false;
}

//------------------------------------------------------------------------------
//TRG
//------------------------------------------------------------------------------

bool dmem_Wr_Trg_Tbottom(ubase_t tnum, int16_t tbottom)
{
	enum {PSIZE = 2, STEP = 10, SHIFT = 0};
	uint32_t adr;
	adr = DMEM_AD_TRG__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Write_Param(adr, &tbottom, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Trg_Tbottom(ubase_t tnum, int16_t* tbottom)
{
	enum {PSIZE = 2, STEP = 10, SHIFT = 0};
	uint32_t adr;
	adr = DMEM_AD_TRG__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Read_Param(adr, tbottom, PSIZE)) return true;
	err_Trg_Rd_Tbottom(tnum);
	return false;
}


bool dmem_Wr_Trg_Ttop(ubase_t tnum, int16_t ttop)
{
	enum {PSIZE = 2, STEP = 10, SHIFT = 3};
	uint32_t adr;
	adr = DMEM_AD_TRG__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Write_Param(adr, &ttop, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Trg_Ttop(ubase_t tnum, int16_t* ttop)
{
	enum {PSIZE = 2, STEP = 10, SHIFT = 3};
	uint32_t adr;
	adr = DMEM_AD_TRG__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Read_Param(adr, ttop, PSIZE)) return true;
	err_Trg_Rd_Ttop(tnum);
	return false;
}


bool dmem_Wr_Trg_State(ubase_t tnum, uint8_t state)
{
	enum {PSIZE = 1, STEP = 10, SHIFT = 6};
	uint32_t adr;
	adr = DMEM_AD_TRG__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Write_Param(adr, &state, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Trg_State(ubase_t tnum, uint8_t* state)
{
	enum {PSIZE = 1, STEP = 10, SHIFT = 6};
	uint32_t adr;
	adr = DMEM_AD_TRG__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Read_Param(adr, state, PSIZE)) return true;
	err_Trg_Rd_State(tnum);
	return false;
}

bool dmem_Wr_Trg_Type(ubase_t tnum, uint8_t type)
{
	enum {PSIZE = 1, STEP = 10, SHIFT = 8};
	uint32_t adr;
	adr = DMEM_AD_TRG__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Write_Param(adr, &type, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Trg_Type(ubase_t tnum, uint8_t* type)
{
	enum {PSIZE = 1, STEP = 10, SHIFT = 8};
	uint32_t adr;
	adr = DMEM_AD_TRG__ + SHIFT + STEP*(uint32_t)tnum;
	if (mem_Read_Param(adr, type, PSIZE)) return true;
	err_Trg_Rd_Type(tnum);
	return false;
}

//------------------------------------------------------------------------------
//TMP
//------------------------------------------------------------------------------

bool dmem_Wr_Tmp_Use(ubase_t snum , uint8_t use_sens)
{
	enum {PSIZE = 1, STEP = 2, SHIFT = 0};
	uint32_t adr;
	adr = DMEM_AD_TMP__ + SHIFT + STEP*(uint32_t)snum;
	if (mem_Write_Param(adr, &use_sens, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Tmp_Use(ubase_t snum , uint8_t* use_sens)
{
	enum {PSIZE = 1, STEP = 2, SHIFT = 0};
	uint32_t adr;
	adr = DMEM_AD_TMP__ + SHIFT + STEP*(uint32_t)snum;
	if (mem_Read_Param(adr, use_sens, PSIZE)) return true;
	err_Tmp_Rd_Use(snum);
	return false;
}

//------------------------------------------------------------------------------
//RMD
//------------------------------------------------------------------------------

bool dmem_Wr_Rmd_TimeOn(ubase_t rnum, time_t* time)
{

	enum {PSIZE = 3, STEP = 60, SHIFT = 0};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Write_Param(adr, time, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Rmd_TimeOn(ubase_t rnum, time_t* time)
{
	enum {PSIZE = 3, STEP = 60, SHIFT = 0};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Read_Param(adr, time, PSIZE)) return true;
	err_Rmd_Rd_TimeOn(rnum);
	return false;
}


bool dmem_Wr_Rmd_DateOn(ubase_t rnum, date_t* date)
{
	enum {PSIZE = 4, STEP = 60, SHIFT = 4};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Write_Param(adr, date, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Rmd_DateOn(ubase_t rnum, date_t* date)
{
	enum {PSIZE = 4, STEP = 60, SHIFT = 4};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Read_Param(adr, date, PSIZE)) return true;
	err_Rmd_Rd_DateOn(rnum);
	return false;
}

bool dmem_Wr_Rmd_Filtr(ubase_t rnum, uint8_t f)
{
	enum {PSIZE = 1, STEP = 60, SHIFT = 9};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Write_Param(adr, &f, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Rmd_Filtr(ubase_t rnum, uint8_t* f)
{
	enum {PSIZE = 1, STEP = 60, SHIFT = 9};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Read_Param(adr, f, PSIZE)) return true;
	err_Rmd_Rd_Filtr(rnum);
	return false;
}

bool dmem_Wr_Rmd_String(ubase_t rnum, const uint8_t *str)
{
	enum {PSIZE = 8, STEP = 60, SHIFT = 11};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (!mem_Write_Param(adr, str, PSIZE)) return 0;
	if (!mem_Write_Param(adr+9, str+8, PSIZE)) return 0;
	if (!mem_Write_Param(adr+18, str+16, PSIZE)) return 0;
	if (!mem_Write_Param(adr+27, str+24, 7)) return 0;
	return 1;
}

bool dmem_Rd_Rmd_String(ubase_t rnum, uint8_t *str)
{
	enum {PSIZE = 8, STEP = 60, SHIFT = 11};
	for (;;)
	{
		uint32_t adr;
		adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
		adr += STEP*(uint32_t)rnum;
		if (!mem_Read_Param(adr, str, PSIZE)) break;
		if (!mem_Read_Param(adr+9, str+8, PSIZE)) break;
		if (!mem_Read_Param(adr+18, str+16, PSIZE)) break;
		if (!mem_Read_Param(adr+27, str+24, 7)) break;
		return 1;
	}
	err_Rmd_Rd_String(rnum);
	return 0;
}

bool dmem_Wr_Rmd_TimeDur(ubase_t rnum, time_t* time)
{
	enum {PSIZE = 3, STEP = 60, SHIFT = 46};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Write_Param(adr, time, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Rmd_TimeDur(ubase_t rnum, time_t* time)
{
	enum {PSIZE = 3, STEP = 60, SHIFT = 46};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Read_Param(adr, time, PSIZE)) return true;
	err_Rmd_Rd_TimeDur(rnum);
	return false;
}

bool dmem_Wr_Rmd_TimeRep(ubase_t rnum, time_t* time)
{
	enum {PSIZE = 3, STEP = 60, SHIFT = 50};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Write_Param(adr, time, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Rmd_TimeRep(ubase_t rnum, time_t* time)
{
	enum {PSIZE = 3, STEP = 60, SHIFT = 50};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Read_Param(adr, time, PSIZE)) return true;
	err_Rmd_Rd_TimeRep(rnum);
	return false;
}

bool dmem_Wr_Rmd_Repnum(ubase_t rnum, uint8_t repnum)
{
	enum {PSIZE = 1, STEP = 60, SHIFT = 54};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Write_Param(adr, &repnum, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Rmd_Repnum(ubase_t rnum, uint8_t* repnum)
{
	enum {PSIZE = 1, STEP = 60, SHIFT = 54};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Read_Param(adr, repnum, PSIZE)) return true;
	err_Rmd_Rd_Repnum(rnum);
	return false;
}

bool dmem_Wr_Rmd_State(ubase_t rnum, uint8_t state)
{
	enum {PSIZE = 1, STEP = 60, SHIFT = 56};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Write_Param(adr, &state, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Rmd_State(ubase_t rnum, uint8_t* state)
{
	enum {PSIZE = 1, STEP = 60, SHIFT = 56};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Read_Param(adr, state, PSIZE)) return true;
	err_Rmd_Rd_State(rnum);
	return false;
}

bool dmem_Wr_Rmd_Melody(ubase_t rnum, uint8_t melody)
{
	enum {PSIZE = 1, STEP = 60, SHIFT = 58};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Write_Param(adr, &melody, PSIZE)) return true;
	return false;
}

bool dmem_Rd_Rmd_Melody(ubase_t rnum, uint8_t* melody)
{
	enum {PSIZE = 1, STEP = 60, SHIFT = 58};
	uint32_t adr;
	adr = (uint32_t)DMEM_AD_RMD__ + SHIFT;
	adr += STEP*(uint32_t)rnum;
	if (mem_Read_Param(adr, melody, PSIZE)) return true;
	err_Rmd_Rd_Melody(rnum);
	return false;
}

bool dmem_Rd_Rmd(ubase_t rnum, DRMD_* rmd)
{
	enum {STEP = 60};
    uint32_t adr;
    adr = (uint32_t)DMEM_AD_RMD__ + STEP*(uint32_t)rnum;
	if (!mem_Read_Param(adr, &rmd->time_on, 3))
    {
	    err_Rmd_Rd_TimeOn(rnum);
        return 0;
    }
    adr += 4;
	if (!mem_Read_Param(adr, &rmd->date_on, 4))
    {
	    err_Rmd_Rd_DateOn(rnum);
        return 0;
    }
    adr += 5;
	if (!mem_Read_Param(adr, &rmd->filtr, 1))
    {
	    err_Rmd_Rd_Filtr(rnum);
        return 0;
    }
    adr += 2;

    ubase_t err = 1;
	for (;;)
	{
		if (!mem_Read_Param(adr, rmd->str, 8)) break;
        adr += 9;
		if (!mem_Read_Param(adr, rmd->str+8, 8)) break;
        adr += 9;
		if (!mem_Read_Param(adr, rmd->str+16, 8)) break;
        adr += 9;
		if (!mem_Read_Param(adr, rmd->str+24, 7)) break;
        adr += 8;
        err = 0;
        break;
	}
	if (err)
    {
        err_Rmd_Rd_String(rnum);
        return 0;
    }

	if (!mem_Read_Param(adr, &rmd->time_dur, 3))
    {
	    err_Rmd_Rd_TimeDur(rnum);
        return 0;
    }
    adr += 4;

	if (!mem_Read_Param(adr, &rmd->time_rep, 3))
    {
	    err_Rmd_Rd_TimeRep(rnum);
        return 0;
    }
    adr += 4;

	if (!mem_Read_Param(adr, &rmd->repnum, 1))
    {
	    err_Rmd_Rd_Repnum(rnum);
        return 0;
    }
    adr += 2;

	if (!mem_Read_Param(adr, &rmd->state, 1))
    {
		err_Rmd_Rd_State(rnum);
        return 0;
    }
    adr += 2;

	if (!mem_Read_Param(adr, &rmd->melody, 1))
    {
	    err_Rmd_Rd_Melody(rnum);
        return 0;
    }
    return 1;
}


bool dmem_Wr_Rmd(ubase_t rnum, DRMD_* rmd)
{
	enum {STEP = 60};
    uint32_t adr;
    adr = (uint32_t)DMEM_AD_RMD__ + STEP*(uint32_t)rnum;
	if (!mem_Write_Param(adr, &rmd->time_on, 3)) return 0;
    adr += 4;
	if (!mem_Write_Param(adr, &rmd->date_on, 4)) return 0;
    adr += 5;
	if (!mem_Write_Param(adr, &rmd->filtr, 1)) return 0;
    adr += 2;

	if (!mem_Write_Param(adr, rmd->str, 8)) return 0;
    adr += 9;
	if (!mem_Write_Param(adr, rmd->str+8, 8)) return 0;
    adr += 9;
	if (!mem_Write_Param(adr, rmd->str+16, 8)) return 0;
    adr += 9;
	if (!mem_Write_Param(adr, rmd->str+24, 7)) return 0;
    adr += 8;

	if (!mem_Write_Param(adr, &rmd->time_dur, 3)) return 0;
    adr += 4;

	if (!mem_Write_Param(adr, &rmd->time_rep, 3)) return 0;
    adr += 4;

	if (!mem_Write_Param(adr, &rmd->repnum, 1)) return 0;
    adr += 2;

	if (!mem_Write_Param(adr, &rmd->state, 1)) return 0;
    adr += 2;

	if (!mem_Write_Param(adr, &rmd->melody, 1)) return 0;
    return 1;
}


//------------------------------------------------------------------------------
//внут. функции
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//работа с общей памятью
//------------------------------------------------------------------------------

void mem_Ini(void)
{
	STA_N(STA_IIC);
	STA_N(STA_MEM);
	MEM_FM24C64_INI(0);
}

bool mem_Open(dir_t direct)
{
	gMem.chip_adr = gMem.adr;
	FM24C64_OFF_PROTECT(0);
	if (mem_Fm24c64_Open(FM24C64_ADR_0, direct))
	{
		STA_N(STA_IIC);
		STA_N(STA_MEM);
		return true;
	}
	FM24C64_ON_PROTECT(0);
	err_FM24C64();
	return false;
}


bool mem_Tx_Byte(uint8_t byte)
{
	gMem.chip_adr = gMem.adr;
	FM24C64_OFF_PROTECT(0);
	if (!mem_Fm24c64_Tx_Data(&byte, 1))
	{
		err_FM24C64();
		FM24C64_ON_PROTECT(0);
		return false;
	}
	FM24C64_ON_PROTECT(0);
	STA_N(STA_IIC);
	STA_N(STA_MEM);
	return true;
}


bool mem_Rx_Byte(uint8_t* byte)
{
	gMem.chip_adr = gMem.adr;
	if (mem_Fm24c64_Rx_Data(byte, 1))
	{
		FM24C64_ON_PROTECT(0);
		STA_N(STA_IIC);
		STA_N(STA_MEM);
		return true;
	}
	gMem.adr = gMem.chip_adr;
	FM24C64_ON_PROTECT(0);
	err_FM24C64();
	return false;
}


bool mem_Tx_Data(uint8_t* data, ubase_t dsize)
{
	gMem.chip_adr = gMem.adr;
	FM24C64_OFF_PROTECT(0);
	ubase_t wsize;
	wsize = mem_Fm24c64_Tx_Data(data, dsize);
	FM24C64_ON_PROTECT(0);
	gMem.adr = gMem.chip_adr;
	if (wsize == dsize)
	{
		STA_N(STA_IIC);
		STA_N(STA_MEM);
		return true;
	}
	err_FM24C64();
	return false;
}


bool mem_Rx_Data(uint8_t* data, ubase_t dsize)
{
	gMem.chip_adr = gMem.adr;
	FM24C64_OFF_PROTECT(0);
	ubase_t wsize;
	wsize = mem_Fm24c64_Rx_Data(data, dsize);
	FM24C64_ON_PROTECT(0);
	gMem.adr = gMem.chip_adr;
	if (wsize == dsize) return true;
	err_FM24C64();
	return false;
}

void mem_Close(dir_t direct)
{
	FM24C64_ON_PROTECT(0);
	mem_Fm24c64_Close(direct);
}

