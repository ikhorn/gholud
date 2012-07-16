/* Функции ввода модуля usr
 * обновлен.....................................................................31.12.2010 20:04:18
*/

#include "usrc.h"
#include "asci.h"
#include "arr.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

#define usr_Filtr_ProcReturnFalse()		if (gUsr.sp->act_strnum != gUsr.strnum || !usr_Key) return 0;
const static USRC_STR_ usr_def_date[] = {USRC_DEF_D,USRC_DEF_M,USRC_DEF_Y,0};	//дата по умолчанию

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

extern bool usr_Write_InpBufSym(ubase_t adr, uint8_t sym);
extern bool usr_Read_InpBufData(ubase_t adr, ubase_t dsize, uint8_t* data);	

bool usr_Scan_InputKey(ubase_t symnum);											//---считка вводных клавиш
bool usr_Scan_InputTextKey(ubase_t symnum);										//---считка вводимых текстовых символов

void usr_Put_Sym(ubase_t symnum);												//---вводит символ
void usr_Cut_Sym(void);															//---вырезает символ

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------

void usr_InputAdd_Sym(ubase_t key, uint8_t* str, ubase_t symnum)
//---вводит символ на текущую позицию ввода, выбирая его из заданной строки
{
	if (usr_Key != key) return;
	if (USR_IS_INPUT)
	{
		if (gUsr.prev_key != usr_Key)											//смена клавиши
		{
			gUsr.prev_key = usr_Key;
			gUsr.click_num = 0;
			if (gUsr.inp_pos < symnum) gUsr.inp_pos++;
		}
		else gUsr.click_num++;
		
		if (!str[gUsr.click_num]) gUsr.click_num = 0;
		gUsr.inp[gUsr.inp_pos-1] = str[gUsr.click_num];
		STM_SET(gUsr.t_sym_input, 1.5);											//навести таймер ввода
		gUsr.refresh = true;
		usr_Key = 0;
	}
}

bool usr_Input_PreStr(uint8_t* str)
//---вставляет строку введенной при старте ввода, возвращает сигнал о начале ввода
{
	if (!gUsr.write)
	{
#if USRC_USE_ENTERKEY_2 == 1
		if (usr_Key != USRC_ENTERKEY_1 && usr_Key != USRC_ENTERKEY_2) return 0;	//ввод данных не начат
#elif USRC_USE_ENTERKEY_2 == 0
		if (usr_Key != USRC_ENTERKEY_1) return 0;								//ввод данных не начат
#endif
		gUsr.write = 1;															//начать ввод данных
		gUsr.inp_pos = 0;
		for (; *str && (gUsr.inp_pos < USRC_INPUT_RAM_STR_SIZE); gUsr.inp_pos++)
		{
			gUsr.inp[gUsr.inp_pos] = *str++;
		}
		usr_Key = 0;
		gUsr.refresh = 1;
		return 1;
	}
	return 0;
}

bool usr_Input_Time(time_t* time)
//---вводит параметр время
{
	usr_Filtr_ProcReturnFalse();												//фильтры обработки
	enum {SYMNUM = 6};
	if (!usr_Scan_InputKey(SYMNUM)) return 0;
	
	for (ubase_t i=0; i<SYMNUM; i++)												//обнулить не введенные значения
	{
		if (!gUsr.inp[i])
		{
			for (ubase_t j=i; j<SYMNUM; j++) gUsr.inp[j] = '0';
			gUsr.inp[SYMNUM] = 0;
			break;
		}
	}
	
	if (!arr_Asci_to_Time(gUsr.inp, time))
	{
		msg_Post(&msgW_WrongInput, MSG_TIME_TYPE, 0);
		return 0;
	}
	return 1;
}


