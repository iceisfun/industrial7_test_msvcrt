/* TC
 * id:    pfp_neg_invalid_conv_char
 * area:  stdio_printf
 * funcs: sprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs printf_s, _printf_s_l: printf_s checks the format string for valid formatting characters (plain printf only checks NULL); Parameter validation
 * note:  Uses sprintf_s: plain sprintf only NULL-checks and its behaviour for unknown conversions is undefined (C11 7.21.6.1p9), so it is not asserted. Buffer contents not asserted beyond far end untouched.
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
static const char *volatile Fy = "%y", *volatile Fb = "%!", *volatile Fz = "a%zq", *volatile Fv = "abc%v def", *volatile Fk = "%k";
TC_CASE(pct_y)    { bad(Fy); }
TC_CASE(pct_bang) { bad(Fb); }
TC_CASE(pct_zq)   { bad(Fz); }
TC_CASE(embedded) { bad(Fv); }
TC_CASE(pct_k)    { bad(Fk); }
TC_CASE(buffer_terminated_or_untouched_far)
{
    char b[32]; int i; const char *volatile f = "%y";
    for (i = 0; i < 32; i++) b[i] = 'X';
    tc_iph_install(); (void)sprintf_s(b, sizeof b, f, 1);
    CHECK(b[31] == 'X');   /* nothing written far beyond the (empty) output */
}

int main(void) { tc_begin(); TC_RUN(pct_y); TC_RUN(pct_bang); TC_RUN(pct_zq); TC_RUN(embedded); TC_RUN(pct_k); TC_RUN(buffer_terminated_or_untouched_far); return tc_end(); }
