/* TC
 * id:    tm_neg_strtime_s_args
 * area:  time
 * funcs:
 * procs: _strtime_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strtime_s (buffer must hold at least 9 bytes; exact code for size 8 is covered by tm_neg_strdate_strtime_s_probe_size8)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

TC_DECL_PROC(int, _strtime_s, (char *, size_t));

TC_CASE(null_buffer)
{
    int r;
    if (!tc_p__strtime_s) TC_SKIP("missing");
    tc_iph_count = 0; errno = 0;
    r = tc_p__strtime_s(NULL, 9);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_zero)
{
    char b[16]; int r;
    if (!tc_p__strtime_s) TC_SKIP("missing");
    tc_iph_count = 0; errno = 0; b[0] = 'x';
    r = tc_p__strtime_s(b, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 'x');
}
TC_CASE(size_eight)
{
    char b[16]; int r;
    if (!tc_p__strtime_s) TC_SKIP("missing");
    tc_iph_count = 0; errno = 0;
    r = tc_p__strtime_s(b, 8);
    CHECK(r != 0); CHECK_EQ_I(errno, r); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_nine_ok)
{
    char b[9]; int r;
    if (!tc_p__strtime_s) TC_SKIP("missing");
    tc_iph_count = 0;
    r = tc_p__strtime_s(b, 9);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(iph_counts_each)
{
    char b[16];
    if (!tc_p__strtime_s) TC_SKIP("missing");
    tc_iph_count = 0;
    tc_p__strtime_s(NULL, 9); tc_p__strtime_s(b, 0); tc_p__strtime_s(b, 8);
    CHECK_EQ_I(tc_iph_count, 3);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_LOAD_PROC(_strtime_s);
    TC_RUN(null_buffer); TC_RUN(size_zero); TC_RUN(size_eight); TC_RUN(size_nine_ok); TC_RUN(iph_counts_each);
    return tc_end();
}
