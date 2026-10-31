/* TC
 * id:    pfo_vswprintf_opt_trunc
 * area:  stdio_printf
 * funcs: __stdio_common_vswprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _vsnwprintf ("returns a negative value if the number of characters to write exceeds count"); C11 7.29.2.7
 * note:  Buffer contents after a failed legacy truncation are NOT asserted here (see pfo_probe_vsprintf_trunc_nul).
 *        Guard characters beyond count must never be written.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

static int callw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static void fillw(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static int guardw(const wchar_t *b, int from) { int i; for (i = from; i < 16; i++) if (b[i] != L'Z') return 0; return 1; }

TC_CASE(opts0)
{
    wchar_t b[16]; volatile size_t n = 4; int r; fillw(b);
    r = callw(0, b, n, L"%ls", L"abcdefgh");
    CHECK(r < 0);
    CHECK(guardw(b, 4));
}
TC_CASE(opts1)
{
    wchar_t b[16]; volatile size_t n = 4; int r; fillw(b);
    r = callw(1, b, n, L"%ls", L"abcdefgh");
    CHECK(r < 0);
    CHECK(guardw(b, 4));
}
TC_CASE(opts2)
{
    wchar_t b[16]; volatile size_t n = 4; int r; fillw(b);
    r = callw(2, b, n, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, 8);
    CHECK_WSTR(b, L"abc");
    CHECK(guardw(b, 4));
}
TC_CASE(fits_all)
{
    wchar_t b[16]; int i; static const unsigned long long o[3] = {0, 1, 2};
    for (i = 0; i < 3; i++) {
        fillw(b);
        CHECK_EQ_I(callw(o[i], b, 16, L"%ls", L"abc"), 3);
        CHECK_WSTR(b, L"abc");
    }
}
int main(void) { tc_begin(); TC_RUN(opts0); TC_RUN(opts1); TC_RUN(opts2); TC_RUN(fits_all); return tc_end(); }
