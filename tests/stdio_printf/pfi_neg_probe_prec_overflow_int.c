/* TC
 * id:    pfi_neg_probe_prec_overflow_int
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   C11 7.21.6.1 (negative * precision is taken as omitted); MS docs printf precision
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, zzp_probe2: child "prec_ovf" exit 0x20): sprintf("%.99999999999d") (precision digits overflow int) terminates
 *        normally and FAILS with -1 WITHOUT calling the invalid-parameter handler (child exit 32 = -1, iph==0). Old expectation (exit
 *        30 success or 31 -1+iph) was wrong. The text-parsing overflow is a plain failure path, not a parameter-validation path.
 *        (2) "%.*d" with INT_MIN precision acts as no precision. Stable.
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(digits_overflow_child)
{
    DWORD code = 0; int ran = tc_spawn_self("ovf", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 32);
}
TC_CASE(star_int_min)
{
    char b[32]; volatile int p = INT_MIN, v = 7; int r; tc_iph_count = 0;
    r = sprintf_s(b, sizeof b, "%.*d", p, v);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "7");
}
int main(void)
{
    tc_begin(); tc_iph_install();
    if (tc_child_name()) {
        static char b[4096]; volatile int v = 1; volatile const char *f = "%.99999999999d"; int r;
        tc_iph_count = 0;
        r = sprintf_s(b, sizeof b, (const char *)f, v);
        if (r != -1) return 30;
        return tc_iph_count == 1 ? 31 : 32;
    }
    TC_RUN(digits_overflow_child); TC_RUN(star_int_min);
    return tc_end();
}
