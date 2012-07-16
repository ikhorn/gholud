/* ������ ������� ��������
 * ���������....................................................................atmega128
 * ��������.....................................................................08.07.2010 11:11:10
*/
/*
#include "pdef.h"

//------------------------------------------------------------------------------
//�������
//------------------------------------------------------------------------------

void delay_Reduced_ms(uint32_t redvalue);										//---�������� ��� ����������� ms
void delay_Reduced_us(uint16_t redvalue);										//---�������� ��� ����������� us

//------------------------------------------------------------------------------
//����� �������
//------------------------------------------------------------------------------

#define delay_ms(X)								delay_Reduced_ms((uint32_t)(X*((fl64_t)CPU_FRQ/11000)))//����������� �������� � ��
#define delay_us(X)								delay_Reduced_us((uint16_t)((X-1)*((fl64_t)CPU_FRQ/7000000)))//����������� �������� � ���
*/
#include "pdef.h"
#include <util/delay.h>

#define delay_ms(X)					_delay_ms(X)
#define delay_us(X)					_delay_us(X)
