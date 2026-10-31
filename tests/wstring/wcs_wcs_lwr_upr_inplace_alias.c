/* TC
 * id:    wcs_wcs_lwr_upr_inplace_alias
 * area:  wstring
 * funcs: _wcslwr _wcsupr
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _strlwr, _wcslwr, _strupr, _wcsupr (convert in place, return the argument; "C" locale: ASCII only)
 */
#include "tc.h"
#include <wchar.h>

#define N 500
static void fill(wchar_t *b, int upper_case_src)
{
    int i;
    for (i = 0; i < N; i++) {
        switch (i % 5) {
        case 0: b[i] = upper_case_src ? L'A' + (i % 26) : L'a' + (i % 26); break;
        case 1: b[i] = upper_case_src ? L'a' + (i % 26) : L'A' + (i % 26); break;
        case 2: b[i] = L'0' + (i % 10); break;
        case 3: b[i] = L'_'; break;
        default: b[i] = L'-'; break;
        }
    }
    b[N] = 0;
}
static int check(const wchar_t *b, int upper)
{
    int i;
    for (i = 0; i < N; i++) {
        wchar_t c = b[i], e;
        switch (i % 5) {
        case 0: case 1: e = upper ? L'A' + (i % 26) : L'a' + (i % 26); break;
        case 2: e = L'0' + (i % 10); break;
        case 3: e = L'_'; break;
        default: e = L'-'; break;
        }
        if (c != e) return i + 1;
    }
    return b[N] == 0 ? 0 : -1;
}
TC_CASE(long_lower) { static wchar_t b[N + 1]; wchar_t *volatile p = b; fill(b, 0); CHECK_EQ_P(_wcslwr(p), b); CHECK_EQ_I(check(b, 0), 0); }
TC_CASE(long_upper) { static wchar_t b[N + 1]; wchar_t *volatile p = b; fill(b, 0); CHECK_EQ_P(_wcsupr(p), b); CHECK_EQ_I(check(b, 1), 0); }
TC_CASE(offset_pointer)
{
    wchar_t b[8] = L"ABCDEF"; wchar_t *volatile p = b + 2;
    CHECK_EQ_P(_wcslwr(p), b + 2); CHECK_WSTR(b, L"ABcdef");
    CHECK_EQ_P(_wcsupr(p), b + 2); CHECK_WSTR(b, L"ABCDEF");
}
TC_CASE(buffer_exact_end)
{
    wchar_t b[4] = { L'a', L'B', L'c', 0 }; wchar_t *volatile p = b;
    _wcsupr(p); CHECK_WSTR(b, L"ABC"); _wcslwr(p); CHECK_WSTR(b, L"abc");
}
TC_CASE(digits_untouched) { wchar_t b[16] = L"a1b2c3-X9"; wchar_t *volatile p = b; _wcsupr(p); CHECK_WSTR(b, L"A1B2C3-X9"); _wcslwr(p); CHECK_WSTR(b, L"a1b2c3-x9"); }
TC_CASE(empty) { wchar_t b[2] = { 0, L'Q' }; wchar_t *volatile p = b; CHECK_EQ_P(_wcslwr(p), b); CHECK_EQ_P(_wcsupr(p), b); CHECK_EQ_I(b[1], L'Q'); }
int main(void) { tc_begin(); TC_RUN(long_lower); TC_RUN(long_upper); TC_RUN(offset_pointer); TC_RUN(buffer_exact_end); TC_RUN(digits_untouched); TC_RUN(empty); return tc_end(); }