#if USRC_USE_FULL_DATE == 1
bool usr_Input_Date(date_t* date)
//---вводит параметр дата
{
	usr_Filtr_ProcReturnFalse();												//фильтры обработки
	enum {SYMNUM = 8};
	if (!usr_Scan_InputKey(SYMNUM)) return 0;
	
	for (ubase_t i=0; i<SYMNUM; i++)												//проинициализировать не введенные значения
	{
		if (!gUsr.inp[i])
		{
			for (ubase_t j=i; j<SYMNUM; j++) gUsr.inp[j] = usr_def_date[j];
			gUsr.inp[SYMNUM] = 0;
			break;
		}
	}
	
	if (!arr_Asci_to_Date(gUsr.inp, date, TDF_FULL_YEAR))
	{
		msg_Post(&msgW_WrongInput, MSG_TIME_TYPE, 0);
		return 0;
	}
	return 1;
}

#elif USRC_USE_FULL_DATE == 0
bool usr_Input_Date(DATE_* date)
//---вводит параметр дата
{
	usr_Filtr_ProcReturnFalse();												//фильтры обработки
	enum {SYMNUM = 6};
	if (!usr_Scan_InputKey(SYMNUM)) return 0;
	for (ubase_t i=0; i<SYMNUM; i++)												//проинициализировать не введенные значения
	{
		if (!gUsr.inp[i])
		{
			for (ubase_t j=i; j<SYMNUM; j++)
			{
				if (j > 3) gUsr.inp[j] = usr_def_date[j+2];
				else gUsr.inp[j] = usr_def_date[j];
			}
			gUsr.inp[SYMNUM] = NULL;
			break;
		}
	}
	
	if (!arr_Asci_to_Date(gUsr.inp, date, TDF_SMAL_YEAR))
	{
		msg_Post(&msgW_WrongInput, MSG_TIME_TYPE, 0);
		return 0;
	}
	return 1;
}
#endif

bool usr_Input_INT32U(uint32_t* param, uint32_t min, uint32_t max, ubase_t symnum)
//---вводит параметр целого типа +
{
	usr_Filtr_ProcReturnFalse();												//фильтры обработки
	if (usr_Key == '.') return 0;
	else if (!usr_Scan_InputKey(symnum)) return 0;
	
	if (!arr_Asci_to_Uint32(gUsr.inp, param))
	{
		msg_Post(&msgW_WrongInput, MSG_TIME_TYPE, 0);
		return 0;
	}
	
	if (max < *param)															//введенный парамент больше порога
	{
		usr_temp_t var;
		var.uint32 = max;
		msg_Post(&msgW_More_INT32U, MSG_TIME_TYPE, &var);
		return 0;
	}
	else if (*param < min)														//введенный параметр меньше порога
	{
		usr_temp_t var;
		var.uint32 = min;
		msg_Post(&msgW_Less_INT32U, MSG_TIME_TYPE, &var);
		return 0;
	}
	return 1;
}


bool usr_Input_INT32S(int32_t* param, int32_t min, int32_t max, ubase_t symnum)
//---вводит параметр целого типа -+
{
	usr_Filtr_ProcReturnFalse();												//фильтры обработки
#if USRC_MINUSKEY == '.'
	if (usr_Key == '.' && gUsr.inp_pos) return 0;
#else
	if (usr_Key == '.') return 0;
#endif
	else if (!usr_Scan_InputKey(symnum)) return 0;

	if (!arr_Asci_to_Int32(gUsr.inp, param))
	{
		msg_Post(&msgW_WrongInput, MSG_TIME_TYPE, 0);
		return 0;
	}
	
	if (max < *param)															//введенный парамент больше порога
	{
		usr_temp_t var;
		var.int32 = max;
		msg_Post(&msgW_More_INT32S, MSG_TIME_TYPE, &var);
		return 0;
	}
	else if (*param < min)														//введенный параметр меньше порога
	{
		usr_temp_t var;
		var.int32 = min;
		msg_Post(&msgW_Less_INT32S, MSG_TIME_TYPE, &var);
		return 0;
	}
	return 1;
}


