/* TC
 * id:    hp_neg_aligned_msize_null
 * area:  heap
 * funcs: _aligned_msize
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_msize: NULL is an invalid parameter
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

TC_CASE(null)
{
    void *volatile p = NULL; size_t r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _aligned_msize(p, 16, 0);
    CHECK_EQ_U(r, (size_t)-1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(null);
    return tc_end();
}
