/* TC
 * id:    pff_neg_ecvt_s_null_args
 * area:  stdio_printf
 * funcs: _ecvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _ecvt_s: error conditions table (EINVAL, invalid parameter handler invoked)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(buffer_null)
{
    int d = 0, s = 0; errno_t r;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    tc_iph_count = 0;
    r = _ecvt_s(NULL, 10, 1.5, 3, &d, &s);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(decpt_null)
{
    char b[16] = "zz"; int s = 0; errno_t r;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    tc_iph_count = 0;
    r = _ecvt_s(b, sizeof b, 1.5, 3, NULL, &s);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(sign_null)
{
    char b[16] = "zz"; int d = 0; errno_t r;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    tc_iph_count = 0;
    r = _ecvt_s(b, sizeof b, 1.5, 3, &d, NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); TC_RUN(buffer_null); TC_RUN(decpt_null); TC_RUN(sign_null); return tc_end(); }
