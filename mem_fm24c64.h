/* Подмодуль модуля mem для работы с памятью fm24c64
 * обновлен.....................................................................10.11.2010 15:13:30
*/

#include "fm24c64.h"

//------------------------------------------------------------------------------
//стандартные адреса памяти
//------------------------------------------------------------------------------

#define MEM_FM24C64_ADR_0						FM24C64_ADR_0					//0xA0
#define MEM_FM24C64_ADR_1						FM24C64_ADR_1					//0xA2
#define MEM_FM24C64_ADR_2						FM24C64_ADR_2					//0xA4

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------
																				//инициализировать память
#define MEM_FM24C64_INI_(CHIP)													\
{																				\
	fm24c64_Ini();																\
	FM24C64_ON_PROTECT(CHIP);													\
}

bool mem_Fm24c64_Open(uint8_t chip, dir_t dir);									//---открывает доступ к памяти
ubase_t mem_Fm24c64_Tx_Data(uint8_t* data, ubase_t dsize);						//---пишет в память, возвращает зписанное количество
ubase_t mem_Fm24c64_Rx_Data(uint8_t* data, ubase_t dsize);						//---читает из памяти, возвращает считанное кол. байт
void mem_Fm24c64_Close(dir_t dir);												//---закрывает доступ к памяти
