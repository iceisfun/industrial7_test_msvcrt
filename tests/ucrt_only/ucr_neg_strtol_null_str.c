/* TC
 * id:    ucr_neg_strtol_null_str
 * area:  ucrt_only
 * funcs: strtol strtoul
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtol, strtoul (NULL string: EINVAL, invalid-parameter handler, returns 0)
 * note:  The value stored in *endptr is not asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v_null;

TC_CASE(strtol_null)
{
    char *e = (char *)1; long r; tc_iph_count = 0; errno = 0;
    r = strtol(v_null, &e, 10);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(strtoul_null)
{
    char *e = (char *)1; unsigned long r; tc_iph_count = 0; errno = 0;
    r = strtoul(v_null, &e, 10);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); if (!tc_iph_install()) return 2; TC_RUN(strtol_null); TC_RUN(strtoul_null); return tc_end(); }
