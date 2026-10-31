/* TC
 * id:    cv_neg_atoi_null
 * area:  conv
 * funcs: atoi atol _atoi64 atof
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs atoi/atof Parameter validation: if str is NULL the invalid parameter handler is invoked, return 0, errno EINVAL
 * note:  Narrowed to ucrtbase; whether msvcr80-120 atoi/atol/_atoi64 validate NULL (vs. crash) is probed in cv_neg_atoi_null_legacy_probe
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile nul;

TC_CASE(atoi_null) { int r; tc_iph_count = 0; errno = 0; r = atoi(nul); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(atol_null) { long r; tc_iph_count = 0; errno = 0; r = atol(nul); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(i64_null) { __int64 r; tc_iph_count = 0; errno = 0; r = _atoi64(nul); CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(atof_null) { double r; tc_iph_count = 0; errno = 0; r = atof(nul); CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(atoi_null); TC_RUN(atol_null); TC_RUN(i64_null); TC_RUN(atof_null);
    return tc_end();
}
