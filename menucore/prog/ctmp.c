#include "ctmp.h"
#include "../../dsp/dsp.h"

//------------------------------------------------------------------------------
//типы преобразования
//------------------------------------------------------------------------------

#define MODE_CELSIUS_TO_FAHRENHEIT					0
#define MODE_CELSIUS_TO_KELVIN						1
#define MODE_KELVIN_TO_CELSIUS						2
#define MODE_KELVIN_TO_FAHRENHEIT					3
#define MODE_FAHRENHEIT_TO_CELSIUS					4
#define MODE_FAHRENHEIT_TO_KELVIN					5

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

no_init_ fl64_t gCtmp_input;

//------------------------------------------------------------------------------
//внеш. функции
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//USR
//------------------------------------------------------------------------------

void ctmp_I_Input(void);
void ctmp_I_Result(void);
//массив обработчиков пунктов
usrcw_ func_t* const listCtmp[] =
{
	ctmp_I_Result,
	ctmp_I_Input
};
usr_wnd_t ctmpW = {listCtmp, list_size(listCtmp)};


void ctmp_I_Input(void)
{
	enum {INP_LENGTH = 9};
	if (USR_IS_INPUT) usr_Disp_Input(INP_LENGTH);
	else
	{
		gCtmp_input = 0;
		usr_Set_Var(uint8_t_a, MODE_CELSIUS_TO_FAHRENHEIT);
		usr_Key = USR_ENTERKEY_1;
	}

	if (usr_Input_FL64(&gCtmp_input, -999999.0, 999999.0, INP_LENGTH))
	{
		usr_Key = USR_ENTERKEY_1;
		usr_Input_FL64(&gCtmp_input, -999999.0, 999999.0, INP_LENGTH);
	}
	else
	{
		if (USR_IS_INPUT) return;												//выходим
		if (usr_WndType() > USR_WND_MENU) return;
		//usr_Key = USR_RETURNKEY;
		//menu_Navigate();
		usr_Exit();
	}
}

void ctmp_I_Result(void)
{
	fl64_t convert;
	uint8_t sym1;
	uint8_t sym2;
	switch (usr_Get_Var(uint8_t_a))
	{
		case MODE_CELSIUS_TO_KELVIN:
		{
			convert = (gCtmp_input + 273.15);
			sym1 = 'C'; sym2 = 'K';
			break;
		}
		case MODE_CELSIUS_TO_FAHRENHEIT:
		{
			convert = (gCtmp_input*1.8 + 32.0);
			sym1 = 'C'; sym2 = 'F';
			break;
		}
		case MODE_KELVIN_TO_CELSIUS:
		{
			convert = (gCtmp_input - 273.15);
			sym1 = 'K'; sym2 = 'C';
			break;
		}
		case MODE_KELVIN_TO_FAHRENHEIT:
		{
			convert = (gCtmp_input*1.8 - 459.67);
			sym1 = 'K'; sym2 = 'F';
			break;
		}
		case MODE_FAHRENHEIT_TO_CELSIUS:
		{
			convert = ((gCtmp_input - 32)/1.8);
			sym1 = 'F'; sym2 = 'C';
			break;
		}
		case MODE_FAHRENHEIT_TO_KELVIN:
		{
			convert = (gCtmp_input/1.8 + (273.15-160.0/9));
			sym1 = 'F'; sym2 = 'K';
			break;
		}
		default:
		{
			usr_Set_Var(uint8_t_a, MODE_CELSIUS_TO_KELVIN);
			convert = (gCtmp_input + 273.15);
			sym1 = 'C'; sym2 = 'K';
			break;
		}
	}
	uint8_t str[6];
	str[0] = DSP_SYM_GRADUS;
	str[1] = sym1;
	str[2] = '-';
	str[3] = DSP_SYM_GRADUS;
	str[4] = sym2;
	str[5] = 0;
	dsp_Print(gUsr.strnum, DSP_STR_LEFT_PUTTED, note_cmd_12, convert, str);
	if (usr_Key != CTMP_DIRECT_KEY) return;
	ubase_t conv_mode;
	conv_mode = usr_Get_Var(uint8_t_a);
	if (conv_mode > 4) conv_mode = 0;
	else conv_mode++;
	usr_Set_Var(uint8_t_a, conv_mode);
	usr_Key = 0;
	USR_REFRESH_DISP();
}



