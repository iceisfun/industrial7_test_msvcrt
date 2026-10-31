/* TC
 * id:    pfp_n_hhn_probe
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_printf_count_output
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs size prefixes (hh supported since VS2015 UCRT)
 * note:  PROBE: %hhn stores exactly one byte holding the count modulo 256; neighbouring bytes untouched.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));

TC_CASE(small)
{
    unsigned char c[4]; char b[32]; memset(c, 0xCC, 4);
    sprintf(b, "abc%hhn", (signed char *)&c[1]);
    CHECK_EQ_U(c[0], 0xCC); CHECK_EQ_U(c[1], 3); CHECK_EQ_U(c[2], 0xCC); CHECK_EQ_U(c[3], 0xCC);
}
TC_CASE(wraps)
{
    unsigned char c[4]; char b[512]; int r; memset(c, 0xCC, 4);
    r = sprintf(b, "%300d%hhn", 1, (signed char *)&c[1]);
    CHECK_EQ_I(r, 300);
    CHECK_EQ_U(c[0], 0xCC); CHECK_EQ_U(c[1], 300 & 0xFF); CHECK_EQ_U(c[2], 0xCC); CHECK_EQ_U(c[3], 0xCC);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__set_printf_count_output) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_printf_count_output\n"); return 2; }
    tc_p__set_printf_count_output(1);
    TC_RUN(small); TC_RUN(wraps);
    tc_p__set_printf_count_output(0);
    return tc_end();
}
