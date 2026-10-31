/* TC
 * id:    tm_neg_wctime64_null
 * area:  time
 * kind:  negative
 * funcs: _wctime64
 * crt:   >=90
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _wctime64: returns NULL if timer represents a date before 1970 or after 3000-12-31 23:59:59; NULL timer invokes invalid parameter handler (EINVAL)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static volatile __time64_t v_neg = -1;

TC_CASE(null_timer)
{
    errno = 0; tc_iph_count = 0;
    CHECK(_wctime64(NULL) == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(minus_one)
{
    __time64_t t = v_neg; errno = 0;
    CHECK(_wctime64(&t) == NULL);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_timer); TC_RUN(minus_one);
    return tc_end();
}
