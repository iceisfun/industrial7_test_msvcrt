/* TC
 * id:    tm_strftime_return_and_fit
 * area:  time
 * funcs: strftime
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.27.3.5 strftime; MS docs strftime
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

TC_CASE(count) { struct tm t; char b[64]; mk(&t, 2020, 2, 29, 1, 2, 3, 6, 59, 0);
    CHECK_EQ_U(strftime(b, sizeof b, "%Y-%m-%d", &t), 10); CHECK_STR(b, "2020-02-29"); }
TC_CASE(exact_fit) { struct tm t; char b[11]; mk(&t, 2020, 2, 29, 1, 2, 3, 6, 59, 0); b[10] = 'x';
    CHECK_EQ_U(strftime(b, 11, "%Y-%m-%d", &t), 10); CHECK_EQ_I(b[10], 0); CHECK_STR(b, "2020-02-29"); }
TC_CASE(empty) { struct tm t; char b[8]; mk(&t, 2020, 2, 29, 1, 2, 3, 6, 59, 0); b[0] = 'x';
    CHECK_EQ_U(strftime(b, sizeof b, "", &t), 0); CHECK_EQ_I(b[0], 0); }
TC_CASE(verbatim) { struct tm t; char b[32]; mk(&t, 2020, 2, 29, 1, 2, 3, 6, 59, 0);
    CHECK_EQ_U(strftime(b, sizeof b, "ab c\xe9\xff%%", &t), 7); CHECK_STR(b, "ab c\xe9\xff%"); }
TC_CASE(guard) { struct tm t; char b[24]; int i; mk(&t, 2020, 2, 29, 1, 2, 3, 6, 59, 0);
    for (i = 0; i < 24; i++) b[i] = 0x5a;
    CHECK_EQ_U(strftime(b + 4, 11, "%Y-%m-%d", &t), 10);
    for (i = 0; i < 4; i++) CHECK_EQ_I(b[i], 0x5a);
    for (i = 15; i < 24; i++) CHECK_EQ_I(b[i], 0x5a); }
int main(void) { tc_begin(); TC_RUN(count); TC_RUN(exact_fit); TC_RUN(empty); TC_RUN(verbatim); TC_RUN(guard); return tc_end(); }
