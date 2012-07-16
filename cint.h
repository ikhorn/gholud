/* Модуль работы с внешними прерываниями
 * процессор....................................................................atmega128
 * обновлен.....................................................................14.12.2010 14:37:57
*/

//------------------------------------------------------------------------------
//рабочие регистры
//------------------------------------------------------------------------------

#define INT_MCU_CSRG							MCUCSR							//регситр статуса происхождения перезагрузки
#define INT_PORF_B								0								//флаг происхождения ресета от выключения притания
#define INT_EXTRF_B								1								//флаг происхождения ресета от RESET
#define INT_BORF_B								2								//флаг происхождения ресета от BROWN-OUT
#define INT_WDRF_B								3								//флаг происхождения ресета от WATCHDOG
#define INT_JTRF_B								4								//запрещает JTAG
#define INT_JTD_B								7

#define INT_MCU_CTRL_RG							MCUCR							//MCU регистр управления
#define INT_IVCE_B								0
#define INT_IVSEL_B								1
#define INT_SM2_B								2
#define INT_SM0_B								3
#define INT_SM1_B								4
#define INT_SE_B								5
#define INT_SRW10_B								6
#define INT_SRE_B								7

#define INT_MASK_RG								EIMSK							//регистр маски прерываний
#define INT_MASK_INT0_B							0
#define INT_MASK_INT1_B							1
#define INT_MASK_INT2_B							2
#define INT_MASK_INT3_B							3
#define INT_MASK_INT4_B							4
#define INT_MASK_INT5_B							5
#define INT_MASK_INT6_B							6
#define INT_MASK_INT7_B							7

#define INT_CTRL1_RG							EICRA							//регистр контроля прерываний (выставляет условие происхождения)
#define INT_CTRL_RG_INT0_0B						0
#define INT_CTRL_RG_INT0_1B						1
#define INT_CTRL_RG_INT1_0B						2
#define INT_CTRL_RG_INT1_1B						3
#define INT_CTRL_RG_INT2_0B						4
#define INT_CTRL_RG_INT2_1B						5
#define INT_CTRL_RG_INT3_0B						6
#define INT_CTRL_RG_INT3_1B						7

#define INT_CTRL2_RG							EICRB							//регистр контроля прерываний (выставляет условие происхождения)
#define INT_CTRL_RG_INT4_0B						0
#define INT_CTRL_RG_INT4_1B						1
#define INT_CTRL_RG_INT5_0B						2
#define INT_CTRL_RG_INT5_1B						3
#define INT_CTRL_RG_INT6_0B						4
#define INT_CTRL_RG_INT6_1B						5
#define INT_CTRL_RG_INT7_0B						6
#define INT_CTRL_RG_INT7_1B						7

#define INT_FLAG_RG								EIFR							//регистр флагов прерываний (выставляет условие происхождения)
#define INT_INT0_FL								0
#define INT_INT1_FL								1
#define INT_INT2_FL								2
#define INT_INT3_FL								3
#define INT_INT4_FL								4
#define INT_INT5_FL								5
#define INT_INT6_FL								6
#define INT_INT7_FL								7

//------------------------------------------------------------------------------
//макро функции
//------------------------------------------------------------------------------


#define INT_EBL_INT()							enable_interrupts_
#define INT_DBL_INT()							disable_interrupts_


#define INT_INT0_VECT							INT0_vect						//INT0

//---разрешить прерывание от INT0
#define INT_EBL_INT0()															\
{																				\
	SETB(INT_MASK_RG, INT_MASK_INT0_B);											\
}

//---запретить прерывание от INT0
#define INT_DBL_INT0()															\
{																				\
	CLRB(INT_MASK_RG, INT_MASK_INT0_B);											\
}

//---очищает флаг прерывания INT0
#define INT_CLR_INT0_FL()														\
{																				\
	SETB(INT_FLAG_RG, INT_INT0_FL);												\
}

