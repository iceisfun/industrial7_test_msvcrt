/* TC
 * id:    wcs_wcsset_basic
 * area:  wstring
 * funcs: _wcsset _wcsnset
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _strset, _wcsset, _strnset, _wcsnset
 * note:  Fill with 0 is deliberately not tested (docs do not define it).
 */
#include "tc.h"
#include <wchar.h>
#include <string.h>

static volatile wchar_t v_c = L'x';
static volatile size_t v_n;

TC_CASE(set_fills_all)
{
    wchar_t b[8] = L"abcde"; wchar_t *r = _wcsset(b, v_c);
    CHECK_EQ_P(r, b); CHECK_WSTR(b, L"xxxxx"); CHECK_EQ_I(b[5], 0);
}
TC_CASE(nset_first_n)
{
    wchar_t b[8] = L"abcde"; wchar_t *r; v_n = 2; r = _wcsnset(b, v_c, v_n);
    CHECK_EQ_P(r, b); CHECK_WSTR(b, L"xxcde");
}
TC_CASE(nset_n_larger_stops_at_nul)
{
    wchar_t b[10] = L"abc"; wchar_t *r; int i; v_n = 8;
    for (i = 4; i < 10; i++) b[i] = L'Z';
    r = _wcsnset(b, v_c, v_n);
    CHECK_EQ_P(r, b); CHECK_WSTR(b, L"xxx"); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], L'Z');
}
TC_CASE(nset_zero_count) { wchar_t b[4] = L"abc"; v_n = 0; _wcsnset(b, v_c, v_n); CHECK_WSTR(b, L"abc"); }
TC_CASE(empty_string)
{
    wchar_t b[4] = { 0, L'Q', L'Q', 0 };
    CHECK_EQ_P(_wcsset(b, v_c), b); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], L'Q');
    v_n = 3; _wcsnset(b, v_c, v_n); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], L'Q');
}
TC_CASE(fill_ffff)
{
    wchar_t b[6] = L"abcd"; v_c = (wchar_t)0xFFFF; _wcsset(b, v_c); v_c = L'x';
    CHECK_EQ_U(b[0], 0xFFFF); CHECK_EQ_U(b[3], 0xFFFF); CHECK_EQ_I(b[4], 0);
    CHECK_EQ_U(wcslen(b), 4);
}
int main(void) { tc_begin(); TC_RUN(set_fills_all); TC_RUN(nset_first_n); TC_RUN(nset_n_larger_stops_at_nul); TC_RUN(nset_zero_count); TC_RUN(empty_string); TC_RUN(fill_ffff); return tc_end(); }
