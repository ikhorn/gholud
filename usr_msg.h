/* Подмодуль msg модуля usr
 * обновлен.....................................................................04.12.2010 22:18:36
*/

#include "stm.h"
#include "usr.h"

//------------------------------------------------------------------------------
//остальное
//------------------------------------------------------------------------------

#define MSG_TIMER_SET()
#define menu_Filtr_Proc()					if (gUsr.sp->act_strnum != gUsr.strnum || !usr_Key) return;

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

extern const USRC_STR_ note_wrong[];
extern const USRC_STR_ note_input[];
extern const USRC_STR_ note_more_eq[];
extern const USRC_STR_ note_less_eq[];
extern const USRC_STR_ note_value[];

extern gusr_t gUsr;
extern flash_ const usr_rstack_t* gUsrRstackEnd;

#if USRC_USE_SUCH_GHOTKEY_1														//обработчики глобальных горячих клавиш
extern void usr_Handle_HotKey_1(void);
#endif

#if USRC_USE_SUCH_GHOTKEY_2
extern void usr_Handle_HotKey_2(void);
#endif

#if USRC_USE_SUCH_GHOTKEY_3
extern void usr_Handle_HotKey_3(void);
#endif

#if USRC_USE_SUCH_GHOTKEY_4
extern void usr_Handle_HotKey_4(void);
#endif

#if USRC_USE_SUCH_GHOTKEY_5
extern void usr_Handle_HotKey_5(void);
#endif

#if USRC_USE_ADD_MSG_SIGNAL == 1
extern void msg_Signal(void);
#endif

void msg_Exit(void);															//---выходит с сообщения

//------------------------------------------------------------------------------
//карта стандартных сообщений
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//							НЕПРАВИЛЬНЫЙ ВВОД
//------------------------------------------------------------------------------
void msg_I_Title_Wrong(void);
void msg_I_Input(void);
funcp_pr_t listWrongInput[] =
{
	msg_I_Title_Wrong,
	msg_I_Input
};
usr_wnd_t msgW_WrongInput = {listWrongInput, list_size(listWrongInput)};


void msg_I_Title_Wrong(void)
{
	usr_Disp_Str(note_wrong);
}

void msg_I_Input(void)
{
	usr_Disp_Str(note_input);
	msg_Scan_Key();
}
//------------------------------------------------------------------------------
//							ЗНАЧЕНИЕ >= INT VALUE
//------------------------------------------------------------------------------
void msg_I_TitleValue(void);
void msg_I_More_INT32U(void);
funcp_pr_t listMoreINT32U[] =
{
	msg_I_TitleValue,
	msg_I_More_INT32U,
};
usr_wnd_t msgW_More_INT32U = {listMoreINT32U, list_size(listMoreINT32U)};


void msg_I_TitleValue(void)
{
	usr_Disp_Str(note_value);
}

void msg_I_More_INT32U(void)
{
	usr_Disp_Str_INT32U(note_more_eq, usr_Get_ptrVar(uint32));
	msg_Scan_Key();
}
//------------------------------------------------------------------------------
//							ЗНАЧЕНИЕ <= INT VALUE
//------------------------------------------------------------------------------
void msg_I_Less_INT32U(void);
funcp_pr_t listLessINT32U[] =
{
	msg_I_TitleValue,
	msg_I_Less_INT32U,
};
usr_wnd_t msgW_Less_INT32U = {listLessINT32U, list_size(listLessINT32U)};


void msg_I_Less_INT32U(void)
{
	usr_Disp_Str_INT32U(note_less_eq, usr_Get_ptrVar(uint32));
	msg_Scan_Key();
}

//------------------------------------------------------------------------------
//							ЗНАЧЕНИЕ >= SIGNED INT VALUE
//------------------------------------------------------------------------------
void msg_I_More_INT32S(void);
funcp_pr_t listMoreINT32S[] =
{
	msg_I_TitleValue,
	msg_I_More_INT32S,
};
usr_wnd_t msgW_More_INT32S = {listMoreINT32S, list_size(listMoreINT32S)};


void msg_I_More_INT32S(void)
{
	usr_Disp_Str_INT32S(note_more_eq, usr_Get_ptrVar(int32));
	msg_Scan_Key();
}
//------------------------------------------------------------------------------
//							ЗНАЧЕНИЕ <= SIGNED INT VALUE
//------------------------------------------------------------------------------
void msg_I_Less_INT32S(void);
funcp_pr_t listLessINT32S[] =
{
	msg_I_TitleValue,
	msg_I_Less_INT32S,
};
usr_wnd_t msgW_Less_INT32S = {listLessINT32S, list_size(listLessINT32S)};


void msg_I_Less_INT32S(void)
{
	usr_Disp_Str_INT32S(note_less_eq, usr_Get_ptrVar(int32));
	msg_Scan_Key();
}

//------------------------------------------------------------------------------
//							ЗНАЧЕНИЕ >= FL VALUE
//------------------------------------------------------------------------------
void msg_I_More_FL32(void);
funcp_pr_t listMoreFL32[] =
{
	msg_I_TitleValue,
	msg_I_More_FL32,
};
usr_wnd_t msgW_More_FL32 = {listMoreFL32, list_size(listMoreFL32)};


