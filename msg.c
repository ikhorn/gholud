#include "msg.h"
#include "err.h"
#include "dsp.h"
#include "rmd.h"
#include "alm.h"
#include "tdf.h"
#include "arr.h"
#include "tmr.h"
#include "trg.h"
#include "rmd.h"
#include "res.h"
#include "wdt.h"
#include "tmp.h"
#include "mexe.h"
#include "mctr.h"
#include "mopr.h"
#include "dmem.h"
#include "note.h"

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

no_init_ gmsg_t gMsg;
void msg_I_ErrCode(void);														//---выводит переданный код ошибки
void msg_I_Link(void);															//---выводит не отвечает
void msg_I_Lost(void);															//---выводит ... утеряно

//------------------------------------------------------------------------------
//карта сообщений
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//						проверка запуска первый раз
//------------------------------------------------------------------------------

void msg_I_Firstly_Title(void);
void menu_I_Firstly_OK(void);

funcp_pr_t listFirstly[] =
{
	msg_I_Firstly_Title,
	menu_I_Firstly_OK
};
usr_wnd_t msgW_Firstly = {listFirstly, list_size(listFirstly)};

void msg_I_Firstly_Title(void)
{
	if (USR_IS_EXECUTING && usr_Get_Execute() == mexe_Set_ByDef)
		usr_Disp_Str(note_waite);
	else usr_Disp_Str(note_device_started);
}

void menu_I_Firstly_OK(void)
{
	if (USR_IS_EXECUTING && usr_Get_Execute() == mexe_Set_ByDef)
	{
		uint8_t curv = usr_Get_ExeVar(uint8_t_c);
		usr_Disp_PorgressStr(ALM_NUM+TMR_NUM+TRG_NUM+RMD_NUM+TMP_NUM, curv);
	}
	else usr_Disp_Str(note_firstly);
	
	if (USR_IS_ENTER)
	{
		dmem_Clear_FirstStart();
		usr_Set_ExeVar(uint8_t_a, 0);
		usr_Set_ExeVar(uint8_t_b, RES_IS_ALM);
		usr_Set_ExeVar(uint8_t_c, 0);
		usr_Set_Executing(mexe_Set_ByDef);
		USR_REFRESH_DISP();
		return;
	}
	else if (USR_IS_KEY(USR_RETURNKEY))
	{
		dmem_Clear_FirstStart();
		wdt_Reset_Dev_by_Wdt();
	}
	msg_Scan_Key();
}
//------------------------------------------------------------------------------
//							"IIC"
//------------------------------------------------------------------------------
void msg_I_IicErr_Title(void);

funcp_pr_t listIicErr[] =
{
	msg_I_IicErr_Title,
	msg_I_ErrCode
};
usr_wnd_t msgW_IicErr = {listIicErr, list_size(listIicErr)};


void msg_I_IicErr_Title(void)
{
	usr_Disp_Str(note_iic);
}
//------------------------------------------------------------------------------
//						"MEM сбой"
//------------------------------------------------------------------------------
void msg_I_MemLinkErr_Title(void);

funcp_pr_t listMemLinkErr[] =
{
	msg_I_MemLinkErr_Title,
	msg_I_Link,
	msg_I_ErrCode
};
usr_wnd_t msgW_MemLinkErr = {listMemLinkErr, list_size(listMemLinkErr)};


void msg_I_MemLinkErr_Title(void)
{
	usr_Disp_Str(note_mem);
}
//------------------------------------------------------------------------------
//LINK
//------------------------------------------------------------------------------
void msg_I_LinkErr_Title(void);

funcp_pr_t listLinkErr[] =
{
	msg_I_LinkErr_Title,
	msg_I_Link,
	msg_I_ErrCode
};
usr_wnd_t msgW_LinkErr = {listLinkErr, list_size(listLinkErr)};


void msg_I_LinkErr_Title(void)
{
	const USRC_STR_ *str;
	switch (usr_Get_Var(uint8_t_b))
	{
		case MSG_RTC:		str = note_rtc; break;
		case MSG_TEMPSENS1:	str = note_tempsens1; break;
		case MSG_TEMPSENS2:	str = note_tempsens2; break;
		case MSG_MEM:		str = note_mem; break;
		default:			str = note_bla_bla; break;
	}
	usr_Disp_Str(str);
}

