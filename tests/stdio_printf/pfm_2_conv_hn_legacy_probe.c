/* TC
 * id:    pfm_2_conv_hn_legacy_probe
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_printf_count_output
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf size prefix h (n not explicitly listed); C11 7.21.6.1 hn = short*
 * note:  PROBE: with count output enabled, %hn stores a 2-byte short (adjacent bytes pre-filled 0xCC stay unchanged) on msvcr80-120.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_DECL_PROC(int, _set_printf_count_output, (int));
static union { long long align; unsigned char c[16]; } u;
static void fill(void) { memset(u.c, 0xCC, sizeof u.c); }

TC_CASE(hn)  { char b[16]; fill(); sprintf(b, "%d%hn", 12, (short *)u.c);
    CHECK_EQ_U(u.c[0], 2); CHECK_EQ_U(u.c[1], 0); CHECK_EQ_U(u.c[2], 0xCC); CHECK_EQ_U(u.c[3], 0xCC); CHECK_STR(b, "12"); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_printf_count_output);
    if (!tc_p__set_printf_count_output) { tc_write("TC FAIL " TC_TEST_ID "::__no _set_printf_count_output\n"); return 2; }
    tc_p__set_printf_count_output(1);
    TC_RUN(hn);
    tc_p__set_printf_count_output(0);
    return tc_end();
}
