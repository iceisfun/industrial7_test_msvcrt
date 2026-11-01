/* TC
 * id:    pfo_neg_vswprintf_options_wrong_type_buffer_guard
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vswprintf __stdio_common_vswprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vswprintf, _vsnwprintf (count is in wide characters), vswprintf_s (buffer[0]=0 on failure), C11 7.29.2.7 vswprintf
 * note:  Guard wchar_t values after the permitted count must never be touched.
 *        OBSERVED Win10 22H2 19045 ucrtbase (run twice): the raw worker with legacy options (0) returns -2 on overflow (the inline _vsnwprintf wrapper in the UCRT headers maps
 *        negative results to -1); the old expectation -1 was for the wrapper, not the worker this test calls.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

#define G 0xA5A5
static int cw(unsigned opt, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(opt, b, n, f, NULL, ap); va_end(ap); return r; }
static int cws(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = G; }
static int guard(const wchar_t *b, int from) { int i; for (i = from; i < 16; i++) if (b[i] != G) return 0; return 1; }

TC_CASE(legacy_overflow)
{
    wchar_t b[16]; int r; fill(b);
    r = cw(0, b, 4, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, -2); CHECK(guard(b, 4));
}
TC_CASE(c99_truncation)
{
    wchar_t b[16]; int r; fill(b);
    r = cw(2, b, 4, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, 8); CHECK_WSTR(b, L"abc"); CHECK(guard(b, 4));
}
TC_CASE(secure_too_small)
{
    wchar_t b[16]; int r; fill(b);
    tc_iph_count = 0;
    r = cws(b, 4, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 0); CHECK(guard(b, 4)); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_query)
{
    int r = cw(2, NULL, 0, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, 8);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(legacy_overflow); TC_RUN(c99_truncation); TC_RUN(secure_too_small); TC_RUN(size_query);
    return tc_end();
}
