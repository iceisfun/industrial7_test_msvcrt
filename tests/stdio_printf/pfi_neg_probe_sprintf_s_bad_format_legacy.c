/* TC
 * id:    pfi_neg_probe_sprintf_s_bad_format_legacy
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs sprintf_s (invalid format -> EINVAL via invalid parameter handler)
 * note:  Probes that an unknown conversion after %d ("%d %y") fails with -1, EINVAL, one handler call on the VS2005-2013 CRTs.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));

TC_CASE(bad_conv)
{
    char b[16]; volatile int v = 1; int r; errno = 0; tc_iph_count = 0;
    r = tc_p_sprintf_s(b, 16, "%d %y", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_LOAD_PROC(sprintf_s); CHECK(tc_p_sprintf_s != NULL);
    if (!tc_p_sprintf_s) return tc_end();
    TC_RUN(bad_conv);
    return tc_end();
}
