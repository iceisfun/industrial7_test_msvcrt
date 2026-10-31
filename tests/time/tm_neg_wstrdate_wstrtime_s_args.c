/* TC
 * id:    tm_neg_wstrdate_wstrtime_s_args
 * area:  time
 * funcs:
 * procs: _wstrdate_s _wstrtime_s
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strdate_s, _wstrdate_s, _wstrtime_s (buffer must hold at least 9 wide chars)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

TC_DECL_PROC(int, _wstrdate_s, (wchar_t *, size_t));
TC_DECL_PROC(int, _wstrtime_s, (wchar_t *, size_t));

TC_CASE(date_null)
{
    int r; if (!tc_p__wstrdate_s) TC_SKIP("missing");
    tc_iph_count = 0; errno = 0; r = tc_p__wstrdate_s(NULL, 9);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(date_size8)
{
    wchar_t b[16]; int r; if (!tc_p__wstrdate_s) TC_SKIP("missing");
    tc_iph_count = 0; errno = 0; r = tc_p__wstrdate_s(b, 8);
    CHECK(r != 0); CHECK_EQ_I(errno, r); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(time_null)
{
    int r; if (!tc_p__wstrtime_s) TC_SKIP("missing");
    tc_iph_count = 0; errno = 0; r = tc_p__wstrtime_s(NULL, 9);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(time_size8)
{
    wchar_t b[16]; int r; if (!tc_p__wstrtime_s) TC_SKIP("missing");
    tc_iph_count = 0; errno = 0; r = tc_p__wstrtime_s(b, 8);
    CHECK(r != 0); CHECK_EQ_I(errno, r); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_LOAD_PROC(_wstrdate_s); TC_LOAD_PROC(_wstrtime_s);
    TC_RUN(date_null); TC_RUN(date_size8); TC_RUN(time_null); TC_RUN(time_size8);
    return tc_end();
}
