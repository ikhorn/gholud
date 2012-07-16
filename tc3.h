/* Модуль 16-битного тамера
 * процессор....................................................................atmega128
 * номер таймера................................................................3
 * обновлен.....................................................................04.12.2010 0:21:16
*/

//------------------------------------------------------------------------------
//варианты инициализации таймера
//------------------------------------------------------------------------------
/*
																				//инициализация таймера, срабатывание по сравнению, выходи отключены, прерывания запрещены
#define TC3_INI(SOURCE, TOP)													\
{																				\
	TC3_SET_SOURCE(TC3_NO_CLOCK);												\
	TC3_MODE_CTC_OC();															\
																				\
	TC3_OFF_OUT_OCA();															\
	TC3_OFF_OUT_OCB();															\
	TC3_OFF_OUT_OCC();															\
																				\
	TC3_SET_COUNTER(0);															\
																				\
	TC3_CLR_ACOMP_INTFL();														\
	TC3_CLR_BCOMP_INTFL();														\
	TC3_CLR_CCOMP_INTFL();														\
	TC3_CLR_OVER_INTFL();														\
																				\
	TC3_DBL_ACOMP_INT();														\
	TC3_DBL_BCOMP_INT();														\
	TC3_DBL_CCOMP_INT();														\
	TC3_DBL_OVER_INT();															\
	TC3_DBL_INPCAPT_INT();														\
																				\
	TC3_SET_TOPA(TOP);															\
	TC3_SET_SOURCE(SOURCE);														\
}
*/

//------------------------------------------------------------------------------
//порты
//------------------------------------------------------------------------------

#define TC3_OCA_PORT						PORTE
#define TC3_OCA_PIN							PINE
#define TC3_OCA_DDR							DDRE
#define TC3_OCA_B							3

#define TC3_OCB_PORT						PORTE
#define TC3_OCB_PIN							PINE
#define TC3_OCB_DDR							DDRE
#define TC3_OCB_B							4

#define TC3_OCC_PORT						PORTE
#define TC3_OCC_PIN							PINE
#define TC3_OCC_DDR							DDRE
#define TC3_OCC_B							5

#define TC3_IC_PORT							PORTE
#define TC3_IC_DDR							DDRE
#define TC3_IC_PIN							PINE
#define TC3_IC_B							7

//------------------------------------------------------------------------------
//рабочие регистры
//------------------------------------------------------------------------------

#define TC3_TCNT_RG							TCNT3								//счетчик таймера
#define TC3_TCNT_RGL						TCNT3L
#define TC3_TCNT_RGH						TCNT3H

#define TC3_IC_RG							ICR3								//регистр захвата
#define TC3_IC_RGL							ICR3L
#define TC3_IC_RGH							ICR3H

#define TC3_OCA_RG							OCR3A								//регистр сравнения А
#define TC3_OCA_RGL							OCR3AL
#define TC3_OCA_RGH							OCR3AH

#define TC3_OCB_RG							OCR3B								//регистр сравнения B
#define TC3_OCB_RGL							OCR3BL
#define TC3_OCB_RGH							OCR3BH

#define TC3_OCC_RG							OCR3C								//регистр сравнения C
#define TC3_OCC_RGL							OCR3CL
#define TC3_OCC_RGH							OCR3CH

#define TC3_TIMSK_RG						TIMSK								//регистр 1 маски прерываний всех таймеров
#define TC3_OCIE2_B							7
#define TC3_TOIE2_B							6
#define TC3_TICIE1_B						5
#define TC3_OCIE1A_B						4
#define TC3_OCIE1B_B						3
#define TC3_TOIE1_B							2
#define TC3_OCIE0_B							1
#define TC3_TOIE0_B							0

#define TC3_ETIMSK_RG						ETIMSK								//регистр 2 маски прерываний всех таймеров
#define TC3_TICIE3_B						5
#define TC3_OCIE3A_B						4
#define TC3_OCIE3B_B						3
#define TC3_TOIE3_B							2
#define TC3_OCIE3C_B						1
#define TC3_OCIE1C_B						0

