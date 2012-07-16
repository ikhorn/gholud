#include "pdef.h"
#include "tc1.h"

//------------------------------------------------------------------------------
//параметры определяющие время
//------------------------------------------------------------------------------

#define STMC_SOURCE							TC1_DIV64							//деление таймера
#define STMC_TOP							14400								//вершина сравнения таймера
#define STMC_ONECLOCK						(fl64_t)(0.25)						//время одного системного клока, с

//------------------------------------------------------------------------------
//тип таймера
//------------------------------------------------------------------------------

	#define ___STMC_ATMEGA128_TC0	0
	#define ___STMC_ATMEGA128_TC1	1
	#define ___STMC_ATMEGA128_TC2	2
	#define ___STMC_ATMEGA128_TC3	3
	#define ___STMC_ATMEGA103_TC0	4
	#define ___STMC_ATMEGA103_TC1	5
	#define ___STMC_ATMEGA103_TC2	6
	#define ___STMC_ATMEGA103_TC3	7
	
#define STMC_TIMER							___STMC_ATMEGA128_TC1
