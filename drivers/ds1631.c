#include "../iic.h"
#include "../mat.h"
#include "ds1631.h"


//------------------------------------------------------------------------------
//команды
//------------------------------------------------------------------------------

#define CMD_START_CONVERT						0x51							//инициирует измерение температуры
#define CMD_STOP_CONVERT						0x22							//инициирует остановку измерения температуры
#define CMD_ACCESS_T							0xAA							//чтение температуры
#define CMD_ACCESS_TH							0xA1							//чтение или запись верхнгего порога - TH регистр
#define CMD_ACCESS_TL							0xA2							//чтение или запись нижнего порога - TL регистр
#define CMD_ACCESS_CONFIG						0xAC							//чтение или запись регистра конфигурации
#define CMD_RESET								0x54							//инициирует перезагрузку с сбиванием всех значений по умолчанию

//------------------------------------------------------------------------------
//статус регистр
//------------------------------------------------------------------------------

#define SHOT_B									0								//бит измерения постоянное/единичное
#define POL_B									1								//бит полярности выхода термостата
#define R0_B									2								//бит настройки разрешения
#define R1_B									3								//бит настройки разрешения
#define NVB_B									4								//бит занятости EEPROM
#define TLF_B									5								//бит понижения температуры ниже нижнего порога
#define THF_B									6								//бит повышения температуры выше верхнего порога
#define DONE_B									7								//бит завершения измерения температуры

#define CHK_MASK								(B(SHOT_B)|B(R0_B)|B(R1_B)|B(POL_B))

//------------------------------------------------------------------------------
//регистр конфигурации
//------------------------------------------------------------------------------

#define CONFIG									(DS1631_RES|DS1631_MEAS|DS1631_POL)

//------------------------------------------------------------------------------
//прототипы функций
//------------------------------------------------------------------------------

//bool ds1632_Waite_EEPROM_Write(void);											//---дожидается записи в EEPROM
bool ds1631_Wr_Config(void);													//---записывает регистр комуникации
bool ds1631_Recover_Config(void);												//---проверяет регистр комуникации

//------------------------------------------------------------------------------
//внешние функции
//------------------------------------------------------------------------------

bool ds1631_Ini()
//---инициализация
{
	iic_Ini();
	if (!ds1631_Wr_Config()) return 0;
	return 1;
}


bool ds1631_Rx_TempCode(int16_t* temp)
//---читает температуру в коде
{
	if (!ds1631_Recover_Config()) return 0;
	if (!iic_Open(DS1631_DEV_ADR, DIR_WR)) return 0;
	if (!iic_Tx_Byte(CMD_ACCESS_T)) return 0;
	if (!iic_Open(DS1631_DEV_ADR, DIR_RD)) return 0;
	uint8_t rg;
	iic_Rx_Byte(&rg, 1);
	*temp = rg << 8;
	iic_Rx_Byte(&rg, 0);
	iic_Close();
	*temp |= rg;
	*temp >>= 4;
	return 1;
}


bool ds1631_Rx_Temp(fl32_t* temp)
//---читает температуру
{
	int16_t t;
	if (!ds1631_Rx_TempCode(&t)) return 0;
	*temp = t*0.0625;
	return 1;
}


bool ds1631_Was_Convert(bool* is_conv)
//---проверяет или происходила конверция
{
	uint8_t config;
	if (!iic_Open(DS1631_DEV_ADR, DIR_WR)) return 0;
	if (!iic_Tx_Byte(CMD_ACCESS_CONFIG)) return 0;
	if (!iic_Open(DS1631_DEV_ADR, DIR_RD)) return 0;
	iic_Rx_Byte(&config, 0);
	iic_Close();
	if (CHKB(config, DONE_B)) *is_conv = 1;
	else *is_conv = 0;
	return 1;
}

int16_t ds1631_Temp_to_Code(fl32_t temp)
//---конвертирует температуру в код
{
	return mat_Round_FL64(temp*(fl64_t)16.0);
}


fl32_t ds1631_Code_to_Temp(int16_t code)
//---конвертирует код в температуру
{
	return (code/16.0);
}

//------------------------------------------------------------------------------
//внутренние функции
//------------------------------------------------------------------------------

bool ds1631_Poll_EepromWr(void)
//---дожидается записи в EEPROM
{
	enum {WAITE = 65000};
	for (uint16_t i=0; i<WAITE; i++)
	{
		uint8_t config;
		if (!iic_Open(DS1631_DEV_ADR, DIR_WR)) return 0;
		if (!iic_Tx_Byte(CMD_ACCESS_CONFIG)) return 0;
		if (!iic_Open(DS1631_DEV_ADR, DIR_RD)) return 0;
		iic_Rx_Byte(&config, 0);
		iic_Close();
		if (CHKB(config, NVB_B)) return 1;
	}
	return 0;
}

bool ds1631_Wr_Config(void)
//---записывает регистр комуникации
{
	if (!iic_Open(DS1631_DEV_ADR, DIR_WR)) return 0;							//остановить преобразование
	if (!iic_Tx_Byte(CMD_STOP_CONVERT)) return 0;
	iic_Close();
	
	if (!iic_Open(DS1631_DEV_ADR, DIR_WR)) return 0;							//записать регистр комуникации
	if (!iic_Tx_Byte(CMD_ACCESS_CONFIG)) return 0;
	if (!iic_Tx_Byte(CONFIG)) return 0;
	iic_Close();
	if (!ds1631_Poll_EepromWr()) return 0;
		
	uint8_t rconfig;															//проверить регистр комуникации
	if (!iic_Open(DS1631_DEV_ADR, DIR_WR)) return 0;
	if (!iic_Tx_Byte(CMD_ACCESS_CONFIG)) return 0;
	if (!iic_Open(DS1631_DEV_ADR, DIR_RD)) return 0;
	iic_Rx_Byte(&rconfig, 0);
	iic_Close();
	if (CONFIG != (rconfig & CHK_MASK)) return 0;
	
	if (!iic_Open(DS1631_DEV_ADR, DIR_WR)) return 0;							//запустить процесс преобразования
	if (!iic_Tx_Byte(CMD_START_CONVERT)) return 0;
	iic_Close();
	return 1;
}

bool ds1631_Recover_Config(void)
//---проверяет регистр комуникации
{
	uint8_t rconfig;															//проверить регистр конфигурации
	if (!iic_Open(DS1631_DEV_ADR, DIR_WR)) return 0;
	if (!iic_Tx_Byte(CMD_ACCESS_CONFIG)) return 0;
	if (!iic_Open(DS1631_DEV_ADR, DIR_RD)) return 0;
	iic_Rx_Byte(&rconfig, 0);
	iic_Close();
	if (CONFIG == (rconfig & CHK_MASK)) return 1;
	if (!ds1631_Wr_Config()) return 0;
	return 1;
}
