/* TC
 * id:    tm_neg_strftime_null_args
 * area:  time
 * funcs: strftime
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strftime, _strftime_l (parameter validation)
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

static char buf[32];
static volatile size_t v_sz;
TC_CASE(null_buf) { struct tm t; size_t r; mk(&t, 2020, 1, 1, 0, 0, 0, 3, 0, 0); errno = 0; tc_iph_count = 0; v_sz = 10;
    r = strftime(NULL, v_sz, "%Y", &t); CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_fmt) { struct tm t; size_t r; mk(&t, 2020, 1, 1, 0, 0, 0, 3, 0, 0); errno = 0; tc_iph_count = 0; v_sz = sizeof buf;
    r = strftime(buf, v_sz, NULL, &t); CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_tm) { size_t r; errno = 0; tc_iph_count = 0; v_sz = sizeof buf;
    r = strftime(buf, v_sz, "%Y", NULL); CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(zero_size) { struct tm t; size_t r; mk(&t, 2020, 1, 1, 0, 0, 0, 3, 0, 0); v_sz = 0; buf[0] = 'x'; tc_iph_count = 0;
    r = strftime(buf, v_sz, "%Y", &t); CHECK_EQ_U(r, 0); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_buf); TC_RUN(null_fmt); TC_RUN(null_tm); TC_RUN(zero_size); return tc_end(); }
