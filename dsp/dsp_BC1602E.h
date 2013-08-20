/*	Подмодуль dsp модуля относящийся к символьному индикатору BC1602E
 *	обновлен....................................................................28.12.2010 13:48:03
 */

#include "../tdf.h"
#include "../prm.h"
#include "dsp.h"

//------------------------------------------------------------------------------
//коррекция кириллицы
//------------------------------------------------------------------------------

static DSPC_TAB_ cyrillic_symbols[64] =
	{'A', 0xA0, 'B', 0xA1, 0xE0, 'E', 0xA3, 0xA4, 0xA5, 0xA6,'K',
	0xA7, 'M', 'H', 'O', 0xA8, 'P', 'C', 'T', 0xA9, 0xAA, 'X', 0xE1,
	0xAB, 0xAC, 0xE2, 0xAD, 0xAE, 'b', 0xAF, 0xB0, 0xB1, 'a', 0xB2,
	0xB3, 0xB4, 0xE3, 'e', 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC,
	0xBD, 'o', 0xBE, 'p', 'c', 0xBF, 'y' , 0xE4, 'x' , 0xE5, 0xC0,
	0xC1, 0xE6, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7};

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

//extern dspt_ uint8_t special_symbols[];
uint8_t dsp_Decode_Sym(uint8_t sym);											//---коррекция символов

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

void dsp_Ini(void)
//---инициализация ГПИ
{
	bc1602e_ini();
	bc1602e_set_spec_sym_pos(0);													//записать специальные символы
	for (ubase_t i = 0; i < sizeof(special_symbols); i++)
	{
		bc1602e_set_current_byte(prm_Rd_B(&special_symbols[i]));
	}
	dsp_Set_SymPos(0);
}

void dsp_Set_Sym(uint8_t pos, uint8_t sym)
//---выводит символ на указанную позицию
{
	dsp_Set_SymPos(pos);
	dsp_Set_CurrentSym(sym);
}


void dsp_Set_CursorPos(uint8_t str, uint8_t pos)
//---устанавливает курсор
{
	if (str) pos += BC1602E_STR_START_1;
	dsp_Set_SymPos(pos);
}


void dsp_Set_CurrentSym(uint8_t sym)
//---выводит символ с учетом русской кириллицы
{
	uint8_t newsym;
	newsym = dsp_Decode_Sym(sym);
	dsp_Set_CurrentByte(newsym);
}

void dsp_Put_String(ubase_t strnum, uint8_t* str)
//---выводит заданную строку
{
	uint8_t str_pos;
    if (strnum) str_pos = BC1602E_STR_START_1;
    else str_pos = BC1602E_STR_START_0;
	bc1602e_set_sym_pos(str_pos);
	for (ubase_t i=0; i < DSP_VIR_STR_LENTH; i++)
	{
		bc1602e_set_current_byte(*str++);
	}
}


void dsp_Clr_Str(ubase_t str)
//---очищает строку
{
	uint8_t str_pos;
    if (str) str_pos = BC1602E_STR_START_1;
    else str_pos = BC1602E_STR_START_0;

	bc1602e_set_sym_pos(str_pos);
	for (ubase_t i=0; i < DSP_VIR_STR_LENTH; i++)
	{
		bc1602e_set_current_byte(' ');
	}
	bc1602e_set_sym_pos(str_pos);
}


void dsp_Clr(void)
//---очищает дисплей
{
	dsp_Clr_Str(0);
    dsp_Clr_Str(1);
    bc1602e_set_sym_pos(0);
}