#define TC3_TIFR_RG							TIFR								//регистр 1 флагов прерываний всех таймеров
#define TC3_OCF2_B							7
#define TC3_TOV2_B							6
#define TC3_ICF1_B							5
#define TC3_OCF1A_B							4
#define TC3_OCF1B_B							3
#define TC3_TOV1_B							2
#define TC3_OCF0_B							1
#define TC3_TOV0_B							0

#define TC3_ETIFR_RG						ETIFR								//регистр 2 флагов прерываний всех таймеров
#define TC3_ICF3_B							5
#define TC3_OCF3A_B							4
#define TC3_OCF3B_B							3
#define TC3_TOV3_B							2
#define TC3_OCF3C_B							1
#define TC3_OCF1C_B							0

#define TC3_CTRL_A_RG						TCCR3A
#define TC3_COMA1_B							7									//бит выбора действия на выходе пина A
#define TC3_COMA0_B							6									//бит выбора действия на выходе пина A
#define TC3_COMB1_B							5									//бит выбора действия на выходе пина B
#define TC3_COMB0_B							4									//бит выбора действия на выходе пина B
#define TC3_COMC1_B							3									//бит выбора действия на выходе пина C
#define TC3_COMC0_B							2									//бит выбора действия на выходе пина C
#define TC3_WGM1_B							1									//бит установления режима
#define TC3_WGM0_B							0									//бит установления режима

#define TC3_CTRL_B_RG						TCCR3B
#define TC3_ICNC_B							7									//бит включения шумо-подавителя на входе
#define TC3_ICES_B							6									//бит выбора события по входу (спадающий или возрастающий фронт)
#define TC3_WGM3_B							4									//бит выбора режима работы таймра/счетчика
#define TC3_WGM2_B							3									//бит выбора режима работы таймра/счетчика
#define TC3_CS2_B							2									//бит выбора предделителя
#define TC3_CS1_B							1									//бит выбора предделителя
#define TC3_CS0_B							0									//бит выбора предделителя

#define TC3_CTRL_C_RG						TCCR3C
#define TC3_FOCA_B							7									//бит принудительного установления пина OCOA в соот. состояние
#define TC3_FOCB_B							6									//бит принудительного установления пина OCOB в соот. состояние
#define TC3_FOCC_B							5 									//бит принудительного установления пина OCOС в соот. состояние

//------------------------------------------------------------------------------
//варианты источника сигнала
//------------------------------------------------------------------------------

#define TC3_NO_CLOCK						0x00								//выключить вообще источник (первая обязательная операция)
#define TC3_DIV1							0x01								//настроить предделитель на /1
#define TC3_DIV8							0x02								//настроить предделитель на /8
#define TC3_DIV64							0x03								//настроить предделитель на /64
#define TC3_DIV256							0x04								//настроить предделитель на /256
#define TC3_DIV1024							0x05								//настроить предделитель на /1024
#define TC3_T0FALL							0x06								//настроить счетчик на внешний T0 по спаду
#define TC3_T0RISE							0x07								//настроить счетчик на внешний T0 по возрастанию

//------------------------------------------------------------------------------
//вектора прерываний
//------------------------------------------------------------------------------

#define TC3_COMPA_INTVECT					TIMER3_COMPA_vect
#define TC3_COMPB_INTVECT					TIMER3_COMPB_vect
#define TC3_COMPC_INTVECT					TIMER3_COMPC_vect
#define TC3_INPCAPT_INTVECT					TIMER3_CAPT_vect

//------------------------------------------------------------------------------
//функции
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//функции для работы с флагами прерываний
//------------------------------------------------------------------------------

#define TC3_CLR_ACOMP_INTFL()				SETB(TC3_ETIFR_RG, TC3_OCF3A_B)		//---очищает флаг прерывания от сравнения с рег. А
#define TC3_CLR_BCOMP_INTFL()				SETB(TC3_ETIFR_RG, TC3_OCF3B_B)		//---очищает флаг прерывания от сравнения с рег. B
#define TC3_CLR_CCOMP_INTFL()				SETB(TC3_ETIFR_RG, TC3_OCF3C_B)		//---очищает флаг прерывания от сравнения с рег. C
#define TC3_CLR_OVER_INTFL()				SETB(TC3_ETIFR_RG, TC3_TOV3_B)		//---очищает флаг от переполнения
#define TC3_CLR_INPCAPT_INTFL()				SETB(TC3_ETIFR_RG, TC3_ICF3_B)		//---очищает флаг от захвата

