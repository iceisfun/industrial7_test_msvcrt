/* TC
 * id:    scn_neg_sscanf_null_params
 * area:  scanf
 * funcs: sscanf
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sscanf, sscanf_l (parameter validation: invalid parameter handler, EOF, errno EINVAL)
 * note:  msvcrt.dll excluded: it has no invalid parameter handler.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

#include <errno.h>

TC_CASE(null_buffer)
{
    int v = 9, r; const char *volatile s = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = sscanf(s, "%d", &v);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(v, 9);
}
TC_CASE(null_format)
{
    int r; const char *volatile f = NULL;
    if (!tc_iph_install()) TC_SKIP("no handler API");
    tc_iph_count = 0; errno = 0;
    r = sscanf("1", f);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); TC_RUN(null_buffer); TC_RUN(null_format); return tc_end(); }
