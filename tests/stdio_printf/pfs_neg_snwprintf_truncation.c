/* TC
 * id:    pfs_neg_snwprintf_truncation
 * area:  stdio_printf
 * funcs: _snwprintf

 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf, _snwprintf: returns -1 if more than count chars needed; NUL not appended when count chars written

 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static const wchar_t *volatile fmt_abcdef = L"abcdef";
TC_CASE(too_small)
{
    wchar_t w[16]; int n; fill(w);
    n = _snwprintf(w, 4, fmt_abcdef);
    CHECK_EQ_I(n, -1);
    CHECK(w[0] == L'a' && w[1] == L'b' && w[2] == L'c' && w[3] == L'd');
    CHECK(w[4] == L'Z');
}
TC_CASE(count_equals_len)
{
    wchar_t w[16]; int n; fill(w);
    n = _snwprintf(w, 6, fmt_abcdef);
    CHECK_EQ_I(n, 6);
    CHECK(w[5] == L'f');
    CHECK(w[6] == L'Z');
}
TC_CASE(count_zero)
{
    wchar_t w[16]; int n; fill(w);
    n = _snwprintf(w, 0, fmt_abcdef);
    CHECK_EQ_I(n, -1);
    CHECK(w[0] == L'Z');
}
int main(void) { tc_begin(); TC_RUN(too_small); TC_RUN(count_equals_len); TC_RUN(count_zero); return tc_end(); }
