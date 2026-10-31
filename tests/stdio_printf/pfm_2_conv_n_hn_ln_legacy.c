/* TC
 * id:    pfm_2_conv_n_hn_ln_legacy
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_printf_count_output
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_printf_count_output, printf type field n and size prefix l
 * note:  With count output enabled, %ln stores 4 bytes (long is 32-bit on Windows); adjacent bytes (pre-filled 0xCC) stay unchanged.
 *        %hn is probed in pfm_2_conv_hn_legacy_probe; %I64n elsewhere (pfp_n_i64_old_probe).
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
static union { long long align; unsigned char c[16]; } u;
static void fill(void) { memset(u.c, 0xCC, sizeof u.c); }

TC_CASE(ln)  { char b[16]; fill(); sprintf(b, "%d%ln", 12345, (long *)u.c);
    CHECK_EQ_U(u.c[0], 5); CHECK_EQ_U(u.c[1], 0); CHECK_EQ_U(u.c[3], 0); CHECK_EQ_U(u.c[4], 0xCC); CHECK_STR(b, "12345"); }
TC_CASE(n)   { char b[16]; fill(); sprintf(b, "%d%n", 12, (int *)u.c);
    CHECK_EQ_U(u.c[0], 2); CHECK_EQ_U(u.c[3], 0); CHECK_EQ_U(u.c[4], 0xCC); }
TC_CASE(restored) { int r = tc_p__set_printf_count_output(0); CHECK_EQ_I(r, 1); CHECK_EQ_I(tc_p__set_printf_count_output(0), 0); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__set_printf_count_output) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_printf_count_output\n"); return 2; }
    tc_p__set_printf_count_output(1);
    TC_RUN(ln); TC_RUN(n); TC_RUN(restored);
    return tc_end();
}
