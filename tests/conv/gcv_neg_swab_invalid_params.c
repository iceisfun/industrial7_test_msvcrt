/* TC
 * id:    gcv_neg_swab_invalid_params
 * area:  conv
 * kind:  negative
 * funcs: _swab _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _swab: NULL src/dest or negative n invokes the invalid parameter handler, errno EINVAL
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char src[4] = {'A','B','C','D'};

static void bad(char *s, char *d, int n)
{
    tc_iph_count = 0; errno = 0;
    _swab(s, d, n);
}
TC_CASE(src_null)
{
    char dst[4] = {'.','.','.','.'};
    bad(NULL, dst, 4);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_MEM(dst, "....", 4);
}
TC_CASE(dst_null)
{
    bad(src, NULL, 4);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(negative_count)
{
    char dst[4] = {'.','.','.','.'};
    bad(src, dst, -2);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_MEM(dst, "....", 4);
}
TC_CASE(recovers)
{
    char dst[4] = {0};
    bad(NULL, dst, 4);
    tc_iph_count = 0;
    _swab(src, dst, 4);
    CHECK_MEM(dst, "BADC", 4);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(src_null); TC_RUN(dst_null); TC_RUN(negative_count); TC_RUN(recovers);
    return tc_end();
}
