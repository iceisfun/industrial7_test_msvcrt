/* TC
 * id:    pfm_3_neg_conv_n_len_i_probe
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _get_printf_count_output
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf type field n; _set_printf_count_output (count output is disabled by default, invalid parameter handler on %n)
 * note:  PROBE: with %n disabled, a bare %In (I without 32/64) still reaches the disabled-%n check: handler once, target untouched.
 *        Legacy CRTs (msvcr80-120) accept I only before 32, 64, d i o u x X, so they are excluded. Skips when %n is enabled.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
static int enabled;
static const char * volatile vf;
static union { long long align; unsigned char c[16]; } u;
static int untouched(void) { int i; for (i = 0; i < 16; i++) if (u.c[i] != 0xCC) return 0; return 1; }

TC_CASE(In) { char b[48]; if (enabled) TC_SKIP("%n enabled");
    memset(u.c, 0xCC, sizeof u.c); tc_iph_count = 0; vf = "%In"; sprintf(b, vf, u.c);
    CHECK_EQ_I(tc_iph_count, 1); CHECK(untouched()); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_get_printf_count_output);
    enabled = tc_p__get_printf_count_output && tc_p__get_printf_count_output() != 0;
    TC_RUN(In);
    return tc_end();
}
