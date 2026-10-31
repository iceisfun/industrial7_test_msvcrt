/* TC
 * id:    tm_neg_strftime_bad_specifier
 * area:  time
 * funcs: strftime
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs strftime: invalid format specifier invokes the invalid parameter handler
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

static char buf[64];
static void bad(const char *fmt)
{
    struct tm t; size_t r; mk(&t, 2020, 6, 15, 12, 30, 30, 1, 166, 0);
    errno = 0; tc_iph_count = 0;
    r = strftime(buf, sizeof buf, fmt, &t);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(Q) { bad("%Q"); }
TC_CASE(bang) { bad("%!"); }
TC_CASE(hash_Q) { bad("%#Q"); }
TC_CASE(after_text) { bad("abc%K"); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(Q); TC_RUN(bang); TC_RUN(hash_Q); TC_RUN(after_text); return tc_end(); }
