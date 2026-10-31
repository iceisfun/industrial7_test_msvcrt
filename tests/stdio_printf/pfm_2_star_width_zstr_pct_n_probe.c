/* TC
 * id:    pfm_2_star_width_zstr_pct_n_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf type field (%Z = ANSI_STRING/UNICODE_STRING address, %n disabled by default in ucrt)
 * note:  PROBES (each case independent): (1) '%*Z' consumes the width int and then an ANSI_STRING* and pads on the left;
 *        (2) '%*%' consumes the int for '*' but prints a plain '%' (the following %d sees the next argument);
 *        (3) ucrt only: '%*n' is rejected like '%n' (invalid parameter, -1) because %n is disabled by default.
 */
#include "tc.h"
#include <stdio.h>

typedef struct { USHORT Length, MaximumLength; char *Buffer; } TC_ANSI;
static volatile int w5 = 5;

TC_CASE(star_Z)
{
    static char txt[] = "ab"; TC_ANSI a; char b[64]; int r;
    a.Length = 2; a.MaximumLength = 3; a.Buffer = txt;
    r = sprintf(b, "[%*Z]|%d", w5, &a, 7);
    CHECK_STR(b, "[   ab]|7"); CHECK_EQ_I(r, 9);
}
TC_CASE(star_percent)
{
    char b[64]; int r = sprintf(b, "%*%|%d", w5, 7);
    CHECK_STR(b, "%|7"); CHECK_EQ_I(r, 3);
}
TC_CASE(star_n)
{
#if TC_CRT_VER >= 140
    char b[64]; int n = 0, r;
    tc_iph_count = 0; b[0] = 0;
    r = sprintf(b, "ab%*n", w5, &n);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(n, 0);
#else
    TC_SKIP("ucrt only");
#endif
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(star_Z); TC_RUN(star_percent); TC_RUN(star_n);
    return tc_end();
}
