/* TC
 * id:    pfm_2_func_target_swprintf_vswprintf_ucrt
 * area:  stdio_printf
 * funcs: swprintf vswprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.2.3 swprintf, 7.29.2.7 vswprintf (count parameter; negative return if n or more characters would be needed); MS docs swprintf (UCRT)
 */
#include "tc.h"
#include <stdarg.h>
#include <wchar.h>

static int callv(wchar_t *b, size_t n, const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = vswprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(swprintf_count_form)
{
    wchar_t b[32]; const wchar_t * volatile f = L"%d|%ls"; int n = swprintf(b, 32, f, 5, L"ab");
    CHECK_EQ_I(n, 4); CHECK_WSTR(b, L"5|ab");
}
TC_CASE(swprintf_exact_fit_fails)
{
    wchar_t b[8]; const wchar_t * volatile f = L"%d"; int n = swprintf(b, 3, f, 123);   /* needs 3 chars + NUL */
    CHECK(n < 0);
}
TC_CASE(swprintf_just_fits)
{
    wchar_t b[8]; const wchar_t * volatile f = L"%d"; int n = swprintf(b, 4, f, 123);
    CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"123");
}
TC_CASE(vswprintf_count_form)
{
    wchar_t b[32]; int n = callv(b, 32, L"%x-%X", 255, 255);
    CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"ff-FF");
    n = callv(b, 3, L"%ls", L"abcdef");
    CHECK(n < 0);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(swprintf_count_form); TC_RUN(swprintf_exact_fit_fails); TC_RUN(swprintf_just_fits); TC_RUN(vswprintf_count_form); return tc_end(); }
