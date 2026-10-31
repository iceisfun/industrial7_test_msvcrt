/* TC
 * id:    pff_neg_gcvt_s_null_args
 * area:  stdio_printf
 * funcs: _gcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gcvt_s: NULL buffer or sizeInBytes of zero gives EINVAL
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(buffer_null)
{
    errno_t r;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    tc_iph_count = 0;
    r = _gcvt_s(NULL, 32, 1.5, 6);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_zero)
{
    char b[32]; errno_t r;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    b[0] = 'Q';
    tc_iph_count = 0;
    r = _gcvt_s(b, 0, 1.5, 6);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 'Q');
}

int main(void) { tc_begin(); TC_RUN(buffer_null); TC_RUN(size_zero); return tc_end(); }
