/* Макро предопределения модуля usr
 * обновлен.....................................................................29.12.2010 12:06:51
*/

#include "../stm.h"
#include "../tdf.h"
#include "usrc.h"

//------------------------------------------------------------------------------
//								USR
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//типы
//------------------------------------------------------------------------------

#ifndef USR_LIST_TYPE
#define USR_LIST_TYPE
#define list_size(X)						(sizeof(X)/sizeof(func_t*))			//размер списка обработчиков
typedef const funcp_t usrcw_ funcp_pr_t;
typedef funcp_pr_t* usr_list_t;
#endif

#ifndef USR_WND_TYPE
#define USR_WND_TYPE
typedef const struct																			//тип окно
{
	usr_list_t item0;
	ubase_t size0;
} usrcw_ usr_wnd_t;
#endif

#ifndef USR_TEMP_TYPE
#define USR_TEMP_TYPE


typedef
union
{
	fl32_t fl32;
	uint32_t uint32;
	int32_t int32;
	uint8_pr_t* uint8_pr;
	uint8_ee_t* uint8_ee;
	usr_wnd_t* pwnd;
	time_t time;
	date_t date;
	uint8_t uint8_t_arr[4];
	struct
	{
		wst_t wst_t_a;
		wst_t wst_t_b;
		wst_t wst_t_c;
		wst_t wst_t_d;
	};
	struct
	{
		funcp_t func_t_a;
		funcp_t func_t_b;
	};
	struct
	{
		uint16_t uint16_t_a;
		uint16_t uint16_t_b;
	};
	struct
	{
		int16_t int16_t_a;
		int16_t int16_t_b;
	};
	struct
	{
		uint8_t uint8_t_a;
		uint8_t uint8_t_b;
		uint8_t uint8_t_c;
		uint8_t uint8_t_d;
	};
	struct
	{
		int8_t int8_t_a;
		int8_t int8_t_b;
		int8_t int8_t_c;
		int8_t int8_t_d;
	};
	struct
	{
		uint32_t fl_1:1;
		uint32_t fl_2:1;
		uint32_t fl_3:1;
		uint32_t fl_4:1;
		uint32_t fl_5:1;
		uint32_t fl_6:1;
		uint32_t fl_7:1;
		uint32_t fl_8:1;
		uint32_t fl_9:1;
		uint32_t fl_10:1;
		uint32_t fl_11:1;
		uint32_t fl_12:1;
		uint32_t fl_13:1;
		uint32_t fl_14:1;
		uint32_t fl_15:1;
		uint32_t fl_16:1;
		uint32_t fl_17:1;
		uint32_t fl_18:1;
		uint32_t fl_19:1;
		uint32_t fl_20:1;
		uint32_t fl_21:1;
		uint32_t fl_22:1;
		uint32_t fl_23:1;
		uint32_t fl_24:1;
		uint32_t fl_25:1;
		uint32_t fl_26:1;
		uint32_t fl_27:1;
		uint32_t fl_28:1;
		uint32_t fl_29:1;
		uint32_t fl_30:1;
		uint32_t fl_31:1;
		uint32_t fl_32:1;
	};
} usr_temp_t;
#endif

#ifndef USR_RSTACK_TYPE
#define USR_RSTACK_TYPE
typedef
struct
{
	const usr_wnd_t* window;													//окно
	ubase_t pnum;																//номер пункта окна
	ubase_t act_strnum;															//номер активной строки
	uint8_t wtype;																//служебная переменная указывающая тип окна
	usr_temp_t temp;															//переменная для передачи параметров между окнами
} usr_rstack_t;
#endif

#ifndef USR_TYPE
#define USR_TYPE
typedef
struct
{
	uint8_t key;																//считанная клавиша
	
	//--дополнительная обработка--
	func_t* exe_handler;														//обработчик процесса
	usr_temp_t exe_temp;														//переменные обработчика процесса
	
	//--стек окон--
	usr_rstack_t rstack[USRC_RETURN_STACK_SIZE];									//стек окон
	usr_rstack_t* sp;															//указатель стека окон
	
	//--вывод--
	ubase_t strnum;																//номер строки, что выводится
	ubase_t strtype;																//тип строки
	func_t* str[USRC_STRNUM];													//текущие обработчики пунктов
	
	//--ввод--
	uint8_t inp[USRC_INPUT_RAM_STR_SIZE+1];										//массив ввода
	ubase_t inp_pos;																//позиция в вводном масссиве
	ubase_t click_num;															//количество кликов во время ввода символа
	uint8_t layout;																//раскладка клавиатуры
	uint8_t prev_key;															//предидущая клавиша

