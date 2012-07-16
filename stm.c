#include "stm.h"


#if (STMC_TIMER == ___STMC_ATMEGA128_TC0)
#include <timers/stm_ATMEGA128_tc0.h>

#elif(STMC_TIMER == ___STMC_ATMEGA128_TC1)
#include "stm_ATMEGA128_tc1.h"

#elif(STMC_TIMER == ___STMC_ATMEGA128_TC2)
#include <timers/stm_ATMEGA128_tc2.h>

#elif(STMC_TIMER == ___STMC_ATMEGA128_TC3)
#include <timers/stm_ATMEGA128_tc3.h>

#endif





