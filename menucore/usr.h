/* Модуль пользовательского интерфейса
 * RAM..........................................................................150 + 
 * обновлен.....................................................................06.03.2011 22:24:00
*/

//------------------------------------------------------------------------------
//описание модуля
//------------------------------------------------------------------------------
// **************************** КОНЦЕПЦИЯ МОДУЛЯ *******************************
// Концепция модуля разбивает пользовательский (интерфейс человек-машина или
// ЧМИ) интерфейс на 4 основных части: 1) Рабочий стол 'wrt' 2) Меню 'menu' 3)
// Сообщения 'msg' 4) Статус панель 'stp'.
// Они создаются путем стандартых функций описанных в этом заглавочном файле.
// Модуль также содержит в себе ряд стандартных сообщений, ряд стандартных
// программ на основе модуля.
//
// *************************** СОЗДАНИЕ МЕНЮ ***********************************
// Основная задача при создании новой ЧМИ заключается в создании рабочего стола,
// меню, статус панели, сообщений и создании используемых строк. Строки,
// поскольку они константные, создаются с помощью специальных макро функций,
// пример создания строк есть в файлах note.asm и note.h в папочке example.
// Также в папочке example есть примеры создания и остальных основных частей
// ЧМИ. Для удобства желательно разбить файлы меню, так как показано в примере,
// на opr_menu, map_menu, ctr_menu. Каждый из этих файлов может выолнять свою
// функцию:
// ~ map_menu - сдесь объявляються окна меню, пункты меню, входные меню пунктов
//   и входные пункты входных меню пунктов. Это как бы карта меню, ее лучше
//   всего создавать с помощью шаблонов.
// ~ ctr_menu - сдесь размещены обработчики пунктов меню объявленных в файле
//   map_menu.
// ~ opr_menu - сдесь размещены действия, которые должны делать обработчики
//   пунктов меню (размещенные в ctr_menu) при вводе или при выполнении
//   указанного какого-то собственного действия.
// Вход в модуль осуществляется с рабочего стола wrtW_Ini, который нужно
// обязательно обявить. Вход в меню начинается с окна menW_Enter, которое нужно
// обязательно обявить.
//
// **************************** ДЕЙСТВИЯ ***************************************
// Когда при выполнении какого-то действия, при наступлении события ввода или
// чего-то еще, требуется много времени, то алгоритм выполнения этого действия
// разбиваеться "методом многократного цыклирования" и помещаеться в
// своеназванную функцию, адрес которой передается функции usr_Set_Executing,
// которая и выполняет это действие! Главное при этом, это то, что текущий пункт
// меню так и остается текущим, и желательно предусмотреть какое-то отображение
// во время выполнения данного действия, например "Подождите пожалуйста", статус
// так называемого 'выполения' можно узнать с помощью usr_Check_Executing. Для
// обмена параметрами с 'exe процессом' (созданным с помощю usr_Set_Executing)
// используються функции usr_Set_ExeVar, usr_Get_prtExeVar, usr_Get_ExeVar.
//
// *************************** ПЕРЕДАЧА ПАРАМЕТРОВ *****************************
// Для передачи промежуточных параметров между подменюшками используеться
// специальная структура типа usr_temp_t и функции usr_Set_Var, usr_Set_InVar,
// usr_Set_OutVar, usr_Get_Var, usr_Get_ptrVar.
// Функции выполняют обмен параметрами типа usr_temp_t между предыдущим, текущим
// и следующим подменю.
//
// ******************************** НАСТРОЙКА **********************************
// Еще одним важным моментом являеться синхронизация модуля с системным
// таймером. Для этого используется функция usr_Check_Timers ее просто нужно
// вставить в обработчик системного тика.
//
// ******************************** СООБЩЕНИЯ **********************************
// Можно отправлять несколько типов сообщений. Все они задаються параметром TYPE
// в функции отправки. На сообщения типа MSG_ASK_TYPE или MSG_ASKTIME_TYPE можно
// получать ответ типа bool с помощью функции msg_Pend. Также для использования
// сообщения типа ASK нужно включить в файле конфигурации константу
// USRC_USE_ASK_MSG.
//

