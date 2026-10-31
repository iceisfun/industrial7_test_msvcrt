/* TC
 * id:    pfi_neg_probe_missing_vararg_unused_flag
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.1 (flags may be repeated per implementation); MS docs printf flag directives
 * note:  Probes that repeated / redundant flags are accepted, not rejected: "%--5d", "%++d", "%  d", "%00005d". Also records the handler count (expected 0).
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile F1 = "%--5d", *volatile F2 = "%++d", *volatile F3 = "%  d", *volatile F4 = "%00005d";

TC_CASE(repeat_minus)  { char b[32]; int r; tc_iph_count = 0; r = sprintf_s(b, sizeof b, (const char *)F1, 3); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(r, 5); CHECK_STR(b, "3    "); }
TC_CASE(repeat_plus)   { char b[32]; int r; tc_iph_count = 0; r = sprintf_s(b, sizeof b, (const char *)F2, 3); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(r, 2); CHECK_STR(b, "+3"); }
TC_CASE(repeat_space)  { char b[32]; int r; tc_iph_count = 0; r = sprintf_s(b, sizeof b, (const char *)F3, 3); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(r, 2); CHECK_STR(b, " 3"); }
TC_CASE(repeat_zero)   { char b[32]; int r; tc_iph_count = 0; r = sprintf_s(b, sizeof b, (const char *)F4, 3); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(r, 5); CHECK_STR(b, "00003"); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(repeat_minus); TC_RUN(repeat_plus); TC_RUN(repeat_space); TC_RUN(repeat_zero);
    return tc_end();
}
