#include "tdf.h"
#include "arr.h"
#include "usr.h"
#include "rtc.h"
#include "alm.h"
#include "tmr.h"
#include "dsp/dsp.h"
#include "rmd.h"
#include "tmp.h"
#include "trg.h"
#include "sgn.h"
#include "def.h"
#include "res.h"
#include "zmr.h"
#include "msg.h"
#include "clc.h"
#include "ctmp.h"
#include "mmap.h"
#include "mexe.h"
#include "mopr.h"
#include "mctr.h"
#include "dmem.h"
#include "delay.h"
#include "light.h"
#include "note.h"

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

void menu_I_DayInf_Ini(void);													//---инициализация проги информации о дне
static uint8_t menu_act_ring_asked = 0;

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

void usr_Handle_HotKey_1(void)
{
	light_Toggle();
}

void usr_Handle_HotKey_2(void)
//---гор. клавиша 'd'
{
	if (gSgn.state)																//сбросить того, кто звонит
	{
		if (gSgn.host_name == RES_IS_ALM) alm_Reset_Ring(gSgn.host_num);
		else if (gSgn.host_name == RES_IS_RMD) rmd_Reset_Ring(gSgn.host_num);
		USR_REFRESH_DISP();
	}
}

void usr_Handle_HotKey_3(void)
//---гор. клавиша '.'
{
	if (menu_act_ring_asked) return;
	uint8_t act_list[2];	/* the list of act resourses */
	uint8_t act_res = 0;	/* the current number of resourse */

	/* check remainders */
	for (uint8_t r = 0; r < RMD_NUM; r++)
	{
		if (rmd_State(r))
		{
			if (act_res < 2)
			{
				act_list[act_res++] = r;
			}
			else
				break;
		}
	}

	/* check alarms */
	for (uint8_t a = 0; a < ALM_NUM; a++)
	{
		if (alm_State(a))
		{
			if (act_res < 2)
			{
				act_list[act_res++] = a | 0x80;
			}
			else
				break;
		}
	}

	if (!act_res)
		return;

	/* if it has one active resourse */
	if (act_res == 1)
	{
		if (act_list[0] & 0x80)
		{
			alm_Reset(act_list[0] & 0x7F);
		}
		else
		{
			rmd_Reset(act_list[0]);
		}
		USR_REFRESH_DISP();
		return;
	}

	/* make list for message with asking */
	usr_Set_InVar(uint8_t_arr[0], act_list[0]);
	usr_Set_InVar(uint8_t_arr[1], act_list[1]);
	usr_Set_InVar(uint8_t_arr[2], 0);

	/* make message with asking */
	menu_act_ring_asked = 1;
	usr_Goto(&menW_ActRing, ITEM_ActRing_Enter, USR_WND_MENU);
}

void wrt_Handle_HotKey_1(void)
//---быстрый вход до будильников '1'
{
	uint8_t psw_use = 1;
	dmem_Rd_Psw_Use(&psw_use);
	if (psw_use)
	{
		usr_Set_InVar(uint8_t_a, MCTRL_GOTO_ALM);
		usr_Set_InVar(uint8_t_c, 0);
		usr_Goto(&menW_ChkPsw, ITEM_ChkPsw_Enter, USR_WND_MENU);
	}
	else
	{
		usr_Set_InVar(uint8_t_a, 0);
		usr_Goto(&menW_Alms, ITEM_Alms_Alm, USR_WND_MENU);
	}
}

void wrt_Handle_HotKey_2(void)
//---быстрый вход до напоминалок '2'
{
	uint8_t psw_use = 1;
	dmem_Rd_Psw_Use(&psw_use);
	usr_Set_InVar(uint8_t_c, 0);
	if (psw_use)
	{
		usr_Set_InVar(uint8_t_a, MCTRL_GOTO_RMD);
		usr_Goto(&menW_ChkPsw, ITEM_ChkPsw_Enter, USR_WND_MENU);
	}
	else usr_Goto(&menW_AskNum, ITEM_AskNum_Num, USR_WND_MENU);
}

void wrt_Handle_HotKey_3(void)
//---быстрый вход до калькулятора '3'
{
	usr_Goto(&clcW, CLC_ITEMNUM, CLC_WND_TYPE);
}

void wrt_Handle_HotKey_4(void)
//---быстрый в конвертор температур
{
	usr_Goto(&ctmpW, CTMP_ITEMNUM, CTMP_WND_TYPE);
}

void menu_I_StrByDefault(void)
{
	usr_Disp_Str(note_bla_bla);
}

void stw_Signal(void)
{
	sgn_Try_On(RES_IS_STW, 0, SGN_MEL_BLINK_1);
}

void stw_Signal_Off(void)
{
	sgn_Off();
}

void menu_Ini(void)
{
	menu_I_DayInf_Ini();
}

//------------------------------------------------------------------------------
//								ВХОД В МЕНЮ
//------------------------------------------------------------------------------

void menu_I_Enter_Title(void)
{
	usr_Disp_Str(note_password);
}

void menu_I_Enter_Enter(void)
{
	uint8_t psw_use = 1;
	dmem_Rd_Psw_Use(&psw_use);
	if (psw_use)
	{
		usr_Set_Var(uint8_t_a, MCTRL_GOTO_MENU);
		usr_Set_Var(uint8_t_c, 0);
		usr_Replace(&menW_ChkPsw, ITEM_ChkPsw_Enter, USR_WND_MENU);
	}
	else usr_Replace(&ENTERMENU_Enter_Enter, ENTERITEM_Enter_Enter, USR_WND_MENU);
}

//------------------------------------------------------------------------------
//								ПРОВЕРКА ПАРОЛЯ
//------------------------------------------------------------------------------

void menu_I_ChkPsw_Title(void)
{
	usr_Disp_2Str(note_ascinput, note_password);
}

