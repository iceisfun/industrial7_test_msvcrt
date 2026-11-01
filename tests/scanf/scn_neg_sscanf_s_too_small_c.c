/* TC
 * id:    scn_neg_sscanf_s_too_small_c
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  observed
 * ref:   MS docs scanf_s, wscanf_s: size argument required for c, C, s, S, [; too small a buffer invokes the invalid parameter handler
 * note:  %5c with a size of 3 cannot fit; the CRT must not write 5 chars. Return value after the failure is not asserted here (see scn_neg_sscanf_s_too_small_ret_probe).
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): a too-small _s destination does NOT call the invalid parameter handler (tc_iph_count stays 0);
 *        the sentinel/overflow checks below still hold. MS docs do not document a handler call for this case. Stable (2 runs).
 *        Observed on Win10 22H2 ucrtbase only; the same expectation on the other CRTs (>=90) is [unverified] and must be confirmed when those CRTs are run in the VM.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vin;
TC_CASE(iph_invoked)
{
    char a[8]; int i; for (i = 0; i < 8; i++) a[i] = 'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vin = "abcdefg";
    (void)sscanf_s((const char *)vin, "%5c", a, (unsigned)3);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(a[3], 'Z'); CHECK_EQ_I(a[4], 'Z'); CHECK_EQ_I(a[7], 'Z');
}
TC_CASE(size5_control)
{
    char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vin = "abcdefg";
    n = sscanf_s((const char *)vin, "%5c", a, (unsigned)5);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(tc_iph_count, 0); CHECK_MEM(a, "abcde", 5); CHECK_EQ_I(a[5], 'Z');
}
int main(void) { tc_begin(); TC_RUN(iph_invoked); TC_RUN(size5_control); return tc_end(); }
