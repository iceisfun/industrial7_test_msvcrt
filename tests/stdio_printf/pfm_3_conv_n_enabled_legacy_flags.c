/* TC
 * id:    pfm_3_conv_n_enabled_legacy_flags
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_printf_count_output
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_printf_count_output; printf type field n (number of characters successfully written so far)
 * note:  With %n enabled, %n stores the character count produced so far, including padding added by width/flags of earlier conversions.
 *        The previous setting is restored after each case. %hn / %I64n are probed in pfm_3_conv_n_enabled_legacy_len_probe.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
static int prev;
static const char * volatile vf;
static void on(void)  { prev = tc_p__set_printf_count_output(1); }
static void off(void) { tc_p__set_printf_count_output(prev); }

TC_CASE(width_d)   { char b[48]; int n = -1, r; on(); vf = "[%5d%n]"; r = sprintf(b, vf, 42, &n); off(); CHECK_STR(b, "[   42]"); CHECK_EQ_I(n, 6); CHECK_EQ_I(r, 7); }
TC_CASE(left_s)    { char b[48]; int n = -1; on(); vf = "%-8s%n|"; sprintf(b, vf, "ab", &n); off(); CHECK_STR(b, "ab      |"); CHECK_EQ_I(n, 8); }
TC_CASE(star_zero) { char b[48]; int n = -1; on(); vf = "%0*d%n"; sprintf(b, vf, 6, 7, &n); off(); CHECK_STR(b, "000007"); CHECK_EQ_I(n, 6); }
TC_CASE(prec_plus) { char b[48]; int n = -1; on(); vf = "%+.4d%n"; sprintf(b, vf, 5, &n); off(); CHECK_STR(b, "+0005"); CHECK_EQ_I(n, 5); }
TC_CASE(empty)     { char b[48]; int n = -1; on(); vf = "%n"; sprintf(b, vf, &n); off(); CHECK_STR(b, ""); CHECK_EQ_I(n, 0); }
TC_CASE(restored)  { int p; p = tc_p__set_printf_count_output(0); CHECK_EQ_I(p, prev == 0 ? 0 : 1); tc_p__set_printf_count_output(prev); }

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__set_printf_count_output) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_printf_count_output\n"); return 2; }
    tc_iph_install();
    TC_RUN(width_d); TC_RUN(left_s); TC_RUN(star_zero); TC_RUN(prec_plus); TC_RUN(empty); TC_RUN(restored);
    return tc_end();
}
