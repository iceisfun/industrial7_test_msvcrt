/* TC
 * id:    pfi_neg_probe_prec_overflow_int
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.1 (negative * precision is taken as omitted); MS docs printf precision
 * note:  Probes (1) "%.99999999999d" (precision digits overflow int) terminates and either fails (-1 with iph==1) or succeeds;
 *        run in a child (exit 30 = success, 31 = -1 with iph==1, 32 = -1 w/o iph). (2) "%.*d" with INT_MIN precision acts as no precision.
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(digits_overflow_child)
{
    DWORD code = 0; int ran = tc_spawn_self("ovf", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code == 30 || code == 31, 1);
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
