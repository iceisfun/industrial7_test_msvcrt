/* TC
 * id:    tm_strftime_c_locale_composites
 * area:  time
 * funcs: strftime
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs strftime (%c, %x, %X: locale's date/time representation)
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). in the C locale ucrtbase %x = MM/DD/YY, %X = HH:MM:SS and %c = 'Sun Mar  5 07:08:09 2017' (%a %b %e %H:%M:%S %Y), which is the C11 7.27.3.5 C-locale %c; only legacy msvcrt gave 'MM/DD/YY HH:MM:SS' for %c. Test now sets setlocale(LC_ALL,"C") explicitly.
 */
#include "tc.h"
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
#include <locale.h>
#endif
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
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(c) { CHECK_STR(fmt("%c"), "Sun Mar  5 07:08:09 2017"); }
int main(void) { tc_begin(); setlocale(LC_ALL, "C"); TC_RUN(x); TC_RUN(X); TC_RUN(c); return tc_end(); }
#else
TC_CASE(c) { CHECK_STR(fmt("%c"), "03/05/17 07:08:09"); }
int main(void) { tc_begin(); TC_RUN(x); TC_RUN(X); TC_RUN(c); return tc_end(); }
#endif
