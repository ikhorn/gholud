#include <math.h>
#include "arr.h"
#include "clc.h"
#include "clcc.h"
#include "dsp/dsp.h"
#include "usr.h"


//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

typedef
struct
{
	uint8_t* num_start;															//указатель на начало числа в строке
	uint8_t mat_sign;																//предыдущий мат. знак
	uint8_t mult_sign;															//предыдущий знак *, /
	uint8_t func_sign;															//предыдущий знак функции
	ubase_t dot;																	//флаг ввода числа с плавающей запятой
	fl64_t temp_res;																//промежуточный результат
} CLC_;

no_init_ fl64_t gClc_res;
bool clc_Calculate_Str(uint8_t *str, fl64_t* res);									//---подсчитывает арифметический результат строки
bool clc_Operate(ubase_t oper, fl64_t *add, fl64_t* res);							//---осуществляет операцию в калькуляторе
bool clc_ChkStr(uint8_t* str);													//проверка строки на правильность ввода

#if CLCC_TYPE == ___CLCC_IMPROVED
	bool clc_Calc(CLC_ *clc, uint8_t* str, fl64_t* res);							//---вычисляет результат в калькуляторе
#elif CLCC_TYPE == ___CLCC_SIMPLE
	bool clc_Calc(INT08U_* num_start, INT08U_* str, INT08U_ prev_sign, ubase_t dot, fl64_t* res);
#endif
//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------
	
//------------------------------------------------------------------------------
//USR
//------------------------------------------------------------------------------

void clc_I_Input(void);
void clc_I_Result(void);
//массив обработчиков пунктов
funcp_pr_t listClc[] =
{
	clc_I_Result,
	clc_I_Input
};
usr_wnd_t clcW = {listClc, list_size(listClc)};



void clc_I_Input(void)
{
	enum {INP_LENGTH = 15};
	if (USR_IS_INPUT) usr_Disp_Input(INP_LENGTH);
	else
	{
		gClc_res = 0;
		usr_Key = USR_ENTERKEY_1;
	}
	
#if CLCC_TYPE == ___CLCC_IMPROVED
	
#if CLCC_USE_POW == 1
	uint8_t signs[] = {'+', '-', '/', '*', '^', 0};
#elif CLCC_USE_POW == 0
	uint8_t signs[] = {'+', '-', '/', '*', 0};
#endif
	uint8_t dot[CLCC_DOTSTR_LENGTH];
	ubase_t i = 0;
	dot[i++] = '.';
#if CLCC_USE_SQRT == 1
	dot[i++] = DSPC_SYM_SQRT;
#endif
#if CLCC_USE_SIN == 1
	dot[i++] = 'S';
#endif
#if CLCC_USE_COS == 1
	dot[i++] = 'C';
#endif
#if CLCC_USE_TAN == 1
	dot[i++] = 't';
#endif
	dot[i] = 0;

#elif CLCC_TYPE == ___CLCC_SIMPLE

#if CLCC_USE_SQRT
	uint8_t signs[] = {'+', '-', '/', '*', DSPC_SYM_SQRT, 0};
#else
	uint8_t signs[] = {'+', '-', '/', '*', 0};
#endif
	uint8_t dot[] = {'.', 0};
	
#endif //CLCC_TYPE
	
	if (usr_Key == 'c') usr_InputAdd_Sym('c', signs, INP_LENGTH);
	else if (usr_Key == '.') usr_InputAdd_Sym('.', dot, INP_LENGTH);			//ввод точки
	else if (usr_Key <= '9' && usr_Key >= '0')									//ввод цифр
	{
		fl64_t param;
		usr_Input_FL64(&param, param, param, INP_LENGTH);
		return;
	}
	else if (usr_Input_String(INP_LENGTH))										//остальной ввод
	{
		uint8_t expression[INP_LENGTH+1];
		usr_Read_InputBufferedStr(0, INP_LENGTH+1, expression);
		
		usr_Key = USR_ENTERKEY_1;												//умышленно начать ввод
		usr_Input_String(INP_LENGTH);
		
		fl64_t prev_res = gClc_res;
		if (!clc_Calculate_Str(expression, &gClc_res))
		{
			usr_temp_t var;
			msg_Post(&msgW_WrongInput, MSG_TIME_TYPE, &var);
			gClc_res = prev_res;
			return;
		}
	}
	else
	{
		if (USR_IS_INPUT) return;												//выходим
		usr_Exit();
	}
}

void clc_I_Result(void)
{
	dsp_Print(gUsr.strnum, DSP_STR_LEFT_PUTTED, note_cmd_7, gClc_res);
}

//------------------------------------------------------------------------------
//внут. функции
//------------------------------------------------------------------------------

