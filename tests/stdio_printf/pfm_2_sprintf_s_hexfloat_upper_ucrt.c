/* TC
 * id:    pfm_2_sprintf_s_hexfloat_upper_ucrt
 * area:  stdio_printf
 * funcs: sprintf_s swprintf_s
 * crt:   >=140
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (a A F conversions); MS docs printf type field (a, A supported from VS2013/UCRT)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile double v1 = 1.0, v15 = 1.5, vm2 = -2.0;

TC_CASE(narrow_a_A)
{
    char b[64]; int r = sprintf_s(b, sizeof b, "%.2a|%.1A|%.0a", v1, vm2, v1);
    CHECK_STR(b, "0x1.00p+0|-0X1.0P+1|0x1p+0"); CHECK_EQ_I(r, 26);
}
TC_CASE(narrow_F) { char b[64]; int r = sprintf_s(b, sizeof b, "%F|%.1F", v15, v15); CHECK_STR(b, "1.500000|1.5"); CHECK_EQ_I(r, 12); }
TC_CASE(wide_a_A)
{
    wchar_t b[64]; int r = swprintf_s(b, 64, L"%.2a|%.1A", v1, vm2);
    CHECK_WSTR(b, L"0x1.00p+0|-0X1.0P+1"); CHECK_EQ_I(r, 19);
}
TC_CASE(wide_F) { wchar_t b[64]; int r = swprintf_s(b, 64, L"%F|%.3F", v15, v15); CHECK_WSTR(b, L"1.500000|1.500"); CHECK_EQ_I(r, 14); }
int main(void) { tc_begin(); TC_RUN(narrow_a_A); TC_RUN(narrow_F); TC_RUN(wide_a_A); TC_RUN(wide_F); return tc_end(); }
