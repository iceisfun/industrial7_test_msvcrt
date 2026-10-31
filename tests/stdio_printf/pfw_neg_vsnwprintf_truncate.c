/* TC
 * id:    pfw_neg_vsnwprintf_truncate
 * area:  stdio_printf
 * funcs: _vsnwprintf
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _vsnprintf/_vsnwprintf
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static int vs(wchar_t *b, size_t n, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = _vsnwprintf(b, n, fmt, ap); va_end(ap); return r; }
static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }

TC_CASE(ret_m1)
{ wchar_t b[16]; fill(b); CHECK_EQ_I(vs(b, 4, L"%ls", L"abcdefgh"), -1); }
TC_CASE(count_chars_written)
{ wchar_t b[16]; fill(b); vs(b, 4, L"%ls", L"abcdefgh"); CHECK(b[0]==L'a' && b[1]==L'b' && b[2]==L'c' && b[3]==L'd'); }
TC_CASE(no_overrun)
{ wchar_t b[16]; fill(b); vs(b, 4, L"%ls", L"abcdefgh"); CHECK(b[4] == L'Z' && b[5] == L'Z' && b[15] == L'Z'); }
TC_CASE(exact_fit_not_error)
{
    wchar_t b[16]; int n; fill(b);
    n = vs(b, 4, L"%ls", L"abcd");
    CHECK_EQ_I(n, 4); CHECK(b[3] == L'd'); CHECK(b[4] == L'Z');
}
int main(void) { tc_begin(); TC_RUN(ret_m1); TC_RUN(count_chars_written); TC_RUN(no_overrun); TC_RUN(exact_fit_not_error); return tc_end(); }
