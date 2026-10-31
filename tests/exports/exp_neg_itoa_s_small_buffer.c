/* TC
 * id:    exp_neg_itoa_s_small_buffer
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _itoa_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _itoa_s
 * note:  buffer too small / NULL buffer. MS docs list EINVAL for too-small buffers (real code is believed to return ERANGE):
 *        the exact code is NOT asserted here, see exp_neg_probe_itoa_s_small_buffer_code.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
typedef int (__cdecl *itoa_t)(int, char *, size_t, int);
static itoa_t p_itoa;

TC_CASE(size3_12345_fails)
{ char b[3]; int r; tc_iph_count = 0; r = p_itoa(12345, b, sizeof b, 10); CHECK(r != 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_buffer_einval)
{ char *volatile b = NULL; errno = 0; tc_iph_count = 0; CHECK_EQ_I(p_itoa(1, b, 8, 10), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(exact_fit_ok)
{ char b[6]; tc_iph_count = 0; CHECK_EQ_I(p_itoa(12345, b, sizeof b, 10), 0); CHECK_STR(b, "12345"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin();
    p_itoa = (itoa_t)tc_crt_proc("_itoa_s");
    if (!p_itoa || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(size3_12345_fails); TC_RUN(null_buffer_einval); TC_RUN(exact_fit_ok);
    return tc_end();
}