	//--служебные флаги--
	ubase_t refresh:	1;															//обновление
	ubase_t write:	1;															//происходит ввод
	ubase_t execute:	1;															//происходит процесс
	ubase_t msg_ebl:	1;															//запрещает вывод сообщений
	
#if USRC_USE_ASK_MSG == 1
	
	ubase_t ask_msg:	1;															//сообщения типа ASK выведено
	ubase_t ans_msg:	1;															//есть ответ на сообщение типа ASK
	ubase_t answer:	1;															//ответ на сообщение типа ASK
	
#endif
	
	//--таймера--
	ubase_t t_msg;																//таймер сообщения
	ubase_t t_sym_input;															//таймер ввода символа при вводе текста
} gusr_t;
#endif

extern void stp_Handler(void);													//обработчик статус панели
//extern void usr_Execute_Default(void);
void usr_Execute_Default(void);													//---выполнение процесса по умолчанию

//#if (USRC_STRNUM > ___USRC_2_STR)												//обработчик строки по умолчанию - тоесть когда строк в подменю меньше чем на индикаторе
extern void menu_I_StrByDefault(void);
//#endif

//------------------------------------------------------------------------------
//типы окон
//------------------------------------------------------------------------------

#define USR_WND_WRT_							1								//окно рабочий стол
#define USR_WND_WRS_							2								//окно рабочее пространство
#define USR_WND_MENU_							3								//окно меню
#define USR_WND_MSG_KEY_						4								//окно сообщение типа KEY
#define USR_WND_MSG_TIME_						5								//окно сообщение типа TIME
#define USR_WND_MSG_BLINK_						6								//окно сообщение типа BLINK
#define USR_WND_MSG_SOUND_						7								//окно сообщение типа SOUND
#define USR_WND_MSG_ASK_						8								//окно сообщение типа ASK
#define USR_WND_MSG_ASKTIME_					9								//окно сообщение типа TIMEASK

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

void menu_NavigateEnterMenu(const usr_wnd_t* enter_window, ubase_t enter_pnum);	//---навигация по меню
																				//---включает скрытое выполнение
#define usr_Set_Executing_(EXE)													\
{																				\
	gUsr.execute = true;														\
	gUsr.exe_handler = EXE;														\
}

#define usr_Get_Execute_()						gUsr.exe_handler

																				//---отключает скрытое выполнение
#define usr_Off_Executing_()													\
{																				\
	gUsr.execute = false;														\
	gUsr.exe_handler = usr_Execute_Default;										\
}


#define usr_Check_Executing()			gUsr.execute							//---проверяет статус выполения
#define USR_IS_EXECUTING_				usr_Check_Executing()

#if USRC_DISP_REFRESH_FRQ == ___USRC_ONE_SYSTICK
																				//---проверяет таймера,обновляет экран if(SYSTIMER_IS_2TICK)
#define USR_CHECK_TIMERS_()														\
{																				\
	USR_REFRESH_DISP();															\
	STM_BEGIN_ACT(gUsr.t_sym_input)												\
		usr_Proc_SymInputTimer();												\
	STM_END_ACT																	\
																				\
	STM_BEGIN_ACT(gUsr.t_msg)													\
		msg_Proc_MsgTimer();													\
	STM_END_ACT																	\
}
#elif USRC_DISP_REFRESH_FRQ == ___USRC_TWO_SYSTICK
#define USR_CHECK_TIMERS_()														\
{																				\
	if (STM_IS_2TICK)                                                           \
    {                                                                           \
        USR_REFRESH_DISP();														\
    }                                                                           \
	STM_BEGIN_ACT(gUsr.t_sym_input)												\
		usr_Proc_SymInputTimer();												\
	STM_END_ACT																	\
																				\
	STM_BEGIN_ACT(gUsr.t_msg)													\
		msg_Proc_MsgTimer();													\
	STM_END_ACT																	\
}
#endif
																				//---передает параметр входному окну
#define usr_Set_InVar_(TYPE, VAR)												\
{																				\
	gUsr.sp++;																	\
	gUsr.sp->temp.TYPE = (VAR);													\
	gUsr.sp--;																	\
}

																				//---передает параметр предидущему окну
#define usr_Set_OutVar_(TYPE, VAR)												\
{																				\
	gUsr.sp--;																	\
	gUsr.sp->temp.TYPE = (VAR);													\
	gUsr.sp++;																	\
}