//---проверяет флаг прерывания INT0
#define INT_WAS_INT0		        CHKB(INT_FLAG_RG, INT_INT0_FL)

//---настраивает тип прерывания INT0 по состоянию лог '0'
#define INT_SET_INT0_LOWLEVTYPE()												\
{																				\
	CLRB(INT_CTRL1_RG, INT_CTRL_RG_INT0_1B);									\
	CLRB(INT_CTRL1_RG, INT_CTRL_RG_INT0_0B);									\
}

//---настраивает тип прерывания INT0 по изменению
#define INT_SET_INT0_TOOGLETYPE()												\
{																				\
	CLRB(INT_CTRL1_RG, INT_CTRL_RG_INT0_1B);									\
	SETB(INT_CTRL1_RG, INT_CTRL_RG_INT0_0B);									\
}

//---настраивает тип прерывания INT0 по спадающему фронту
#define INT_SET_INT0_FALLTYPE()													\
{																				\
	SETB(INT_CTRL1_RG, INT_CTRL_RG_INT0_1B);									\
	CLRB(INT_CTRL1_RG, INT_CTRL_RG_INT0_0B);									\
}

//---настраивает тип прерывания INT0 по нарастающему фронту
#define INT_SET_INT0_RISETYPE()													\
{																				\
	SETB(INT_CTRL1_RG, INT_CTRL_RG_INT0_1B);									\
	SETB(INT_CTRL1_RG, INT_CTRL_RG_INT0_0B);									\
}


#define INT_INT1_VECT							INT1_vect						//INT1

//---разрешить прерывание от INT1
#define INT_EBL_INT1()															\
{																				\
	SETB(INT_MASK_RG, INT_MASK_INT1_B);											\
}

//---запретить прерывание от INT1
#define INT_DBL_INT1()															\
{																				\
	CLRB(INT_MASK_RG, INT_MASK_INT1_B);											\
}

//---очищает флаг прерывания INT1
#define INT_CLR_INT1_FL()														\
{																				\
	SETB(INT_FLAG_RG, INT_INT1_FL);												\
}

//---проверяет флаг прерывания INT1
#define INT_WAS_INT1		    	CHKB(INT_FLAG_RG, INT_INT1_FL)

//---настраивает тип прерывания INT1 по состоянию лог '0'
#define INT_SET_INT1_LOWLEVTYPE()												\
{																				\
	CLRB(INT_CTRL1_RG, INT_CTRL_RG_INT1_1B);									\
	CLRB(INT_CTRL1_RG, INT_CTRL_RG_INT1_0B);									\
}

//---настраивает тип прерывания INT1 по изменению
#define INT_SET_INT1_TOOGLETYPE()												\
{																				\
	CLRB(INT_CTRL1_RG, INT_CTRL_RG_INT1_1B);									\
	SETB(INT_CTRL1_RG, INT_CTRL_RG_INT1_0B);									\
}

//---настраивает тип прерывания INT1 по спадающему фронту
#define INT_SET_INT1_FALLTYPE()													\
{																				\
	SETB(INT_CTRL1_RG, INT_CTRL_RG_INT1_1B);									\
	CLRB(INT_CTRL1_RG, INT_CTRL_RG_INT1_0B);									\
}

//---настраивает тип прерывания INT1 по нарастающему фронту
#define INT_SET_INT1_RISETYPE()													\
{																				\
	SETB(INT_CTRL1_RG, INT_CTRL_RG_INT1_1B);									\
	SETB(INT_CTRL1_RG, INT_CTRL_RG_INT1_0B);									\
}


#define INT_INT2_VECT							INT2_vect						//INT2

//---разрешить прерывание от INT2
#define INT_EBL_INT2()															\
{																				\
	SETB(INT_MASK_RG, INT_MASK_INT2_B);											\
}

//---запретить прерывание от INT2
#define INT_DBL_INT2()															\
{																				\
	CLRB(INT_MASK_RG, INT_MASK_INT2_B);											\
}

