/* TC
 * id:    tm_neg_strftime_l_args
 * area:  time
 * funcs: _strftime_l
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strftime (invalid parameter, EINVAL), _strftime_l
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <locale.h>

static struct tm mk(void)
{
    struct tm t; t.tm_year = 120; t.tm_mon = 0; t.tm_mday = 15; t.tm_hour = 12; t.tm_min = 30; t.tm_sec = 45;
    t.tm_wday = 3; t.tm_yday = 14; t.tm_isdst = 0; return t;
}
static char buf[64];
static char *volatile v_buf = buf;
static const char *volatile v_fmt = "%Y";
static struct tm g_tm;

TC_CASE(null_buffer) { size_t n; tc_iph_count = 0; errno = 0; n = _strftime_l(NULL, 10, v_fmt, &g_tm, NULL);
    CHECK_EQ_U(n, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_format) { size_t n; tc_iph_count = 0; errno = 0; buf[0] = 'x'; n = _strftime_l(v_buf, 64, NULL, &g_tm, NULL);
    CHECK_EQ_U(n, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_tm) { size_t n; tc_iph_count = 0; errno = 0; n = _strftime_l(v_buf, 64, v_fmt, NULL, NULL);
    CHECK_EQ_U(n, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(bad_spec) { size_t n; tc_iph_count = 0; errno = 0; n = _strftime_l(v_buf, 64, "%Q", &g_tm, NULL);
    CHECK_EQ_U(n, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(too_small) { size_t n; tc_iph_count = 0; n = _strftime_l(v_buf, 4, "%Y-%m-%d", &g_tm, NULL);
    CHECK_EQ_U(n, 0); CHECK_EQ_I(tc_iph_count, 0);
    n = _strftime_l(v_buf, 4, "%Y", &g_tm, NULL); CHECK_EQ_U(n, 0); /* 4 chars + NUL needs 5 */
    n = _strftime_l(v_buf, 5, "%Y", &g_tm, NULL); CHECK_EQ_U(n, 4); CHECK_STR(buf, "2020"); }
int main(void)
{
    tc_begin(); g_tm = mk();
    if (tc_iph_install()) { TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(null_tm); TC_RUN(bad_spec); }
    TC_RUN(too_small);
    return tc_end();
}
