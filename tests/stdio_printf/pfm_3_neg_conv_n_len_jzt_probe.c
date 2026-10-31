/* TC
 * id:    pfm_3_neg_conv_n_len_jzt_probe
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _get_printf_count_output
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf type field n; _set_printf_count_output (count output is disabled by default, invalid parameter handler on %n)
 * note:  PROBE: as pfm_3_neg_conv_n_len_l_i_probe for %jn %zn %tn (size prefixes known to the CRT from msvcr120 on). Also %hhn and %lln.
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

TC_CASE(jn) { char b[48]; if (enabled) TC_SKIP("%n enabled");
    memset(u.c, 0xCC, sizeof u.c); tc_iph_count = 0; vf = "%jn"; sprintf(b, vf, u.c);
    CHECK_EQ_I(tc_iph_count, 1); CHECK(untouched()); }
TC_CASE(zn) { char b[48]; if (enabled) TC_SKIP("%n enabled");
    memset(u.c, 0xCC, sizeof u.c); tc_iph_count = 0; vf = "%zn"; sprintf(b, vf, u.c);
    CHECK_EQ_I(tc_iph_count, 1); CHECK(untouched()); }
TC_CASE(tn) { char b[48]; if (enabled) TC_SKIP("%n enabled");
    memset(u.c, 0xCC, sizeof u.c); tc_iph_count = 0; vf = "%tn"; sprintf(b, vf, u.c);
    CHECK_EQ_I(tc_iph_count, 1); CHECK(untouched()); }
TC_CASE(hhn) { char b[48]; if (enabled) TC_SKIP("%n enabled");
    memset(u.c, 0xCC, sizeof u.c); tc_iph_count = 0; vf = "%hhn"; sprintf(b, vf, u.c);
    CHECK_EQ_I(tc_iph_count, 1); CHECK(untouched()); }
TC_CASE(lln) { char b[48]; if (enabled) TC_SKIP("%n enabled");
    memset(u.c, 0xCC, sizeof u.c); tc_iph_count = 0; vf = "%lln"; sprintf(b, vf, u.c);
    CHECK_EQ_I(tc_iph_count, 1); CHECK(untouched()); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_get_printf_count_output);
    enabled = tc_p__get_printf_count_output && tc_p__get_printf_count_output() != 0;
    TC_RUN(jn); TC_RUN(zn); TC_RUN(tn); TC_RUN(hhn); TC_RUN(lln);
    return tc_end();
}