void menu_I_ChkPsw_Enter(void)
{	
	enum {SYMNUM = 9};
	if (USR_IS_INPUT) usr_Disp_Input_Psw(SYMNUM);
	else
	{
		if (usr_Get_Var(uint8_t_c)) usr_Key = USR_RETURNKEY;
		else
		{
			usr_Set_Var(uint8_t_c, 1);
			usr_Key = USR_ENTERKEY_1;
		}
	}
	
	uint32_t psw;
	if (usr_Input_INT32U(&psw, 0, 999999999, 9))
	{
		uint32_t real_psw;
		if (!dmem_Rd_Psw(&real_psw))
		{
			if (DEF_PSW != psw) msg_Post(&msgW_WrongInput, MSG_TIME_TYPE, 0);
		}
		if (real_psw == psw || DEF_PSW == psw)
		{
			switch (usr_Get_Var(uint8_t_a))
			{
				case MCTRL_GOTO_ALM:
				{
					usr_Set_Var(uint8_t_a, 0);
					usr_Replace(&menW_Alms, ITEM_Alms_Alm, USR_WND_MENU);
					break;
				}
				case MCTRL_GOTO_RMD:
				{
					usr_Set_Var(uint8_t_c, 0);
					usr_Replace(&menW_AskNum, ITEM_AskNum_Num, USR_WND_MENU);
					break;
				}
				case MCTRL_GOTO_PSW:
				{
					usr_Set_Var(uint8_t_c, 0);
					usr_Replace(&menW_Password, ITEM_Password_InpPassword, USR_WND_MENU);
					break;
				}
				default:
				{
					usr_Replace(&ENTERMENU_Enter_Enter, ENTERITEM_Enter_Enter, USR_WND_MENU);
					break;
				}
			}
		}
		else
		{
			msg_Post(&msgW_WrongInput, MSG_TIME_TYPE, 0);
		}
	}
	menu_Navigate();
}

//------------------------------------------------------------------------------
//						БЫСТРЫЙ НОМЕР НАПОМИНАЛКИ
//------------------------------------------------------------------------------

void menu_I_AskNum_Title(void)
{
	usr_Disp_2Str(note_ascinput, note_reminder);
}

void menu_I_AskNum_Num(void)
{
	enum {SYMNUM = 2};
	if (USR_IS_INPUT) usr_Disp_Input(SYMNUM);
	else
	{
		if (usr_Get_Var(uint8_t_c)) usr_Key = USR_RETURNKEY;
		else
		{
			usr_Set_Var(uint8_t_c, 1);
			usr_Key = USR_ENTERKEY_1;
		}
	}
	
	uint32_t rnum;
	if (usr_Input_INT32U(&rnum, 1, RMD_NUM, SYMNUM))
	{
		usr_Set_Var(uint8_t_a, rnum-1);
		usr_Replace(&ENTERMENU_AskNum_Num, ENTERITEM_AskNum_Num, USR_WND_MENU);
	}
	menu_Navigate();
}

//------------------------------------------------------------------------------
//								МЕНЮ
//------------------------------------------------------------------------------
void menu_I_Menu_Title(void)
{
	usr_Disp_Str(note_menu);
}

void menu_I_Menu_Alms(void)
{
	usr_Disp_Str(note_aclocks);
	usr_Set_InVar(uint8_t_a, 0);
	menu_Navigate_(Menu_Alms);
}

void menu_I_Menu_Tmrs(void)
{
	usr_Disp_Str(note_timers);
	usr_Set_InVar(uint8_t_a, 0);
	menu_Navigate_(Menu_Tmrs);
}

void menu_I_Menu_Trgs(void)
{
	usr_Disp_Str(note_trg);
	usr_Set_InVar(uint8_t_a, 0);
	menu_Navigate_(Menu_Trgs);
}

void menu_I_Menu_Rmds(void)
{
	usr_Disp_Str(note_reminders);
    if (USR_IS_KEY('1'))
    {
		usr_Set_InVar(uint8_t_c, 0);
		usr_Goto(&menW_AskNum, ITEM_AskNum_Num, USR_WND_MENU);
    }
    else
    {
	    usr_Set_InVar(uint8_t_a, 0);
	    menu_Navigate_(Menu_Rmds);
    }
}

void menu_I_Menu_TempSens(void)
{
	usr_Disp_Str(note_temp_sens);
	if (USR_IS_ENTER)
	{
		usr_Set_InVar(uint8_t_a, 0);
	}
	menu_Navigate_(Menu_TempSens);
}


void menu_I_Menu_Progs(void)
{
	usr_Disp_Str(note_progs);
	menu_Navigate_(Menu_Progs);
}

void menu_I_Menu_General(void)
{
	usr_Disp_Str(note_general);
	menu_Navigate_(Menu_General);
}

//------------------------------------------------------------------------------
//						БУДИЛЬНИКИ
//------------------------------------------------------------------------------
void menu_I_Alms_Title(void)
{
	usr_Disp_Str(note_aclocks);
}

void menu_I_Alms_Alm(void)
{
	time_t time;
	uint8_t sym;
	ubase_t almnum = usr_Get_Var(uint8_t_a);
	if (!dmem_Rd_Alm_TimeOn(almnum, &time))
	{
		time.sec = 0x00;
		time.min = 0x00;
		time.hour = 0x00;
	};
	if (CHKB(gAlm.state_vector, almnum)) sym = NOTE_SYM_BELL;
	else sym = DSPC_SYM_SPACE;
	dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_27, note_aclock, almnum+1, &time, sym);
	
	mopr_ListUpDown_Num(ALM_NUM-1);
	menu_Navigate_(Alms_Alm);
}

//------------------------------------------------------------------------------
//						БУДИЛЬНИК
//------------------------------------------------------------------------------
void menu_I_Alm_Title(void)
{
	usr_Disp_Str_INT16U(note_aclock, usr_Get_Var(uint8_t_a)+1);
}

void menu_I_Alm_OnOff(void)
{
	ubase_t state;
	if (CHKB(gAlm.state_vector, usr_Get_Var(uint8_t_a))) state = 1;
	else state = 0;
	mopr_Disp_CheckmarkStr(note_on, state);
	if (USR_IS_ENTER)
	{
		state = !state;
		if (state) alm_On(usr_Get_Var(uint8_t_a));
		else alm_Off(usr_Get_Var(uint8_t_a));
		USR_REFRESH_DISP();
	}
	else menu_Navigate();
}

void menu_I_Alm_TimeOn(void)
{
	if (USR_IS_INPUT)
	{
		usr_Disp_Input_Time();
	}
	else
	{
		time_t time;
		if (!dmem_Rd_Alm_TimeOn(usr_Get_Var(uint8_t_a), &time))
		{
			time.sec = 0x00;
			time.min = 0x00;
			time.hour = 0x00;
		};
		mopr_Disp_Str_Time(note_timeon, &time);
		
		if (USR_ITEM_ACTIVE)
		{
			if (usr_Key == USR_ENTERKEY_1) usr_Set_Var(uint8_t_b, MCTRL_DEF_INPUT);
			else if (usr_Key == USR_ENTERKEY_2) usr_Set_Var(uint8_t_b, MCTRL_ALT_INPUT);
		}
	}
	
	time_t time;
	if (usr_Input_Time(&time))
	{
		if (usr_Get_Var(uint8_t_b) == MCTRL_ALT_INPUT)
		{
			usr_Set_Var(uint8_t_b, MCTRL_DEF_INPUT);
			uint32_t sec;
			if (!tdf_Plus_Time(&gRtc.time, &time, &sec)) return;
			if (!tdf_Sec_to_Time(sec, &time)) return;
		}
		alm_Set_TimeOn(usr_Get_Var(uint8_t_a), &time);
	}
	else menu_Navigate();
}