//------------------------------------------------------------------------------
//						"RTC был перевод времени"
//------------------------------------------------------------------------------
/*
void msg_I_WasTimeShift_Title(void);
void msg_I_WasTimeShift_Time(void);

funcp_pr_t listWasTimeShift[] =
{
	msg_I_WasTimeShift_Title,
	msg_I_WasTimeShift_Time
};
usr_wnd_t msgW_WasTimeShift = {listWasTimeShift, list_size(listWasTimeShift)};


void msg_I_WasTimeShift_Title(void)
{
	usr_Disp_Str(note_converted);
}

void msg_I_WasTimeShift_Time(void)
{
	usr_Disp_Str((usr_Get_Var(wst_t_a) == WST_WINTER) ? note_down_1_hour : note_up_1_hour);
	msg_Scan_Key();
}
*/
//------------------------------------------------------------------------------
//ALM
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//						"Сообщение будильник сработал"
//------------------------------------------------------------------------------
void msg_I_AlmWas_Title(void);
void msg_I_AlmWas_Time(void);

funcp_pr_t listAlmWas[] =
{
	msg_I_AlmWas_Title,
	msg_I_AlmWas_Time
};
usr_wnd_t msgW_AlmWas = {listAlmWas, list_size(listAlmWas)};

void msg_I_AlmWas_Title(void)
{
	usr_Disp_Str_INT16U(note_aclock, usr_Get_Var(uint8_t_a)+1);
}

void msg_I_AlmWas_Time(void)
{
	time_t time;
	dmem_Rd_Alm_TimeOn(usr_Get_Var(uint8_t_a), &time);
	mopr_Disp_Str_Time(note_ring, &time);
	if (USR_IS_ENTER) alm_Reset(usr_Get_Var(uint8_t_a));
		
	if (usr_Key == USR_ENTERKEY_1 || usr_Key == USR_ENTERKEY_2 ||
		usr_Key == USR_RETURNKEY)
	{
		alm_Reset(usr_Get_Var(uint8_t_a));
	}
		
	msg_Scan_Key();
}

//------------------------------------------------------------------------------
//						"Утеряно"
//------------------------------------------------------------------------------

funcp_pr_t listAlmLost[] =
{
	msg_I_AlmWas_Title,
	msg_I_Lost,
	msg_I_ErrCode
};
usr_wnd_t msgW_AlmLost = {listAlmLost, list_size(listAlmLost)};

//------------------------------------------------------------------------------
//RMD
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//						"RMD Signal"
//------------------------------------------------------------------------------
void msg_I_RmdSignal_Title(void);
void msg_I_RmdSignal_Str(void);

funcp_pr_t listRmdSignal[] =
{
	msg_I_RmdSignal_Title,
	msg_I_RmdSignal_Str
};
usr_wnd_t msgW_RmdSignal = {listRmdSignal, list_size(listRmdSignal)};


void msg_I_RmdSignal_Title(void)
{
	mopr_Disp_AboutRmd();
}

void msg_I_RmdSignal_Str(void)
{
	uint8_t str[31];
	if (!dmem_Rd_Rmd_String(usr_Get_Var(uint8_t_a), str)) *str = 0;
	uint8_t out[16];
	arr_Roll_Str(str, usr_Get_ptrVar(uint8_t_c), 15, out);
//	if (USR_IS_ENTER) rmd_Reset(usr_Get_Var(uint8_ta));

	if (usr_Key == USR_ENTERKEY_1 || usr_Key == USR_ENTERKEY_2 ||
		usr_Key == USR_RETURNKEY)
	{
		rmd_Reset(usr_Get_Var(uint8_t_a));
	}
	dsp_Print(usr_CurStrNum(), DSP_STR_LEFT_PUTTED, note_cmd_8, out);
	msg_Scan_Key();
}

//------------------------------------------------------------------------------
//						"Утеряно"
//------------------------------------------------------------------------------

void msg_I_RmdLost_Title(void);

funcp_pr_t listRmdLost[] =
{
	msg_I_RmdLost_Title,
	msg_I_Lost,
	msg_I_ErrCode
};
usr_wnd_t msgW_RmdLost = {listRmdLost, list_size(listRmdLost)};

void msg_I_RmdLost_Title(void)
{
	usr_Disp_Str_INT16U(note_timer, usr_Get_Var(uint8_t_a)+1);
}
//------------------------------------------------------------------------------
//TMR
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//						"Утеряно"
//------------------------------------------------------------------------------

