/* TC
 * id:    pfp_n_i64_old_probe
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_printf_count_output
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf size prefixes (I64)
 * note:  PROBE: for %I64n on pre-UCRT CRTs the low 32 bits of the target hold the count in every case (case low_dword);
 *        the probe is whether the upper 4 bytes are left untouched (the old engine stores an int for %n unless l/h is given).
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
static union { long long align; unsigned char c[16]; } u;

TC_CASE(low_dword)
{
    char b[16]; memset(u.c, 0xCC, sizeof u.c);
    sprintf(b, "abc%I64n", (long long *)u.c);
    CHECK_EQ_U(u.c[0], 3); CHECK_EQ_U(u.c[1], 0); CHECK_EQ_U(u.c[2], 0); CHECK_EQ_U(u.c[3], 0); CHECK_EQ_U(u.c[8], 0xCC);
}
TC_CASE(upper_untouched)
{
    char b[16]; memset(u.c, 0xCC, sizeof u.c);
    sprintf(b, "abc%I64n", (long long *)u.c);
    CHECK_EQ_U(u.c[4], 0xCC); CHECK_EQ_U(u.c[7], 0xCC);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__set_printf_count_output) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_printf_count_output\n"); return 2; }
    tc_p__set_printf_count_output(1);
    TC_RUN(low_dword); TC_RUN(upper_untouched);
    tc_p__set_printf_count_output(0);
    return tc_end();
}
