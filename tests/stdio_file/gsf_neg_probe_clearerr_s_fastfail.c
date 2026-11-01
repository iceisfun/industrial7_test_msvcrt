/* TC
 * id:    gsf_neg_probe_clearerr_s_fastfail
 * area:  stdio_file
 * funcs: clearerr_s
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs Parameter validation
 * note:  PROBE: exact termination status of clearerr_s(NULL) under the default invalid-parameter handler; expected fast-fail 0xC0000409 on ucrtbase; older CRTs not covered.
 *        HARNESS FIX (class F, Win10 22H2): mingw crt2.o installs a no-op invalid-parameter handler at startup, so the child survived (exit 77 instead of
 *        0xC0000409); the child now calls tc_iph_default() (tc.h) to restore the CRT default before the call under test.
 *        OBSERVED (Win10 22H2): clearerr_s(NULL) under the default handler -> 0xC0000409, stable over 3 runs.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

static FILE *volatile nullfp;
TC_CASE(status)
{
    DWORD code = 0; CHECK(tc_spawn_self("nullclr", &code, 20000));
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { tc_iph_default(); clearerr_s(nullfp); return 77; }
    TC_RUN(status);
    return tc_end();
}