#include "usrm.h"
#include "../pdef.h"

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
		func_t* func_t_a;
		func_t* func_t_b;
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

//------------------------------------------------------------------------------
//						ПОЛЬЗОВАТЕЛСКИЙ ИНТЕРФЕЙС
//------------------------------------------------------------------------------

extern gusr_t gUsr;
void usr_Ini(void);																//---инициализация модуля пользовательского интерфейса
void usr(void);																	//---обновляет содержимое экрана

/*отдельный процесс*/
#define usr_Set_Executing(EXE_HANDLER)			usr_Set_Executing_(EXE_HANDLER)	//---включает скрытое выполнение
#define usr_Off_Executing()						usr_Off_Executing_()			//---отключает скрытое выполнение
#define USR_IS_EXECUTING						USR_IS_EXECUTING_				//---проверяет статус выполнения процесса
#define usr_Get_Execute()						usr_Get_Execute_()				//---читает ссылку обработчик

/*обмен данными*/
#define usr_Set_Var(TYPE, VAR)					usr_Set_Var_(TYPE, VAR)			//---передает параметр текущему окну
#define usr_Set_InVar(TYPE, VAR)				usr_Set_InVar_(TYPE, VAR)		//---передает параметр входному окну
#define usr_Set_OutVar(VAR)						usr_Set_OutVar_(VAR)			//---передает параметр предидущему окну
#define usr_Set_ExeVar(TYPE, VAR)				usr_Set_ExeVar_(TYPE, VAR)		//---передает параметр обработчику процесса

#define usr_Get_Var(TYPE)						usr_Get_Var_(TYPE)				//---считывает параметр окна
#define usr_Get_ptrVar(TYPE)					usr_Get_ptrVar_(TYPE)			//---считывает указатель на параметр окна
#define usr_Get_ExeVar(TYPE)					usr_Get_ExeVar_(TYPE)			//---считывает параметр обработчика процесса
#define usr_Get_ptrExeVar(TYPE)					usr_Get_ptrExeVar_(TYPE)		//---считывает укзатель на параметр обработчика процесса

/*навигация*/
void usr_Goto(const usr_wnd_t* enter_window, ubase_t enter_pnum, ubase_t wnd_type);//---переходит на окно
void usr_Replace(const usr_wnd_t* enter_window, ubase_t enter_pnum, ubase_t wnd_type);	//---заменяет текущее окно
void usr_Exit(void);															//---возвращает на предидущее окно
void usr_Exit_to_Wrt(void);														//---возвращает на рабочий стол

/*отображение строк*/
void usr_Disp_WaiteStr(void);													//---отображает строку ожидания ...
void usr_Disp_PorgressStr(uint16_t maxv, uint16_t curv);							//---отображает строку прогресса
void usr_Disp_Input(ubase_t symnum);												//---вводит строку ввода
void usr_Disp_Input_Time(void);													//---отображает введеное время
void usr_Disp_Input_Date(void);													//---отображает введеную дату
void usr_Disp_Input_Psw(ubase_t symnum);											//---отображает ввод пароля
void usr_Disp_Str(const USRC_STR_ *str);										//---выводит строку
void usr_Disp_2Str(const USRC_STR_ *str1, const USRC_STR_ *str2);				//---выводит сдвоенную строку
void usr_Disp_Str_INT16U(const USRC_STR_ *str, uint16_t val);					//---выводит строку с номером в конце
void usr_Disp_Str_INT32U(const USRC_STR_ *str, uint32_t* val);					//---выводит строку и дополнительно целое 32-х битное число без знака
void usr_Disp_Str_INT32S(const USRC_STR_ *str, int32_t* val);					//---выводит строку и дополнительно целое 32-х битное число со знаком
void usr_Disp_Str_FL32(const USRC_STR_ *str, fl32_t* val);						//---выводит строку и дополнительно число float
void usr_Disp_2Str_INT16U(const USRC_STR_ *str1, const USRC_STR_ *str2, uint16_t val);//---выводит сдвоенную с номером в конце
void usr_Disp_StrNum_Str(const USRC_STR_ *str1, uint16_t val, const USRC_STR_ *str2);//---выводит троку с номером + строка