void menu_I_Alm_Melody(void)
{
	ubase_t anum = usr_Get_Var(uint8_t_a);
	mopr_Disp_MelodyType(gAlm.melody[anum]);
	if (USR_IS_ENTER)
	{
		usr_Set_InVar(uint8_t_b, RES_IS_ALM);
		usr_Set_InVar(uint8_t_a, anum);
		usr_Set_InVar(uint8_t_c, 0);
	}
	menu_Navigate_(Alm_Melody);
}

void menu_I_Alm_TimeDur(void)
{
	if (USR_IS_INPUT)
	{
		usr_Disp_Input_Time();
	}
	else
	{
		time_t time;
		if (!dmem_Rd_Alm_TimeDur(usr_Get_Var(uint8_t_a), &time))
		{
			time.sec = 0x00;
			time.min = 0x00;
			time.hour = 0x00;
		};
		mopr_Disp_Str_Time(note_timedur, &time);
	}
	
	time_t time;
	if (usr_Input_Time(&time))
	{
		alm_Set_TimeDur(usr_Get_Var(uint8_t_a), &time);
	}
	else menu_Navigate();
}

void menu_I_Alm_TimeRep(void)
{
	if (USR_IS_INPUT)
	{
		usr_Disp_Input_Time();
	}
	else
	{
		time_t time;
		if (!dmem_Rd_Alm_TimeRep(usr_Get_Var(uint8_t_a), &time))
		{
			time.sec = 0x00;
			time.min = 0x00;
			time.hour = 0x00;
		};
		mopr_Disp_Str_Time(note_timerep, &time);
	}
	
	time_t time;
	if (usr_Input_Time(&time))
	{
		alm_Set_TimeRep(usr_Get_Var(uint8_t_a), &time);
	}
	else menu_Navigate();
}

void menu_I_Alm_Repnum(void)
{
	enum {SYMNUM = 3};
	if (USR_IS_INPUT) usr_Disp_Input(SYMNUM);
	else
	{
		uint8_t repnum;
		dmem_Rd_Alm_Repnum(usr_Get_Var(uint8_t_a), &repnum);
		usr_Disp_Str_INT16U(note_repnum, gAlm.repnum[usr_Get_Var(uint8_t_a)]);
	}
	
	uint32_t rnum;
	if (usr_Input_INT32U(&rnum, 1, 255, SYMNUM))
	{
		alm_Set_Repnum(usr_Get_Var(uint8_t_a), rnum);
	}
	else menu_Navigate();
}

void menu_I_Alm_Days(void)
{
	ubase_t anum;
	anum = usr_Get_Var(uint8_t_a);
	mopr_Disp_OnDays(gAlm.day[anum]);
	if (USR_IS_ENTER)
	{
		usr_Set_InVar(uint8_t_b, RES_IS_ALM);
		usr_Set_InVar(uint8_t_a, anum);
	}
	menu_Navigate_(Alm_Days);
}

void menu_I_Alm_Message(void)
{
	ubase_t message;
	if (CHKB(gAlm.message_vector, usr_Get_Var(uint8_t_a))) message = 1;
	else message = 0;
	mopr_Disp_CheckmarkStr(note_message, message);
	if (USR_IS_ENTER)
	{
		alm_Set_Message(usr_Get_Var(uint8_t_a), !message);
		USR_REFRESH_DISP();
	}
	menu_Navigate();
}

void menu_I_Alm_OneTime(void)
{
	ubase_t times;
	if (CHKB(gAlm.times_vector, usr_Get_Var(uint8_t_a))) times = 1;
	else times = 0;
	mopr_Disp_CheckmarkStr(note_one_time, times);
	if (USR_IS_ENTER)
	{
		alm_Set_Times(usr_Get_Var(uint8_t_a), !times);
		USR_REFRESH_DISP();
	}
	menu_Navigate();
}
//------------------------------------------------------------------------------
//						ДНИ НЕДЕЛИ
//------------------------------------------------------------------------------
void menu_I_Days_Title(void)
{
	usr_Disp_Str(note_weekdays);
}

void menu_I_Days_AllDays(void)
{
	day_t day;
	if (usr_Get_Var(uint8_t_b) == RES_IS_ALM)
	{
		day = gAlm.day[usr_Get_Var(uint8_t_a)];
		if (USR_IS_ENTER)
		{
			if (DAY_ALL == day) day = DAY_NO;
			else day = DAY_ALL;
			alm_Set_Day(usr_Get_Var(uint8_t_a), day);
		}
	}
	else if (usr_Get_Var(uint8_t_b) == RES_IS_TMR)
	{
		day = gTmr.day[usr_Get_Var(uint8_t_a)];
		if (USR_IS_ENTER)
		{
			if (DAY_ALL == day) day = DAY_NO;
			else day = DAY_ALL;
			tmr_Set_Day(usr_Get_Var(uint8_t_a), day);
		}
	}
	else
		usr_Exit();

	mopr_Disp_CheckmarkStr(note_all, (DAY_ALL == day) ? 1:0);
	menu_Navigate();
}

void menu_I_Days_Monday(void)
{
	mopr_Set_Day(note_monday, DAY_MON);
}

void menu_I_Days_Tuesday(void)
{
	mopr_Set_Day(note_tuesday, DAY_TUE);
}

void menu_I_Days_Wednesday(void)
{
	mopr_Set_Day(note_wednesday, DAY_WED);
}

void menu_I_Days_Thursday(void)
{
	mopr_Set_Day(note_thursday, DAY_THU);
}

void menu_I_Days_Friday(void)
{
	mopr_Set_Day(note_friday, DAY_FRI);
}

void menu_I_Days_Saturday(void)
{
	mopr_Set_Day(note_saturday, DAY_SAT);
}

void menu_I_Days_Sunday(void)
{
	mopr_Set_Day(note_sunday, DAY_SUN);
}

