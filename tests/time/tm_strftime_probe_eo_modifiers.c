/* TC
 * id:    tm_strftime_probe_eo_modifiers
 * area:  time
 * funcs: strftime
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs strftime (E and O modifiers are listed for UCRT)
 * note:  PROBES that UCRT accepts the E and O modifiers in the C locale and yields the base conversion.
 */
#include "tc.h"
#include <time.h>
#include <wchar.h>
#include <errno.h>
#include <stdlib.h>

static void mk(struct tm *t, int Y, int Mo, int D, int h, int m, int s, int wd, int yd, int dst)
{
    t->tm_year = Y - 1900; t->tm_mon = Mo - 1; t->tm_mday = D; t->tm_hour = h; t->tm_min = m; t->tm_sec = s;
    t->tm_wday = wd; t->tm_yday = yd; t->tm_isdst = dst;
}

static void both(const char *base, const char *mod, struct tm *t)
{
    char a[128], b[128]; size_t na, nb;
    na = strftime(a, sizeof a, base, t); nb = strftime(b, sizeof b, mod, t);
    CHECK(na > 0); CHECK_EQ_U(nb, na); CHECK_STR(b, a);
}
TC_CASE(Ex) { struct tm t; mk(&t, 2021, 3, 9, 14, 5, 7, 2, 67, 0); both("%x", "%Ex", &t); }
TC_CASE(EX) { struct tm t; mk(&t, 2021, 3, 9, 14, 5, 7, 2, 67, 0); both("%X", "%EX", &t); }
TC_CASE(Ec) { struct tm t; mk(&t, 2021, 3, 9, 14, 5, 7, 2, 67, 0); both("%c", "%Ec", &t); }
TC_CASE(others) { struct tm t; mk(&t, 2021, 3, 9, 14, 5, 7, 2, 67, 0);
    both("%y", "%Ey", &t); both("%Y", "%EY", &t); both("%d", "%Od", &t); both("%H", "%OH", &t); }
int main(void) { tc_begin(); TC_RUN(Ex); TC_RUN(EX); TC_RUN(Ec); TC_RUN(others); return tc_end(); }
