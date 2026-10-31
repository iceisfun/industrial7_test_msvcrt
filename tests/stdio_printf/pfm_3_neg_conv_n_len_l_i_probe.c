/* TC
 * id:    pfm_3_neg_conv_n_len_l_i_probe
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _get_printf_count_output
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf type field n; _set_printf_count_output (count output is disabled by default, invalid parameter handler on %n)
 * note:  PROBE: with %n disabled, %Ln %wn %I32n %I64n each invoke the invalid-parameter handler exactly once and write nothing, i.e. the size prefix does not
 *        bypass the disabled-%n check.
 *        (%In, bare I, is split out to pfm_3_neg_conv_n_len_i_probe: legacy CRTs do not treat a bare I as a size prefix before n.)
 *        The 16-byte target is pre-filled with 0xCC; all bytes must be unchanged afterwards. Skips when %n is enabled.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
static int enabled;
static const char * volatile vf;
static union { long long align; unsigned char c[16]; } u;
static int untouched(void) { int i; for (i = 0; i < 16; i++) if (u.c[i] != 0xCC) return 0; return 1; }

TC_CASE(Ln) { char b[48]; if (enabled) TC_SKIP("%n enabled");
    memset(u.c, 0xCC, sizeof u.c); tc_iph_count = 0; vf = "%Ln"; sprintf(b, vf, u.c);
    CHECK_EQ_I(tc_iph_count, 1); CHECK(untouched()); }
TC_CASE(wn) { char b[48]; if (enabled) TC_SKIP("%n enabled");
    memset(u.c, 0xCC, sizeof u.c); tc_iph_count = 0; vf = "%wn"; sprintf(b, vf, u.c);
    CHECK_EQ_I(tc_iph_count, 1); CHECK(untouched()); }
TC_CASE(I32n) { char b[48]; if (enabled) TC_SKIP("%n enabled");
    memset(u.c, 0xCC, sizeof u.c); tc_iph_count = 0; vf = "%I32n"; sprintf(b, vf, u.c);
    CHECK_EQ_I(tc_iph_count, 1); CHECK(untouched()); }
TC_CASE(I64n) { char b[48]; if (enabled) TC_SKIP("%n enabled");
    memset(u.c, 0xCC, sizeof u.c); tc_iph_count = 0; vf = "%I64n"; sprintf(b, vf, u.c);
    CHECK_EQ_I(tc_iph_count, 1); CHECK(untouched()); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_get_printf_count_output);
    enabled = tc_p__get_printf_count_output && tc_p__get_printf_count_output() != 0;
    TC_RUN(Ln); TC_RUN(wn); TC_RUN(I32n); TC_RUN(I64n);
    return tc_end();
}
