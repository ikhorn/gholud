#include "../crc.h"
#include "../mat.h"
#include "../wire.h"
#include "ds18b20.h"

//------------------------------------------------------------------------------
//������� ������������
//------------------------------------------------------------------------------

#define CONF_R0									5
#define CONF_R1									6

//------------------------------------------------------------------------------
//�������������� ��������
//------------------------------------------------------------------------------

#define FUNC_CONVERT_T							0x44
#define FUNC_WRITE_SCRATCHPAD					0x4E
#define FUNC_READ_SCRATCHPAD					0xBE
#define FUNC_COPY_SCRATCHPAD					0x48
#define FUNC_RECALL_E2							0xB8
#define FUNC_READ_POWER_SUPPLY					0xB4

//------------------------------------------------------------------------------
//��������� �����������
//------------------------------------------------------------------------------

#define TH										0x7D							// +125
#define TL										0xC9							// -55

//------------------------------------------------------------------------------
//���������
//------------------------------------------------------------------------------

void ds18b20_Rx_Data(uint8_t* data, ubase_t dsize);								//---��������� ������
void ds18b20_Tx_Data(uint8_t* data, ubase_t dsize);								//---�������� ������
bool ds18b20_Func(uint8_t func);												//---�������� �������
bool ds18b20_Rd_Scratchpad(uint8_t* srp);										//---������ scratchpad �������

//------------------------------------------------------------------------------
//����. �������
//------------------------------------------------------------------------------

bool ds18b20_Ini(void)
//---������������� DS18B20
{
	wire_Ini();
	
	uint8_t wscp[3];															//�������� ��������
	if (!ds18b20_Func(FUNC_WRITE_SCRATCHPAD)) return 0;
	wscp[0] = TH;
	wscp[1] = TL;
	wscp[2] = DS18B20_RES;
	ds18b20_Tx_Data(wscp, 3);
	
	uint8_t rscp[9];															//��������� ��������
	if (!ds18b20_Rd_Scratchpad(rscp)) return 0;
	if (rscp[2] != TH) return 0;
	if (rscp[3] != TL) return 0;
	if ((rscp[4] & (B(CONF_R0)|B(CONF_R1))) != DS18B20_RES) return 0;
	
	if (!ds18b20_Func(FUNC_COPY_SCRATCHPAD)) return 0;							//��������� ������� ����������� ���������� � E2
	if (!ds18b20_Start_Convert()) return 0;
	return 1;
}

bool ds18b20_Start_Convert(void)
//---��������� ������� ��������������
{
	return ds18b20_Func(FUNC_CONVERT_T);
}

bool ds18b20_Convert_Is_Done(void)
//---��������� ��� ����������� ������ �������
{
	return wire_Rx_Bit();
}

bool ds18b20_Rd_Temp(int16_t* code)
//---������ �����������
{
	uint8_t data[9];
	if (!ds18b20_Rd_Scratchpad(data)) return 0;
	if ((data[4] & (B(CONF_R0)|B(CONF_R1))) != DS18B20_RES) return 0;
	*code = data[1] << 8;
	*code |= data[0];
	return 1;
}

int16_t ds18b20_Temp_to_Code(fl32_t temp)
//---������������ ����������� � ���
{
	return mat_Round_FL64(temp*(fl64_t)16.0);
}


fl32_t ds18b20_Code_to_Temp(int16_t code)
//---������������ ��� � �����������
{
	return (code/16.0);
}

//------------------------------------------------------------------------------
//����. �������
//------------------------------------------------------------------------------

void ds18b20_Rx_Data(uint8_t* data, ubase_t dsize)
//---��������� ������
{
	for (ubase_t i=0; i<dsize; i++)
	{
		wire_Rx_Byte(data++);
	}
}

void ds18b20_Tx_Data(uint8_t* data, ubase_t dsize)
//---�������� ������
{
	for (ubase_t i=0; i<dsize; i++)
	{
		wire_Tx_Byte(*data++);
	}
}

bool ds18b20_Func(uint8_t func)
//---�������� �������
{
	uint8_t data[2];
	if (!wire_Start()) return 0;
	data[0] = WIRE_ROM_SKIP;
	data[1] = func;
	ds18b20_Tx_Data(data, 2);
	return 1;
}

bool ds18b20_Rd_Scratchpad(uint8_t* srp)
//---������ scratchpad �������
{
	if (!wire_Start()) return 0;												//��������� ��������
	if (!ds18b20_Func(FUNC_READ_SCRATCHPAD)) return 0;
	ds18b20_Rx_Data(srp, 9);
	if (crc_Count_Crc8(srp, 8) != srp[8]) return 0;								//��������� crc
	return 1;
}
