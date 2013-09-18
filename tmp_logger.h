/*
 * Temperature logger, used to collect data change for a month
 */

#ifndef TMP_LOGGER
#define TMP_LOGGER

#include "bcd.h"
#include "tdf.h"
#include "wbuf.h"

/* points that indication is not logged */
#define TMPLOG_NOT_LOGGED		0

#define TMPLOG_DAY_COUNT		7
#define TMPLOG_WEEK_COUNT		4
#define TMPLOG_MONTH_COUNT		2

/*
 * struct to keep data for one indication, needs for statistic
 * date - date of indication
 * month - month of indication
 * hour - hour of indication
 * min - minutes of indication
 * tmp_code - code of temperature (needs to be converted to see)
 */
struct tmp_indication_day
{
	bcd_t hour;
	bcd_t min;
	int16_t value;
};

struct tmp_indication_week
{
	bcd_t date;
	bcd_t month;
	bcd_t hour;
	bcd_t min;
	int16_t value;
};

struct tmp_indication_month
{
	bcd_t date;
	bcd_t hour;
	bcd_t min;
	int16_t value;
};

/*
 * struct to keep data for statistic of one day
 */
struct tmp_day
{
	bcd_t date;
	bcd_t month;
	struct tmp_indication_day max;
	struct tmp_indication_day min;
};

/*
 * struct to keep data for statistic of one week
 */
struct tmp_week
{
	struct tmp_indication_week max;
	struct tmp_indication_week min;
};

/*
 * struct to keep data for statistic of one month
 */
struct tmp_month
{
	bcd_t month;
	struct tmp_indication_month max;
	struct tmp_indication_month min;
};

/*
 * collect data for temperature log
 */
struct tmp_logger
{
	bcd_t start_date;
	bcd_t start_month;
	time_t *time;
	date_t *date;
	int16_t *value;
	WBUF_DECLARE(struct tmp_day, TMPLOG_DAY_COUNT, last_days);
	WBUF_DECLARE(struct tmp_week, TMPLOG_WEEK_COUNT, last_weeks);
	WBUF_DECLARE(struct tmp_month, TMPLOG_MONTH_COUNT, last_monthes);
};

void tmplog_init(struct tmp_logger *logger, time_t *time, date_t *date, int16_t* value);
void tmplog_log(struct tmp_logger *logger);

#endif
