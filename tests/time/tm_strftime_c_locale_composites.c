/* TC
 * id:    tm_strftime_c_locale_composites
 * area:  time
 * funcs: strftime
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs strftime (%c, %x, %X: locale's date/time representation)
 * note:  Microsoft's C locale uses MM/DD/YY and HH:MM:SS, unlike the C standard's C-locale %c ("Sun Mar  5 07:08:09 2017"). This is pure MS behaviour.
 */
#include "tc.h"
#include <time.h>

static const char *fmt(const char *f)
{
    static char buf[64]; volatile const char *vf = f; struct tm t; char *p = (char *)&t; size_t i;
    for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = 117; t.tm_mon = 2; t.tm_mday = 5; t.tm_hour = 7; t.tm_min = 8; t.tm_sec = 9; t.tm_wday = 0; t.tm_yday = 63;
    if (strftime(buf, sizeof buf, (const char *)vf, &t) == 0) return "(zero)";
    return buf;
}
TC_CASE(x) { CHECK_STR(fmt("%x"), "03/05/17"); }
TC_CASE(X) { CHECK_STR(fmt("%X"), "07:08:09"); }
TC_CASE(c) { CHECK_STR(fmt("%c"), "03/05/17 07:08:09"); }
int main(void) { tc_begin(); TC_RUN(x); TC_RUN(X); TC_RUN(c); return tc_end(); }
