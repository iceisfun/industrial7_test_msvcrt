/* TC
 * id:    exp_neg_sprintf_s_small_buffer
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: sprintf_s
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_s
 * note:  overflow: returns -1, buffer[0]==0, iph invoked. errno not asserted.
 */
#include "tc.h"
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
typedef int (__cdecl *sps_t)(char *, size_t, const char *, ...);
static sps_t p_sps;

TC_CASE(buffer_too_small_returns_minus1)
{ char b[4]; volatile int n = 0; (void)n; tc_iph_count = 0; CHECK_EQ_I(p_sps(b, sizeof b, "%s", "abcdef"), -1); }
TC_CASE(buffer0_set_to_nul)
{ char b[4]; memset(b, 'z', sizeof b); tc_iph_count = 0; p_sps(b, sizeof b, "%s", "abcdef"); CHECK_EQ_I(b[0], 0); }
TC_CASE(iph_called)
{ char b[4]; tc_iph_count = 0; p_sps(b, sizeof b, "%s", "abcdef"); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(exact_fit_ok)
{ char b[4]; tc_iph_count = 0; CHECK_EQ_I(p_sps(b, sizeof b, "%s", "abc"), 3); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin();
    p_sps = (sps_t)tc_crt_proc("sprintf_s");
    if (!p_sps || !tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(buffer_too_small_returns_minus1); TC_RUN(buffer0_set_to_nul); TC_RUN(iph_called); TC_RUN(exact_fit_ok);
    return tc_end();
}
