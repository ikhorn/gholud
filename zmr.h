/* Модуль работы с бузером
 * обновлен.....................................................................04.12.2010 0:56:45
*/

#include "pdef.h"

//------------------------------------------------------------------------------
//сетка частот
//------------------------------------------------------------------------------

#define ZMR_031HZ							58982
#define ZMR_062HZ							29491
#define ZMR_125HZ							14746
#define ZMR_250HZ							7373
#define ZMR_500HZ							3686
#define ZMR_01KHZ							1843
#define ZMR_02KHZ							922
#define ZMR_03KHZ							614
#define ZMR_04KHZ							461
#define ZMR_08KHZ							230
#define ZMR_16KHZ							115

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

void zmr_Ini(void);
void zmr_On(uint16_t frq);														//---включает зумер на заданной частоте
void zmr_Off(void);																//---отключает зумер
