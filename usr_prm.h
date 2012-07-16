/* Подмодуль prm модуля usr
 * обновлен.....................................................................03.12.2010 17:54:04
*/


/*
//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

extern usrcs_ const uint8_t note_input[];

//------------------------------------------------------------------------------
//								КАЛЬКУЛЯТОР
//------------------------------------------------------------------------------

void prm_Handle_CalculateString(void);
void prm_Handle_CalculateResult(void);
usrcw_ func_t* listCalculator[] = 
{
	prm_Handle_CalculateResult,
	prm_Handle_CalculateString
};
USR_WND_ prmW_Calculator = {listCalculator, list_size(listCalculator)};


void prm_Handle_CalculateString(void)
{
	enum {INP_LENGTH = 15};
	
	MENU_IS_INPUT ?				usr_Disp_Input(INP_LENGTH):
								usr_Disp_Str(note_input);
			
	if (!MENU_IS_INPUT)
	{
		usr_Set_Var(FL32, 0);
		usr_Set_ExeVar(INT32U, 0);
		usr_Key = USRC_ENTERKEY_1;
	}
	
	if ((usr_Key <= '9' && usr_Key >= '2') || usr_Key == '.')					//ввод цифр
	{
		fl64_t param;
		menu_Input_FL64(&param, param, param, INP_LENGTH);
		return;
	}
	else if (!menu_Input_String(INP_LENGTH))									//остальной ввод
	{
		if (!MENU_IS_INPUT)														//выходим
		{
			usr_Key = USRC_RETURNKEY;
			menu_Navigate();
		}
		return;
	}

	uint8_t expression[15];
	menu_Read_InputBufferedStr(0, 15, expression);
	
	usr_Key = USRC_ENTERKEY_1;
	menu_Input_String(INP_LENGTH);
	
	union
	{
		struct
		{
			uint32_t hi_tet;
			uint32_t lo_tet;
		};
		fl64_t res;
	};
	
	hi_tet = usr_Get_Var(INT32U);
	lo_tet = usr_Get_ExeVar(INT32U);
	
	fl64_t prev_res = res;
	
	if (!arr_Calculate_Str(expression, &res))
	{
		USR_TEMP_ var;
		msg_Post(&msgW_WrongInput, MSG_TIME_TYPE, &var);
		res = prev_res;
		return;
	}

	usr_Set_Var(INT32U, hi_tet);
	usr_Set_ExeVar(INT32U, lo_tet);
}


void prm_Handle_CalculateResult(void)
{
	union
	{
		struct
		{
			uint32_t hi_tet;
			uint32_t lo_tet;
		};
		fl64_t res;
	};
	hi_tet = usr_Get_Var(INT32U);
	lo_tet = usr_Get_ExeVar(INT32U);
	
	dsp_Print(gUsr.strnum, DSP_STR_LEFT_PUTTED, note_cmd_7, res);
}


//------------------------------------------------------------------------------
//						КОНВЕРТОР
//------------------------------------------------------------------------------
*/




