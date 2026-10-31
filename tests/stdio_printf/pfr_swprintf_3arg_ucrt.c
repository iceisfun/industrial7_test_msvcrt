/* TC
 * id:    pfr_swprintf_3arg_ucrt
 * area:  stdio_printf
 * funcs: swprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.2.3 swprintf; MS docs swprintf (UCRT: conforming 3-arg form)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(fit)
{
    wchar_t b[10]; volatile size_t n = 10; int r;
    r = swprintf(b, n, L"%d", 42);
    CHECK_EQ_I(r, 2);
    CHECK_WSTR(b, L"42");
}
TC_CASE(exact_size)
{
    wchar_t b[8]; volatile size_t n = 4; int r;
    r = swprintf(b, n, L"%ls", L"abc");
    CHECK_EQ_I(r, 3);
    CHECK_WSTR(b, L"abc");
}
TC_CASE(empty)
{
    wchar_t b[4]; volatile size_t n = 4; int r;
    b[0] = L'Z';
    r = swprintf(b, n, L"%ls", L"");
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(b[0], 0);
}

int main(void) { tc_begin(); TC_RUN(fit); TC_RUN(exact_size); TC_RUN(empty); return tc_end(); }