bool dsp_Check_Disp(void)
//---check displaying
{
	enum {CHECK_POS = 79, CHECK_SYMBOL = ' '};
	uint8_t sym_pos;
	bc1602e_get_pos(&sym_pos);													//прочитать текущую позицию курсора
    bc1602e_set_sym_pos(CHECK_POS);												//перейти на проверочную позицию
	uint8_t sym;
	bc1602e_get_sym(&sym);														//скопировать символ
	bc1602e_cmd(BC1602E_MOVE_CURSOR_LEFT);
	bc1602e_set_current_byte(CHECK_SYMBOL);										//вставить проверочный символ
	bc1602e_cmd(BC1602E_MOVE_CURSOR_LEFT);
	uint8_t check_sym = 0;
	bc1602e_get_sym(&check_sym);												//прочитать проверочный символ
	bc1602e_cmd(BC1602E_MOVE_CURSOR_LEFT);				
    bc1602e_set_current_byte(sym);												//вставить символ, тот что был
    bc1602e_set_sym_pos(sym_pos);												//вернуться на прежнюю позицию
	if (check_sym == CHECK_SYMBOL) return 1;			    					//проверить соответствие проверочного символа
	return 0;
}


void dsp_Print(ubase_t strnum, ubase_t strtype, const DSPC_CSTR_ *cmd_str,...)
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
 *				%T -- вывод времени time_t*
 *				%D -- вывод даты date_t*
 * пример: вывод знакового числа в формате hex типа int32_t : %lxd или %xld
 * пример: вывод беззнакового числа в формате hex типа int32_t : %lxu или %xlu
 * пример: вывод знакового числа  типа int32_t 3 знака: %3ld
