/* TC
 * id:    pfp_neg_invalid_conv_char_win7
 * area:  stdio_printf
 * funcs: sprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf_s (checks format string for valid formatting characters); Parameter validation (VS2005+)
 * note:  PROBE: 9.0-12.0 CRTs sprintf_s report EINVAL + handler for unknown conversion chars. Uses sprintf_s, not sprintf (plain sprintf does not validate). msvcr80 dropped: sprintf_s availability in its import lib not relied on.
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
static const char *volatile Fy = "%y", *volatile Fb = "%!";
TC_CASE(pct_y)    { bad(Fy); }
TC_CASE(pct_bang) { bad(Fb); }

int main(void) { tc_begin(); TC_RUN(pct_y); TC_RUN(pct_bang); return tc_end(); }