//------------------------------------------------------------------------------
//						МЕЛОДИЯ
//------------------------------------------------------------------------------
void menu_I_Melody_Title(void)
{
	usr_Disp_Str(note_melody);
}

void menu_I_Melody_Set(void)
{
	mopr_Set_Melody();
}

//------------------------------------------------------------------------------
//							ТАЙМЕРА
//------------------------------------------------------------------------------

void menu_I_Tmrs_Title(void)
{
	usr_Disp_Str(note_timers);
}

void menu_I_Tmrs_Tmr(void)
{
	uint8_t sym;
	ubase_t tnum = usr_Get_Var(uint8_t_a);
	if (CHKB(gTmr.state_vector, tnum)) sym = DSP_SYM_CHECKMARK;
	else sym = DSPC_SYM_SPACE;
	dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_27, note_timer, tnum+1, &gTmr.timeon[tnum], sym);
	
	mopr_ListUpDown_Num(TMR_NUM-1);
	menu_Navigate_(Tmrs_Tmr);
}

//------------------------------------------------------------------------------
//							ТАЙМЕР
//------------------------------------------------------------------------------

void menu_I_Tmr_Title(void)
{
	usr_Disp_Str_INT16U(note_timer, usr_Get_Var(uint8_t_a)+1);
}

void menu_I_Tmr_OnOff(void)
{
	ubase_t state;
	if (CHKB(gTmr.state_vector, usr_Get_Var(uint8_t_a))) state = 1;
	else state = 0;
	mopr_Disp_CheckmarkStr(note_on, state);
	if (USR_IS_ENTER)
	{
		if (state) tmr_Off(usr_Get_Var(uint8_t_a));
		else tmr_On(usr_Get_Var(uint8_t_a));
		USR_REFRESH_DISP();
	}
	menu_Navigate();
}

void menu_I_Tmr_TimeOn(void)
{
	if (USR_IS_INPUT) usr_Disp_Input_Time();
	else mopr_Disp_Str_Time(note_timeon, &gTmr.timeon[usr_Get_Var(uint8_t_a)]);
	
	time_t time;
	if (usr_Input_Time(&time))
	{
		tmr_Set_TimeOn(usr_Get_Var(uint8_t_a), &time);
	}
	else menu_Navigate();
}

void menu_I_Tmr_TimeOff(void)
{
	if (USR_IS_INPUT) usr_Disp_Input_Time();
	else mopr_Disp_Str_Time(note_timeoff, &gTmr.timeoff[usr_Get_Var(uint8_t_a)]);
	
	time_t time;
	if (usr_Input_Time(&time))
	{
		tmr_Set_TimeOff(usr_Get_Var(uint8_t_a), &time);
	}
	else menu_Navigate();
}

void menu_I_Tmr_Days(void)
{
	ubase_t tnum;
	tnum = usr_Get_Var(uint8_t_a);
	mopr_Disp_OnDays(gTmr.day[tnum]);
	if (USR_IS_ENTER)
	{
		usr_Set_InVar(uint8_t_b, RES_IS_TMR);
		usr_Set_InVar(uint8_t_a, tnum);
	}
	menu_Navigate_(Tmr_Days);
}

void menu_I_Tmr_Type(void)
{
	mopr_Disp_CheckmarkStr(note_inv, !CHKB(gTmr.type_vector, usr_Get_Var(uint8_t_a)));
	if (USR_IS_ENTER)
	{
		tmr_Set_Type(usr_Get_Var(uint8_t_a), !CHKB(gTmr.type_vector, usr_Get_Var(uint8_t_a)));
		USR_REFRESH_DISP();
	}
	menu_Navigate();
}

//------------------------------------------------------------------------------
//						ТЕРМОРЕГУЛЯТОРЫ
//------------------------------------------------------------------------------
void menu_I_Trgs_Title(void)
{
	usr_Disp_Str(note_trg);
}

void menu_I_Trgs_Trg(void)
{
	fl32_t tbottom, ttop;
	TMP_1_CODE_TO_TEMP(gTrg.tbottom[usr_Get_Var(uint8_t_a)], tbottom);
	TMP_1_CODE_TO_TEMP(gTrg.ttop[usr_Get_Var(uint8_t_a)], ttop);
	uint8_t onsym;
	if (CHKB(gTrg.state_vector, usr_Get_Var(uint8_t_a))) onsym = DSP_SYM_CHECKMARK;
	else onsym = DSP_SYM_SPACE;
	dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_29, usr_Get_Var(uint8_t_a)+1, tbottom, ttop, onsym);

	mopr_ListUpDown_Num(TRG_NUM-1);
	menu_Navigate_(Trgs_Trg);
}

//------------------------------------------------------------------------------
//						ТЕРМОСТАТ
//------------------------------------------------------------------------------
void menu_I_Trg_Title(void)
{
	usr_Disp_Str_INT16U(note_trg, usr_Get_Var(uint8_t_a)+1);
}

void menu_I_Trg_OnOff(void)
{
	ubase_t state;
	if (CHKB(gTrg.state_vector, usr_Get_Var(uint8_t_a))) state = 1;
	else state = 0;
	mopr_Disp_CheckmarkStr(note_on, state);
	if (USR_IS_ENTER)
	{
		if (state) trg_Off(usr_Get_Var(uint8_t_a));
		else
		{
			if (CHKB(gTmp.state_vector, usr_Get_Var(uint8_t_a))) trg_On(usr_Get_Var(uint8_t_a));
			else
			{
				usr_temp_t var;
				var.uint8_t_a = usr_Get_Var(uint8_t_a);
				msg_Post(&msgW_TmpOnSens, MSG_TIME_TYPE, &var);
			}
		}
		USR_REFRESH_DISP();
	}
	menu_Navigate();
}

void menu_I_Trg_Tbottom(void)
{
	enum {SYMNUM = 5};
	if (USR_IS_INPUT)
	{
		usr_Disp_Input(SYMNUM);
	}
	else
	{
		fl32_t tbottom;
		if (usr_Get_Var(uint8_t_a))
		{
			TMP_1_CODE_TO_TEMP(gTrg.tbottom[usr_Get_Var(uint8_t_a)], tbottom);
		}
		else
		{
			TMP_2_CODE_TO_TEMP(gTrg.tbottom[usr_Get_Var(uint8_t_a)], tbottom);
		}
		dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_23, note_tbottom, tbottom, note_gC);
	}

	fl64_t tbottom;
	if (usr_Input_FL64(&tbottom, -55.0, 125.0, SYMNUM))
	{
		int16_t tb;
		TMP_1_TEMP_TO_CODE(tbottom, tb);
		trg_Set_Tbottom(usr_Get_Var(uint8_t_a), tb);
	}
	else menu_Navigate();
}

