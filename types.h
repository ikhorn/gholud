/* Module		: types
 * Created on	: 28 сент. 2011
 * Author		: Ivan Khoronzhook
 * Firstly used	: PATIMA
 */

#ifndef TYPES_H_
#define TYPES_H_

typedef unsigned char						ubase_t;
typedef signed char							base_t;
typedef unsigned char						uint8_t;
typedef signed char							int8_t;
typedef unsigned int						uint16_t;
typedef signed int							int16_t;
typedef unsigned long int					uint32_t;
typedef signed long int						int32_t;
typedef unsigned long long int				uint64_t;
typedef signed long long int				int64_t;
typedef float 								fl32_t;
typedef double								fl64_t;
typedef void func_t(void);
typedef func_t*								funcp_t;;
typedef uint8_t								bool;
#define true			1
#define false			0


typedef uint8_t flash_						uint8_pr_t;
typedef uint8_t eeprom_						uint8_ee_t;


typedef enum
{
	MEM_WR = 0,
	MEM_RD = 1
} mem_dir_t;

typedef enum
{
	DIR_WR = 0,
	DIR_RD = 1
} dir_t;

typedef enum
{
	SP_1200 = 0,
	SP_2400 = 1,
	SP_4800 = 2,
	SP_9600 = 3,
	SP_14400 = 4,
	SP_19200 = 5,
	SP_28800 = 6,
	SP_38400 = 7,
	SP_57600 = 8,
	SP_76800 = 9,
	SP_115200 = 10,
	SP_230400 = 11,
	SP_250000 = 12,
	SP_500000 = 13,
	SP_1000000 = 14
} nspeed_t_;


#endif /* TYPES_H_ */
