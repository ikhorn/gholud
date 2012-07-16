#ifndef ARR_H_
#define ARR_H_

#include "pdef.h"
#include "tdf.h"

//------------------------------------------------------------------------------
//								Arrays
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//format translates
//------------------------------------------------------------------------------

bool arr_Asci_to_Fl64(uint8_t *asci, fl64_t *num);								//---translate ASCI line to number of fl64_t type
bool arr_Asci_to_Uint32(uint8_t *asci, uint32_t *num);							//---translate ASCI line to number of uint32_t type
bool arr_Asci_to_Int32(uint8_t* asci, int32_t* num);							//---translate ASCI line to number of int32_t type
bool arr_Asci_to_Time(uint8_t *line, time_t *time);								//---translate ASCI line to time
bool arr_Asci_to_Date(uint8_t *line, date_t *date, ubase_t type);				//---translate ASCI line to date, type - full or not full year
void arr_Time_to_Asci(time_t *time, uint8_t *line);								//---add a time (bcd) to an ASCI line on a given position
void arr_Date_to_Asci(date_t *date, uint8_t *line, ubase_t type);				//---add a date (bcd) to an ASCI line on a given position, type - full or not full year

//------------------------------------------------------------------------------
//operations
//------------------------------------------------------------------------------

ubase_t arr_Str_Size(uint8_t* str);												//---defines a size of a string by the NUL symbol at the end
void arr_Roll_Str(uint8_t* str, ubase_t* pos, ubase_t out_size, uint8_t* out);	//---roll an array in a given line
ubase_t arr_Clear_Str(uint8_t* str, uint8_t* syms);								//---clear line "str" from symbols
ubase_t arr_Is_Sym(uint8_t* str, uint8_t sym);									//---find a symbol in a lin

#endif
