#include "err.h"
#include "usr.h"
#include "tdf.h"
#include "tmp.h"
#include "sgn.h"
#include "rmd.h"
#include "def.h"
#include "rtc.h"
#include "alm.h"
#include "tmr.h"
#include "trg.h"
#include "rmd.h"
#include "dsp.h"
#include "light.h"
#include "delay.h"
#include "pcf8583.h"
#include "ds1631.h"
#include "ds18b20.h"
#include "fm24c64.h"
#include "note.h"
#include "dmem.h"
#include "msg.h"

//------------------------------------------------------------------------------
//IIC
//------------------------------------------------------------------------------
/*
 * err_IIC - ошибка IIC
 */
void err_IIC(void)
{
	if (STA_IS_E(STA_IIC))
		return;

	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_IIC;
	msg_Post(&msgW_IicErr, MSG_KEY_TYPE, &var);
	STA_E(STA_IIC);
	STA_E(STA_RTC);
	STA_E(STA_MEM);
	STA_E(STA_TMP_1);
}
//------------------------------------------------------------------------------
//MEM
//------------------------------------------------------------------------------
void err_FM24C64(void)
{
	delay_ms(5);
	time_t time;
	if (!pcf8583_Rd_Time(&time))
	{
		int16_t temp_code;
		if (!ds1631_Rx_TempCode(&temp_code))
		{
			err_IIC();
			return;
		}
	}
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_MEM_LINK;
	var.uint8_t_b = MSG_MEM;
	msg_Post(&msgW_LinkErr, MSG_KEY_TYPE, &var);
	STA_E(STA_MEM);
}

//------------------------------------------------------------------------------
//RTC
//------------------------------------------------------------------------------

