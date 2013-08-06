/*	Модуль графического интерфейса (ГИ)
 *	Поддерживаемые индикаторы:
 *	BC1602E_(BOLYMIN)
 *	WH1602D_(WINSTAR)
 *	BG12864F
 *	обновлен....................................................................01.02.2011 23:05:04
 */

#include "pdef.h"
#include "dspc.h"

#ifndef DSP_H_
#define DSP_H_

//------------------------------------------------------------------------------
//минимальный набор
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//типы строк
//------------------------------------------------------------------------------
//DSP_STR_WOW_PUTTED															//свободное пространство забито знаком '!'
//DSP_STR_STAR_PUTTED															//свободное пространство забито знаком '*'
//DSP_STR_ARROW_PUTTED															//дополнительно используеться указательная стрелка '>'
//DSP_STR_SPACE_PUTTED															//свободное пространство забито знаком ' '
//DSP_STR_LEFT_PUTTED															//строка по левому краю
//DSP_STR_RIGHT_PUTTED															//строка по правому краю
//------------------------------------------------------------------------------
//специальные символы
//------------------------------------------------------------------------------
//#define DSP_SYM_ARROW1						DSPC_SYM_ARROW1					//символ левая стрелочка
//#define DSP_SYM_ARROW2						DSPC_SYM_ARROW2					//символ правая стрелочка
//#define DSP_SYM_STAR							DSPC_SYM_STAR					//символ звездочка
//#define DSP_SYM_WOW							DSPC_SYM_WOW					//символ восклицание
//#define DSP_SYM_SPACE							DSPC_SYM_SPACE					//символ пробел
//#define DSP_SYM_PROGRESS_FUL					DSPC_SYM_PROGRESS_FUL			//символ строки прогресса полный
//#define DSP_SYM_PROGRESS_EMP					DSPC_SYM_PROGRESS_EMP			//символ строки прогресса пустой
//#define DSP_SYM_CHECKMARK						DSPC_SYM_CHECKMARK				//символ галочка
//#define DSP_SYM_SQRT							DSPC_SYM_SQRT					//символ корень квадратный
//#define DSP_SYM_GRADUS						DSPC_SYM_GRADUS					//символ градус
//------------------------------------------------------------------------------
//константы
//------------------------------------------------------------------------------
//DSP_STR_LENTH																	//длина строки
//DSP_VIR_STR_LENTH																//длина строки с учетом места на оповещающие значки
//DSP_STR_NUM																	//количество строк
//DSP_SPEC_STR																	//адрес начала спец. символов
//------------------------------------------------------------------------------
//стандартные командные строки для Printf
//------------------------------------------------------------------------------
//		note_cmd_1[] =		"%m";
//		note_cmd_2[] =		"%m %u";
//		note_cmd_3[] =		"%m %m";
//		note_cmd_4[] =		"%m %m %u";
//		note_cmd_5[] =		"%m %lu";
//		note_cmd_6[] =		"%m %f";
//		note_cmd_7[] =		"%f";
//		note_cmd_8[] =		"%s";
//		note_cmd_9[] =		"%m %ld";
//		note_cmd_10[] =		"%m%u %m";
//		note_cmd_11[] =		"%m %T";
//		note_cmd_12[] =		"%f %s";
//		note_cmd_D[] =		"%D";
//		note_cmd_T[] =		"%T";
//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------
//void dsp_Ini(void);															//---инициализирует дисплей
//void dsp_Clr(void);															//---очищает дисплей
//void dsp_Print(ubase_t strnum, ubase_t strtype, const uint8_t dspc_ *cmd_str,...);//---выводит комплексную строку
//void dsp_Set_Sym(uint8_t pos, uint8_t sym);									//---выводит символ на указанную позицию c учетом киррилици
//void dsp_Set_CurrentSym(uint8_t sym);											//---выводит символ с учетом кириллицы
//void dsp_Put_String(ubase_t strnum, uint8_t* str);								//---выводит заданную строку
//void dsp_Clr_Str(ubase_t line_num);											//---очищает строку
//void dsp_Set_CurrentByte(byte);												//---выводит символ в адресном пространстве индикатора (АПИ)
//void dsp_Set_Byte(pos, byte);													//---выводит символ на указанную позицию в АПИ
//void dsp_Set_SymPos(pos)														//---устанавливает адрес символа
//void dsp_Set_CursorPos(uint8_t str, uint8_t pos);								//---устанавливает курсор
//void dsp_Set_CursorBlink(void)												//---включает блинк курсора
//void dsp_Off_Cursor(void)														//---выключает курсор
//void dsp_Shift_CursorLeft(void)												//---сдвинуть курсор влево

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//##############################################################################
//###################		 							########################
//###################	ИНДИКАТОРЫ МОДЕЛИ ___WH1602D	########################
//###################									########################
//##############################################################################
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#if (DSPC_INICATOR_MODEL == ___WH1602D)
#ifndef WH1602D
#define WH1602D
#include <wh1602d.h>
#endif
//------------------------------------------------------------------------------
//типы строк
//------------------------------------------------------------------------------

