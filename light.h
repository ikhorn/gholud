/* Модуль работы с подсветкой
 * обновлен.....................................................................05.12.2010 20:28:11
*/

#include "pdef.h"

typedef struct
{
	uint8_t def_state;
	uint8_t state;
} gLIGHT_;

extern gLIGHT_ gLight;


//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

void light_Ini(void);
void light_Toggle(void);														//---переключает состояние подсветки
bool light_Set_DefState(uint8_t def_state);										//---устанавливает состояние подсветки по умолчанию
void light_Default(void);														//---устанавливает значение по умолчанию


