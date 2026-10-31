/* TC
 * id:    wcs_neg_wcscat_s
 * area:  wstring
 * funcs: 
 * procs: wcscat_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcat_s, wcscat_s
 * note:  error returns and invalid-parameter handler; dest content asserted only where docs state it
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(int, wcscat_s, (wchar_t *, size_t, const wchar_t *));
static const wchar_t *volatile pcd = L"cd";
static const wchar_t *volatile pcdef = L"cdef";
static void init(wchar_t *d, int n, const wchar_t *s) { int i = 0; for (; i < n; i++) d[i] = 0x5555; for (i = 0; s[i]; i++) d[i] = s[i]; d[i] = 0; }
TC_CASE(too_small) { wchar_t d[4]; int r; if (!tc_p_wcscat_s || !tc_iph_install()) TC_SKIP("missing"); init(d, 4, L"ab"); tc_iph_count = 0;
    r = tc_p_wcscat_s(d, 4, pcdef); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(unterminated) { wchar_t d[4] = {'a', 'b', 'c', 'd'}; int r; if (!tc_p_wcscat_s || !tc_iph_install()) TC_SKIP("missing"); tc_iph_count = 0;
    r = tc_p_wcscat_s(d, 4, pcd); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_args) { wchar_t d[8]; int r; if (!tc_p_wcscat_s || !tc_iph_install()) TC_SKIP("missing"); init(d, 8, L"ab"); tc_iph_count = 0;
    r = tc_p_wcscat_s(NULL, 8, pcd); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    r = tc_p_wcscat_s(d, 8, NULL); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 2); }
TC_CASE(size_zero) { wchar_t d[4]; int r; if (!tc_p_wcscat_s || !tc_iph_install()) TC_SKIP("missing"); init(d, 4, L"a"); tc_iph_count = 0;
    r = tc_p_wcscat_s(d, 0, pcd); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(d[0], 'a'); }
TC_CASE(exact_fit) { wchar_t d[5]; int r; if (!tc_p_wcscat_s || !tc_iph_install()) TC_SKIP("missing"); init(d, 5, L"ab"); tc_iph_count = 0;
    r = tc_p_wcscat_s(d, 5, pcd); CHECK_EQ_I(r, 0); CHECK_WSTR(d, L"abcd"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(wcscat_s); TC_RUN(too_small); TC_RUN(unterminated); TC_RUN(null_args); TC_RUN(size_zero); TC_RUN(exact_fit); return tc_end(); }