#define DSP_STR_WOW_PUTTED						1								//свободное пространство забито знаком '!'
#define DSP_STR_STAR_PUTTED						2								//свободное пространство забито знаком '*'
#define DSP_STR_ARROW_PUTTED					3								//дополнительно используеться указательная стрелка '>'
#define DSP_STR_SPACE_PUTTED					4								//свободное пространство забито знаком ' '
#define DSP_STR_LEFT_PUTTED						5								//строка по левому краю
#define DSP_STR_RIGHT_PUTTED					6								//строка по правому краю

//------------------------------------------------------------------------------
//специальные символы
//------------------------------------------------------------------------------

#define DSP_SYM_ARROW1							DSPC_SYM_ARROW1					//символ левая стрелочка
#define DSP_SYM_ARROW2							DSPC_SYM_ARROW2					//символ правая стрелочка
#define DSP_SYM_STAR							DSPC_SYM_STAR					//символ звездочка
#define DSP_SYM_WOW								DSPC_SYM_WOW					//символ восклицание
#define DSP_SYM_SPACE							DSPC_SYM_SPACE					//символ пробел
#define DSP_SYM_PROGRESS_FUL					DSPC_SYM_PROGRESS_FUL			//символ строки прогресса полный
#define DSP_SYM_PROGRESS_EMP					DSPC_SYM_PROGRESS_EMP			//символ строки прогресса пустой
#define DSP_SYM_CHECKMARK						DSPC_SYM_CHECKMARK				//символ галочка
#define DSP_SYM_SQRT							DSPC_SYM_SQRT					//символ корень квадратный
#define DSP_SYM_GRADUS							DSPC_SYM_GRADUS					//символ градус

//------------------------------------------------------------------------------
//константы
//------------------------------------------------------------------------------

#define DSP_STR_LENTH							16								//длина строки
#define DSP_VIR_STR_LENTH						15								//длина строки с учетом места на оповещающие значки
#define DSP_STR_NUM								2								//количество строк
#define DSP_SPEC_STR							0x00							//адрес начала спец. символов

#define DSP_STR_START_ADR_0						WH1602D_STR_START_0				//адреса начала строк ЖКИ
#define DSP_STR_START_ADR_1						WH1602D_STR_START_1

//------------------------------------------------------------------------------
//стандартные командные строки для Printf
//------------------------------------------------------------------------------

flash_ const static ubase_t note_cmd_1[] =		"%m";
flash_ const static ubase_t note_cmd_2[] =		"%m %u";
flash_ const static ubase_t note_cmd_3[] =		"%m %m";
flash_ const static ubase_t note_cmd_4[] =		"%m %m %u";
flash_ const static ubase_t note_cmd_5[] =		"%m %lu";
flash_ const static ubase_t note_cmd_6[] =		"%m %f";
flash_ const static ubase_t note_cmd_7[] =		"%f";
flash_ const static ubase_t note_cmd_8[] =		"%s";
flash_ const static ubase_t note_cmd_9[] =		"%m %ld";
flash_ const static ubase_t note_cmd_10[] =		"%m%u %m";
flash_ const static ubase_t note_cmd_11[] =		"%m %T";
flash_ const static ubase_t note_cmd_12[] =		"%f %s";
flash_ const static ubase_t note_cmd_D[] =		"%D";
flash_ const static ubase_t note_cmd_T[] =		"%T";

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

