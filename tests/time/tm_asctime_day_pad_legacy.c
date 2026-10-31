/* TC
 * id:    tm_asctime_day_pad_legacy
 * area:  time
 * funcs: asctime _wasctime
 * crt:   <140
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs asctime
 * note:  PROBE: pre-UCRT CRTs (msvcrt..msvcr120) zero-pad the day ("May 05") rather than space-pad; recalled from real-world ctime output like "Thu Jan 01 00:00:00 1970"
 */
#include "tc.h"
#include <time.h>
static struct tm t;
static void mk(int d) { t.tm_year = 117; t.tm_mon = 4; t.tm_mday = d; t.tm_hour = 10; t.tm_min = 11; t.tm_sec = 12; t.tm_wday = 5; t.tm_yday = 0; t.tm_isdst = 0; }
TC_CASE(narrow) { mk(5); CHECK_STR(asctime(&t), "Fri May 05 10:11:12 2017\n"); }
TC_CASE(wide) { mk(5); CHECK_WSTR(_wasctime(&t), L"Fri May 05 10:11:12 2017\n"); }
int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide); return tc_end(); }
