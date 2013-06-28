#ifndef ARR_H_
#define ARR_H_

#include "pdef.h"
#include "tdf.h"

//------------------------------------------------------------------------------
//format translates
//------------------------------------------------------------------------------

bool arr_Asci_to_Fl64(uint8_t *asci, fl64_t *num);
bool arr_Asci_to_Uint32(uint8_t *asci, uint32_t *num);
bool arr_Asci_to_Int32(uint8_t* asci, int32_t* num);
bool arr_Asci_to_Time(uint8_t *line, time_t *time);
bool arr_Asci_to_Date(uint8_t *line, date_t *date, ubase_t type);
void arr_Time_to_Asci(time_t *time, uint8_t *line);
void arr_Date_to_Asci(date_t *date, uint8_t *line, ubase_t type);

//------------------------------------------------------------------------------
//operations
//------------------------------------------------------------------------------

ubase_t arr_Str_Size(uint8_t* str);
void arr_Roll_Str(uint8_t* str, ubase_t* pos, ubase_t out_size, uint8_t* out);
ubase_t arr_Clear_Str(uint8_t* str, uint8_t* syms);
ubase_t arr_Is_Sym(uint8_t* str, uint8_t sym);

#endif
