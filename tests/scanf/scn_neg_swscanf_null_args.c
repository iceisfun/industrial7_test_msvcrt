/* TC
 * id:    scn_neg_swscanf_null_args
 * area:  scanf
 * funcs: swscanf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sscanf, swscanf: NULL buffer or format invokes the invalid parameter handler, returns EOF, errno EINVAL
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static const wchar_t *volatile vin;
static const wchar_t *volatile vfmt;

TC_CASE(null_string)
{
    int a = 5, n; vin = NULL; vfmt = L"%d";
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    n = swscanf(vin, vfmt, &a);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(a, 5);
}
TC_CASE(null_format)
{
    int a = 5, n; vin = L"12"; vfmt = NULL;
    CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    n = swscanf(vin, vfmt, &a);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(a, 5);
}
int main(void) { tc_begin(); TC_RUN(null_string); TC_RUN(null_format); return tc_end(); }
