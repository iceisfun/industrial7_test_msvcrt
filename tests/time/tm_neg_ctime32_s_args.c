/* TC
 * id:    tm_neg_ctime32_s_args
 * area:  time
 * kind:  negative
 * funcs: _ctime32_s
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs ctime_s, _ctime32_s, _ctime64_s (buffer must be >= 26 chars; negative time invalid; NULL -> EINVAL)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static volatile __time32_t v_t0 = 0, v_neg = -1;

TC_CASE(null_buffer)
{
    __time32_t t = v_t0; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_ctime32_s(NULL, 26, &t), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(size_25)
{
    char buf[40]; __time32_t t = v_t0; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_ctime32_s(buf, 25, &t), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(negative_timer)
{
    char buf[40]; __time32_t t = v_neg; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_ctime32_s(buf, sizeof buf, &t), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(null_timer)
{
    char buf[40]; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_ctime32_s(buf, sizeof buf, NULL), EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_buffer); TC_RUN(size_25); TC_RUN(negative_timer); TC_RUN(null_timer);
    return tc_end();
}
