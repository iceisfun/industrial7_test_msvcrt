/* TC
 * id:    tm_mkgmtime64_probe_fields
 * area:  time
 * kind:  positive
 * funcs: _mkgmtime64
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _mkgmtime64
 * note:  PROBE: (1) input tm_isdst is ignored by _mkgmtime64 (UTC conversion), (2) after the call the struct is
 *        written back normalized (year/mon/mday/hour/min, tm_wday, tm_yday) and tm_isdst is 0 even if input was 1.
 *        The MS page does not spell this out for _mkgmtime; believed true of the implementation.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

static void settz(const char *s) { _putenv(s); _tzset(); }
static void mk(struct tm *t, int y, int mo, int d, int h, int mi, int s, int dst)
{
    volatile int v = y; t->tm_year = v - 1900; t->tm_mon = mo - 1; t->tm_mday = d;
    t->tm_hour = h; t->tm_min = mi; t->tm_sec = s; t->tm_isdst = dst; t->tm_wday = 0; t->tm_yday = 0;
}

TC_CASE(isdst_input_ignored)
{
    struct tm a, b; settz("TZ=PST8PDT");
    mk(&a, 2000, 7, 1, 12, 0, 0, 1); mk(&b, 2000, 7, 1, 12, 0, 0, 0);
    CHECK_EQ_I(_mkgmtime64(&a), 962452800); CHECK_EQ_I(_mkgmtime64(&b), 962452800);
}
TC_CASE(month_12)
{
    struct tm t; mk(&t, 1999, 1, 1, 0, 0, 0, 1); t.tm_mon = 12;
    CHECK_EQ_I(_mkgmtime64(&t), 946684800);
    CHECK_EQ_I(t.tm_year, 100); CHECK_EQ_I(t.tm_mon, 0); CHECK_EQ_I(t.tm_mday, 1);
    CHECK_EQ_I(t.tm_wday, 6); CHECK_EQ_I(t.tm_yday, 0); CHECK_EQ_I(t.tm_isdst, 0);
}
TC_CASE(hour_48)
{
    struct tm t; mk(&t, 2000, 1, 1, 48, 0, 0, 0);
    CHECK_EQ_I(_mkgmtime64(&t), 946857600);
    CHECK_EQ_I(t.tm_mday, 3); CHECK_EQ_I(t.tm_hour, 0); CHECK_EQ_I(t.tm_wday, 1); CHECK_EQ_I(t.tm_yday, 2);
}
TC_CASE(mday_0)
{
    struct tm t; mk(&t, 2000, 3, 0, 0, 0, 0, 0);
    CHECK_EQ_I(_mkgmtime64(&t), 951782400);
    CHECK_EQ_I(t.tm_mon, 1); CHECK_EQ_I(t.tm_mday, 29); CHECK_EQ_I(t.tm_wday, 2); CHECK_EQ_I(t.tm_yday, 59);
}
TC_CASE(minute_neg)
{
    struct tm t; mk(&t, 2000, 1, 1, 0, 0, 0, 0); t.tm_min = -1;
    CHECK_EQ_I(_mkgmtime64(&t), 946684740);
    CHECK_EQ_I(t.tm_year, 99); CHECK_EQ_I(t.tm_mon, 11); CHECK_EQ_I(t.tm_mday, 31);
    CHECK_EQ_I(t.tm_hour, 23); CHECK_EQ_I(t.tm_min, 59); CHECK_EQ_I(t.tm_wday, 5); CHECK_EQ_I(t.tm_yday, 364);
}
int main(void) { tc_begin(); TC_RUN(isdst_input_ignored); TC_RUN(month_12); TC_RUN(hour_48); TC_RUN(mday_0); TC_RUN(minute_neg); settz("TZ="); return tc_end(); }
