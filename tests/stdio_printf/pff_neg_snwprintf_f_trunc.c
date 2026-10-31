/* TC
 * id:    pff_neg_snwprintf_f_trunc
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _snwprintf: count is in wide characters; no NUL when the output does not fit; returns -1
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define G 0x5A5A
static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = G; }

TC_CASE(truncated)
{
    wchar_t wb[16]; volatile double v = 1.5; int r;
    fill(wb);
    r = _snwprintf(wb, 4, L"%f", v);
    CHECK_EQ_I(r, -1);
    CHECK(wb[0] == L'1' && wb[1] == L'.' && wb[2] == L'5' && wb[3] == L'0');
    CHECK_EQ_I(wb[4], G);
}
TC_CASE(exact_fit)
{
    wchar_t wb[16]; volatile double v = 1.5; int r;
    fill(wb);
    r = _snwprintf(wb, 8, L"%f", v);
    CHECK_EQ_I(r, 8);
    CHECK(wb[0] == L'1' && wb[7] == L'0');
    CHECK_EQ_I(wb[8], G);
}
TC_CASE(one_over)
{
    wchar_t wb[16]; volatile double v = 1.5; int r;
    fill(wb);
    r = _snwprintf(wb, 7, L"%f", v);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(wb[6], L'0');
    CHECK_EQ_I(wb[7], G);
}
TC_CASE(roomy)
{
    wchar_t wb[16]; volatile double v = 1.5; int r;
    fill(wb);
    r = _snwprintf(wb, 9, L"%f", v);
    CHECK_EQ_I(r, 8);
    CHECK_EQ_I(wb[8], 0);
}
int main(void) { tc_begin(); TC_RUN(truncated); TC_RUN(exact_fit); TC_RUN(one_over); TC_RUN(roomy); return tc_end(); }
