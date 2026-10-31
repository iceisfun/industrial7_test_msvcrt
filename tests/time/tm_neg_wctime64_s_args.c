/* TC
 * id:    tm_neg_wctime64_s_args
 * area:  time
 * kind:  negative
 * funcs: _wctime64_s
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _wctime_s, _wctime32_s, _wctime64_s (buffer >= 26 wide chars; time before 1970 or after 3000-12-31 23:59:59 invalid)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static volatile __time64_t v_t0 = 0, v_neg = -1, v_big = 32535216000LL;

TC_CASE(size_25)
{
    wchar_t buf[40]; __time64_t t = v_t0; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_wctime64_s(buf, 25, &t), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(null_buffer)
{
    __time64_t t = v_t0; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_wctime64_s(NULL, 26, &t), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(negative_timer)
{
    wchar_t buf[40]; __time64_t t = v_neg; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_wctime64_s(buf, 40, &t), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(past_year_3000)
{
    wchar_t buf[40]; __time64_t t = v_big; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_wctime64_s(buf, 40, &t), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    /* whether the invalid-parameter handler runs for the upper range check is not documented: see tm_neg_probe_wctime64_s_3000_iph */
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(size_25); TC_RUN(null_buffer); TC_RUN(negative_timer); TC_RUN(past_year_3000);
    return tc_end();
}
