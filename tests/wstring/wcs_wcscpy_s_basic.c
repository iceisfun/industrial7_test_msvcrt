/* TC
 * id:    wcs_wcscpy_s_basic
 * area:  wstring
 * funcs: 
 * procs: wcscpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strcpy_s, wcscpy_s
 * note:  wcscpy_s via GetProcAddress (mingw libs lack it for old CRTs)
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(int, wcscpy_s, (wchar_t *, size_t, const wchar_t *));
static const wchar_t *volatile pa = L"abc";
static const wchar_t *volatile pe = L"";
static const wchar_t *volatile ph = L"\x0161\x0100\xFF00";
TC_CASE(ret0) { wchar_t d[8]; int r; if (!tc_p_wcscpy_s) TC_SKIP("missing"); r = tc_p_wcscpy_s(d, 8, pa); CHECK_EQ_I(r, 0); CHECK_WSTR(d, L"abc"); }
TC_CASE(exact) { wchar_t d[4]; int r; if (!tc_p_wcscpy_s) TC_SKIP("missing"); r = tc_p_wcscpy_s(d, 4, pa); CHECK_EQ_I(r, 0); CHECK_WSTR(d, L"abc"); }
TC_CASE(empty) { wchar_t d[2] = {7, 7}; int r; if (!tc_p_wcscpy_s) TC_SKIP("missing"); r = tc_p_wcscpy_s(d, 1, pe); CHECK_EQ_I(r, 0); CHECK_EQ_I(d[0], 0); }
TC_CASE(high) { wchar_t d[8]; int r; if (!tc_p_wcscpy_s) TC_SKIP("missing"); r = tc_p_wcscpy_s(d, 8, ph); CHECK_EQ_I(r, 0);
    CHECK_EQ_I(d[0], 0x161); CHECK_EQ_I(d[1], 0x100); CHECK_EQ_I(d[2], 0xFF00); CHECK_EQ_I(d[3], 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(wcscpy_s); TC_RUN(ret0); TC_RUN(exact); TC_RUN(empty); TC_RUN(high); return tc_end(); }
