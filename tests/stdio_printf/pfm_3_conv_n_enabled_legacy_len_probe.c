/* TC
 * id:    pfm_3_conv_n_enabled_legacy_len_probe
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_printf_count_output
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf size prefixes; _set_printf_count_output
 * note:  PROBE: with %n enabled, %hn stores a 2-byte count, %I32n 4 bytes. %I64n: the legacy CRT's %n store only distinguishes short/long/int (as far as recalled, no 64-bit branch), so
 *        %I64n is expected to store a 4-byte int and leave bytes 4..7 at 0xCC (case I64n_high_untouched; the other assertions hold either way);
 *        Neighbouring bytes (pre-filled 0xCC) stay unchanged. %hn is probed by pfm_2_conv_hn_legacy_probe too.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
static int prev;
static const char * volatile vf;
static union { long long align; unsigned char c[16]; } u;
static void on(void)  { memset(u.c, 0xCC, sizeof u.c); prev = tc_p__set_printf_count_output(1); }
static void off(void) { tc_p__set_printf_count_output(prev); }

TC_CASE(hn)   { char b[48]; on(); vf = "%5d%hn"; sprintf(b, vf, 1, u.c); off();
    CHECK_EQ_U(u.c[0], 5); CHECK_EQ_U(u.c[1], 0); CHECK_EQ_U(u.c[2], 0xCC); CHECK_EQ_U(u.c[3], 0xCC); }
TC_CASE(I32n) { char b[48]; on(); vf = "%5d%I32n"; sprintf(b, vf, 1, u.c); off();
    CHECK_EQ_U(u.c[0], 5); CHECK_EQ_U(u.c[3], 0); CHECK_EQ_U(u.c[4], 0xCC); }
TC_CASE(I64n) { char b[48]; on(); vf = "%5d%I64n"; sprintf(b, vf, 1, u.c); off();
    CHECK_EQ_U(u.c[0], 5); CHECK_EQ_U(u.c[3], 0); CHECK_EQ_U(u.c[8], 0xCC); }
TC_CASE(I64n_high_untouched) { char b[48]; on(); vf = "%5d%I64n"; sprintf(b, vf, 1, u.c); off();
    CHECK_EQ_U(u.c[4], 0xCC); CHECK_EQ_U(u.c[7], 0xCC); }

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__set_printf_count_output) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_printf_count_output\n"); return 2; }
    tc_iph_install();
    TC_RUN(hn); TC_RUN(I32n); TC_RUN(I64n); TC_RUN(I64n_high_untouched);
    return tc_end();
}
