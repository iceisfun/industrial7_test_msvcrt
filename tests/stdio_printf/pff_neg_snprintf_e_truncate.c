/* TC
 * id:    pff_neg_snprintf_e_truncate
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf: returns -1 if more than count chars needed; no NUL if exactly count; never writes past count
 */
#include "tc.h"
#include <stdio.h>

static volatile double v = 1.5;
static void fill(char *b) { int i; for (i = 0; i < 32; i++) b[i] = (char)90; }

TC_CASE(e_truncated)
{
    char b[32]; int n; fill(b);
    n = _snprintf(b, 5, "%e", v);
    CHECK_EQ_I(n, -1);
    CHECK(b[0]=='1' && b[1]=='.' && b[2]=='5' && b[3]=='0' && b[4]=='0'); CHECK(b[5]=='Z');
}
TC_CASE(f_truncated)
{
    char b[32]; int n; fill(b);
    n = _snprintf(b, 4, "%.5f", v);   /* "1.50000" */
    CHECK_EQ_I(n, -1);
    CHECK(b[0]=='1' && b[1]=='.' && b[2]=='5' && b[3]=='0'); CHECK(b[4]=='Z');
}
TC_CASE(g_exact_no_nul)
{
    char b[32]; int n; fill(b);
    n = _snprintf(b, 3, "%g", v);     /* "1.5": exactly count */
    CHECK_EQ_I(n, 3);
    CHECK(b[0]=='1' && b[1]=='.' && b[2]=='5'); CHECK(b[3]=='Z');
}
TC_CASE(g_fits_with_nul)
{
    char b[32]; int n; fill(b);
    n = _snprintf(b, 4, "%g", v);
    CHECK_EQ_I(n, 3); CHECK_STR(b, "1.5");
}
int main(void) { tc_begin(); TC_RUN(e_truncated); TC_RUN(f_truncated); TC_RUN(g_exact_no_nul); TC_RUN(g_fits_with_nul); return tc_end(); }