/*ввод*/
void usr_InputAdd_Sym(ubase_t key, uint8_t* str, ubase_t symnum);					//---вводит символ на текущую позицию ввода, выбирая его из заданной строки
bool usr_Input_PreStr(uint8_t* str);												//---вставляет строку введенной при старте ввода, возвращает сигнал о начале ввода
bool usr_Input_INT32U(uint32_t* param, uint32_t min, uint32_t max, ubase_t symnum);	//---вводит параметр целого типа uint32_t
bool usr_Input_INT32S(int32_t* param, int32_t min, int32_t max, ubase_t symnum);	//---вводит параметр целого типа int32_t
bool usr_Input_FL64(fl64_t* param, fl64_t min, fl64_t max, ubase_t symnum);			//---вводит параметр плавающего типа
bool usr_Input_Time(time_t* time);												//---вводит параметр время
bool usr_Input_Date(date_t* date);												//---вводит параметр дата
bool usr_Input_String(ubase_t symnum);											//---вводит стоку
ubase_t usr_Read_InputBufferedStr(ubase_t adr, ubase_t dsize, uint8_t* data);		//---считывает данные из введенного буфера - возвращает количество считанных данных

/*остальное*/
#define usr_Key									usr_Key_						//---нажатая клавишу
#define USR_REFRESH_DISP()						USR_REFRESH_DISP_()				//---обновляет содержимое экрана
#define usr_CurStrNum()							usr_CurStrNum_()				//---текущая выводимая строка
#define usr_CurStrType							usr_CurStrType_					//---текущий тип строки
#define usr_CurWndSize()						usr_CurWndSize_()				//---текущий размер окна


#define USR_ITEM_ACTIVE							USR_ITEM_ACTIVE_				//---пунке выделен
#define USR_IS_INPUT							USR_IS_INPUT_					//---происходит ввод для текущего пункта
#define USR_IS_KEY(KEY)							USR_IS_KEY_(KEY)				//---проверяет нажата ли клавиша для текущего пункта
#define USR_IS_ENTER							USR_IS_ENTER_					//---осуществляется вход в пункте


/*рабочие клавиши*/
#define USR_UPKEY								USRC_UPKEY						//клавиша вверх
#define USR_DOWNKEY								USRC_DOWNKEY					//клавиша вниз
#define USR_LEFTKEY								USRC_LEFTKEY					//клавиша влево
#define USR_RIGHTKEY							USRC_RIGHTKEY					//клавиша вправо
#define USR_ENTERKEY_1							USRC_ENTERKEY_1					//клавиша входа и ввода
#define USR_ENTERKEY_2							USRC_ENTERKEY_2					//альтернативная клавиша входа
#define USR_RETURNKEY							USRC_RETURNKEY					//клавиша возвращения или стирания
#define USR_TOGGLEMENU_KEY						USRC_TOGGLEMENU_KEY				//клавиша быстрого входа и выхода из меню
#define USR_MINUSKEY							USRC_MINUSKEY					//клавиша ввода символа '-'

