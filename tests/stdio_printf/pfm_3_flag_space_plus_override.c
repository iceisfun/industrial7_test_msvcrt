/* TC
 * id:    pfm_3_flag_space_plus_override
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p6 / 7.29.2.1 (if both space and + flags appear, the space flag is ignored); MS docs printf flag directives
 * note:  Order of the two flags does not matter. Wide cases use _snwprintf (count form, exported by every CRT) instead of swprintf, whose
 *        prototype differs between pre-UCRT and UCRT. Exponents have 3 digits before ucrtbase and 2 on ucrtbase.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
#define XE "e+00"
#define WXE L"e+00"
#else
#define XE "e+000"
#define WXE L"e+000"
#endif

static volatile int v5 = 5, vm5 = -5, v0 = 0;
static volatile double d1 = 1.0, dm1 = -1.0;

TC_CASE(int_both_orders)
{
    char b[64];
    sprintf(b, "[% +d][%+ d][% +i][%+ i]", v5, v5, v5, v5);
    CHECK_STR(b, "[+5][+5][+5][+5]");
}
TC_CASE(int_negative_and_zero)
{
    char b[64];
    sprintf(b, "[% +d][%+ d][% +d]", vm5, vm5, v0);
    CHECK_STR(b, "[-5][-5][+0]");
}
TC_CASE(float_efg)
{
    char b[96];
    sprintf(b, "[%+ e][% +f][%+ g]", d1, d1, d1);
    CHECK_STR(b, "[+1.000000" XE "][+1.000000][+1]");
}
TC_CASE(float_negative)
{
    char b[96];
    sprintf(b, "[% +f][%+ g]", dm1, dm1);
    CHECK_STR(b, "[-1.000000][-1]");
}
TC_CASE(combo_with_width_minus)
{
    char b[96];
    sprintf(b, "[% -+6d][%-+ 6d][% +6d][% +06d]", v5, v5, v5, v5);
    CHECK_STR(b, "[+5    ][+5    ][    +5][+00005]");
}
TC_CASE(wide_int)
{
    wchar_t b[64]; int n;
    n = _snwprintf(b, 64, L"[% +d][%+ d][% +d][% -+6d]", v5, v5, vm5, v5);
    CHECK_WSTR(b, L"[+5][+5][-5][+5    ]"); CHECK_EQ_I(n, 20);
}
TC_CASE(wide_float)
{
    wchar_t b[96];
    _snwprintf(b, 96, L"[% +e][% +f][% +g]", d1, d1, d1);
    CHECK_WSTR(b, L"[+1.000000" WXE L"][+1.000000][+1]");
}

int main(void)
{
    tc_begin();
    TC_RUN(int_both_orders); TC_RUN(int_negative_and_zero); TC_RUN(float_efg); TC_RUN(float_negative);
    TC_RUN(combo_with_width_minus); TC_RUN(wide_int); TC_RUN(wide_float);
    return tc_end();
}
