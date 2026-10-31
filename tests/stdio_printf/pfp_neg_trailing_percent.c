/* TC
 * id:    pfp_neg_trailing_percent
 * area:  stdio_printf
 * funcs: sprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation
 * note:  PROBE: sprintf_s with a format ending inside a conversion spec is rejected (-1, EINVAL, handler). sprintf (non-_s) is not used: it does not validate the format.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

/* A bad format must fail: return < 0, errno EINVAL, invalid-parameter handler called. */
static void bad(const char *fmt)
{
    char b[32]; int n, i;
    for (i = 0; i < 32; i++) b[i] = 'X';
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    n = sprintf_s(b, sizeof b, fmt, 1, 2);
    CHECK(n < 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
static const char *volatile Fa = "abc%", *volatile Fb = "%", *volatile Fc = "%-", *volatile Fd = "%5";
TC_CASE(abc_pct)  { bad(Fa); }
TC_CASE(only_pct) { bad(Fb); }
TC_CASE(pct_dash) { bad(Fc); }
TC_CASE(pct_five) { bad(Fd); }

int main(void) { tc_begin(); TC_RUN(abc_pct); TC_RUN(only_pct); TC_RUN(pct_dash); TC_RUN(pct_five); return tc_end(); }