#define TC3_WAS_ACOMP_INT					CHKB(TC3_ETIFR_RG, TC3_OCF3A_B)
#define TC3_WAS_BCOMP_INT					CHKB(TC3_ETIFR_RG, TC3_OCF3B_B)
#define TC3_WAS_OVER_INT					CHKB(TC3_ETIFR_RG, TC3_TOV3_B)
#define TC3_WAS_INPCAPT_INT					CHKB(TC3_ETIFR_RG, TC3_ICF3_B)

//------------------------------------------------------------------------------
//функции настройки прерываний
//------------------------------------------------------------------------------

#define TC3_EBL_ACOMP_INT()					SETB(TC3_ETIMSK_RG, TC3_OCIE3A_B)	//---разрешает прерывания от сравнения с рег. A
#define TC3_DBL_ACOMP_INT()					CLRB(TC3_ETIMSK_RG, TC3_OCIE3A_B)	//---запрещает прерывания от сравнения с рег. A
#define TC3_EBL_BCOMP_INT()					SETB(TC3_ETIMSK_RG, TC3_OCIE3B_B)	//---разрешает прерывания от сравнения с рег. В
#define TC3_DBL_BCOMP_INT()					CLRB(TC3_ETIMSK_RG, TC3_OCIE3B_B)	//---запрещает прерывания от сравнения с рег. B
#define TC3_EBL_CCOMP_INT()					SETB(TC3_ETIMSK_RG, TC3_OCIE3C_B)	//---разрешает прерывания от сравнения с рег. C
#define TC3_DBL_CCOMP_INT()					CLRB(TC3_ETIMSK_RG, TC3_OCIE3C_B)	//---запрещает прерывания от сравнения с рег. C
#define TC3_EBL_OVER_INT()					SETB(TC3_ETIMSK_RG, TC3_TOIE3_B)	//---разрешает прерывания по переполнению счетчика
#define TC3_DBL_OVER_INT()					CLRB(TC3_ETIMSK_RG, TC3_TOIE3_B)	//---запрещает прерывания по переполнению счетчика
#define TC3_EBL_INPCAPT_INT()				SETB(TC3_ETIMSK_RG, TC3_TICIE3_B)	//---разрешает прерывания по входу
#define TC3_DBL_INPCAPT_INT()				CLRB(TC3_ETIMSK_RG, TC3_TICIE3_B)	//---запрещает прерывания по входу

//------------------------------------------------------------------------------
//функции настройки и считки значений
//------------------------------------------------------------------------------

																				//---настраивает счетчик
#define TC3_SET_COUNTER(X)														\
{																				\
	TC3_TCNT_RGH = (uint8_t)((X)>>8);											\
	TC3_TCNT_RGL = (uint8_t)(X);												\
}
																				//---читает счетчик
#define TC3_READ_COUNTER(X)														\
{																				\
	(X) = TC3_TCNT_RGL;															\
	(X) |= ((uint16_t)TC3_TCNT_RGH<<8);											\
}
																				//---читает регистр захвата
#define TC3_READ_ICR(X)															\
{																				\
	(X) = TC3_IC_RGL;															\
	(X) |= ((uint16_t)TC3_IC_RGH<<8);											\
}
																				//---настраивает A вершину
#define TC3_SET_TOPA(X)															\
{																				\
	TC3_OCA_RGH = (uint8_t)((X)>>8);											\
	TC3_OCA_RGL = (uint8_t)(X);													\
}
																				//---настраивает B вершину
#define TC3_SET_TOPB(X)															\
{																				\
	TC3_OCB_RGH = (uint8_t)((X)>>8);											\
	TC3_OCB_RGL = (uint8_t)(X);													\
}
																				//---настраивает C вершину
#define TC3_SET_TOPC(X)															\
{																				\
	TC3_OCC_RGH = (uint8_t)((X)>>8);											\
	TC3_OCC_RGL = (uint8_t)(X);													\
}

