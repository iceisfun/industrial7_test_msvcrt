/* TC
 * id:    exp_neg_itoa_s_bad_radix
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _itoa_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _itoa_s
 * note:  radix must be 2..36
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
typedef int (__cdecl *itoa_t)(int, char *, size_t, int);
static itoa_t p_itoa;

TC_CASE(radix1_einval)
{ char b[40]; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_itoa(5, b, sizeof b, 1), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(radix37_einval)
{ char b[40]; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_itoa(5, b, sizeof b, 37), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(radix2_ok)
{ char b[40]; CHECK_EQ_I(p_itoa(5, b, sizeof b, 2), 0); CHECK_STR(b, "101"); }
TC_CASE(radix36_ok)
{ char b[40]; CHECK_EQ_I(p_itoa(35, b, sizeof b, 36), 0); CHECK_STR(b, "z"); }
int main(void)
{
    tc_begin();
    p_itoa = (itoa_t)tc_crt_proc("_itoa_s");
    if (!p_itoa || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(radix1_einval); TC_RUN(radix37_einval); TC_RUN(radix2_ok); TC_RUN(radix36_ok);
    return tc_end();
}