/*типы окон*/
#define USR_WND_WRT								USR_WND_WRT_					//окно рабочий стол
#define USR_WND_WRS								USR_WND_WRS_					//окно рабочее пространство
#define USR_WND_MENU							USR_WND_MENU_					//окно меню
#define USR_WND_MSG_KEY							USR_WND_MSG_KEY_				//окно сообщение типа KEY
#define USR_WND_MSG_TIME						USR_WND_MSG_TIME_				//окно сообщение типа TIME
#define USR_WND_MSG_BLINK						USR_WND_MSG_BLINK_				//окно сообщение типа BLINK
#define USR_WND_MSG_SOUND						USR_WND_MSG_SOUND_				//окно сообщение типа SOUND
#define USR_WND_MSG_ASK							USR_WND_MSG_ASK_				//окно сообщение типа ASK
#define USR_WND_MSG_ASKTIME						USR_WND_MSG_ASKTIME_			//окно сообщение типа TIMEASK
#define usr_WndType()							usr_WndType_()					//---читает тип текущего окна

//------------------------------------------------------------------------------
//							РАБОЧИЙ СТОЛ
//------------------------------------------------------------------------------

void wrt_Scan_Key(const usr_wnd_t* left_window, const usr_wnd_t* right_window);	//---навигация по рабочему столу

//------------------------------------------------------------------------------
//								МЕНЮ
//------------------------------------------------------------------------------

#define menu_Navigate_(ITEM)	menu_Navigate__(ITEM)							//---навигация по меню
void menu_Navigate(void);														//---навигация оконесного меню
void menu_Off_Key(uint8_t key);													//---отключает навигационную клавишу

//------------------------------------------------------------------------------
//								СООБЩЕНИЯ
//------------------------------------------------------------------------------

bool msg_Post(const usr_wnd_t *window, ubase_t type, usr_temp_t* var);				//---выводит сообщение
bool msg_Pend(bool* ans);														//---принимает ответ от сообщения
void msg_Scan_Key(void);														//---навигация по сообщению с учетом типа

#define msg_Check_PostReady()					msg_Check_PostReady_()			//---проверяет готовность к передаче сообщения типа ASK
#define msg_Check_PendReady()					msg_Check_PendReady_()			//---проверяет готовность к приему сообщения типа ASK
#define msg_On_MsgSend()						msg_On_MsgSend_()				//---разрешает вывод сообщений
#define msg_Off_MsgSend()						msg_Off_MsgSend_()				//---запрещает вывод сообщений

																				//СТАНДАРТНЫЕ СООБЩЕНИЯ
extern usr_wnd_t msgW_WrongInput;												//"Неправильный ввод"
extern usr_wnd_t msgW_Less_INT32U;												//"значение <= INT value" INT32U
extern usr_wnd_t msgW_More_INT32U;												//"значение >= INT value" INT32U
extern usr_wnd_t msgW_Less_INT32S;												//"значение <= INT value" INT32S
extern usr_wnd_t msgW_More_INT32S;												//"значение >= INT value" INT32S
extern usr_wnd_t msgW_Less_FL32;													//"значение <= FL value" FL32
extern usr_wnd_t msgW_More_FL32;													//"значение >= FL value" FL32

																				//ТИПЫ СООБЩЕНИЙ
#define MSG_KEY_TYPE							USR_WND_MSG_KEY_				//сообщение типа "вых. нажатием клавиши"
#define MSG_TIME_TYPE							USR_WND_MSG_TIME_				//сообщение типа "вых. нажатием клавиши или срабатыванием стандартного таймера"
#define MSG_BLINK_TYPE							USR_WND_MSG_BLINK_				//сообщение типа "вых. нажатием клавиши с подмигиванием"
#define MSG_SOUND_TYPE							USR_WND_MSG_SOUND_				//сообщение типа "вых. нажатием клавиши с звуком"
#define MSG_ASK_TYPE							USR_WND_MSG_ASK_				//сообщение типа "вых. нажатием клавиши и сохранением результата"
#define MSG_ASKTIME_TYPE						USR_WND_MSG_ASKTIME_			//сообщение типа "вых. нажатием клавиши или таймера и сохранением результата"

//------------------------------------------------------------------------------
//								ФУНКЦИИ STM
//------------------------------------------------------------------------------

#define USR_CHECK_TIMERS()						USR_CHECK_TIMERS_()				//---проверяет таймера (нужно расположить в системном тике)
