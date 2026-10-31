/* TC
 * id:    stl_neg_rand_s_null
 * area:  stdlib
 * kind:  negative
 * funcs: rand_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs rand_s: randomValue == NULL invokes the invalid parameter handler, returns EINVAL
 */
#define _CRT_RAND_S
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(null_arg)
{
    errno_t r; volatile unsigned int *volatile p = NULL;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = rand_s((unsigned int *)p);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_arg); return tc_end(); }
