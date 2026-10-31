/* TC
 * id:    pfw_vsnwprintf_basic
 * area:  stdio_printf
 * funcs: _vsnwprintf
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs vsnprintf, _vsnprintf, _vsnprintf_l, _vsnwprintf, _vsnwprintf_l
 * note:  _vsnwprintf return/NUL semantics identical to _snwprintf
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }
static int wrap(wchar_t *b, size_t cnt, const wchar_t *fmt, ...)
{ va_list ap; int n; va_start(ap, fmt); n = _vsnwprintf(b, cnt, fmt, ap); va_end(ap); return n; }

TC_CASE(int_and_wide_str)
{
    wchar_t b[32]; fill(b, 32);
    wrap(b, 32, L"%d:%ls", 42, L"xyz");
    CHECK_WSTR(b, L"42:xyz");
}
TC_CASE(return_count)
{
    wchar_t b[32]; int n; fill(b, 32);
    n = wrap(b, 32, L"%d-%d", 100, 200);
    CHECK_EQ_I(n, 7);
}
TC_CASE(exact_fit_no_nul)
{
    wchar_t b[16]; int n; fill(b, 16);
    n = wrap(b, 4, L"%ls", L"wxyz");
    CHECK_EQ_I(n, 4); CHECK(b[3] == L'z'); CHECK(b[4] == L'Z');
}
TC_CASE(zero_length_result)
{
    wchar_t b[8]; int n; fill(b, 8);
    n = wrap(b, 8, L"%ls", L"");
    CHECK_EQ_I(n, 0); CHECK(b[0] == 0);
}

int main(void) { tc_begin(); TC_RUN(int_and_wide_str); TC_RUN(return_count); TC_RUN(exact_fit_no_nul); TC_RUN(zero_length_result); return tc_end(); }