#define usr_Set_Var_(TYPE, VAR)			gUsr.sp->temp.TYPE = (VAR)				//---передает параметр текущему окну
#define usr_Set_ExeVar_(TYPE, VAR)		gUsr.exe_temp.TYPE = (VAR)				//---передает параметр обработчику процесса

#define usr_Get_Var_(TYPE)				gUsr.sp->temp.TYPE						//---считывает параметр окна
#define usr_Get_ptrVar_(TYPE)			&gUsr.sp->temp.TYPE						//---считывает указатель на параметр окна

#define usr_Get_ExeVar_(TYPE)			gUsr.exe_temp.TYPE						//---считывает параметр обработчика процесса
#define usr_Get_ptrExeVar_(TYPE)		&gUsr.exe_temp.TYPE						//---считывает укзатель на параметр обработчика процесса

#define usr_Key_						gUsr.key								//---считывает нажатую клавишу
#define usr_CurStrNum_()				gUsr.strnum								//---текущая выводимая строка
#define usr_CurStrType_					gUsr.strtype							//---текущий тип строки
#define usr_CurWndSize_()				gUsr.sp->window->size					//---текущий размер окна
#define USR_REFRESH_DISP_()				gUsr.refresh = true;					//---обновляет содержимое экрана
#define usr_WndType_()					gUsr.sp->wtype							//---читает тип текущего окна

#define USR_IS_INPUT_					(gUsr.write && USR_ITEM_ACTIVE_)		//---выдает состояние ввода для текущего пункта
#define USR_ITEM_ACTIVE_				(gUsr.strnum == gUsr.sp->act_strnum)	//---выдает состояние готовности реагировать на ввод
#define USR_IS_KEY_(KEY)				(USR_ITEM_ACTIVE && usr_Key == (KEY))	//---проверяет нажата ли клавиша для текущей строки

//------------------------------------------------------------------------------
//								МЕНЮ
//------------------------------------------------------------------------------

void usr_Proc_SymInputTimer(void);												//---обрабатывает таймер смены символа при вводе текста
#define menu_Navigate__(ITEM)			menu_NavigateEnterMenu(&(ENTERMENU_##ITEM), ENTERITEM_##ITEM)//---навигация по меню
																				//---осуществляется вход в пункте
#if USRC_USE_ENTERKEY_2 == 1
#define USR_IS_ENTER_		(USR_ITEM_ACTIVE && (usr_Key == USRC_ENTERKEY_1 || usr_Key == USRC_ENTERKEY_2))
#elif USRC_USE_ENTERKEY_2 == 0
#define USR_IS_ENTER_		(USR_ITEM_ACTIVE && usr_Key == USRC_ENTERKEY_1)
#endif



//------------------------------------------------------------------------------
//								СООБЩЕНИЯ
//------------------------------------------------------------------------------

void msg_Proc_MsgTimer(void);													//---обрабатывает срабатывание тайера сообщения

#define msg_Check_PostReady_()				!gUsr.ask_msg						//---проверяет готовность к передаче сообщения типа ASK
#define msg_Check_PendReady_()				gUsr.ans_msg						//---проверяет готовность к приему сообщения типа ASK
#define msg_On_MsgSend_()					gUsr.msg_ebl = false;				//---разрешает вывод сообщений
#define msg_Off_MsgSend_()					gUsr.msg_ebl = true;				//---запрещает вывод сообщений



























//------------------------------------------------------------------------------
//								
//------------------------------------------------------------------------------

#if USR_USE_TEXT_EDITOR == 1 && USR_TEXT_BUF_TYPE == ___USR_LARGE_RAM
	#if USR_INPUT_RAM_STR_SIZE <= USR_VISIBLE_STR_LENGTH
	#error USR MODULE: USR_INPUT_RAM_STR_SIZE should be more than USR_VISIBLE_STR_LENGTH when using buf type __USR_LARGE_RAM
	#endif
#elif USR_USE_TEXT_EDITOR == 1 && (USR_TEXT_BUF_TYPE == ___USR_SMALL_RAM || USR_TEXT_BUF_TYPE == ___USRC_EXT)
	#if USR_INPUT_RAM_STR_SIZE > USR_VISIBLE_STR_LENGTH
	#error USR MODULE: USR_INPUT_RAM_STR_SIZE should be <= than USR_VISIBLE_STR_LENGTH when using buf type __USR_SMALL_RAM or ___USRC_EXT
	#endif
#endif

