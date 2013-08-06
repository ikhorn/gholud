/* Модуль драйвера символьного ЖКИ индикатора для 8-ми битной шины данных BC1602E (BOLYMIN)
 * тип контроллера..............................................................KS0066
 * обновлен.....................................................................19.11.2010 16:38:35
*/

#include "../pdef.h"

//------------------------------------------------------------------------------
//порты
//------------------------------------------------------------------------------

#define BC1602E_RS_PORT								PORTC
#define BC1602E_RS_DDR								DDRC
#define BC1602E_RS_B								6

#define BC1602E_RW_PORT								PORTC
#define BC1602E_RW_DDR								DDRC
#define BC1602E_RW_B								7

#define BC1602E_EBL_PORT							PORTC
#define BC1602E_EBL_DDR								DDRC
#define BC1602E_EBL_B								4

#define BC1602E_DATA_PORT							PORTA
#define BC1602E_DATA_DDR							DDRA
#define BC1602E_DATA_PIN							PINA
#define BC1602E_DATA_6B								6
#define BC1602E_DATA_7B								7

#define BC1602E_LIGHT_PORT							PORTC
#define BC1602E_LIGHT_DDR							DDRC
#define BC1602E_LIGHT_PIN							PINC
#define BC1602E_LIGHT_B								5

//------------------------------------------------------------------------------
//команды
//------------------------------------------------------------------------------

#define BC1602E_INIT_BYTE							0x38						//байт инициализации
#define BC1602E_CLEAR_DISP      	 				0x01						//очистить весь дисплей
#define BC1602E_RETURN_HOME      					0x02						//вернуть курсор на начальную позицию
#define BC1602E_INCREMENTAC_SHIFTOFF				0x06						//счетчик курсора увеличивать без сдвига дисплея
#define BC1602E_DECREMENTAC_SHIFTOFF				0x04						//счетчик курсора уменьшать без сдвига дисплея
#define BC1602E_INCREMENTAC_SHIFTLEFT				0x07						//счетчик курсора увеличивать с сдвигом дисплея влево
#define BC1602E_DECREMENTAC_SHIFTRIGHT				0x05						//счетчик курсора уменьшать с сдвигом дисплея вправо
#define BC1602E_DISPON								0x0C						//включить дисплей
#define BC1602E_DISPOFF								0x08						//выключить дисплей
#define BC1602E_DISPON_CURSOR_ON					0x0E						//показывать курсор без мигания
#define BC1602E_DISPON_BLINKCURSOR_ON				0x0F						//показывать курсор с миганием
#define BC1602E_MOVE_CURSOR_LEFT					0x10						//сдвинуть курсор влево
#define BC1602E_MOVE_CURSOR_RIGHT					0x14						//сдвинуть курсор вправо
#define BC1602E_SHIFT_DISP_LEFT						0x18						//сдвинуть дисплей влево
#define BC1602E_SHIFT_DISP_RIGHT					0x1C						//сдвинуть дисплей вправо
#define BC1602E_DATA8_LINE2_CHAR510					0x3C
#define BC1602E_DATA8_LINE2_CHAR57					0x38
#define BC1602E_DATA8_LINE1_CHAR510					0x34
#define BC1602E_DATA8_LINE1_CHAR57					0x30						//команды для настройки шины, кол.линий и разрешения (5x7 или 5x10) символов
#define BC1602E_DATA4_LINE2_CHAR510					0x2C
#define BC1602E_DATA4_LINE2_CHAR57					0x28
#define BC1602E_DATA4_LINE1_CHAR510					0x24
#define BC1602E_DATA4_LINE1_CHAR57					0x20

//------------------------------------------------------------------------------
//константы
//------------------------------------------------------------------------------

#define BC1602E_STR_NUM								2							//количество строк
#define BC1602E_STR_LENTH							16							//длина строки

#define BC1602E_STR_START_0							0							//адрес старта строк ЖКИ
#define BC1602E_STR_START_1							64

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

void bc1602e_Ini(void);
void bc1602e_Cmd(uint8_t cmd);
void bc1602e_Set_SpecSymPos(uint8_t adr);
void bc1602e_Set_SpecSymbols(void);
void bc1602e_Set_SymPos(uint8_t adr);
void bc1602e_Set_CurrentByte(uint8_t sym);
void bc1602e_Set_Byte(uint8_t pos, uint8_t byte);
void bc1602e_Get_Sym(uint8_t *sym);
void bc1602e_Get_Pos(uint8_t *AC);

//bool bc1602e_Check(void);


static inline void bc1602e_On_Light(void) __attribute__((always_inline));
static inline void bc1602e_Off_Light(void) __attribute__((always_inline));
static inline void bc1602e_Inv_Light(void) __attribute__((always_inline));


//---включает подсветку
void bc1602e_On_Light(void)
{
	SETB(BC1602E_LIGHT_DDR, BC1602E_LIGHT_B);
	SETB(BC1602E_LIGHT_PORT, BC1602E_LIGHT_B);
}

//---выключает подсветку
void bc1602e_Off_Light(void)
{
	SETB(BC1602E_LIGHT_DDR, BC1602E_LIGHT_B);
	CLRB(BC1602E_LIGHT_PORT, BC1602E_LIGHT_B);
}

//---меняет состояние подсветки на противоположное
void bc1602e_Inv_Light(void)
{
	SETB(BC1602E_LIGHT_DDR, BC1602E_LIGHT_B);
	INVB(BC1602E_LIGHT_PORT, BC1602E_LIGHT_B);
}



