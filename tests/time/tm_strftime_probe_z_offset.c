/* TC
 * id:    tm_strftime_probe_z_offset
 * area:  time
 * funcs: strftime _tzset _putenv
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs strftime (%z offset from UTC, +HHMM)
 * note:  PROBES that %z gives "+0000" with TZ=UTC0 and tm_isdst 0 on ucrtbase (pre-UCRT CRTs are documented to print a zone name for %z, so they are excluded).
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

TC_CASE(offset)
{
    char buf[64]; struct tm t; char *p = (char *)&t; size_t i, n;
    for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = 117; t.tm_mon = 2; t.tm_mday = 5; t.tm_isdst = 0;
    CHECK_EQ_I(_putenv("TZ=UTC0"), 0);
    _tzset();
    n = strftime(buf, sizeof buf, "%z", &t);
    CHECK_EQ_U(n, 5);
    CHECK_STR(buf, "+0000");
}
int main(void) { tc_begin(); TC_RUN(offset); return tc_end(); }
