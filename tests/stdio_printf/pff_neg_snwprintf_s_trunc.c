/* TC
 * id:    pff_neg_snwprintf_s_trunc
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snwprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snprintf_s_l, _snwprintf_s: _TRUNCATE truncates and returns -1 without invoking the handler
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(truncate)
{
    wchar_t wb[16]; volatile double v = 1.5; int r, i;
    for (i = 0; i < 16; i++) wb[i] = 0x5A5A;
    tc_iph_count = 0;
    r = _snwprintf_s(wb, 5, _TRUNCATE, L"%f", v);
    CHECK_EQ_I(r, -1);
    CHECK_WSTR(wb, L"1.50");
    CHECK_EQ_I(wb[5], 0x5A5A);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(fits)
{
    wchar_t wb[16]; volatile double v = 1.5; int r;
    tc_iph_count = 0;
    r = _snwprintf_s(wb, 9, _TRUNCATE, L"%f", v);
    CHECK_EQ_I(r, 8);
    CHECK_WSTR(wb, L"1.500000");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(truncate); TC_RUN(fits); return tc_end(); }
