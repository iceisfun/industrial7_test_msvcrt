/* TC
 * id:    tm_mkgmtime32_probe_max
 * area:  time
 * kind:  positive
 * funcs: _mkgmtime32
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _mkgmtime32
 * note:  PROBE: docs state the range ends 23:59:59 Jan 18 2038 UTC; probes whether the exact 32-bit maximum
 *        03:14:07 Jan 19 2038 UTC (2147483647) is accepted, and 03:14:08 rejected (-1).
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s)
{
    volatile int v = y; t->tm_year = v - 1900; t->tm_mon = mo - 1; t->tm_mday = d;
    t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_isdst = 0; t->tm_wday = 0; t->tm_yday = 0;
}
TC_CASE(int_max) { struct tm t; mk(&t, 2038, 1, 19, 3, 14, 7); CHECK_EQ_I(_mkgmtime32(&t), 2147483647); }
TC_CASE(int_max_plus1) { struct tm t; mk(&t, 2038, 1, 19, 3, 14, 8); CHECK_EQ_I(_mkgmtime32(&t), -1); }
int main(void) { tc_begin(); TC_RUN(int_max); TC_RUN(int_max_plus1); return tc_end(); }