#if CLCC_TYPE == ___CLCC_IMPROVED

bool clc_Calculate_Str(uint8_t *str, fl64_t* res)
//---подсчитывает арифметический результат строки
{
	CLC_ clc;
	clc.num_start = 0;
	clc.mat_sign = 0;
	clc.mult_sign = 0;
	clc.func_sign = 0;
	clc.dot = 0;
	clc.temp_res = 0;
	
	if (!clc_ChkStr(str)) return 0;
	if (*str == '-' || *str == '+') clc.mat_sign = *str++;
	for (; *str; str++)
	{
		if ((*str >= '0' && *str <= '9') || *str == '.' || *str == ',')			//число или запятая
		{
			if (!clc.num_start) clc.num_start = str;
			if (*str == '.' || *str == ',') clc.dot = 1;
		}
		else if (*str == '+')
		{
			if (!clc_Calc(&clc, str, res)) return 0;
			//str++;
			if (!(*(str+1)))													//последний знак операции в строке
			{
				if (clc_Operate(*str, res, res)) return 1;
				return 0;
			}
			clc.num_start = 0;
			clc.mat_sign = *str;												//назначить новый предидущий знак
		}
		else if (*str == '-')
		{
			if (!clc_Calc(&clc, str, res)) return 0;
			str--;
			if ((*str >= '0' && *str <= '9') || *str == '.' || *str == ',')		//если перед минусом число
			{
				clc.num_start = 0;												//читать как знак
				clc.mat_sign = *++str;
			}
			else clc.num_start = ++str;											//читать как отрицательное
		}
		else
		{
			if (!clc_Calc(&clc, str, res)) return 0;
			//str++;
			if (!(*(str+1))) 													//последний знак операции в строке
			{
				if (clc.temp_res) if (!clc_Operate(clc.mat_sign, &clc.temp_res, res))return 0;
				if (!clc_Operate(*str, res, res))return 0;
				return 1;
			}
			if (*str == '*' || *str == '/'
	#if CLCC_USE_POW == 1
				|| *str == '^'
	#endif
				) clc.mult_sign = *str;
			else
			{
				clc.func_sign = *str;
				if (clc.num_start) clc.mult_sign = '*';
			}
			clc.num_start = 0;
		}
	}
	if (clc_Calc(&clc, str, res)) return 1;
	return 0;
}


bool clc_Calc(CLC_ *clc, uint8_t* str, fl64_t* res)
//---вычисляет результат в калькуляторе
{
	if (!clc->num_start) return 1;

	fl64_t add = 0;																//определить add часть
	uint8_t sym = *str;
	*str = 0;
	if (clc->dot)																//число с плавающей точкой
	{
		if (!arr_Asci_to_Fl64(clc->num_start, &add)) return 0;
		clc->dot = false;
	}
	else																		//целое число
	{
		int32_t add_;
		if (!arr_Asci_to_Int32(clc->num_start, &add_)) return 0;
		add = (fl64_t)add_;
	}
	*str = sym;
	
	if (clc->func_sign)															//был предидущий знак функции
	{
		if (!clc_Operate(clc->func_sign, &add, &add)) return 0;
		clc->func_sign = 0;
	}
	if (clc->mult_sign)															//был предидущий знак высокого приоритета *,/
	{
		if (clc->temp_res) 
		{
			if (!clc_Operate(clc->mult_sign, &add, &clc->temp_res)) return 0;
		}
		else 
		{
			if (!clc_Operate(clc->mult_sign, &add, res)) return 0;
			clc->temp_res = *res;
		}
		clc->mult_sign = 0;
		add = 0;
	}
	if (*str == '+' || *str == '-' || !*str)									//знак +,- или конец строки
	{
		if (clc->temp_res)
		{
			if (!clc_Operate(clc->mat_sign, &clc->temp_res, res)) return 0;
			clc->temp_res = 0;
		}
		else if (!clc_Operate(clc->mat_sign, &add, res)) return 0;
		add = 0;
	}
	if (!clc->temp_res) clc->temp_res = add;
	return 1;
}


bool clc_Operate(ubase_t oper, fl64_t *add, fl64_t* res)
//---осуществляет операцию в калькуляторе
{
	switch (oper)
	{
		case '+': *res += *add; break;
		case '-': *res -= *add; break;
		case ':':
		case '/': *res /= *add; break;
		case '*': *res *= *add; break;
#if CLCC_USE_SQRT == 1
		case DSPC_SYM_SQRT: *res = sqrt(*add); break;
#endif
#if CLCC_USE_SIN == 1
		case 'S': *res = sin(*add); break;
#endif
#if CLCC_USE_COS == 1
		case 'C': *res = cos(*add); break;
#endif
#if CLCC_USE_TAN == 1
		case 't': *res = tan(*add); break;
#endif
#if CLCC_USE_POW == 1
		case '^': *res = pow(*res, *add); break;
#endif
		case 0: *res = *add; break;
		default: return 0;
	}
	return 1;
}