void dsp_Ini(void);
void dsp_Clr(void);																//---очищает дисплей
void dsp_Print(ubase_t strnum, ubase_t strtype, const uint8_t dspc_ *cmd_str,...);//---выводит комплексную строку
/* Типы вывода
 *				%m -- вывод строки flash
 *				%s -- вывод строки RAM
 *				%c -- вывод символа
 *				%f -- вывод числа float
 *				%u -- вывод числа INT unsigned
 *				%d -- вывод числа INT signed
 *				%# -- вывод на строку(всегда в начале)
 *				%l -- вывод числа INT32
 *				%x -- вывод hex числа
 *				%T -- вывод времени TIME_*
 *				%D -- вывод даты DATE_*
 * пример: вывод знакового числа в формате hex типа int32_t : %lxd или %xld
 * пример: вывод беззнакового числа в формате hex типа int32_t : %lxu или %xlu
 * пример: вывод знакового числа  типа int32_t 3 знака: %3ld
*/

void dsp_Set_Sym(uint8_t pos, uint8_t sym);										//---выводит символ на указанную позицию c учетом киррилицы
void dsp_Set_CurrentSym(uint8_t sym);											//---выводит символ с учетом кириллицы
void dsp_Put_String(ubase_t strnum, uint8_t* str);									//---выводит заданную строку
void dsp_Clr_Str(ubase_t str);													//---очищает строку
void dsp_Set_CursorPos(uint8_t str, uint8_t pos);								//---устанавливает курсор

//------------------------------------------------------------------------------
//макро функции
//------------------------------------------------------------------------------

#define dsp_On_Light()						WH1602D_ON_LIGHT()					//---включает подсветку
#define dsp_Off_Light()						WH1602D_OFF_LIGHT()					//---выключает подсветку
#define dsp_Toggle_Light()					WH1602D_TOGGLE_LIGHT()				//---меняет состояние подсветки на противоположное

#define dsp_Set_CurrentByte(BYTE)			wh1602d_Set_CurrentByte(BYTE)		//---выводит символ в адресном пространстве индикатора (АПИ)
#define dsp_Set_Byte(POS, BYTE)				wh1602d_Set_Byte(POS, BYTE)			//---выводит символ на указанную позицию в АПИ
#define dsp_Set_SymPos(POS)					wh1602d_Set_SymPos(POS)				//---устанавливает адрес символа

#define dsp_Set_CursorBlink()				wh1602d_Cmd(WH1602D_DISPON_BLINKCURSOR_ON)//---включает блинк курсора
#define dsp_Off_Cursor()					wh1602d_Cmd(WH1602D_DISPON)			//---выключает курсор
#define dsp_Shift_CursorLeft()				wh1602d_Cmd(WH1602D_DECREMENTAC_SHIFTOFF)//---сдвинуть курсор влево

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//##############################################################################
//###################									########################
//###################	ИНДИКАТОРЫ МОДЕЛИ ___BC1602E	########################
//###################									########################
//##############################################################################
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#elif (DSPC_INICATOR_MODEL == ___BC1602E)
#ifndef BC1602E
#define BC1602E
#include "drivers/bc1602e.h"
#endif
//------------------------------------------------------------------------------
//типы строк
//------------------------------------------------------------------------------

#define DSP_STR_WOW_PUTTED						1								//свободное пространство забито знаком '!'
#define DSP_STR_STAR_PUTTED						2								//свободное пространство забито знаком '*'
#define DSP_STR_ARROW_PUTTED					3								//дополнительно используеться указательная стрелка '>'
#define DSP_STR_SPACE_PUTTED					4								//свободное пространство забито знаком ' '
#define DSP_STR_LEFT_PUTTED						5								//строка по левому краю
#define DSP_STR_RIGHT_PUTTED					6								//строка по правому краю

//------------------------------------------------------------------------------
//специальные символы
//------------------------------------------------------------------------------

