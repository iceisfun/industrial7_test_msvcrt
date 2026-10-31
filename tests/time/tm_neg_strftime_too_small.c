/* TC
 * id:    tm_neg_strftime_too_small
 * area:  time
 * funcs: strftime
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.27.3.5: returns 0 if the result does not fit
 * note:  Buffer contents after failure are indeterminate and not asserted.
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

TC_CASE(size10) { struct tm t; char b[32]; mk(&t, 2020, 2, 29, 1, 2, 3, 6, 59, 0);
    CHECK_EQ_U(strftime(b, 10, "%Y-%m-%d", &t), 0); }
TC_CASE(size1) { struct tm t; char b[32]; mk(&t, 2020, 2, 29, 1, 2, 3, 6, 59, 0);
    CHECK_EQ_U(strftime(b, 1, "%Y", &t), 0); }
TC_CASE(boundary) { struct tm t; char b[32]; mk(&t, 2020, 2, 29, 1, 2, 3, 6, 59, 0);
    CHECK_EQ_U(strftime(b, 11, "%Y-%m-%d", &t), 10); CHECK_EQ_U(strftime(b, 10, "%Y-%m-%d", &t), 0); }
TC_CASE(guard) { struct tm t; char b[16]; int i; mk(&t, 2020, 2, 29, 1, 2, 3, 6, 59, 0);
    for (i = 0; i < 16; i++) b[i] = 0x5a;
    CHECK_EQ_U(strftime(b, 8, "%Y-%m-%d", &t), 0);
    for (i = 8; i < 16; i++) CHECK_EQ_I(b[i], 0x5a); }
int main(void) { tc_begin(); if (TC_AT_LEAST(80)) tc_iph_install(); TC_RUN(size10); TC_RUN(size1); TC_RUN(boundary); TC_RUN(guard); return tc_end(); }