//---очищает флаг прерывания INT2
#define INT_CLR_INT2_FL()														\
{																				\
	SETB(INT_FLAG_RG, INT_INT2_FL);												\
}

//---проверяет флаг прерывания INT2
#define INT_WAS_INT2		        	CHKB(INT_FLAG_RG, INT_INT2_FL)

//---настраивает тип прерывания INT2 по состоянию лог '0'
#define INT_SET_INT2_LOWLEVTYPE()												\
{																				\
	CLRB(INT_CTRL1_RG, INT_CTRL_RG_INT2_1B);									\
	CLRB(INT_CTRL1_RG, INT_CTRL_RG_INT2_0B);									\
}

//---настраивает тип прерывания INT2 по изменению
#define INT_SET_INT2_TOOGLETYPE()												\
{																				\
	CLRB(INT_CTRL1_RG, INT_CTRL_RG_INT2_1B);									\
	SETB(INT_CTRL1_RG, INT_CTRL_RG_INT2_0B);									\
}

//---настраивает тип прерывания INT2 по спадающему фронту
#define INT_SET_INT2_FALLTYPE()													\
{																				\
	SETB(INT_CTRL1_RG, INT_CTRL_RG_INT2_1B);									\
	CLRB(INT_CTRL1_RG, INT_CTRL_RG_INT2_0B);									\
}

//---настраивает тип прерывания INT2 по нарастающему фронту
#define INT_SET_INT2_RISETYPE()													\
{																				\
	SETB(INT_CTRL1_RG, INT_CTRL_RG_INT2_1B);									\
	SETB(INT_CTRL1_RG, INT_CTRL_RG_INT2_0B);									\
}


#define INT_INT4_VECT							INT4_vect						//INT4
#define INT_INT4_PORT							PORTE
#define INT_INT4_PIN							PINE
#define INT_INT4_DDR							DDRE
#define INT_INT4_B								4

//---разрешить прерывание от INT4
#define INT_EBL_INT4()															\
{																				\
	SETB(INT_MASK_RG, INT_MASK_INT4_B);											\
}

//---запретить прерывание от INT4
#define INT_DBL_INT4()															\
{																				\
	CLRB(INT_MASK_RG, INT_MASK_INT4_B);											\
}

//---проверяет флаг прерывания от INT4
#define INT_WAS_INT4					CHKB(INT_FLAG_RG, INT_INT4_FL)

//---очищает флаг прерывания INT4
#define INT_CLR_INT4_FL()														\
{																				\
	SETB(INT_FLAG_RG, INT_INT4_FL);												\
}

//---настраивает тип прерывания INT4 по состоянию лог '0'
#define INT_SET_INT4_LOWLEVTYPE()												\
{																				\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT4_1B);									\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT4_0B);									\
}

//---настраивает тип прерывания INT4 по изменению
#define INT_SET_INT4_TOOGLETYPE()												\
{																				\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT4_1B);									\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT4_0B);									\
}

//---настраивает тип прерывания INT4 по спадающему фронту
#define INT_SET_INT4_FALLTYPE()													\
{																				\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT4_1B);									\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT4_0B);									\
}

//---настраивает тип прерывания INT4 по нарастающему фронту
#define INT_SET_INT4_RISETYPE()													\
{																				\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT4_1B);									\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT4_0B);									\
}


#define INT_INT5_VECT							INT5_vect						//INT5

//---разрешить прерывание от INT5
#define INT_EBL_INT5()															\
{																				\
	SETB(INT_MASK_RG, INT_MASK_INT5_B);											\
}

//---запретить прерывание от INT5
#define INT_DBL_INT5()															\
{																				\
	CLRB(INT_MASK_RG, INT_MASK_INT5_B);											\
}

//---очищает флаг прерывания INT5
#define INT_CLR_INT5_FL()														\
{																				\
	SETB(INT_FLAG_RG, INT_INT5_FL);												\
}

