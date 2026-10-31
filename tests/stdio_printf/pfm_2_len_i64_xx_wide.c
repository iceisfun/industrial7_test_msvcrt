/* TC
 * id:    pfm_2_len_i64_xx_wide
 * area:  stdio_printf
 * funcs: _snwprintf swprintf_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf Size prefixes (I, I32, I64) and wprintf type fields x X o u
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stddef.h>

static wchar_t b[64];
TC_CASE(I64x)  { volatile unsigned long long v = 0x123456789abcdefULL; _snwprintf(b, 63, L"%I64x", v); CHECK_WSTR(b, L"123456789abcdef"); }
TC_CASE(I32X)  { volatile unsigned v = 0xDEADBEEFu; _snwprintf(b, 63, L"%I32X", v); CHECK_WSTR(b, L"DEADBEEF"); }
TC_CASE(I64o)  { volatile unsigned long long v = 8; _snwprintf(b, 63, L"%I64o", v); CHECK_WSTR(b, L"10"); }
TC_CASE(I64u)  { volatile unsigned long long v = ~0ULL; _snwprintf(b, 63, L"%I64u", v); CHECK_WSTR(b, L"18446744073709551615"); }
TC_CASE(Ix)    { volatile size_t v = 255; _snwprintf(b, 63, L"%Ix", v); CHECK_WSTR(b, L"ff"); }
TC_CASE(swprintf_s_I64X)
{
    volatile unsigned long long v = 0xCAFEBABEDEADULL; int r;
    r = swprintf_s(b, 64, L"%I64X", v); CHECK_WSTR(b, L"CAFEBABEDEAD"); CHECK_EQ_I(r, 12);
}
int main(void)
{
    tc_begin();
    TC_RUN(I64x); TC_RUN(I32X); TC_RUN(I64o); TC_RUN(I64u); TC_RUN(Ix); TC_RUN(swprintf_s_I64X);
    return tc_end();
}