bool usr_Input_FL64(fl64_t* param, fl64_t min, fl64_t max, ubase_t symnum)
//---вводит параметр целого типа
{
	usr_Filtr_ProcReturnFalse();												//фильтры обработки
	
	if (!usr_Scan_InputKey(symnum)) return 0;
	if (!arr_Asci_to_Fl64(gUsr.inp, param))
	{
		msg_Post(&msgW_WrongInput, MSG_TIME_TYPE, 0);
		return 0;
	}
	
	if (max < *param)															//введенный параметр больше порога
	{
		usr_temp_t var;
		var.fl32 = max;
		msg_Post(&msgW_More_FL32, MSG_TIME_TYPE, &var);
		return 0;
	}
	else if (*param < min)														//введенный параметр меньше порога
	{
		usr_temp_t var;
		var.fl32 = min;
		msg_Post(&msgW_Less_FL32, MSG_TIME_TYPE, &var);
		return 0;
	}
	return 1;
}


#if USRC_USE_TEXT_EDITOR == 1
bool usr_Input_String(ubase_t symnum)
//---вводит стоку
{
	usr_Filtr_ProcReturnFalse();												//фильтры обработки
	if (!usr_Scan_InputTextKey(symnum)) return 0;
	return 1;
}
#endif


void usr_Proc_SymInputTimer(void)
//---обрабатывает таймер смены символа при вводе текста
{
	gUsr.prev_key = 0;
}


#if USRC_USE_TEXT_EDITOR == 1
ubase_t usr_Read_InputBufferedStr(ubase_t adr, ubase_t dsize, uint8_t* data)
//---считывает данные из введенного буфера - возвращает количество считанных данных
{
	if (adr+dsize > gUsr.inp_pos)
	{
		dsize = gUsr.inp_pos - adr + 1;
		if (!dsize) return false;
	}

#if USRC_TEXT_BUF_TYPE == ___USRC_SMALL_RAM || USRC_TEXT_BUF_TYPE == ___USRC_LARGE_RAM
	
	ubase_t realsize = 0;
	for (ubase_t i=adr; realsize < dsize; i++, realsize++)
	{
		*data++ = gUsr.inp[i];
	}
	return realsize;
	
#elif USRC_TEXT_BUF_TYPE == ___USRC_EXT
	
	if (!usr_Read_InpBufData(adr, dsize, data)) return false;
	return dsize;
	
#endif
}
#endif

//------------------------------------------------------------------------------
//внут. функции
//------------------------------------------------------------------------------

void usr_Shift_LeftInputArray(void);											//---сдвигает символы в вводном массиве в лево


