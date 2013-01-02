/* Интерфейс 1 wire
 * обновлен...........................................21.12.2010 14:38:35
*/

#include "pdef.h"

//------------------------------------------------------------------------------
//порты
//------------------------------------------------------------------------------

#define WIRE_DQ_PORT			PORTC
#define WIRE_DQ_DDR			DDRC
#define WIRE_DQ_PIN			PINC
#define WIRE_DQ_B			2

//------------------------------------------------------------------------------
//commands
//------------------------------------------------------------------------------

#define WIRE_ROM_SEARCH			0xF0	/* поиск устройств */
#define WIRE_ROM_READ			0x33	/* чтение адреса одного устройства */
#define WIRE_ROM_MATCH			0x55	/* обращение к конкретному устройству */
#define WIRE_ROM_SKIP			0xCC	/* обращение ко всем устройствам */
#define WIRE_ROM_ALARM_SEARCH		0xEC	/* поиск устройств c флагом alarm */
#define WIRE_ROM_OVERDRIVE_SKIP		0x3C	/* переводит все устройства, которые поддерживают, в ускоренный режим */
#define WIRE_ROM_OVERDRIVE_MATCH	0x69	/* переводит конкретное устройство в ускоренный режим */

void wire_Ini(void);
bool wire_Start(void);
void wire_Tx_Byte(uint8_t byte);
void wire_Rx_Byte(uint8_t* byte);
bool wire_Rx_Bit(void);




