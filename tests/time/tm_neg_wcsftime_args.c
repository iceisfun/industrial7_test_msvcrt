/* TC
 * id:    tm_neg_wcsftime_args
 * area:  time
 * funcs: wcsftime
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcsftime (parameter validation)
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

static wchar_t buf[32];
static volatile size_t v_sz;
static struct tm T;
static void inv(size_t r) { CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_buf) { mk(&T, 2020, 6, 15, 12, 0, 0, 1, 166, 0); errno = 0; tc_iph_count = 0; v_sz = 10; inv(wcsftime(NULL, v_sz, L"%Y", &T)); }
TC_CASE(null_fmt) { mk(&T, 2020, 6, 15, 12, 0, 0, 1, 166, 0); errno = 0; tc_iph_count = 0; v_sz = 32; inv(wcsftime(buf, v_sz, NULL, &T)); }
TC_CASE(null_tm) { errno = 0; tc_iph_count = 0; v_sz = 32; inv(wcsftime(buf, v_sz, L"%Y", NULL)); }
TC_CASE(zero_size) { mk(&T, 2020, 6, 15, 12, 0, 0, 1, 166, 0); v_sz = 0; CHECK_EQ_U(wcsftime(buf, v_sz, L"%Y", &T), 0); }
TC_CASE(too_small) { mk(&T, 2020, 6, 15, 12, 0, 0, 1, 166, 0); v_sz = 4; CHECK_EQ_U(wcsftime(buf, v_sz, L"%Y", &T), 0); }
TC_CASE(bad_spec) { mk(&T, 2020, 6, 15, 12, 0, 0, 1, 166, 0); errno = 0; tc_iph_count = 0; v_sz = 32; inv(wcsftime(buf, v_sz, L"%Q", &T)); }
TC_CASE(bad_mon) { mk(&T, 2020, 6, 15, 12, 0, 0, 1, 166, 0); T.tm_mon = 12; errno = 0; tc_iph_count = 0; v_sz = 32; inv(wcsftime(buf, v_sz, L"%b", &T)); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_buf); TC_RUN(null_fmt); TC_RUN(null_tm); TC_RUN(zero_size); TC_RUN(too_small); TC_RUN(bad_spec); TC_RUN(bad_mon); return tc_end(); }