bool usr_Scan_InputKey(ubase_t symnum)
//---считка вводных клавиш
{
	if (!gUsr.write)
	{
	#if USRC_USE_ENTERKEY_2 == 1
		if (usr_Key != USRC_ENTERKEY_1 && usr_Key != USRC_ENTERKEY_2) return 0;	//ввод данных не начат
	#elif USRC_USE_ENTERKEY_2 == 0
		if (usr_Key != USRC_ENTERKEY_1) return 0;								//ввод данных не начат
	#endif
		gUsr.write = 1;															//начать ввод данных
		gUsr.inp_pos = 0;
		usr_Key = 0;
		gUsr.refresh = 1;
		return 0;
	}
																				//ввод
	if (usr_Key == USRC_RETURNKEY)
	{
		if (!gUsr.inp_pos)														//все цифры стерты
		{
	#if USRC_CURSOR == ___USRC_INDICATOR_CURSOR
			dsp_Off_Cursor();
	#endif
			gUsr.write = 0;
		}
		else --gUsr.inp_pos;													//не все цифры стерты
		STM_OFF(gUsr.t_sym_input);
		gUsr.refresh = 1;
	}
	else if (usr_Key == USRC_ENTERKEY_1)
	{
		if (gUsr.inp_pos)
		{
		#if USRC_CURSOR == ___USRC_INDICATOR_CURSOR
			dsp_Off_Cursor();
		#endif
			gUsr.write = 0;
			usr_Key = 0;
			gUsr.inp[gUsr.inp_pos] = 0;
			gUsr.refresh = 1;
			return 1;
		}
	}
#if (USRC_USE_MINUSKEY == 1)
	else if (usr_Key == USRC_MINUSKEY && !gUsr.inp_pos)			////////////////
	{
		gUsr.inp[gUsr.inp_pos++] = '-';
		gUsr.refresh = 1;
	}
#endif
	else if (usr_Key == '.')
	{
		if (gUsr.inp_pos < symnum) 												//есть свободные ячейки
		{
			for (ubase_t i=0; i<gUsr.inp_pos; i++)
			{
				if (gUsr.inp[i] == '.')
				{
					usr_Key = 0;
					return 0;
				}
			}
			gUsr.inp[gUsr.inp_pos++] = '.';
			gUsr.refresh = 1;
		}
	}
	else if (usr_Key >= '0' && usr_Key <= '9')					////////////////
	{
		if (gUsr.inp_pos < symnum) 												//есть свободные ячейки
		{
			gUsr.inp[gUsr.inp_pos++] = usr_Key;
			gUsr.refresh = 1;
		}
	}
	
#if USRC_USE_SUCH_GHOTKEY_1														//глобальные горячие клавиши
	else if (usr_Key == USRC_USE_SUCH_GHOTKEY_1)
	{
		usr_Handle_HotKey_1();
	}
#endif
#if USRC_USE_SUCH_GHOTKEY_2
	else if (usr_Key == USRC_USE_SUCH_GHOTKEY_2)
	{
		usr_Handle_HotKey_2();
	}
#endif
#if USRC_USE_SUCH_GHOTKEY_3
	else if (usr_Key == USRC_USE_SUCH_GHOTKEY_3)
	{
		usr_Handle_HotKey_3();
	}
#endif
#if USRC_USE_SUCH_GHOTKEY_4
	else if (usr_Key == USRC_USE_SUCH_GHOTKEY_4)
	{
		usr_Handle_HotKey_4();
	}
#endif
#if USRC_USE_SUCH_GHOTKEY_5
	else if (usr_Key == USRC_USE_SUCH_GHOTKEY_5:
	{
		usr_Handle_HotKey_5();
	}
#endif	
	
	usr_Key = 0;
	return 0;
}


bool usr_Scan_InputTextKey(ubase_t symnum)
//---считка вводимых текстовых символов
{
	if (!gUsr.write)
	{
	#if USRC_USE_ENTERKEY_2 == 1
		if (usr_Key != USRC_ENTERKEY_1 && usr_Key != USRC_ENTERKEY_2) return 0;	//ввод данных не начат
	#elif USRC_USE_ENTERKEY_2 == 0
		if (usr_Key != USRC_ENTERKEY_1) return 0;								//ввод данных не начат
	#endif
		gUsr.write = 1;															//начать ввод данных
		gUsr.inp_pos = 0;
		usr_Key = 0;
		gUsr.refresh = 1;
		gUsr.prev_key = 0;
		return 0;
	}
																				//идет ввод
	if (gUsr.prev_key != usr_Key)												//смена клавиши
	{
		gUsr.prev_key = usr_Key;
		gUsr.click_num = 0;
	}
	else gUsr.click_num++;
	
	if (usr_Key == USRC_RETURNKEY)
	{
		if (!gUsr.inp_pos)														//все цифры стерты
		{
		#if USRC_CURSOR == ___USRC_INDICATOR_CURSOR
			dsp_Off_Cursor();
		#endif
			gUsr.write = false;
		}
		else
		{
			
#if USRC_TEXT_BUF_TYPE == ___USRC_SMALL_RAM || USRC_TEXT_BUF_TYPE == ___USRC_LARGE_RAM
			--gUsr.inp_pos;														//не все цифры стерты
#elif USRC_TEXT_BUF_TYPE == ___USRC_EXT
			usr_Cut_Sym();
#endif
		}
		
		STM_OFF(gUsr.t_sym_input);
		gUsr.refresh = true;
	}
	else if (usr_Key == USRC_ENTERKEY_1)
	{
		if (gUsr.inp_pos)
		{
		#if USRC_CURSOR == ___USRC_INDICATOR_CURSOR
			dsp_Off_Cursor();
		#endif
			gUsr.write = false;
			usr_Key = 0;
			gUsr.inp[gUsr.inp_pos] = 0;
			gUsr.refresh = true;
			return true;
		}
	}
	else if (usr_Key == '0' || usr_Key == '1')									//СПЕЦ. СИМВОЛЫ
	{
		uint8_t layout = ' ' - 1 + (usr_Key & 0x0F)*0x1A;						//определить стартовый символ
		if (gUsr.click_num)														//перекручиваем сиволы
		{
			if (gUsr.click_num > (16 - (usr_Key & 0x0F)*9))  gUsr.click_num = 0;
			else usr_Key = gUsr.click_num + layout;
		}
		else if ((gUsr.inp_pos < symnum))										//начали перекрутку символов
		{
			gUsr.inp_pos++;
			
#if USRC_TEXT_BUF_TYPE == ___USRC_EXT
			
			if (gUsr.inp_pos >= DSP_VIR_STR_LENTH)
			{
				usr_Shift_LeftInputArray();
			}
#endif
		}
		
#if USRC_TEXT_BUF_TYPE == ___USRC_SMALL_RAM || USRC_TEXT_BUF_TYPE == ___USRC_LARGE_RAM
		
		gUsr.inp[gUsr.inp_pos-1] = usr_Key;
		
#elif USRC_TEXT_BUF_TYPE == ___USRC_EXT
		
		usr_Put_Sym(symnum);
#endif
		
		STM_SET(gUsr.t_sym_input, 1.5);											//навести таймер ввода
		gUsr.refresh = true;
	}
	else if (usr_Key == '.')													//РАСКЛАДКА КЛАВИАТУРЫ
	{
		ubase_t step;
		//if (gUsr.layout == 'а'-1) gUsr.layout = 'A'-1;
		if (gUsr.layout == ASCI_a_-1) gUsr.layout = 'A'-1;
		else
		{
			if (gUsr.layout == 'a'-1) step = 0x5F;
			else step = ' ';
			gUsr.layout = gUsr.layout + step;
		}
	}
	else if (usr_Key >= '2' && usr_Key <= '9')									//СИМВОЛЫ
	{
		ubase_t dig_sym_num;
		ubase_t max_sym;
	
		if (gUsr.layout <= ('a'-1))												//латинница
		{
			dig_sym_num = 3; max_sym = (usr_Key == '9') ? 5:3;			
		}
		else if (gUsr.layout >= (ASCI_A_-1))									//кириллица
		{
			dig_sym_num = 4; max_sym = 4;
		}
		else
		{
			gUsr.layout = 'A'; dig_sym_num = 3; max_sym = 3;
		}
	
		if (gUsr.click_num)														//перекручиваем сиволы
		{
			if (gUsr.click_num > max_sym)  gUsr.click_num = 0;
			else
			{
				usr_Key = ((usr_Key & 0x0F) - 2)*dig_sym_num + gUsr.layout + gUsr.click_num;
#if USRC_LANGUAGE == ___USRC_UKR
				if (usr_Key == 'ъ' || usr_Key == 'Ъ') usr_Key = 0x27;
				else if (usr_Key == 'ы') usr_Key = 'ї';
				else if (usr_Key == 'Ы') usr_Key = 'Ї';
				else if (usr_Key == 'э') usr_Key = 'є';
				else if (usr_Key == 'Э') usr_Key = 'Є';
#endif
			}
		}
		else if (gUsr.inp_pos < symnum)											//новая позиция
		{
			gUsr.inp_pos++;
#if USRC_TEXT_BUF_TYPE == ___USRC_EXT
			
			if (gUsr.inp_pos >= DSP_VIR_STR_LENTH)								//пересортировать символы
			{
				usr_Shift_LeftInputArray();
			}
#endif
		}
		
#if USRC_TEXT_BUF_TYPE == ___USRC_SMALL_RAM || USRC_TEXT_BUF_TYPE == ___USRC_LARGE_RAM
		
		gUsr.inp[gUsr.inp_pos-1] = usr_Key;
		
#elif USRC_TEXT_BUF_TYPE == ___USRC_EXT
		
		usr_Put_Sym(symnum);
#endif
		
		STM_SET(gUsr.t_sym_input, 1.5);											//навести таймер ввода
		gUsr.refresh = true;
	}
#if USRC_USE_SUCH_GHOTKEY_1														//глобальные горячие клавиши
	else if (usr_Key == USRC_USE_SUCH_GHOTKEY_1)
	{
		usr_Handle_HotKey_1();
	}
#endif
#if USRC_USE_SUCH_GHOTKEY_2
	else if (usr_Key == USRC_USE_SUCH_GHOTKEY_2)
	{
		usr_Handle_HotKey_2();
	}
#endif
#if USRC_USE_SUCH_GHOTKEY_3
	else if (usr_Key == USRC_USE_SUCH_GHOTKEY_3)
	{
		usr_Handle_HotKey_3();
	}
#endif
#if USRC_USE_SUCH_GHOTKEY_4
	else if (usr_Key == USRC_USE_SUCH_GHOTKEY_4)
	{
		usr_Handle_HotKey_4();
	}
#endif
#if USRC_USE_SUCH_GHOTKEY_5
	else if (usr_Key == USRC_USE_SUCH_GHOTKEY_5:
	{
		usr_Handle_HotKey_5();
	}
#endif	
	usr_Key = 0;
	return false;
}


#if USRC_USE_TEXT_EDITOR == 1 && USRC_TEXT_BUF_TYPE == ___USRC_EXT				//функции для внешнего буфера
void usr_Shift_LeftInputArray(void)
//---сдвигает символы в вводном массиве в лево
{
	for	(ubase_t i=0; i < DSP_VIR_STR_LENTH-2; i++)								//пересортировать символы в ram
	{
		gUsr.inp[i] = gUsr.inp[i+1];
	}
}


void usr_Put_Sym(ubase_t symnum)
//---вводит символ
{
	if (gUsr.inp_pos < DSP_VIR_STR_LENTH) gUsr.inp[gUsr.inp_pos-1] = gUsr.key;	//записать символ в RAM
	else gUsr.inp[DSP_VIR_STR_LENTH-2] = gUsr.key;

	if (!usr_Write_InpBufSym(gUsr.inp_pos-1, gUsr.key))							//записать
	{
		#if USRC_CURSOR == ___USRC_INDICATOR_CURSOR
			dsp_Off_Cursor();
		#endif
		gUsr.write = false;
	}
}


void usr_Cut_Sym(void)
//---вырезает символ
{
	if (gUsr.inp_pos < DSP_VIR_STR_LENTH)										//все сиволы помещаються на экране
	{
		--gUsr.inp_pos;
		return;
	}
	
	for (ubase_t k=DSP_VIR_STR_LENTH-2; k; --k)
	{
		gUsr.inp[k] = gUsr.inp[k-1];
	}
	
	ubase_t newsym;
	if (!usr_Read_InpBufData(gUsr.inp_pos-DSP_VIR_STR_LENTH+2, 1, &newsym))
	{
		#if USRC_CURSOR == ___USRC_INDICATOR_CURSOR
			dsp_Off_Cursor();
		#endif
		gUsr.write = false;
	}
	gUsr.inp[0] = newsym;
	--gUsr.inp_pos;
}
#endif
