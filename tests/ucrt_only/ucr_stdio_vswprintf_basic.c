/* TC
 * id:    ucr_stdio_vswprintf_basic
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vswprintf
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Format specification syntax (%ls %hs %lc %hc), swprintf/_snwprintf return values, _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR (0x2)
 * note:  explicit size prefixes avoid the %s ambiguity in wide functions
 */
#include "tc.h"
#include <stdarg.h>

typedef int (__cdecl *vsw_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsw_t p_vsw;
static int callw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsw(o, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(size_modifiers)
{
    wchar_t b[64]; int r;
    if (!p_vsw) TC_SKIP("not exported");
    r = callw(0, b, 64, L"%ls %hs %lc %hc", L"wide", "narrow", (wint_t)L'x', 'y');
    CHECK_WSTR(b, L"wide narrow x y"); CHECK_EQ_I(r, 15);
}
TC_CASE(numbers)
{
    wchar_t b[64]; int r;
    if (!p_vsw) TC_SKIP("not exported");
    r = callw(0, b, 64, L"%d|%5.1f|%x", -12, 3.14159, 255u);
    CHECK_WSTR(b, L"-12|  3.1|ff"); CHECK_EQ_I(r, 12);
}
TC_CASE(count_is_wchars)
{
    wchar_t b[64]; int r;
    if (!p_vsw) TC_SKIP("not exported");
    r = callw(0, b, 64, L"%ls", L"\x00e9\x4e2d\x6587");
    CHECK_EQ_I(r, 3); CHECK_EQ_I(b[0], 0xe9); CHECK_EQ_I(b[2], 0x6587); CHECK_EQ_I(b[3], 0);
}
TC_CASE(std_snprintf_truncation)
{
    wchar_t b[4]; int r;
    if (!p_vsw) TC_SKIP("not exported");
    r = callw(2, b, 4, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, 8); CHECK_WSTR(b, L"abc");
}
TC_CASE(std_snprintf_query_size)
{
    int r;
    if (!p_vsw) TC_SKIP("not exported");
    r = callw(2, NULL, 0, L"%d", 12345);
    CHECK_EQ_I(r, 5);
}
TC_CASE(legacy_truncation_negative)
{
    wchar_t b[4]; int r;
    if (!p_vsw) TC_SKIP("not exported");
    r = callw(0, b, 4, L"%ls", L"abcdefgh");
    CHECK(r < 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vsw = (vsw_t)tc_crt_proc("__stdio_common_vswprintf");
    TC_RUN(size_modifiers); TC_RUN(numbers); TC_RUN(count_is_wchars); TC_RUN(std_snprintf_truncation);
    TC_RUN(std_snprintf_query_size); TC_RUN(legacy_truncation_negative);
    return tc_end();
}