bool clc_ChkStr(uint8_t* str)
//проверка строки на правильность ввода
{
	for (; *str; str++)
	{
		if ((*str >= '0' && *str <= '9') || *str == '.' || *str == ',') continue;
		else if (*str == '+' || *str == '-')
		{
			str++;
			if (*str == '+' || *str == '-' || *str == '*' || *str == '/') return 0;
#if CLCC_USE_POW == 1
			if (*str == '^') return 0;
#endif
			str--;
		}
		else if (*str == '*' || *str == '/')
		{
			str++;
			if (*str == '+' || *str == '*' || *str == '/') return 0;
#if CLCC_USE_POW == 1
			if (*str == '^') return 0;
#endif
			str--;
		}
#if CLCC_USE_SQRT == 1
		else if (*str == DSPC_SYM_SQRT)
		{
			str++;
			if (!(*str >= '0' && *str <= '9') && *str) return 0;
			str--;
		}
#endif
#if (CLCC_USE_SIN + CLCC_USE_COS + CLCC_USE_TAN + CLCC_USE_POW) != 0
		else if (
	#if CLCC_USE_SIN == 1
				*str == 'S' ||
	#endif
	#if CLCC_USE_COS == 1
				*str == 'C' ||
	#endif
	#if CLCC_USE_TAN == 1
				*str == 't' ||
	#endif
	#if CLCC_USE_POW == 1
				*str == '^' ||
	#endif
				0)
		{
			str++;
			if (*str == '-')
			{
				str++;
				if (!(*str >= '0' && *str <= '9')) return 0;
				str--;
			}
			else if (!(*str >= '0' && *str <= '9') && *str) return 0;
			str--;
		}
#endif
	
	}
	return 1;
}


#elif CLCC_TYPE == ___CLCC_SIMPLE

bool clc_Operate(ubase_t oper, fl64_t *add, fl64_t* res)
//---осуществляет операцию в калькуляторе
{
	switch (oper)
	{
		case '+': *res += *add; break;
		case '-': *res -= *add; break;
		case ':':
		case '/': *res /= *add; break;
		case '*': *res *= *add; break;
#if CLCC_USE_SQRT == 1
		case DSPC_SYM_SQRT: *res = sqrt(*add); break;
#endif
		default: return 0;
	}
	return 1;
}

bool clc_Calc(INT08U_* num_start, INT08U_* str, INT08U_ prev_sign, ubase_t dot, fl64_t* res)
//---вычисляет результат в калькуляторе
{
	if (!num_start) return true;

	fl64_t add = 0;																//определить add часть
	if (dot)																	//число с плавающей точкой
	{
		uint8_t sym = *str;
		*str = 0;
		if (!arr_Asci_to_FL64(num_start, &add)) return 0;
		*str = sym;
		dot = false;
	}
	else																		//целое число
	{
		uint8_t sym = *str;
		*str = 0;
		INT32U_ add_;
		if (!arr_Asci_to_INT32U(num_start, &add_)) return 0;
		*str = sym;
		add = (fl64_t)add_;
	}
	
	if (prev_sign)																//был предидущий символ
	{
		if (!clc_Operate(prev_sign, &add, res)) return 0;
	}
	else *res = add;
	return 1;
}

bool clc_Calculate_Str(INT08U_ *str, fl64_t* res)
//---подсчитывает арифметический результат строки
{
	INT08U_* num_start = 0;														//указатель на начало числа в строке
	INT08U_ prev_sign = 0;														//предидущий мат. знак
	ubase_t dot = 0;																//флаг ввода числа с плавающей запятой
	
	if (*str >= '0' && *str <= '9') *res = 0;

	for (; *str; str++)
	{
		if (*str >= '0' && *str <= '9')											//число
		{
			if (!num_start) num_start = str;
		}
		else if (*str == '.' || *str == ',') dot = 1;							//запятая
		else if (*str == ' ') continue;
		else																	//знаки
		{
//			if (!num_start) *res = *prev_res;									//первый знак в стоке
			if (!clc_Calc(num_start, str, prev_sign, dot, res)) return 0;
			str++;
			if (!(*str--))														//последний знак операции в строке
			{
				if (clc_Operate(*str, res, res)) return 1;
				return 0;
			}
			
			num_start = 0;
			prev_sign = *str;													//назначить новый предидущий знак
		}
	}

	if (clc_Calc(num_start, str, prev_sign, dot, res)) return 1;
	return 0;
}
#endif //CLCC_TYPE