void msg_I_TmrLost_Title(void);

funcp_pr_t listTmrLost[] =
{
	msg_I_TmrLost_Title,
	msg_I_Lost,
	msg_I_ErrCode
};
usr_wnd_t msgW_TmrLost = {listTmrLost, list_size(listTmrLost)};

void msg_I_TmrLost_Title(void)
{
	usr_Disp_Str_INT16U(note_timer, usr_Get_Var(uint8_t_a)+1);
}

//------------------------------------------------------------------------------
//TRG
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//						"Tн > Tb"
//------------------------------------------------------------------------------
void msg_I_TrgTbmoreTt_Title(void);
void msg_I_TrgTbmoreTt_TbMoreTt(void);

funcp_pr_t listTrgTbmoreTt[] =
{
	msg_I_TrgTbmoreTt_Title,
	msg_I_TrgTbmoreTt_TbMoreTt,
};
usr_wnd_t msgW_TrgTbmoreTt = {listTrgTbmoreTt, list_size(listTrgTbmoreTt)};


void msg_I_TrgTbmoreTt_Title(void)
{
	usr_Disp_Str(note_not_can);
}

void msg_I_TrgTbmoreTt_TbMoreTt(void)
{
	usr_Disp_Str(note_Tb_more_Tt);
	msg_Scan_Key();
}
//------------------------------------------------------------------------------
//						"Утеряно"
//------------------------------------------------------------------------------

void msg_I_TrgLost_Title(void);

funcp_pr_t listTrgLost[] =
{
	msg_I_TrgLost_Title,
	msg_I_Lost,
	msg_I_ErrCode
};
usr_wnd_t msgW_TrgLost = {listTrgLost, list_size(listTrgLost)};

void msg_I_TrgLost_Title(void)
{
	usr_Disp_Str_INT16U(note_trg, usr_Get_Var(uint8_t_a)+1);
}
//------------------------------------------------------------------------------
//TMP
//------------------------------------------------------------------------------
void msg_I_TmpOnSens_Title(void);
void msg_I_TmpOnSens_Sens(void);

funcp_pr_t listTmpOnSens[] =
{
	msg_I_TmpOnSens_Title,
	msg_I_TmpOnSens_Sens
};
usr_wnd_t msgW_TmpOnSens = {listTmpOnSens, list_size(listTmpOnSens)};

void msg_I_TmpOnSens_Title(void)
{
	usr_Disp_Str(note_will_on);
}

void msg_I_TmpOnSens_Sens(void)
{
	usr_Disp_Str_INT16U(note_temp_sens, usr_Get_Var(uint8_t_a)+1);
	msg_Scan_Key();
}


void msg_Ini(void)
{

}

//------------------------------------------------------------------------------
//Внут. функции
//------------------------------------------------------------------------------

void msg_I_ErrCode(void)
//---выводит переданный код ошибки
{
	uint8_t code;
	code = usr_Get_Var(uint8_t_d);
	usr_Disp_Str_INT16U(note__error, code);
	msg_Scan_Key();
}

void msg_I_Link(void)
//---выводит не отвечает
{
	usr_Disp_Str(note_not_respond);
	msg_Scan_Key();
}

void msg_I_Lost(void)
//---утеряно
{
	const USRC_STR_ *str;
	switch (usr_Get_Var(uint8_t_b))
	{
		case MSG_TIMEON:	str = note_timeon; break;
		case MSG_TIMEDUR:	str = note_timedur; break;
		case MSG_TIMEREP:	str = note_timerep; break;
		case MSG_REPNUM:	str = note_repnum; break;
		case MSG_DAY:		str = note_wdays; break;
		case MSG_STATE:		str = note_state; break;
		case MSG_MELODY:	str = note_melody; break;
		case MSG_TYPE:		str = note_outtype; break;
		case MSG_TBOTTOM:	str = note_tbottom; break;
		case MSG_TTOP:		str = note_ttop; break;
		case MSG_DATEON:	str = note_dateon; break;
		case MSG_FILTR:		str = note_filtr; break;
		case MSG_STR:		str = note_str; break;
		case MSG_MESSAGE:	str = note_message; break;
		case MSG_TIMES: 	str = note_one_time; break;
		default:			str = note_bla_bla; break;
	}
	usr_Disp_2Str(str, note_lost);
	msg_Scan_Key();
}

