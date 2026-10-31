/* TC
 * id:    pfr_vswprintf_3arg_ucrt
 * area:  stdio_printf
 * funcs: vswprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.2.7 vswprintf
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

static int cv(wchar_t *b, size_t n, const wchar_t *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = vswprintf(b, n, f, ap); va_end(ap);
    return r;
}

TC_CASE(fit)
{
    wchar_t b[16]; volatile size_t n = 16; int r;
    r = cv(b, n, L"%d-%ls", 9, L"zz");
    CHECK_EQ_I(r, 4);
    CHECK_WSTR(b, L"9-zz");
}
TC_CASE(empty)
{
    wchar_t b[4]; volatile size_t n = 4; int r;
    b[0] = L'Z';
    r = cv(b, n, L"%ls", L"");
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(wide_chars)
{
    wchar_t b[8]; volatile size_t n = 8; int r;
    r = cv(b, n, L"%ls", L"\x4e2d\x6587");
    CHECK_EQ_I(r, 2);
    CHECK_EQ_I(b[0], 0x4e2d);
    CHECK_EQ_I(b[1], 0x6587);
    CHECK_EQ_I(b[2], 0);
}

int main(void) { tc_begin(); TC_RUN(fit); TC_RUN(empty); TC_RUN(wide_chars); return tc_end(); }
