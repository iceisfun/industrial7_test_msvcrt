/* TC
 * id:    gsf_neg_probe_clearerr_s_fastfail
 * area:  stdio_file
 * funcs: clearerr_s
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation
 * note:  PROBE: exact termination status of clearerr_s(NULL) under the default invalid-parameter handler; expected fast-fail 0xC0000409 on ucrtbase; older CRTs not covered.
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
    if (tc_child_name()) { clearerr_s(nullfp); return 77; }
    TC_RUN(status);
    return tc_end();
}
