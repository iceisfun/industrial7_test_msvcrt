/* TC
 * id:    tm_neg_gmtime64_null_time
 * area:  time
 * funcs: _gmtime64
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gmtime64 (invalid parameter section)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

TC_CASE(null_ptr)
{
    struct tm *r; int have = tc_iph_install();
    CHECK(have);
    tc_iph_count = 0; errno = 0;
    r = _gmtime64(NULL);
    CHECK(r == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_ptr); return tc_end(); }