//---настраивает тип прерывания INT5 по состоянию лог '0'
#define INT_SET_INT5_LOWLEVTYPE()												\
{																				\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT5_1B);									\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT5_0B);									\
}

//---настраивает тип прерывания INT5 по изменению
#define INT_SET_INT5_TOOGLETYPE()												\
{																				\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT5_1B);									\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT5_0B);									\
}

//---настраивает тип прерывания INT5 по спадающему фронту
#define INT_SET_INT5_FALLTYPE()													\
{																				\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT5_1B);									\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT5_0B);									\
}

//---настраивает тип прерывания INT5 по нарастающему фронту
#define INT_SET_INT5_RISETYPE()													\
{																				\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT5_1B);									\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT5_0B);									\
}


#define INT_INT6_VECT							INT6_vect						//INT6

//---разрешить прерывание от INT6
#define INT_EBL_INT6()															\
{																				\
	SETB(INT_MASK_RG, INT_MASK_INT6_B);											\
}

//---запретить прерывание от INT6
#define INT_DBL_INT6()															\
{																				\
	CLRB(INT_MASK_RG, INT_MASK_INT6_B);											\
}

//---очищает флаг прерывания INT6
#define INT_CLR_INT6_FL()														\
{																				\
	SETB(INT_FLAG_RG, INT_INT6_FL);												\
}

//---настраивает тип прерывания INT6 по состоянию лог '0'
#define INT_SET_INT6_LOWLEVTYPE()												\
{																				\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT6_1B);									\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT6_0B);									\
}

//---настраивает тип прерывания INT6 по изменению
#define INT_SET_INT6_TOOGLETYPE()												\
{																				\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT6_1B);									\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT6_0B);									\
}

//---настраивает тип прерывания INT6 по спадающему фронту
#define INT_SET_INT6_FALLTYPE()													\
{																				\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT6_1B);									\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT6_0B);									\
}

//---настраивает тип прерывания INT6 по нарастающему фронту
#define INT_SET_INT6_RISETYPE()													\
{																				\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT6_1B);									\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT6_0B);									\
}


#define INT_INT7_VECT							INT7_vect						//INT7
#define INT_INT7_PORT							PORTE
#define INT_INT7_PIN							PINE
#define INT_INT7_DDR							DDRE
#define INT_INT7_B								7

//---разрешить прерывание от INT7
#define INT_EBL_INT7()															\
{																				\
	SETB(INT_MASK_RG, INT_MASK_INT7_B);											\
}

//---запретить прерывание от INT7
#define INT_DBL_INT7()															\
{																				\
	CLRB(INT_MASK_RG, INT_MASK_INT7_B);											\
}

//---очищает флаг прерывания INT7
#define INT_CLR_INT7_FL()														\
{																				\
	SETB(INT_FLAG_RG, INT_INT7_FL);												\
}

//---настраивает тип прерывания INT7 по состоянию лог '0'
#define INT_SET_INT7_LOWLEVTYPE()												\
{																				\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT7_1B);									\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT7_0B);									\
}

//---настраивает тип прерывания INT7 по изменению
#define INT_SET_INT7_TOOGLETYPE()												\
{																				\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT7_1B);									\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT7_0B);									\
}

//---настраивает тип прерывания INT7 по спадающему фронту
#define INT_SET_INT7_FALLTYPE()													\
{																				\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT7_1B);									\
	CLRB(INT_CTRL2_RG, INT_CTRL_RG_INT7_0B);									\
}

//---настраивает тип прерывания INT6 по нарастающему фронту
#define INT_SET_INT7_RISETYPE()													\
{																				\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT7_1B);									\
	SETB(INT_CTRL2_RG, INT_CTRL_RG_INT7_0B);									\
}


																				//RESET

#define INT_WAS_WDT_RESET					CHKB(INT_MCU_CSRG, INT_WDRF_B)		//проверяет или был ресет от WDT
#define INT_CLR_WDT_RESET_FL()				CLRB(INT_MCU_CSRG, INT_WDRF_B)