void menu_I_Trg_Ttop(void)
{
	enum {SYMNUM = 5};
	if (USR_IS_INPUT)
	{
		usr_Disp_Input(SYMNUM);
	}
	else
	{
		fl32_t ttop;
		if (usr_Get_Var(uint8_t_a))
		{
			TMP_1_CODE_TO_TEMP(gTrg.ttop[usr_Get_Var(uint8_t_a)], ttop);
		}
		else
		{
			TMP_2_CODE_TO_TEMP(gTrg.ttop[usr_Get_Var(uint8_t_a)], ttop);
		}
		dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_23, note_ttop, ttop, note_gC);
	}

	fl64_t ttop;
	if (usr_Input_FL64(&ttop, -55.0, 125.0, SYMNUM))
	{
		int16_t tt;
		TMP_1_TEMP_TO_CODE(ttop, tt);
		trg_Set_Ttop(usr_Get_Var(uint8_t_a), tt);
	}
	else menu_Navigate();
}

void menu_I_Trg_Type(void)
{
	mopr_Disp_CheckmarkStr(note_inv, !CHKB(gTrg.type_vector, usr_Get_Var(uint8_t_a)));
	if (USR_IS_ENTER)
	{
		trg_Set_Type(usr_Get_Var(uint8_t_a), !CHKB(gTrg.type_vector, usr_Get_Var(uint8_t_a)));
		USR_REFRESH_DISP();
	}
	
	menu_Navigate();
}

//------------------------------------------------------------------------------
//						НАПОМИНАЛКИ
//------------------------------------------------------------------------------
void menu_I_Rmds_Title(void)
{
	usr_Disp_Str(note_reminders);
}

void menu_I_Rmds_Rmd(void)
{
	mopr_Disp_AboutRmd();
	mopr_ListUpDown_Num(RMD_NUM-1);
    if (USR_IS_KEY(USR_ENTERKEY_2))
    {
        ubase_t rnum;
        rnum = usr_Get_Var(uint8_t_a);
		usr_Set_InVar(uint8_t_a, rnum);
		usr_Set_InVar(uint8_t_c, 0);
        usr_Goto(&menW_RmdExchange, ITEM_Rmds_Rmd, USR_WND_MENU);
    }
    else if (USR_IS_KEY('1'))
    {
		usr_Set_Var(uint8_t_c, 0);
		usr_Replace(&menW_AskNum, ITEM_AskNum_Num, USR_WND_MENU);
    }
    else
    {
	    menu_Navigate_(Rmds_Rmd);
    }
}

/*******************************************************************************
                        СМЕНА НОМЕРА НАПОМИНАЛКИ
*******************************************************************************/
void menu_I_RmdExchange_Title(void)
{
	usr_Disp_Str(note_ask_exchange_at);
}

void menu_I_RmdExchange_NewNumber(void)
{
	enum {SYMNUM = 2};
	if (USR_IS_INPUT) usr_Disp_Input(SYMNUM);
	else
	{
		if (usr_Get_Var(uint8_t_c)) usr_Key = USR_RETURNKEY;
		else
		{
			usr_Set_Var(uint8_t_c, 1);
			usr_Key = USR_ENTERKEY_1;
		}
	}
	
	uint32_t rnum;
	if (usr_Input_INT32U(&rnum, 1, RMD_NUM, SYMNUM))
	{
		usr_Set_Var(uint8_t_c, 0);
        //exchange data of rmds
        DRMD_ cur_rmd;
        DRMD_ int_rmd;
        if (!dmem_Rd_Rmd(usr_Get_Var(uint8_t_a), &cur_rmd)) return;
        if (!dmem_Rd_Rmd(rnum-1, &int_rmd)) return;
        if (!dmem_Wr_Rmd(rnum-1, &cur_rmd)) return;
        if (!dmem_Wr_Rmd(usr_Get_Var(uint8_t_a), &int_rmd)) return;
        if (!rmd_Reset(rnum-1)) return;
        if (!rmd_Reset(usr_Get_Var(uint8_t_a))) return;
		usr_Exit();
	} menu_Navigate();
} 
//------------------------------------------------------------------------------
//						НАПОМИНАЛКА
//------------------------------------------------------------------------------
void menu_I_Rmd_Title(void)
{
	usr_Disp_Str_INT16U(note_reminder, usr_Get_Var(uint8_t_a)+1);
}

void menu_I_Rmd_OnOff(void)
{
	ubase_t state;
	if (CHKB(gRmd[usr_Get_Var(uint8_t_a)].state, RMD_STATE_B)) state = 1;
	else state = 0;
	mopr_Disp_CheckmarkStr(note_on, state);
	if (USR_IS_ENTER)
	{
		if (state) rmd_Off(usr_Get_Var(uint8_t_a));
		else rmd_On(usr_Get_Var(uint8_t_a));
		USR_REFRESH_DISP();
	}
	menu_Navigate();
}

void menu_I_Rmd_TimeOn(void)
{
	if (USR_IS_INPUT)
	{
		usr_Disp_Input_Time();
	}
	else
	{
		time_t time;
		if (!dmem_Rd_Rmd_TimeOn(usr_Get_Var(uint8_t_a), &time))
		{
			time.sec = 0x00;
			time.min = 0x00;
			time.hour = 0x00;
		}
		mopr_Disp_Str_Time(note_timeon, &time);
		
		if (USR_ITEM_ACTIVE)
		{
			if (usr_Key == USR_ENTERKEY_1) usr_Set_Var(uint8_t_b, MCTRL_DEF_INPUT);
			else if (usr_Key == USR_ENTERKEY_2) usr_Set_Var(uint8_t_b, MCTRL_ALT_INPUT);
		}
	}
	
	time_t time;
	if (usr_Input_Time(&time))
	{
		if (usr_Get_Var(uint8_t_b) == MCTRL_ALT_INPUT)
		{
			usr_Set_Var(uint8_t_b, MCTRL_DEF_INPUT);
			uint32_t sec;
			if (!tdf_Plus_Time(&gRtc.time, &time, &sec)) return;
			if (!tdf_Sec_to_Time(sec, &time)) return;
		}
		rmd_Set_TimeOn(usr_Get_Var(uint8_t_a), &time);
	}
	else menu_Navigate();
}