void err_Rtc(ubase_t err)
{
	if (err == pcf8583_LOST)
	{
		date_t date = DEF_DATE;
		time_t time = DEF_TIME;
		pcf8583_Wr_Date(&date);
		pcf8583_Wr_Time(&time);
		return;
	}
	if (err != pcf8583_LINK) return;
	delay_ms(5);
	if (!dmem_Check_Fm24c64())
	{
		int16_t temp_code;
		if (!ds1631_Rx_TempCode(&temp_code))
		{
			err_IIC();
			return;
		}
	}

	if (STA_IS_E(STA_RTC)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_RTC_LINK;
	var.uint8_t_b = MSG_RTC;
	msg_Post(&msgW_LinkErr, MSG_KEY_TYPE, &var);
	STA_E(STA_RTC);
}

void err_Rtc_Rd_AvtoShift(void)
{
	gRtc.avto_shift = DEF_RTC_AVTOSHIFT;
	if (STA_IS_E(STA_MEM)) return;
	dmem_Wr_Rtc_AvtoShift(DEF_RTC_AVTOSHIFT);
}

void err_Rtc_Rd_Pore(void)
{
	gRtc.pore = WST_WINTER;
	tdf_Count_Pore(&gRtc.date, &gRtc.time, gRtc.day, &gRtc.pore);
	if (STA_IS_E(STA_MEM)) return;
	dmem_Wr_Rtc_Pore(gRtc.pore);
}

void err_Rtc_Rd_HourSgn(void)
{
	if (STA_IS_E(STA_MEM)) return;
	dmem_Wr_Rtc_HourSgn(DEF_RTC_HOURSGN);
}

//------------------------------------------------------------------------------
//TMP
//------------------------------------------------------------------------------

void err_Ds1631(void)
{
	delay_ms(5);
	time_t time;
	if (!pcf8583_Rd_Time(&time)) {
		if (!dmem_Check_Fm24c64()) {
			err_IIC();
			return;
		}
	}
	ds1631_Ini();
	gTmp.code[0] = 0;

	if (STA_IS_E(STA_TMP_1))
		return;

	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_TEMPSENS1_LINK;
	var.uint8_t_b = MSG_TEMPSENS1;
	msg_Post(&msgW_LinkErr, MSG_KEY_TYPE, &var);
	STA_E(STA_TMP_1);
}

void err_Ds18b20(void)
{
	ds18b20_Ini();
	gTmp.code[1] = 0;

	if (STA_IS_E(STA_TMP_2))
		return;

	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_TEMPSENS2_LINK;
	var.uint8_t_b = MSG_TEMPSENS2;
	msg_Post(&msgW_LinkErr, MSG_KEY_TYPE, &var);
	TMP_2_OFF_TMR();
	STA_E(STA_TMP_2);
}

void err_Tmp_Rd_Use(ubase_t snum)
{
	if (STA_IS_E(STA_MEM))
		return;
	tmp_Set_Use(snum, DEF_TMP_USE);
}

//------------------------------------------------------------------------------
//LIGHT
//------------------------------------------------------------------------------

void err_LightState_Rd(void)
{
	gLight.def_state = DEF_LIGTH_STATE;
	if (STA_IS_E(STA_MEM)) return;
	dmem_Wr_Light_DefState(DEF_LIGTH_STATE);
}

//------------------------------------------------------------------------------
//PSW
//------------------------------------------------------------------------------

void err_Psw_Rd_Use(void)
{
	if (STA_IS_E(STA_MEM)) return;
	dmem_Wr_Psw_Use(DEF_PSW_USE);
}

void err_Psw_Rd_Psw(void)
{
	if (STA_IS_E(STA_MEM)) return;
	dmem_Wr_Psw(DEF_PSW);
}

//------------------------------------------------------------------------------
//ALM
//------------------------------------------------------------------------------
void err_Alm_Rd_TimeOn(ubase_t anum)
{
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_ALM_LOST_TIMEON;
	var.uint8_t_a = anum;
	var.uint8_t_b = MSG_TIMEON;
	msg_Post(&msgW_AlmLost, MSG_KEY_TYPE, &var);
	time_t time = DEF_ALM_TIMEON;
	dmem_Wr_Alm_TimeOn(anum, &time);
}

void err_Alm_Rd_TimeDur(ubase_t anum)
{
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_ALM_LOST_TIMEDUR;
	var.uint8_t_a = anum;
	var.uint8_t_b = MSG_TIMEDUR;
	msg_Post(&msgW_AlmLost, MSG_KEY_TYPE, &var);
	time_t time = DEF_ALM_TIMEDUR;
	dmem_Wr_Alm_TimeDur(anum, &time);
}

void err_Alm_Rd_TimeRep(ubase_t anum)
{
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_ALM_LOST_TIMEREP;
	var.uint8_t_a = anum;
	var.uint8_t_b = MSG_TIMEREP;
	msg_Post(&msgW_AlmLost, MSG_KEY_TYPE, &var);
	time_t time = DEF_ALM_TIMEREP;
	dmem_Wr_Alm_TimeRep(anum, &time);
}

void err_Alm_Rd_Repnum(ubase_t anum)
{
	gAlm.repnum[anum] = DEF_ALM_REPNUM;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_ALM_LOST_REPNUM;
	var.uint8_t_a = anum;
	var.uint8_t_b = MSG_REPNUM;
	msg_Post(&msgW_AlmLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Alm_Repnum(anum, DEF_ALM_REPNUM);
}

void err_Alm_Rd_Day(ubase_t anum)
{
	gAlm.day[anum] = DEF_ALM_DAY;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_ALM_LOST_DAY;
	var.uint8_t_a = anum;
	var.uint8_t_b = MSG_DAY;
	msg_Post(&msgW_AlmLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Alm_Day(anum, DEF_ALM_DAY);
}

void err_Alm_Rd_State(ubase_t anum)
{
	if (DEF_ALM_STATE) SETB(gAlm.state_vector, anum);
	else CLRB(gAlm.state_vector, anum);
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_ALM_LOST_STATE;
	var.uint8_t_a = anum;
	var.uint8_t_b = MSG_STATE;
	msg_Post(&msgW_AlmLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Alm_State(anum, DEF_ALM_STATE);
}

void err_Alm_Rd_Melody(ubase_t anum)
{
	gAlm.melody[anum] = DEF_ALM_MELODY;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_ALM_LOST_MELODY;
	var.uint8_t_a = anum;
	var.uint8_t_b = MSG_MELODY;
	msg_Post(&msgW_AlmLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Alm_Melody(anum, DEF_ALM_MELODY);
}

void err_Alm_Rd_Message_Times(ubase_t anum)
{
	if (DEF_ALM_MELODY) SETB(gAlm.message_vector, anum);
	else CLRB(gAlm.message_vector, anum);
	if (DEF_ALM_TIMES) SETB(gAlm.times_vector, anum);
	else CLRB(gAlm.times_vector, anum);
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_ALM_LOST_MESSAGE;
	var.uint8_t_a = anum;
	var.uint8_t_b = MSG_MESSAGE;
	msg_Post(&msgW_AlmLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Alm_Message(anum, DEF_ALM_MESSAGE);
	var.uint8_t_d = ERR_CODE_ALM_LOST_TIMES;
	var.uint8_t_a = anum;
	var.uint8_t_b = MSG_TIMES;
	msg_Post(&msgW_AlmLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Alm_Message(anum, DEF_ALM_TIMES);
}

//------------------------------------------------------------------------------
//TMR
//------------------------------------------------------------------------------

void err_Tmr_Rd_TimeOn(ubase_t tnum)
{
	time_t time = DEF_TMR_TIMEON;
	gTmr.timeon[tnum] = time;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_TMR_LOST_TIMEON;
	var.uint8_t_a = tnum;
	var.uint8_t_b = MSG_TIMEON;
	msg_Post(&msgW_TmrLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Tmr_TimeOn(tnum, &time);
}

void err_Tmr_Rd_TimeOff(ubase_t tnum)
{
	time_t time = DEF_TMR_TIMEOFF;
	gTmr.timeoff[tnum] = time;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_TMR_LOST_TIMEOFF;
	var.uint8_t_a = tnum;
	var.uint8_t_b = MSG_TIMEOFF;
	msg_Post(&msgW_TmrLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Tmr_TimeOff(tnum, &time);
}

void err_Tmr_Rd_Day(ubase_t tnum)
{
	gTmr.day[tnum] = DEF_TMR_DAY;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_TMR_LOST_DAY;
	var.uint8_t_a = tnum;
	var.uint8_t_b = MSG_DAY;
	msg_Post(&msgW_TmrLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Tmr_Day(tnum, DEF_TMR_DAY);
}

void err_Tmr_Rd_State(ubase_t tnum)
{
	if (DEF_TMR_STATE) SETB(gTmr.state_vector, tnum);
	else CLRB(gTmr.state_vector, tnum);
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_TMR_LOST_STATE;
	var.uint8_t_a = tnum;
	var.uint8_t_b = MSG_STATE;
	msg_Post(&msgW_TmrLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Tmr_State(tnum, DEF_TMR_STATE);
}

void err_Tmr_Rd_Type(ubase_t tnum)
{
	if (DEF_TMR_TYPE) SETB(gTmr.type_vector, tnum);
	else CLRB(gTmr.type_vector, tnum);
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_TMR_LOST_TYPE;
	var.uint8_t_a = tnum;
	var.uint8_t_b = MSG_TYPE;
	msg_Post(&msgW_TmrLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Tmr_Type(tnum, DEF_TMR_TYPE);
}

//------------------------------------------------------------------------------
//TRG
//------------------------------------------------------------------------------

void err_Trg_Rd_Tbottom(ubase_t tnum)
{
	gTrg.tbottom[tnum] = DEF_TRG_TBOTTOM;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_TRG_LOST_TBOTTOM;
	var.uint8_t_a = tnum;
	var.uint8_t_b = MSG_TBOTTOM;
	msg_Post(&msgW_TrgLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Trg_Tbottom(tnum, DEF_TRG_TBOTTOM);
}

void err_Trg_Rd_Ttop(ubase_t tnum)
{
	gTrg.ttop[tnum] = DEF_TRG_TTOP;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_TRG_LOST_TTOP;
	var.uint8_t_a = tnum;
	var.uint8_t_b = MSG_TTOP;
	msg_Post(&msgW_TrgLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Trg_Ttop(tnum, DEF_TRG_TTOP);
}

void err_Trg_Rd_State(ubase_t tnum)
{
	if (DEF_TRG_STATE) SETB(gTrg.state_vector, tnum);
	else CLRB(gTrg.state_vector, tnum);
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_TRG_LOST_STATE;
	var.uint8_t_a = tnum;
	var.uint8_t_b = MSG_STATE;
	msg_Post(&msgW_TrgLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Trg_State(tnum, DEF_TRG_STATE);
}

void err_Trg_Rd_Type(ubase_t tnum)
{
	if (DEF_TRG_TYPE) SETB(gTrg.type_vector, tnum);
	else CLRB(gTrg.type_vector, tnum);
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_TRG_LOST_TYPE;
	var.uint8_t_a = tnum;
	var.uint8_t_b = MSG_TYPE;
	msg_Post(&msgW_TrgLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Trg_Type(tnum, DEF_TRG_TYPE);
}

//------------------------------------------------------------------------------
//RMD
//------------------------------------------------------------------------------

void err_Rmd_Rd_TimeOn(ubase_t rnum)
{
	time_t time = DEF_RMD_TIMEON;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_RMD_LOST_TIMEON;
	var.uint8_t_a = rnum;
	var.uint8_t_b = MSG_TIMEON;
	msg_Post(&msgW_RmdLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Rmd_TimeOn(rnum, &time);
}

void err_Rmd_Rd_DateOn(ubase_t rnum)
{
	date_t date = DEF_RMD_DATEON;
	gRmd[rnum].dateon = date;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_RMD_LOST_DATEON;
	var.uint8_t_a = rnum;
	var.uint8_t_b = MSG_DATEON;
	msg_Post(&msgW_RmdLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Rmd_DateOn(rnum, &date);
}

void err_Rmd_Rd_Filtr(ubase_t rnum)
{
	gRmd[rnum].filtr = DEF_RMD_FILTR;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_RMD_LOST_FILTR;
	var.uint8_t_a = rnum;
	var.uint8_t_b = MSG_FILTR;
	msg_Post(&msgW_RmdLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Rmd_Filtr(rnum, DEF_RMD_FILTR);
}

void err_Rmd_Rd_String(ubase_t rnum)
{
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_RMD_LOST_STR;
	var.uint8_t_a = rnum;
	var.uint8_t_b = MSG_STR;
	msg_Post(&msgW_RmdLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Rmd_String(rnum, note_defualt_reminder_string);
}

void err_Rmd_Rd_State(ubase_t rnum)
{
	if (DEF_RMD_STATE) SETB(gRmd[rnum].state, RMD_STATE_B);
	else CLRB(gRmd[rnum].state, RMD_STATE_B);
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_RMD_LOST_STATE;
	var.uint8_t_a = rnum;
	var.uint8_t_b = MSG_STATE;
	msg_Post(&msgW_RmdLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Rmd_State(rnum, DEF_RMD_STATE);
}

void err_Rmd_Rd_TimeDur(ubase_t rnum)
{
	time_t time = DEF_RMD_TIMEDUR;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_RMD_LOST_TIMEDUR;
	var.uint8_t_a = rnum;
	var.uint8_t_b = MSG_TIMEDUR;
	msg_Post(&msgW_RmdLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Rmd_TimeDur(rnum, &time);
}

void err_Rmd_Rd_TimeRep(ubase_t rnum)
{
	time_t time = DEF_RMD_TIMEREP;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_RMD_LOST_TIMEREP;
	var.uint8_t_a = rnum;
	var.uint8_t_b = MSG_TIMEREP;
	msg_Post(&msgW_RmdLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Rmd_TimeRep(rnum, &time);
}

void err_Rmd_Rd_Repnum(ubase_t rnum)
{
	gRmd[rnum].repnum = DEF_RMD_REPNUM;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_RMD_LOST_REPNUM;
	var.uint8_t_a = rnum;
	var.uint8_t_b = MSG_REPNUM;
	msg_Post(&msgW_RmdLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Rmd_Repnum(rnum, DEF_RMD_REPNUM);
}

void err_Rmd_Rd_Melody(ubase_t rnum)
{
	gRmd[rnum].melody = DEF_RMD_MELODY;
	if (STA_IS_E(STA_MEM)) return;
	usr_temp_t var;
	var.uint8_t_d = ERR_CODE_RMD_LOST_MELODY;
	var.uint8_t_a = rnum;
	var.uint8_t_b = MSG_MELODY;
	msg_Post(&msgW_RmdLost, MSG_KEY_TYPE, &var);
	dmem_Wr_Rmd_Melody(rnum, DEF_RMD_MELODY);
}
