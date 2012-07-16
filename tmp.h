/* Модуль датчиков измерения температуры
 * проэкт.......................................................................ЖОЛУДЬ
 * обновлен.....................................................................21.12.2010 16:55:23
*/

#include "ds1631.h"
#include "ds18b20.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

#define TMP_NUM								2

//------------------------------------------------------------------------------
//прототипы
//------------------------------------------------------------------------------

#ifndef TMP
#define TMP
typedef struct
{
	int16_t code[2];
	uint8_t state_vector;
	ubase_t timer_2;
} gtmp_t;
#endif

extern gtmp_t gTmp;

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

void tmp_Ini(void);
void tmp(void);
bool tmp_Set_Use(ubase_t sens, uint8_t use_state);								//---устанавливает использвание датчика температуры
void tmp_Default(ubase_t snum);													//---записывает значения по умолчанию

#define TMP_1_TEMP_TO_CODE(TEMP, CODE)			CODE = ds1631_Temp_to_Code(TEMP)//преобразовывает температуру в код датчика 1
#define TMP_1_CODE_TO_TEMP(CODE, TEMP)			TEMP = ds1631_Code_to_Temp(CODE)//преобразовывает код датчика 1 в температуру

#define TMP_2_TEMP_TO_CODE(TEMP, CODE)			CODE = ds18b20_Temp_to_Code(TEMP)//преобразовывает температуру в код датчика 2
#define TMP_2_CODE_TO_TEMP(CODE, TEMP)			TEMP = ds18b20_Code_to_Temp(CODE)//преобразовывает код датчика 2 в температуру


#define TMP_2_SET_TMR()							STM_SET(gTmp.timer_2, 2.0);
#define TMP_2_OFF_TMR()							STM_OFF(gTmp.timer_2);

#define TMP_CHECK_TIMERS()														\
{																				\
	STM_BEGIN_ACT(gTmp.timer_2)													\
		ds18b20_Ini();															\
	STM_END_ACT																	\
}