void msg_I_More_FL32(void)
{
	usr_Disp_Str_FL32(note_more_eq, usr_Get_ptrVar(fl32));
	msg_Scan_Key();
}
//------------------------------------------------------------------------------
//							ЗНАЧЕНИЕ <= FL VALUE
//------------------------------------------------------------------------------
void msg_I_Less_FL32(void);
funcp_pr_t listLessFL32[] =
{
	msg_I_TitleValue,
	msg_I_Less_FL32,
};
usr_wnd_t msgW_Less_FL32 = {listLessFL32, list_size(listLessFL32)};


void msg_I_Less_FL32(void)
{
	usr_Disp_Str_FL32(note_less_eq, usr_Get_ptrVar(fl32));
	msg_Scan_Key();
}
//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

bool msg_Post(const usr_wnd_t *window, ubase_t type, usr_temp_t* var)
//---выводит сообщение
{
	if (!gUsr.msg_ebl) return false;
	if (gUsr.sp >= gUsrRstackEnd) return false;
		
#if USRC_USE_ADD_MSG_SIGNAL == 1
		
	msg_Signal();
		
#endif

#if USRC_USE_ASK_MSG == 1

	if (type == MSG_ASK_TYPE || type == MSG_ASKTIME_TYPE)						//сообщение должно получить ответ
	{
		if (gUsr.ask_msg) return false;											//сообщение с получением ответа уже выведено
		gUsr.ask_msg = true;
		gUsr.ans_msg = false;
	}

#endif

	gUsr.sp++;																	//создать сообщение
	gUsr.sp->window = window;
	gUsr.sp->pnum = 1;
	gUsr.sp->act_strnum = 1;
	gUsr.sp->wtype = type;
	gUsr.sp->temp = *var;
	
	usr_Open_Window();

	STM_SET(gUsr.t_msg, USRC_MSG_TIME);											//навести таймер
	dsp_Off_Cursor();
	return true;
}

#if USRC_USE_ASK_MSG == 1
bool msg_Pend(bool* ans)
//---принимает ответ от сообщения
{
	if (!gUsr.ans_msg) return false;
	gUsr.ans_msg = false;
	gUsr.ask_msg = false;
	*ans = gUsr.answer;
	return true;
}
#endif


void msg_Scan_Key(void)
//---навигация по сообщению с учетом типа сообщения
{
	menu_Filtr_Proc();
	switch (usr_Key)
	{
		case USRC_UPKEY:
		{
			usr_ScrollUp_Window();
			break;
		}
		case USRC_DOWNKEY: 
		{
			usr_ScrollDown_Window();
			break;
		}
#if USRC_USE_ENTERKEY_2 == 1
		case USRC_ENTERKEY_2:
		{
			msg_Exit();
			break;
		}
#endif		
		case USRC_ENTERKEY_1:
		case USRC_RETURNKEY:
		{
			
#if USRC_USE_ASK_MSG == 1
			
			if (gUsr.sp->add == MSG_ASK_TYPE || gUsr.sp->add == MSG_ASKTIME_TYPE)//сообщение должно отправить ответ
			{
				gUsr.ans_msg = true;
				if (usr_Key == USRC_ENTERKEY_1) gUsr.answer = true;
				else gUsr.answer = false;
			}
#endif
			msg_Exit();
			break;
		}
	#if USRC_USE_SUCH_GHOTKEY_1													//глобальные горячие клавиши
		case USRC_USE_SUCH_GHOTKEY_1:
		{
			usr_Handle_HotKey_1();
			break;
		}
	#endif
	#if USRC_USE_SUCH_GHOTKEY_2
		case USRC_USE_SUCH_GHOTKEY_2:
		{
			usr_Handle_HotKey_2();
			break;
		}
	#endif
	#if USRC_USE_SUCH_GHOTKEY_3
		case USRC_USE_SUCH_GHOTKEY_3:
		{
			usr_Handle_HotKey_3();
			break;
		}
	#endif
	#if USRC_USE_SUCH_GHOTKEY_4
		case USRC_USE_SUCH_GHOTKEY_4:
		{
			usr_Handle_HotKey_4();
			break;
		}
	#endif
	#if USRC_USE_SUCH_GHOTKEY_5
		case USRC_USE_SUCH_GHOTKEY_5:
		{
			usr_Handle_HotKey_5();
			break;
		}
	#endif
		default: break;
	}
	usr_Key = 0;
}


void msg_Proc_MsgTimer(void)
//---обрабатывает срабатывание тайера сообщения
{
#if USRC_USE_ASK_MSG == 1
	if (gUsr.sp->wtype != MSG_ASKTIME_TYPE && gUsr.sp->wtype != MSG_TIME_TYPE) return;
#elif USRC_USE_ASK_MSG == 0
	if (gUsr.sp->wtype != MSG_TIME_TYPE) return;								//временное сообщение
#endif
	msg_Exit();
}

//------------------------------------------------------------------------------
//внутренние функции
//------------------------------------------------------------------------------

void msg_Exit(void)
//---выходит с сообщения
{
	STM_SET(gUsr.t_msg, USRC_MSG_TIME);											//перенавести таймер выхода
	--gUsr.sp;
	usr_Open_Window();
}