void menu_I_Rmd_DateOn(void)
{
	enum {SYMNUM = 3};
	ubase_t input_type = usr_Get_Var(uint8_t_b);
	if (USR_IS_INPUT)
	{
		if (input_type == MCTRL_ALT_INPUT) usr_Disp_Input(SYMNUM);
		else usr_Disp_Input_Date(); 
	}
	else
	{
		day_t day;
		day = tdf_Date_to_Day(&gRmd[usr_Get_Var(uint8_t_a)].dateon);
		mopr_Disp_Str_Date(&gRmd[usr_Get_Var(uint8_t_a)].dateon, day, DSP_STR_SPACE_PUTTED);
		if (USR_ITEM_ACTIVE)
		{
			if (usr_Key == USR_ENTERKEY_1) usr_Set_Var(uint8_t_b, MCTRL_DEF_INPUT);
			else if (usr_Key == USR_ENTERKEY_2) usr_Set_Var(uint8_t_b, MCTRL_ALT_INPUT);
		}
	}
	
	if (input_type == MCTRL_ALT_INPUT)
	{
		uint32_t daynum;
		if (usr_Input_INT32U(&daynum, 1, 255, SYMNUM))
		{
			usr_Set_Var(uint8_t_b, MCTRL_DEF_INPUT);
			date_t date;
			if (!tdf_Date_Plus_Day(&gRtc.date, (uint8_t)daynum, &date)) return;
			rmd_Set_DateOn(usr_Get_Var(uint8_t_a), &date);
		}
		return;
	}

	date_t date;
	if (usr_Input_Date(&date))
	{
		rmd_Set_DateOn(usr_Get_Var(uint8_t_a), &date);
	}
	else menu_Navigate();
}

void menu_I_Rmd_Filtr(void)
{
	mopr_Disp_FiltrType(gRmd[usr_Get_Var(uint8_t_a)].filtr);
	if (USR_IS_ENTER)
	{
		ubase_t rnum;
		rnum = usr_Get_Var(uint8_t_a);
		usr_Set_InVar(uint8_t_a, rnum);
	}
	if (USR_IS_KEY(USR_DOWNKEY))
	{
		usr_Set_Var(uint8_t_c, 0);
	}
	menu_Navigate_(Rmd_Filtr);
}


void menu_I_Rmd_String(void)
{
	enum {SYMNUM = 30};
	if (USR_IS_INPUT) usr_Disp_Input(SYMNUM);
	else
	{
		uint8_t str[31];
		if (!dmem_Rd_Rmd_String(usr_Get_Var(uint8_t_a), str)) *str = 0;
		if (usr_Input_PreStr(str)) return;
		uint8_t out[16];
		arr_Roll_Str(str, usr_Get_ptrVar(uint8_t_c), 15, out);
		dsp_Print(usr_CurStrNum(), DSP_STR_LEFT_PUTTED, note_cmd_8, out);
	}
	
	if (usr_Input_String(SYMNUM))
	{
		uint8_t str[31];
		usr_Read_InputBufferedStr(0, 31, str);
		str[30] = 0;
		dmem_Wr_Rmd_String(usr_Get_Var(uint8_t_a), str);
		usr_Set_Var(uint8_t_c, 0);
	}
	else menu_Navigate();
}


void menu_I_Rmd_Melody(void)
{
	mopr_Disp_MelodyType(gRmd[usr_Get_Var(uint8_t_a)].melody);
	if (USR_IS_ENTER)
	{
		usr_Set_InVar(uint8_t_b, RES_IS_RMD);
		ubase_t rnum;
		rnum = usr_Get_Var(uint8_t_a);
		usr_Set_InVar(uint8_t_a, rnum);
		usr_Set_InVar(uint8_t_c, 0);
	}
	if (USR_IS_KEY(USR_UPKEY))
	{
		usr_Set_Var(uint8_t_c, 0);
	}
	menu_Navigate_(Rmd_Melody);
}

void menu_I_Rmd_TimeDur(void)
{
	if (USR_IS_INPUT)
	{
		usr_Disp_Input_Time();
	}
	else
	{
		time_t time;
		if (!dmem_Rd_Rmd_TimeDur(usr_Get_Var(uint8_t_a), &time))
		{
			time.sec = 0x00;
			time.min = 0x00;
			time.hour = 0x00;
		}
		mopr_Disp_Str_Time(note_timedur, &time);
	}
	
	time_t time;
	if (usr_Input_Time(&time))
	{
		rmd_Set_TimeDur(usr_Get_Var(uint8_t_a), &time);
	}
	else menu_Navigate();
}

void menu_I_Rmd_TimeRep(void)
{
	if (USR_IS_INPUT)
	{
		usr_Disp_Input_Time();
	}
	else
	{
		time_t time;
		if (!dmem_Rd_Rmd_TimeRep(usr_Get_Var(uint8_t_a), &time))
		{
			time.sec = 0x00;
			time.min = 0x00;
			time.hour = 0x00;
		}
		mopr_Disp_Str_Time(note_timerep, &time);
	}
	
	time_t time;
	if (usr_Input_Time(&time))
	{
		rmd_Set_TimeRep(usr_Get_Var(uint8_t_a), &time);
	}
	else menu_Navigate();
}

void menu_I_Rmd_Repnum(void)
{
	enum {SYMNUM = 3};
	if (USR_IS_INPUT) usr_Disp_Input(SYMNUM);
	else
	{
		uint8_t repnum;
		dmem_Rd_Rmd_Repnum(usr_Get_Var(uint8_t_a), &repnum);
		usr_Disp_Str_INT16U(note_repnum, repnum);
	}
	
	uint32_t rnum;
	if (usr_Input_INT32U(&rnum, 1, 255, SYMNUM))
	{
		rmd_Set_Repnum(usr_Get_Var(uint8_t_a), rnum);
	}
	else menu_Navigate();
}

//------------------------------------------------------------------------------
//						ПОВТОР
//------------------------------------------------------------------------------

void menu_I_Filtr_Title(void)
{
	usr_Disp_Str(note_filtr);
}

void menu_I_Filtr_Month(void)
{
	mopr_Set_RmdFiltr(note_month, RMD_FILTR_MONTH);
}

void menu_I_Filtr_Year(void)
{
	mopr_Set_RmdFiltr(note_year, RMD_FILTR_YEAR);
}

void menu_I_Filtr_Off(void)
{
	mopr_Set_RmdFiltr(note_off, RMD_FILTR_OFF);
}