#define DSP_SYM_ARROW1							DSPC_SYM_ARROW1					//символ левая стрелочка
#define DSP_SYM_ARROW2							DSPC_SYM_ARROW2					//символ правая стрелочка
#define DSP_SYM_STAR							DSPC_SYM_STAR					//символ звездочка
#define DSP_SYM_WOW								DSPC_SYM_WOW					//символ восклицание
#define DSP_SYM_SPACE							DSPC_SYM_SPACE					//символ пробел
#define DSP_SYM_PROGRESS_FUL					DSPC_SYM_PROGRESS_FUL			//символ строки прогресса полный
#define DSP_SYM_PROGRESS_EMP					DSPC_SYM_PROGRESS_EMP			//символ строки прогресса пустой
#define DSP_SYM_CHECKMARK						DSPC_SYM_CHECKMARK				//символ галочка
#define DSP_SYM_SQRT							DSPC_SYM_SQRT					//символ корень квадратный
#define DSP_SYM_GRADUS							DSPC_SYM_GRADUS					//символ градус

//------------------------------------------------------------------------------
//константы
//------------------------------------------------------------------------------

#define DSP_STR_LENTH							16								//длина строки
#define DSP_VIR_STR_LENTH						15								//длина строки с учетом места на оповещающие значки
#define DSP_STR_NUM								2								//количество строк
#define DSP_SPEC_STR							0x00							//адрес начала спец. символов

#define DSP_STR_START_ADR_0						BC1602E_STR_START_0				//адреса начала строк ЖКИ
#define DSP_STR_START_ADR_1						BC1602E_STR_START_1

//------------------------------------------------------------------------------
//стандартные командные строки для Printf
//------------------------------------------------------------------------------

const static DSPC_STR_ note_cmd_1[] =		"%m";
const static DSPC_STR_ note_cmd_2[] =		"%m %u";
const static DSPC_STR_ note_cmd_3[] =		"%m %m";
const static DSPC_STR_ note_cmd_4[] =		"%m %m %u";
const static DSPC_STR_ note_cmd_5[] =		"%m %lu";
const static DSPC_STR_ note_cmd_6[] =		"%m %f";
const static DSPC_STR_ note_cmd_7[] =		"%f";
const static DSPC_STR_ note_cmd_8[] =		"%s";
const static DSPC_STR_ note_cmd_9[] =		"%m %ld";
const static DSPC_STR_ note_cmd_10[] =		"%m%u %m";
const static DSPC_STR_ note_cmd_11[] =		"%m %T";
const static DSPC_STR_ note_cmd_12[] =		"%f %s";
const static DSPC_STR_ note_cmd_D[] =		"%D";
const static DSPC_STR_ note_cmd_T[] =		"%T";

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

void dsp_Ini(void);
void dsp_Clr(void);																//---очищает дисплей
void dsp_Print(ubase_t strnum, ubase_t strtype, const DSPC_CSTR_ *cmd_str,...);//---выводит комплексную строку
/* Типы вывода
 *				%m -- вывод строки flash
 *				%s -- вывод строки RAM
 *				%c -- вывод символа
 *				%f -- вывод числа float
 *				%u -- вывод числа INT unsigned
 *				%d -- вывод числа INT signed
 *				%# -- вывод на строку(всегда в начале)
 *				%l -- вывод числа INT32
 *				%x -- вывод hex числа
 *				%T -- вывод времени TIME_*
 *				%D -- вывод даты DATE_*
 * пример: вывод знакового числа в формате hex типа int32_t : %lxd или %xld
 * пример: вывод беззнакового числа в формате hex типа int32_t : %lxu или %xlu
 * пример: вывод знакового числа  типа int32_t 3 знака: %3ld
*/

void dsp_Set_Sym(uint8_t pos, uint8_t sym);										//---выводит символ на указанную позицию c учетом киррилицы
void dsp_Set_CurrentSym(uint8_t sym);											//---выводит символ с учетом кириллицы
void dsp_Put_String(ubase_t strnum, uint8_t* str);								//---выводит заданную строку
void dsp_Clr_Str(ubase_t str);													//---очищает строку
void dsp_Set_CursorPos(uint8_t str, uint8_t pos);								//---устанавливает курсор
bool dsp_Check_Disp(void);                                                      //---check displaying

//------------------------------------------------------------------------------
//макро функции
//------------------------------------------------------------------------------

#define dsp_On_Light()						bc1602e_On_Light()					//---включает подсветку
#define dsp_Off_Light()						bc1602e_Off_Light()					//---выключает подсветку
#define dsp_Toggle_Light()					bc1602e_Inv_Light()					//---меняет состояние подсветки на противоположное

