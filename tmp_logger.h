/*
 * Temperature logger, used to collect data change for a month
 */

#ifndef TMP_LOGGER
#define TMP_LOGGER

#include "bcd.h"
#include "tdf.h"

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
struct tmp_indication
{
	bcd_t date;
	bcd_t month;
	bcd_t hour;
	bcd_t min;
	int16_t value;
};

struct tmp_indication_day
{
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
	struct tmp_indication max;
	struct tmp_indication min;
};

/*
 * struct to keep data for statistic of one month
 */
struct tmp_month
{
	struct tmp_indication max;
	struct tmp_indication min;
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
	struct tmp_day last_days[TMPLOG_DAY_COUNT];
	struct tmp_week last_weeks[TMPLOG_WEEK_COUNT];
	struct tmp_month last_month[TMPLOG_MONTH_COUNT];
};

void tmplog_init(struct tmp_logger *logger, time_t *time, date_t *date, int16_t* value);
void tmplog_log(struct tmp_logger *logger);

#endif
