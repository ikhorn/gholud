/* ������� MEM ������
 * ��������.....................................................................10.11.2010 17:26:09
*/

#include "memc.h"


#if MEM_USE_FM24C64 == 1
#include "mem_fm24c64.h"
#endif
#if MEM_USE_AT45DB161B == 1
#include <tmem/mem_at45db161b.h>
#endif
