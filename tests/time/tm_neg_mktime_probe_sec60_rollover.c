/* TC
 * id:    tm_neg_mktime_probe_sec60_rollover
 * area:  time
 * funcs: _mktime64
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mktime64 (valid through 23:59:59 Dec 31 3000 UTC)
 * note:  PROBES whether a tm that is in range before normalisation (3000-12-31 23:59:60) but normalises to 3001-01-01 00:00:00
 *        is rejected with -1 (range check after normalisation, as the docs imply) or accepted (check on tm_year only). TZ=UTC0.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

TC_CASE(sec60_rolls_to_3001)
{
    struct tm t; memset(&t, 0, sizeof t);
    _putenv("TZ=UTC0"); _tzset();
    t.tm_year = 1100; t.tm_mon = 11; t.tm_mday = 31; t.tm_hour = 23; t.tm_min = 59; t.tm_sec = 60;
    CHECK_EQ_I(_mktime64(&t), -1);
}
int main(void) { tc_begin(); TC_RUN(sec60_rolls_to_3001); return tc_end(); }