#define dsp_Set_CurrentByte(BYTE)			bc1602e_Set_CurrentByte(BYTE)		//---выводит символ в адресном пространстве индикатора (АПИ)
#define dsp_Set_Byte(POS, BYTE)				bc1602e_Set_Byte(POS, BYTE)			//---выводит символ на указанную позицию в АПИ
#define dsp_Set_SymPos(POS)					bc1602e_Set_SymPos(POS)				//---устанавливает адрес символа

#define dsp_Set_CursorBlink()				bc1602e_Cmd(BC1602E_DISPON_BLINKCURSOR_ON)//---включает блинк курсора
#define dsp_Off_Cursor()					bc1602e_Cmd(BC1602E_DISPON)			//---выключает курсор
#define dsp_Shift_CursorLeft()				bc1602e_Cmd(BC1602E_DECREMENTAC_SHIFTOFF)//---сдвинуть курсор влево

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//##############################################################################
//#######################									####################
//####################### 	ИНДИКАТОРЫ МОДЕЛИ ___BG12864F	####################
//#######################									####################
//##############################################################################
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#elif (DSPC_INICATOR_MODEL == ___BG12864F)
#ifndef BG12864F
#define BG12864F
#include <bg12864f.h>
#endif

//------------------------------------------------------------------------------
//типы строк
//------------------------------------------------------------------------------

#define DSP_STR_WOW_PUTTED						1								//свободное пространство забито знаком '!'
#define DSP_STR_STAR_PUTTED						2								//свободное пространство забито знаком '*'
#define DSP_STR_ARROW_PUTTED					3								//дополнительно используеться указательная стрелка '>'
#define DSP_STR_SPACE_PUTTED					4								//свободное пространство забито знаком ' '
#define DSP_STR_LEFT_PUTTED						5								//строка по левому краю
#define DSP_STR_RIGHT_PUTTED					6								//строка по правому краю

//------------------------------------------------------------------------------
//специальные символы
//------------------------------------------------------------------------------

#define DSP_SYM_ARROW1							DSPC_SYM_ARROW1					//символ левая стрелочка
#define DSP_SYM_ARROW2							DSPC_SYM_ARROW2					//символ правая стрелочка
#define DSP_SYM_STAR							DSPC_SYM_STAR					//символ звездочка
#define DSP_SYM_WOW								DSPC_SYM_WOW					//символ восклицание
#define DSP_SYM_SPACE							DSPC_SYM_SPACE					//символ пробел
#define DSP_SYM_PROGRESS_FUL					DSPC_SYM_PROGRESS_FUL			//символ строки прогресса полный
#define DSP_SYM_PROGRESS_EMP					DSPC_SYM_PROGRESS_EMP			//символ строки прогресса пустой
#define DSP_SYM_CHECKMARK						DSPC_SYM_CHECKMARK				//символ галочка
#define DSP_SYM_SQRT							DSPC_SYM_SQRT					//символ корень квадратный
#define DSP_SYM_GRADUS							DSPC_SYM_GRADUS					//символ градус

//#define DSP_SYM_GRADUS							0xA0							//градус
//------------------------------------------------------------------------------
//константы
//------------------------------------------------------------------------------

#define DSP_STR_LENTH							21								//длина строки
#define DSP_VIR_STR_LENTH						DSP_STR_LENTH-1					//длина строки с учетем места на оповещающие значки
#define DSP_STR_NUM								8								//количество строк
#define DSP_SPEC_STR							0x9C							//адрес начала спец. символов
		


#define DSP_STR_START_ADR_0						0x00							//адреса начала строк ЖКИ
#define DSP_STR_START_ADR_1						0x16
#define DSP_STR_START_ADR_2						0x2C
#define DSP_STR_START_ADR_3						0x42
#define DSP_STR_START_ADR_4						0x58
#define DSP_STR_START_ADR_5						0x6E
#define DSP_STR_START_ADR_6						0x84
#define DSP_STR_START_ADR_7						0x9A

//------------------------------------------------------------------------------
//стандартные командные строки для Printf
//------------------------------------------------------------------------------

