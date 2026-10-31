/* TC
 * id:    gsm_wcsset_wcsnset_basic
 * area:  string
 * funcs: _wcsset _wcsnset
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _strset, _strnset, _wcsset, _wcsnset (wide-character versions)
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>

static void guard(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'G'; }

TC_CASE(set_all)
{
    wchar_t b[10]; wchar_t *volatile r; guard(b, 10);
    b[0] = L'a'; b[1] = L'b'; b[2] = L'c'; b[3] = 0;
    r = _wcsset(b, L'x'); CHECK_WSTR(b, L"xxx"); CHECK_EQ_P(r, b); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], L'G');
}
TC_CASE(set_empty) { wchar_t b[4] = L""; wchar_t *volatile r = _wcsset(b, L'x'); CHECK_EQ_P(r, b); CHECK_EQ_I(b[0], 0); }
TC_CASE(nset_lengths)
{
    wchar_t b[10]; wchar_t *volatile r; guard(b, 10);
    b[0] = L'a'; b[1] = L'b'; b[2] = L'c'; b[3] = L'd'; b[4] = 0;
    r = _wcsnset(b, L'z', 2); CHECK_WSTR(b, L"zzcd"); CHECK_EQ_P(r, b);
    _wcsnset(b, L'y', 4); CHECK_WSTR(b, L"yyyy"); CHECK_EQ_I(b[4], 0); CHECK_EQ_I(b[5], L'G');
    guard(b, 10); b[0] = L'a'; b[1] = 0;
    _wcsnset(b, L'q', 8); CHECK_WSTR(b, L"q"); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], L'G'); CHECK_EQ_I(b[9], L'G');
}
TC_CASE(nset_zero) { wchar_t b[6] = L"abc"; _wcsnset(b, L'z', 0); CHECK_WSTR(b, L"abc"); }
TC_CASE(non_ascii)
{
    wchar_t b[6] = L"abc";
    _wcsset(b, 0x20AC);
    CHECK_EQ_U(b[0], 0x20AC); CHECK_EQ_U(b[1], 0x20AC); CHECK_EQ_U(b[2], 0x20AC); CHECK_EQ_U(b[3], 0);
    _wcsnset(b, 0x4E2D, 1); CHECK_EQ_U(b[0], 0x4E2D); CHECK_EQ_U(b[1], 0x20AC);
}
int main(void) { tc_begin(); TC_RUN(set_all); TC_RUN(set_empty); TC_RUN(nset_lengths); TC_RUN(nset_zero); TC_RUN(non_ascii); return tc_end(); }
