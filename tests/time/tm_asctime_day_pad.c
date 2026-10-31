/* TC
 * id:    tm_asctime_day_pad
 * area:  time
 * funcs: asctime _wasctime
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.27.3.1 (day is %3d: space padded); MS docs do not state it
 * note:  PROBE: whether ucrtbase asctime/_wasctime space-pad (C standard) vs zero-pad the single-digit day; MS docs silent
 */
#include "tc.h"
#include <time.h>
static struct tm t;
static void mk(int d) { t.tm_year = 117; t.tm_mon = 4; t.tm_mday = d; t.tm_hour = 10; t.tm_min = 11; t.tm_sec = 12; t.tm_wday = 5; t.tm_yday = 0; t.tm_isdst = 0; }
TC_CASE(narrow) { mk(5); CHECK_STR(asctime(&t), "Fri May  5 10:11:12 2017\n"); }
TC_CASE(wide) { mk(5); CHECK_WSTR(_wasctime(&t), L"Fri May  5 10:11:12 2017\n"); }
int main(void) { tc_begin(); TC_RUN(narrow); TC_RUN(wide); return tc_end(); }
