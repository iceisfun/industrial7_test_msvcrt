/* TC
 * id:    tm_neg_wcsftime_l_args
 * area:  time
 * funcs: _wcsftime_l
 * crt:   >=90
 * tier:  p2
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcsftime (invalid parameter, EINVAL), _wcsftime_l
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <wchar.h>

static struct tm g_tm;
static wchar_t buf[64];
static wchar_t *volatile v_buf = buf;
static const wchar_t *volatile v_fmt = L"%Y";

TC_CASE(null_buffer) { size_t n; tc_iph_count = 0; errno = 0; n = _wcsftime_l(NULL, 10, v_fmt, &g_tm, NULL);
    CHECK_EQ_U(n, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_tm) { size_t n; tc_iph_count = 0; errno = 0; n = _wcsftime_l(v_buf, 64, v_fmt, NULL, NULL);
    CHECK_EQ_U(n, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(bad_spec) { size_t n; tc_iph_count = 0; errno = 0; n = _wcsftime_l(v_buf, 64, L"%Q", &g_tm, NULL);
    CHECK_EQ_U(n, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(too_small) { size_t n; tc_iph_count = 0; n = _wcsftime_l(v_buf, 4, L"%Y-%m-%d", &g_tm, NULL);
    CHECK_EQ_U(n, 0); CHECK_EQ_I(tc_iph_count, 0);
    n = _wcsftime_l(v_buf, 5, L"%Y", &g_tm, NULL); CHECK_EQ_U(n, 4); CHECK_WSTR(buf, L"2020"); }
int main(void)
{
    g_tm.tm_year = 120; g_tm.tm_mon = 0; g_tm.tm_mday = 15; g_tm.tm_wday = 3; g_tm.tm_yday = 14;
    tc_begin();
    if (tc_iph_install()) { TC_RUN(null_buffer); TC_RUN(null_tm); TC_RUN(bad_spec); }
    TC_RUN(too_small);
    return tc_end();
}
