/* TC
 * id:    tm_mktime64_probe_sec_int_max
 * area:  time
 * funcs: _mktime64 _mkgmtime64
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs mktime (fields are normalised; range ends 3000-12-31)
 * note:  PROBE: tm_sec = INT_MAX at 1970-01-01 00:00:00 UTC normalises to 2147483647 (2038-01-19 03:14:07), which is
 *        within the 64-bit range, so the result is expected to be 2147483647 rather than -1.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
static volatile int vi;
TC_CASE(mktime64) { struct tm t = {0}; _putenv("TZ=UTC0"); _tzset(); t.tm_year = 70; t.tm_mday = 1; vi = INT_MAX; t.tm_sec = vi; CHECK_EQ_I(_mktime64(&t), 2147483647LL); }
TC_CASE(mkgmtime64) { struct tm t = {0}; t.tm_year = 70; t.tm_mday = 1; vi = INT_MAX; t.tm_sec = vi; CHECK_EQ_I(_mkgmtime64(&t), 2147483647LL); }
int main(void) { tc_begin(); TC_RUN(mktime64); TC_RUN(mkgmtime64); return tc_end(); }