*/
{
	va_list lst;
	va_start(lst, cmd_str);
#if (DSPC_USE_HEX == 1)
	ubase_t base = 10;															//база исчесления
#else
	#define base 10
#endif
	uint8_t line[DSP_STR_LENTH];												//массив выходной строки
	uint8_t* cell = line;
	ubase_t prec = 8;															//размер целой части
#if (DSPC_USE_FLOAT == 1)
	ubase_t fract_prec;															//размер дробной части
#endif
#if (DSPC_USE_ONSTR == 1)
	uint8_t *out_line;
#endif
	struct
	{
		ubase_t on_line:1;														//флаг оповещающий, что идет вывод на строку
		ubase_t prec_sel:1;														//флаг оповещающий, что выбрана точность отображения
		ubase_t is_int32_t:1;
		ubase_t is_signed:1;
	} status;
	status.on_line = false;
	status.prec_sel = false;
	status.is_int32_t = false;
	status.is_signed = false;

	ubase_t cmd_sym; 															//символ командной строки
	if (strtype == DSP_STR_ARROW_PUTTED) *cell++ = DSP_SYM_ARROW1;				//если со стрелкой - поместить стрелку
	while ((cmd_sym = prm_Rd_B(cmd_str++)))
	{
		prec = 8;
		fract_prec = 6;
		if (cmd_sym == '%')														// считка параметра
		{
		#if (DSPC_USE_ACCURACY == 1) && (DSPC_USE_FLOAT == 1 || DSPC_USE_INT == 1)
			cmd_select:
		#endif
			switch (cmd_sym = prm_Rd_B(cmd_str++))
			{
			#if (DSPC_USE_FLASH_STR == 1)
				case 'm':														//вставляеться флеш строка
				{
					DSPC_STR_ *c = va_arg(lst, DSPC_STR_*);

					uint8_t byte;
					for(;;) {
						byte = prm_Rd_B(c++);
						if (byte)
							*cell++ = byte;
						else
							break;
					}
					continue;
				}
			#endif
			#if (DSPC_USE_EEPROM_STR == 1)
				case 'e':														//вставляется eeprom строка
				{
					uint8_t eeprom_ *c = va_arg(lst, uint8_t eeprom_*);
					while (*c) *cell++ = *c++;
					continue;
				}
			#endif
			#if (DSPC_USE_RAM_STR == 1)
				case 's':														//вставляеться ram строка
				{
					uint8_t* c = va_arg(lst, uint8_t*);
					while (*c)
					{
						if (!status.on_line)
						{
							*cell++ = dsp_Decode_Sym(*c++);
						}
						else *cell++ = *c++;
					}
					continue;
				}
			#endif
			#if (DSPC_USE_SYMBOL == 1)
				case 'c':														//вставляеться символ
				{
					*cell++ = dsp_Decode_Sym(va_arg(lst, int16_t));
					continue;
				}
			#endif
			#if (DSPC_USE_HEX == 1)
				case 'x':
				{
					base = 16;
					goto cmd_select;
				}
			#endif
			#if (DSPC_USE_LONG == 1)
				case 'l':
				{
					status.is_int32_t = true;
					goto cmd_select;
				}	
			#endif
			#if (DSPC_USE_SIGNED_INT == 1)
				case 'd': status.is_signed = true;
			#endif
			#if ((DSPC_USE_INT == 1) || (DSPC_USE_LONG == 1) || (DSPC_USE_HEX == 1))
				case 'u':														//целое
				{
					uint32_t whole;
				#if (DSPC_USE_LONG == 1)
					if (status.is_int32_t)
					{
						whole = va_arg(lst, uint32_t);
						status.is_int32_t = false;
					}
					else
					{
					#if (DSPC_USE_SIGNED_INT == 1)
						whole = va_arg(lst, int16_t);
						if (!status.is_signed) whole &= 0x0000FFFF;
					#else
						whole = va_arg(lst, uint16_t);
					#endif
					}
				#else
				#if (DSPC_USE_SIGNED_INT == 1)
					whole = va_arg(lst, int16_t);
					if (!status.is_signed) whole &= 0x0000FFFF;
				#else
					whole = va_arg(lst, uint16_t);
				#endif
				#endif
					
				#if (DSPC_USE_SIGNED_INT == 1)
					if (status.is_signed)
					{
						status.is_signed = false;
						if ((int32_t)whole < 0)
						{
							whole = -whole;
							*cell++ = '-';
							prec--;
						}
					}
				#endif
					uint32_t td;
					td = whole ? 1:base;										//вычислить размер числа
					
					while (td <= whole) td *= base;
					for (;prec && (td/=base); prec--)
					{
					#if (DSPC_USE_HEX == 1)
						uint8_t sym = (whole/td)%base;
						if (sym > 9) *cell++ = sym + ('7');
						else *cell++ = sym|('0');
					#else
						*cell++ = ((whole/td)%base)|('0');
					#endif
					}
				#if (DSPC_USE_HEX == 1)
					base = 10;
				#endif
					if (status.prec_sel) for (ubase_t i = 0 ; i < prec; i++) *cell++ = ' ';//дозабить свободное пространство ' '
					prec = 8;													//вернуть точность по умолчанию
					status.prec_sel = false;
					continue;
				}
			#endif
			#if ((DSPC_USE_ACCURACY == 1) && (DSPC_USE_FLOAT == 1))
				case '.':														//следующим вводиться разделительная точка
				{
					cmd_sym = prm_Rd_B(cmd_str++);
					fract_prec = cmd_sym&0x0F;									//получить точность после запятой
					goto cmd_select;
				}
			#endif
			#if (DSPC_USE_FLOAT == 1)
				case 'f':														//вставляеться fl32_t
				{
					fl64_t flt = va_arg(lst, fl64_t);					
					if (flt >= 999999999.0 || flt < -999999999.0) break;		//отфильтровать числа, которые невозможно вывести!
					
					ubase_t whole_size = 0;//было 1
					
					if (flt < 0.0)												//с учетом отрицательного числа
					{
						flt = -flt;
						*cell++ = '-';
						prec--;						////////////////////////////////////////////////////////
						//whole_size++;
					}
					
					if (!fract_prec) flt += 0.5;								//если дробную часть не выводим - округлить целую часть
					uint32_t whole = (uint32_t)flt;								//отделить целую часть
					
					uint32_t td = 1;
					do
					{
						td *= 10;
						whole_size++;											//определить количество розрядов целой части
					} while (td <= whole);
					
					if (prec <= whole_size)										//кол. разрядов истекло
					{
						flt += 0.5;
						whole = (uint32_t)flt;									//переокруглить целую часть
						
						for (;prec && (td/=base); --prec)						
						{
							*cell++ = ((whole/td)%10)|'0';							
						}
						//if (status.prec_sel) for(ubase_t i=0 ; i<prec; i++) *cell++ = ' ';//дозабить свободное пространство ' '
						//if (td/=base) *cell++ = ((whole/td)%10)|'0';
						*cell++ = ' ';
						prec = 8;												//вернуть точность по умолчанию
						status.prec_sel = false;
						continue;
					}

					ubase_t fract_rest = prec - whole_size;
					if (fract_prec > fract_rest) fract_prec = fract_rest;		//откоректировать кол. разрядов дробной части
					flt -= (fl64_t)whole;										//отделить дробную часть
				
					uint32_t fract_td = 1;										//сделать дробную часть целой
					for (ubase_t i=0; i<fract_prec; i++)
					{
						flt *= 10.0;
						fract_td *=10;
					};
					fract_prec = 6;
					flt += 0.5;													//округлить целую часть
					uint32_t fract = (uint32_t)flt;
					
					if (fract >= fract_td)										//если округление привело к увелечению на 1 разряд
					{
						fract -= fract_td;
						whole++;
						if (td <= whole) td *= 10;
					}
					
					for (;(td/=base); --prec)									//вывести целую часть
					{
						*cell++ = ((whole/td)%10)|'0';
					}
					
					*cell++ = '.';
					
					for (;(fract_td/=base); --prec)								//вывести дробную часть
					{
						*cell++ = ((fract/fract_td)%10)|'0';
					}
					if (status.prec_sel) for (ubase_t i=0 ; i<prec; i++) *cell++ = ' ';//дозабить свободное пространство ' '
					prec = 8;													//вернуть точность по умолчанию
					status.prec_sel = false;
					continue;
				}
			#endif
			#if (DSPC_USE_TIME == 1)
				case 'T':														//вывод времени
				{
					time_t* time = va_arg(lst, time_t*);
					*cell++ = (time->hour>>4)|0x30;
					*cell++ = (time->hour&0x0F)|0x30;
					*cell++ = DSPC_TIME_SEPARATOR;
					*cell++ = (time->min>>4)|0x30;
					*cell++ = (time->min&0x0F)|0x30;
					*cell++ = DSPC_TIME_SEPARATOR;
					*cell++ = (time->sec>>4)|0x30;
					*cell++ = (time->sec&0x0F)|0x30;
					continue;
				}
			#endif
			#if (DSPC_USE_DATE == 1)
				case 'D':
				{
					date_t* date = va_arg(lst, date_t*);
					*cell++ = (date->date>>4)|0x30;
					*cell++ = (date->date&0x0F)|0x30;
					*cell++ = DSPC_DATE_SEPARATOR;
					*cell++ = (date->month>>4)|0x30;
					*cell++ = (date->month&0x0F)|0x30;
					*cell++ = DSPC_DATE_SEPARATOR;
				#if DSPC_USE_FULLYAER == 1
					*cell++ = (date->ctry>>4)|0x30;
					*cell++ = (date->ctry&0x0F)|0x30;
				#endif
					*cell++ = (date->year>>4)|0x30;
					*cell++ = (date->year&0x0F)|0x30;
					continue;
				}
			#endif
			#if DSPC_USE_ONSTR == 1
				case '#':														//осуществляеться вывод на строку
				{
					status.on_line = true;										//обозначить вывод на строку
					out_line = va_arg(lst, uint8_t*);
					continue;
				}
			#endif
				default:														//вводяться цифры дополнительной точности
				{
				#if (DSPC_USE_ACCURACY == 1) && (DSPC_USE_FLOAT == 1 || DSPC_USE_INT == 1)
					if (cmd_sym >= '0' && cmd_sym <= '9')
					{
						prec = cmd_sym&0x0F;									//получить размер всего числа
						status.prec_sel = true;									//обозначить что точность задается
						goto cmd_select;
					}
				#endif
					*cell = 0;
					va_end(lst);												//корректно закончить считку параметров
					return;
				}
			}
		}
		else *cell++ = cmd_sym;													//вводиться символ из строки управления
	}
	va_end(lst);																//корректно закончить считку параметров
#if DSPC_ARROW_TYPE == ___DSPC_DOUBLE_ARROW
	if (strtype == DSP_STR_ARROW_PUTTED) *cell++ = DSP_SYM_ARROW2;
#endif
	*cell = 0;
	
	/// Подготовится к выводу
	ubase_t strsize = cell - line;
	cell = line;
	
	ubase_t fullsize;															//определить размер забиваемой символами области строки
	if (strtype == DSP_STR_LEFT_PUTTED) fullsize = 0;
	else
	{
		if (strsize >= DSP_VIR_STR_LENTH) strsize = DSP_VIR_STR_LENTH;
		fullsize = (DSP_VIR_STR_LENTH - strsize);
		if (strtype != DSP_STR_RIGHT_PUTTED) fullsize >>= 1;
	}
	
	uint8_t add_sym;															//определить дополнительный символ
	switch (strtype)
	{
		case DSP_STR_WOW_PUTTED: add_sym = DSP_SYM_WOW;break;
		case DSP_STR_STAR_PUTTED: add_sym = DSP_SYM_STAR;break;
        default: add_sym = DSP_SYM_SPACE; break;
	}
	
#if DSPC_USE_ONSTR == 1
	if (status.on_line)															//Если вывод на строку
	{
		for (ubase_t i=0, s=0; i<DSP_VIR_STR_LENTH; i++)
		{
			if (i < fullsize)
			{
				if (fullsize == i+1) *out_line = ' ';
				else *out_line = add_sym;
			}
			else if (s < strsize)
			{
				*out_line = *cell++;
				s++;
			}
			else
			{
				if (strtype == DSP_STR_LEFT_PUTTED) break;
				if (s == strsize)
				{
					*out_line = ' ';
					s++;
				}
				else *out_line = add_sym;
			}
			out_line++;
		}
		*out_line=0;
		return;
	}
#endif
	
	/// Если вывод на ЖКИ
	ubase_t str_pos;																//определить стартовое положение курсора
#if DSP_STR_NUM == 2
    if(strnum) str_pos = BC1602E_STR_START_1;
    else str_pos = BC1602E_STR_START_0;
#elif DSP_STR_NUM == 4
    switch (strnum)
	{
		case 0: str_pos = BC1602E_STR_START_0; break;
		case 1: str_pos = BC1602E_STR_START_1; break;
		case 2: str_pos = BC1602E_STR_START_2; break;
		case 3: str_pos = BC1602E_STR_START_3; break;
	}
#endif
	
	bc1602e_set_sym_pos(str_pos);												//стать на стартовую позицию и приготовиться к передаче

	for (ubase_t i=0, s=0; i<DSP_VIR_STR_LENTH; i++)								//вывести строку
	{
		if (i < fullsize)
		{
			if (fullsize == i+1) dsp_Set_CurrentByte(' ');
			else dsp_Set_CurrentByte(add_sym);
		}
		else if (s < strsize)
		{
			dsp_Set_CurrentByte(*cell++);
			s++;
		}
		else
		{
			if (s == strsize)
			{
				dsp_Set_CurrentByte(' ');
				s++;
			}
			else dsp_Set_CurrentByte(add_sym);
		}
	}
}


//------------------------------------------------------------------------------
//внутренние функции
//------------------------------------------------------------------------------

uint8_t dsp_Decode_Sym(uint8_t sym)
//---коррекция символов
{
	if (sym >= 0xC0)
	{
		return prm_Rd_B(&cyrillic_symbols[(uint8_t)(sym - 0xC0)]);
	}
#if DSPC_LENGUAGE == ___DSPC_UKR
	else if (sym == 'Ї' || sym == 'ї') return 0x08;
	else if (sym == 'є') return 0x07;
	else if (sym == 'Є') return = 0x06;
#endif
	return sym;
}