//------------------------------------------------------------------------------
//						ОБЩИЕ НАСТРОЙКИ
//------------------------------------------------------------------------------
void menu_I_General_Title(void)
{
	usr_Disp_Str(note_general);
}

void menu_I_General_TimeDate(void)
{
	usr_Disp_Str(note_timedate);
	menu_Navigate_(General_TimeDate);
}

void menu_I_General_Lightning(void)
{
	mopr_Disp_CheckmarkStr(note_lightning, gLight.def_state);
	if (USR_IS_ENTER)
	{
		light_Set_DefState(!gLight.def_state);
		USR_REFRESH_DISP();
	}
	menu_Navigate();
}

void menu_I_General_HourSgn(void)
{
	mopr_Disp_CheckmarkStr(note_hour_sgn, gRtc.hour_sgn);
	if (USR_IS_ENTER)
	{
		rtc_Set_HourSgn(!gRtc.hour_sgn);
		USR_REFRESH_DISP();
	}
	menu_Navigate();
}

void menu_I_General_ByDefault(void)
{
	usr_Disp_Str(note_bydef);
	menu_Navigate_(General_ByDefault);
}

void menu_I_General_AvtoTimeShift(void)
{
	mopr_Disp_CheckmarkStr(note_shift_time, gRtc.avto_shift);
	if (USR_IS_ENTER)
	{
		rtc_Set_AvtoShift(!gRtc.avto_shift);
		USR_REFRESH_DISP();
	}
	menu_Navigate();
}

void menu_I_General_UsePassword(void)
{
	uint8_t use_psw;
	if (!dmem_Rd_Psw_Use(&use_psw)) use_psw = 1;
	mopr_Disp_CheckmarkStr(note_use_password, use_psw);
	if (USR_IS_ENTER)
	{
		dmem_Wr_Psw_Use(!use_psw);
		USR_REFRESH_DISP();
	}
	menu_Navigate();
}

void menu_I_General_NewPassword(void)
{
	usr_Disp_Str(note_new_password);
	if (USR_IS_ENTER)
	{
		usr_Set_InVar(uint8_t_a, MCTRL_GOTO_PSW);
		usr_Set_InVar(uint8_t_c, 0);
	}
	menu_Navigate_(General_NewPassword);
}

//------------------------------------------------------------------------------
//						НАСТРОЙКА ВРЕМЕНИ/ДАТЫ
//------------------------------------------------------------------------------

void menu_I_TimeDate_Title(void)
{
	usr_Disp_Str(note_timedate);
}

void menu_I_TimeDate_Time(void)
{
	if (USR_IS_INPUT) usr_Disp_Input_Time();
	else mopr_Disp_RtcTime();
	
	time_t time;
	if (usr_Input_Time(&time))
	{
		rtc_Set_Time(&time);
	}
	else menu_Navigate();
}

void menu_I_TimeDate_Date(void)
{
	if (USR_IS_INPUT) usr_Disp_Input_Date();
	else mopr_Disp_RtcDate();
	
	date_t date;
	if (usr_Input_Date(&date))
	{
		rtc_Set_Date(&date);
	}
	else menu_Navigate();
}

//------------------------------------------------------------------------------
//						ПРОВЕРКА УВЕРЕННОСТИ
//------------------------------------------------------------------------------

void menu_I_Confidence_Title(void)
{
	if (USR_IS_EXECUTING && (usr_Get_Execute() == mexe_Set_ByDef)) usr_Disp_Str(note_waite);
	else usr_Disp_Str(note_you_exactly);
}

void menu_I_Confidence_OK(void)
{
	
	if (USR_IS_EXECUTING && (usr_Get_Execute() == mexe_Set_ByDef))
	{
		uint8_t curv = usr_Get_ExeVar(uint8_t_c);
		usr_Disp_PorgressStr(ALM_NUM+TMR_NUM+TRG_NUM+RMD_NUM+TMP_NUM, curv);
	}
	else usr_Disp_Str(note_in_own_mind);
	
	if (USR_IS_ENTER)
	{
		usr_Set_ExeVar(uint8_t_a, 0);
		usr_Set_ExeVar(uint8_t_b, RES_IS_ALM);
		usr_Set_ExeVar(uint8_t_c, 0);
		usr_Set_Executing(mexe_Set_ByDef);
		USR_REFRESH_DISP();
	}
	menu_Navigate();
}

//------------------------------------------------------------------------------
//							НОВЫЙ ПАРОЛЬ
//------------------------------------------------------------------------------

void menu_I_Password_Title(void)
{
	usr_Disp_2Str(note_ascinput, note_new_password);
}

void menu_I_Password_InpPassword(void)
{
	enum {SYMNUM = 9};
	if (USR_IS_INPUT) usr_Disp_Input_Psw(SYMNUM);
	else
	{
		if (usr_Get_Var(uint8_t_c)) usr_Key = USR_RETURNKEY;
		else
		{
			usr_Set_Var(uint8_t_c, 1);
			usr_Key = USR_ENTERKEY_1;
		}
	}
	
	uint32_t psw;
	if (usr_Input_INT32U(&psw, 0, 999999999, SYMNUM))
	{
		dmem_Wr_Psw(psw);
		usr_Exit();
	}
	else menu_Navigate();
}

//------------------------------------------------------------------------------
//				Temperature sensor select
//------------------------------------------------------------------------------

void menu_I_TempSens_Title(void)
{
	usr_Disp_Str(note_temp_sens);
}

void menu_I_TempSens_Select(void)
{
	uint8_t use_sens;

	if (CHKB(gTmp.state_vector, usr_Get_Var(uint8_t_a)))
		use_sens = 1;
	else
		use_sens = 0;

	if (use_sens) {
		dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_36,
			  DSP_SYM_CHECKMARK, note_temp_sens,
			  usr_Get_Var(uint8_t_a)+1);
	} else {
		usr_Disp_Str_INT16U(note_temp_sens, usr_Get_Var(uint8_t_a)+1);
	}

	mopr_ListUpDown_Num(TMP_NUM-1);
	menu_Navigate_(TempSens_Select);
}

//------------------------------------------------------------------------------
//				Temperature sensor settings
//------------------------------------------------------------------------------

void menu_I_TempSensSettings_Title(void)
{
	usr_Disp_Str_INT16U(note_temp_sens, usr_Get_Var(uint8_t_a)+1);
}