//------------------------------------------------------------------------------
//функции настройки источника сигнала для таймера
//------------------------------------------------------------------------------
																				//выставляет делитель тактового сигнала
#define TC3_SET_SOURCE(DIV)														\
{																				\
	TC3_CTRL_B_RG &= ~(B(TC3_CS0_B)|B(TC3_CS1_B)|B(TC3_CS2_B));					\
	TC3_CTRL_B_RG |= (DIV);														\
}

#define TC3_OFF()							TC3_SET_SOURCE(TC3_NO_CLOCK);		//выключает таймер

//------------------------------------------------------------------------------
//функции настройки действий на выходе
//------------------------------------------------------------------------------
																				//ВХОД ICA
																				//настроить захват по спадающему фронту
#define TC3_SET_INCAPT_FALLEDGE()												\
{																				\
	CLRB(TC3_IC_DDR, TC3_IC_B);													\
	CLRB(TC3_IC_PORT, TC3_IC_B);												\
	CLRB(TC3_CTRL_B_RG, TC3_ICES_B);											\
}

																				//настроить захват по наростающему фронту
#define TC3_SET_INCAPT_RISEEDGE()												\
{																				\
	CLRB(TC3_IC_DDR, TC3_IC_B);													\
	CLRB(TC3_IC_PORT, TC3_IC_B);												\
	SETB(TC3_CTRL_B_RG, TC3_ICES_B);											\
}
																				//включает шумовой фильтр по входу
#define TC3_ON_NOISECANCELER()													\
{																				\
	SETB(TC3_CTRL_B_RG, TC3_ICNC_B);											\
}

																				//отключает шумовой фильтр по входу
#define TC3_OFF_NOISECANCELER()													\
{																				\
	CLRB(TC3_CTRL_B_RG, TC3_ICNC_B);											\
}
																				//ВЫХОД OCA
																				//отключение выхода OCA
#define TC3_OFF_OUT_OCA()														\
{																				\
	CLRB(TC3_CTRL_A_RG, TC3_COMA0_B);											\
	CLRB(TC3_CTRL_A_RG, TC3_COMA1_B);											\
}

																				//настроить на переключение выход OCA
#define TC3_SET_OUT_TOGGLE_OCA()												\
{																				\
	CLRB(TC3_OCA_PORT, TC3_OCA_B);												\
	SETB(TC3_OCA_DDR, TC3_OCA_B);												\
	SETB(TC3_CTRL_A_RG, TC3_COMA0_B);											\
	CLRB(TC3_CTRL_A_RG, TC3_COMA1_B);											\
}

																				//настроить на очищение выход OCA
#define TC3_SET_OUT_CLR_OCA()													\
{																				\
	CLRB(TC3_OCA_PORT, TC3_OCA_B);												\
	SETB(TC3_OCA_DDR, TC3_OCA_B);												\
	CLRB(TC3_CTRL_A_RG, TC3_COMA0_B);											\
	SETB(TC3_CTRL_A_RG, TC3_COMA1_B);											\
}

																				//настроить на установку выход OCA
#define TC3_SET_OUT_SET_OCA()													\
{																				\
	CLRB(TC3_OCA_PORT, TC3_OCA_B);												\
	SETB(TC3_OCA_DDR, TC3_OCA_B);												\
	SETB(TC3_CTRL_A_RG, TC3_COMA0_B);											\
	SETB(TC3_CTRL_A_RG, TC3_COMA1_B);											\
}

																				//форсировать установку на выходе OCA в соот. уровень
#define TC3_FORCE_OUT_OCA()														\
{																				\
	SETB(TC3_CTRL_C_RG, TC3_FOCA_B);											\
}

																				//ВЫХОД OCB
																				//отключение выхода OCB
#define TC3_OFF_OUT_OCB()														\
{																				\
	CLRB(TC3_CTRL_A_RG, TC3_COMB0_B);											\
	CLRB(TC3_CTRL_A_RG, TC3_COMB1_B);											\
}

																				//настроить на переключение выход OCB