flash_ const static ubase_t note_cmd_1[] =		"%m";
flash_ const static ubase_t note_cmd_2[] =		"%m %u";
flash_ const static ubase_t note_cmd_3[] =		"%m %m";
flash_ const static ubase_t note_cmd_4[] =		"%m %m %u";
flash_ const static ubase_t note_cmd_5[] =		"%m %lu";
flash_ const static ubase_t note_cmd_6[] =		"%m %f";
flash_ const static ubase_t note_cmd_7[] =		"%f";
flash_ const static ubase_t note_cmd_8[] =		"%s";
flash_ const static ubase_t note_cmd_9[] =		"%m %ld";
flash_ const static ubase_t note_cmd_10[] =		"%m%u %m";
flash_ const static ubase_t note_cmd_11[] =		"%m %T";
flash_ const static ubase_t note_cmd_12[] =		"%f %s";
flash_ const static ubase_t note_cmd_D[] =		"%D";
flash_ const static ubase_t note_cmd_T[] =		"%T";

//------------------------------------------------------------------------------
//функции для работы с строчным индикатором
//------------------------------------------------------------------------------

void dsp_Ini(void);
void dsp_Clr(void);																//---стирает индикатор

void dsp_Print(ubase_t strnum, ubase_t strtype, const uint8_t dspc_ *cmd_str,...);//---выводит комплексную строку, где strnum - номер строки, strtype - тип строки, cmd_str - командная строка
/* Типы вывода
 *				%m -- вывод строки flash
 *				%s -- вывод строки RAM
 *				%c -- вывод символа
 *				%f -- вывод числа float
 *				%u -- вывод числа INT unsigned
 *				%d -- вывод числа INT signed
 *				%# -- вывод на строку(всегда в начале)
 *				%l -- вывод числа INT32
 *				%x -- вывод hex числа
 *				%T -- вывод времени TIME_*
 *				%D -- вывод даты DATE_*
 * пример: вывод знакового числа в формате hex типа int32_t : %lxd или %xld
 * пример: вывод беззнакового числа в формате hex типа int32_t : %lxu или %xlu
 * пример: вывод знакового числа  типа int32_t 3 знака: %3ld
*/

void dsp_Set_SymPos(uint8_t pos);												//---устанавливает адрес символа
void dsp_Set_Sym(uint8_t pos, uint8_t sym);										//---выводит символ на указанную позицию с учетом кириллицы
void dsp_Set_CurrentSym(uint8_t sym);											//---выводит символ с учетом кириллицы

void dsp_Set_CurrentByte(uint8_t byte);											//---выводит символ в адресном пространстве индикатора (АПИ)
void dsp_Set_Byte(uint8_t pos, uint8_t byte);									//---выводит символ на указанную позицию в АПИ
void dsp_Put_String(ubase_t strnum, uint8_t* str);									//---выводит заданную строку

void dsp_Clr_Str(ubase_t strnum);												//---очищает строку
void dsp_Set_CursorBlink(void);													//---включает блинк курсора
void dsp_Off_Cursor(void);														//---выключает курсор
void dsp_Shift_CursorLeft(void);												//---сдвинуть курсор влево
void dsp_Set_CursorPos(uint8_t str, uint8_t pos);								//---устанавливает курсор

//------------------------------------------------------------------------------
//функции для работы с графическим индикатором
//------------------------------------------------------------------------------

void dsp_Set_Graph_Mode(void);													//---включает графический режим
void dsp_Off_Graph_Mode(void);													//---выключает графический режим
void dsp_Write_Graph(uint16_t adr, uint8_t* data, uint16_t dsize);				//---выводит данные в графический индикатор
void dsp_Set_Pixel(uint8_t y, uint8_t x);										//---устанавливает точку на индикаторе
void dsp_Clr_Pixel(uint8_t y, uint8_t x);										//---стирает точку на индикаторе


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//##############################################################################
//#######################								  ######################
//#######################	ИНДИКАТОР МОДЕЛИ ___RLD5621	  ######################
//#######################								  ######################
//##############################################################################
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#elif (DSPC_INICATOR_MODEL == ___BG12864F)
#ifndef RLD5621
#define RLD5621
#include <rld5621.h>
#endif

void dsp_Ini(void);																//---инициализация
void dsp_Set_Sym(ubase_t pos, ubase_t sym);										//---выводит символ на указанную позицию
void dsp_Out_Num(uint8_t num);													//---выводит двух символьное число

#endif
#endif