void menu_I_TempSensSettings_On(void)
{
	uint8_t use_sens;

	if (CHKB(gTmp.state_vector, usr_Get_Var(uint8_t_a)))
		use_sens = 1;
	else
		use_sens = 0;

	mopr_Disp_CheckmarkStr(note_use_sens, use_sens);

	if (USR_IS_ENTER) {
		tmp_Set_Use(usr_Get_Var(uint8_t_a), !use_sens);
		USR_REFRESH_DISP();
	}

	menu_Navigate();
}

void menu_I_TempSensSettings_StatisticOn(void)
{
	uint8_t use_statistic;

	if (CHKB(gTmp.state_vector, usr_Get_Var(uint8_t_a)))
		use_statistic = 1;
	else
		use_statistic = 0;

	mopr_Disp_CheckmarkStr(note_use_statistic, use_statistic);

	if (USR_IS_ENTER) {
		//tmp_Set_StatisticUsage(usr_Get_Var(uint8_t_a), !use_statistic);
		USR_REFRESH_DISP();
	}

	menu_Navigate();
}

//------------------------------------------------------------------------------
//						НАСТР ДАТЧИКОВ ТЕМПЕРАТУРЫ
//------------------------------------------------------------------------------

/*
void menu_I_TempSens_SensOn(void)
{
	uint8_t use_sens;
	if (CHKB(gTmp.state_vector, usr_Get_Var(uint8_t_a))) use_sens = 1;
    else use_sens = 0;    

	if (use_sens)
	{
		dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_36, DSP_SYM_CHECKMARK, note_use_sens, usr_Get_Var(uint8_t_a)+1);
	}
	else usr_Disp_Str_INT16U(note_use_sens, usr_Get_Var(uint8_t_a)+1);

	if (USR_IS_ENTER)
	{
		tmp_Set_Use(usr_Get_Var(uint8_t_a), !use_sens);
		USR_REFRESH_DISP();
	}
	mopr_ListUpDown_Num(TMP_NUM-1);
	menu_Navigate();
}
*/

//------------------------------------------------------------------------------
//программы
//------------------------------------------------------------------------------

void menu_I_Progs_Title(void)
{
	usr_Disp_Str(note_progs);
}

void menu_I_Progs_Clc(void)
{
	usr_Disp_Str(note_calc);
	menu_Navigate_(Progs_Clc);
}

void menu_I_Progs_Ctmp(void)
{
	usr_Disp_Str(note_conv_temp);
	menu_Navigate_(Progs_Ctmp);
}

void menu_I_Progs_DayInf(void)
{
	usr_Disp_Str(note_day_inf);
	menu_Navigate_(Progs_DayInf);
}

//------------------------------------------------------------------------------
//информация о дне
//------------------------------------------------------------------------------

typedef struct
{
	date_t date;
	fl32_t lunar_day;
	ubase_t lunar_phase;
	day_t day;
} DAYINF_;

DAYINF_ gDayinf;

void menu_I_DayInf_Ini(void)
//---инициализация проги информации о дне
{
	gDayinf.date.date = 0x01;
	gDayinf.date.month = 0x11;
	gDayinf.date.year = 0x10;
	gDayinf.date.ctry = 0x20;
	gDayinf.lunar_day = 24.3;
	gDayinf.lunar_phase = 8;
	gDayinf.day = DAY_MON;
}

void menu_I_DayInf_Inf(void)
{
	dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_38, note_LD, gDayinf.lunar_day, note_Ph, gDayinf.lunar_phase, mopr_DayStr(gDayinf.day));
}

void menu_I_DayInf_Date(void)
{
	if (USR_IS_INPUT) usr_Disp_Input_Date();
	else dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_D, &gDayinf.date);
	
	if (usr_Input_Date(&gDayinf.date))
	{
		fl32_t lunar_distance;
		gDayinf.lunar_day = tdf_Lunar_FlDay_and_Distance(&gDayinf.date, &lunar_distance);
		gDayinf.lunar_phase = tdf_Lunar_Phase(gDayinf.lunar_day);
		gDayinf.day = tdf_Date_to_Day(&gDayinf.date);
	}
	else menu_Navigate();
}


//------------------------------------------------------------------------------
//						ВЫБОР БУДИЛЬНИКА ДЛЯ ОТКЛЮЧЕНИЯ
//------------------------------------------------------------------------------

void menu_I_ActRing_Title(void)
{
	uint8_t line[DSP_VIR_STR_LENTH + 1];
	uint8_t sym = 0;
	for (uint8_t i = 0; i < 2; i++)
	{
		if (usr_Get_Var(uint8_t_arr[i]) & 0x80)
		{
			line[sym++] = 'A';
			//line[sym++] = (usr_Get_Var(uint8_t_arr[i]) & 0x7F) + 0x31;
			line[sym++] = ((usr_Get_Var(uint8_t_arr[i]) & 0x7F)/10) | 0x30;
			line[sym++] = ((usr_Get_Var(uint8_t_arr[i]) & 0x7F)%10) + 0x31;
			line[sym++] = '-';
			line[sym++] = i + 0x31;
		}
		else
		{
			line[sym++] = 'H';
			line[sym++] = ((usr_Get_Var(uint8_t_arr[i]))/10) | 0x30;
			line[sym++] = ((usr_Get_Var(uint8_t_arr[i]))%10) + 0x31;
			line[sym++] = '-';
			line[sym++] = i + 0x31;
		}
		line[sym++] = DSP_SYM_SPACE;
	}
	line[sym] = 0;
	dsp_Print(usr_CurStrNum(), usr_CurStrType, note_cmd_8, line);
}

void menu_I_ActRing_Enter(void)
{	
	enum {SYMNUM = 2};
	if (USR_IS_INPUT) usr_Disp_Input(SYMNUM);
	else
	{
		if (usr_Get_Var(uint8_t_arr[2])) usr_Key = USR_RETURNKEY;
		else
		{
			usr_Set_Var(uint8_t_arr[2], 1);
			usr_Key = USR_ENTERKEY_1;
		}
	}
	
	uint32_t rnum;
	if (usr_Input_INT32U(&rnum, 1, 2, SYMNUM))
	{
		rnum--;
		if (usr_Get_Var(uint8_t_arr[rnum]) & 0x80)
		{
			alm_Reset(usr_Get_Var(uint8_t_arr[rnum]) & 0x7F);
		}
		else
		{
			rmd_Reset(usr_Get_Var(uint8_t_arr[rnum]));
		}
		menu_act_ring_asked = 0;
		usr_Exit();
	}
	
	if (USR_IS_KEY(USR_RETURNKEY))
	{
		menu_act_ring_asked = 0;
	}
	menu_Navigate();
}
