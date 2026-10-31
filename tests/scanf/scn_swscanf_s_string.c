/* TC
 * id:    scn_swscanf_s_string
 * area:  scanf
 * funcs: swscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs wscanf_s, swscanf_s: size is a count of elements (wchar_t for %s/%c in wide functions, bytes for %hs/%hc)
 */
#include "tc.h"
#include <wchar.h>

static volatile const wchar_t *v;
TC_CASE(ls_size) { wchar_t a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = L'Z'; v = L"abc def"; n = swscanf_s((const wchar_t *)v, L"%ls", a, (unsigned)8); CHECK_EQ_I(n, 1); CHECK_WSTR(a, L"abc"); CHECK_EQ_U(a[4], L'Z'); }
TC_CASE(ls_exact) { wchar_t a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = L'Z'; v = L"abc"; n = swscanf_s((const wchar_t *)v, L"%ls", a, (unsigned)4); CHECK_EQ_I(n, 1); CHECK_WSTR(a, L"abc"); CHECK_EQ_U(a[4], L'Z'); }
TC_CASE(lc_size) { wchar_t c = L'Z'; int n; v = L"qr"; n = swscanf_s((const wchar_t *)v, L"%lc", &c, (unsigned)1); CHECK_EQ_I(n, 1); CHECK_EQ_U(c, L'q'); }
TC_CASE(l_scanset) { wchar_t a[16]; int n; v = L"abc123"; n = swscanf_s((const wchar_t *)v, L"%l[a-z]", a, (unsigned)16); CHECK_EQ_I(n, 1); CHECK_WSTR(a, L"abc"); }
TC_CASE(hs_narrow_bytes) { char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z'; v = L"abc def"; n = swscanf_s((const wchar_t *)v, L"%hs", a, (unsigned)4); CHECK_EQ_I(n, 1); CHECK_STR(a, "abc"); CHECK_EQ_I(a[4], 'Z'); }
int main(void) { tc_begin(); TC_RUN(ls_size); TC_RUN(ls_exact); TC_RUN(lc_size); TC_RUN(l_scanset); TC_RUN(hs_narrow_bytes); return tc_end(); }
