/* TC
 * id:    scn_neg_sscanf_null_format
 * area:  scanf
 * funcs: sscanf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sscanf, sscanf_l, swscanf, swscanf_l: if buffer or format is NULL the invalid parameter handler is invoked, returns EOF, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile vfmt;
static const char *volatile vin;

TC_CASE(null_format)
{
    int n; vfmt = NULL; vin = "12";
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    n = sscanf(vin, vfmt);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(null_format_with_arg)
{
    int a = 5, n; vfmt = NULL; vin = "12";
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    n = sscanf(vin, vfmt, &a);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(a, 5);
}
TC_CASE(valid_control)
{
    int a = 0, n; vin = "12";
    CHECK(tc_iph_install()); tc_iph_count = 0;
    n = sscanf(vin, "%d", &a);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(null_format); TC_RUN(null_format_with_arg); TC_RUN(valid_control); return tc_end(); }
