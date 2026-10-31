/* TC
 * id:    scn_neg_sscanf_null_str_or_format
 * area:  scanf
 * funcs: sscanf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sscanf, _sscanf_l: NULL str/format invokes the invalid parameter handler, returns EOF, errno EINVAL
 * note:  
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

static volatile const char *v;
TC_CASE(null_str)
{
    int a = 0, n; if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0; v = NULL;
    n = sscanf((const char *)v, "%d", &a);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    int n; if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0; v = NULL;
    n = sscanf("1", (const char *)v);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_str); TC_RUN(null_format); return tc_end(); }
