/* TC
 * id:    pff_neg_snprintf_g_truncate
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf: returns -1 if the output is longer than count; no NUL is appended when len == count; nothing is written past count
 */
#include "tc.h"
#include <stdio.h>

static volatile double v = 1.5;

TC_CASE(too_long)
{   /* "%#g" of 1.5 is "1.50000" (7 chars) */
    char b[16]; int n, i; for (i = 0; i < 16; i++) b[i] = 'Z';
    n = _snprintf(b, 4, "%#g", v);
    CHECK_EQ_I(n, -1);
    CHECK_MEM(b, "1.50", 4);
    for (i = 4; i < 16; i++) CHECK_EQ_I(b[i], 'Z');
}
TC_CASE(exact_no_nul)
{
    char b[16]; int n, i; for (i = 0; i < 16; i++) b[i] = 'Z';
    n = _snprintf(b, 7, "%#g", v);
    CHECK_EQ_I(n, 7);
    CHECK_MEM(b, "1.50000", 7);
    CHECK_EQ_I(b[7], 'Z');   /* no terminator written */
}
TC_CASE(one_more_fits)
{
    char b[16]; int n; n = _snprintf(b, 8, "%#g", v);
    CHECK_EQ_I(n, 7); CHECK_STR(b, "1.50000");
}
int main(void) { tc_begin(); TC_RUN(too_long); TC_RUN(exact_no_nul); TC_RUN(one_more_fits); return tc_end(); }
