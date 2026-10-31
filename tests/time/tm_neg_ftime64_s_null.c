/* TC
 * id:    tm_neg_ftime64_s_null
 * area:  time
 * funcs: _ftime64_s _ftime32_s
 * crt:   >=90 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _ftime_s (NULL timeptr -> EINVAL, invalid parameter handler)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <sys/timeb.h>

static struct __timeb64 *volatile v64;
static struct __timeb32 *volatile v32;

TC_CASE(null_both)
{
    int r;
    tc_iph_count = 0; errno = 0; r = _ftime64_s(v64);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL);
    errno = 0; r = _ftime32_s(v32);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 2);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) TC_SKIP("no iph api");
    TC_RUN(null_both); return tc_end();
}