#define TC3_SET_OUT_TOGGLE_OCB()												\
{																				\
	CLRB(TC3_OCB_PORT, TC3_OCB_B);												\
	SETB(TC3_OCB_DDR, TC3_OCB_B);												\
	SETB(TC3_CTRL_A_RG, TC3_COMB0_B);											\
	CLRB(TC3_CTRL_A_RG, TC3_COMB1_B);											\
}

																				//настроить на очищение выход OCB
#define TC3_SET_OUT_CLR_OCB()													\
{																				\
	CLRB(TC3_OCB_PORT, TC3_OCB_B);												\
	SETB(TC3_OCB_DDR, TC3_OCB_B);												\
	CLRB(TC3_CTRL_A_RG, TC3_COMB0_B);											\
	SETB(TC3_CTRL_A_RG, TC3_COMB1_B);											\
}

																				//настроить на установку выход OCB
#define TC3_SET_OUT_SET_OCB()													\
{																				\
	CLRB(TC3_OCB_PORT, TC3_OCB_B);												\
	SETB(TC3_OCB_DDR, TC3_OCB_B);												\
	SETB(TC3_CTRL_A_RG, TC3_COMB0_B);											\
	SETB(TC3_CTRL_A_RG, TC3_COMB1_B);											\
}

																				//форсировать установку на выходе OCB в соот. уровень
#define TC3_FORCE_OUT_OCB()														\
{																				\
	SETB(TC3_CTRL_C_RG, TC3_FOCB_B);											\
}

																				//ВЫХОД OCC
																				//отключение выхода OCC
#define TC3_OFF_OUT_OCC()														\
{																				\
	CLRB(TC3_CTRL_A_RG, TC3_COMC0_B);											\
	CLRB(TC3_CTRL_A_RG, TC3_COMC1_B);											\
}

																				//настроить на переключение выход OCC
#define TC3_SET_OUT_TOGGLE_OCC()												\
{																				\
	CLRB(TC3_OCC_PORT, TC3_OCC_B);												\
	SETB(TC3_OCC_DDR, TC3_OCC_B);												\
	SETB(TC3_CTRL_A_RG, TC3_COMC0_B);											\
	CLRB(TC3_CTRL_A_RG, TC3_COMC1_B);											\
}

																				//настроить на очищение выход OCC
#define TC3_SET_OUT_CLR_OCC()													\
{																				\
	CLRB(TC3_OCC_PORT, TC3_OCC_B);												\
	SETB(TC3_OCC_DDR, TC3_OCC_B);												\
	CLRB(TC3_CTRL_A_RG, TC3_COMC0_B);											\
	SETB(TC3_CTRL_A_RG, TC3_COMC1_B);											\
}

																				//настроить на установку выход OCC
#define TC3_SET_OUT_SET_OCC()													\
{																				\
	CLRB(TC3_OCC_PORT, TC3_OCC_B);												\
	SETB(TC3_OCC_DDR, TC3_OCC_B);												\
	SETB(TC3_CTRL_A_RG, TC3_COMC0_B);											\
	SETB(TC3_CTRL_A_RG, TC3_COMC1_B);											\
}

																				//форсировать установку на выходе OCC в соот. уровень
#define TC3_FORCE_OUT_OCC()														\
{																				\
	SETB(TC3_CTRL_C_RG, TC3_FOCC_B);											\
}

//------------------------------------------------------------------------------
//функции настройки режима работы
//------------------------------------------------------------------------------
																				//нормальный режим работы
#define TC3_MODE_NORMAL()														\
{																				\
	CLRB(TC3_CTRL_A_RG, TC3_WGM0_B);											\
	CLRB(TC3_CTRL_A_RG, TC3_WGM1_B);											\
	CLRB(TC3_CTRL_B_RG, TC3_WGM2_B);											\
	CLRB(TC3_CTRL_B_RG, TC3_WGM3_B);											\
}

																				//режим сравнения с выходным регистром OCR
#define TC3_MODE_CTC_OC()														\
{																				\
	CLRB(TC3_CTRL_A_RG, TC3_WGM0_B);											\
	CLRB(TC3_CTRL_A_RG, TC3_WGM1_B);											\
	CLRB(TC3_CTRL_B_RG, TC3_WGM3_B);											\
	SETB(TC3_CTRL_B_RG, TC3_WGM2_B);											\
}

