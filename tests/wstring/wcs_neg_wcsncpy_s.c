/* TC
 * id:    wcs_neg_wcsncpy_s
 * area:  wstring
 * funcs: 
 * procs: wcsncpy_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncpy_s, wcsncpy_s; STRUNCATE=80, _TRUNCATE=(size_t)-1
 * note:  error returns, truncation, handler
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(int, wcsncpy_s, (wchar_t *, size_t, const wchar_t *, size_t));
#define TRUNC ((size_t)-1)
#define STRUNC 80
static const wchar_t *volatile pl = L"abcdef";
TC_CASE(count_ge_size) { wchar_t d[4]; int r; if (!tc_p_wcsncpy_s || !tc_iph_install()) TC_SKIP("missing"); d[0] = 'x'; tc_iph_count = 0;
    r = tc_p_wcsncpy_s(d, 4, pl, 6); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(truncate) { wchar_t d[4]; int r; if (!tc_p_wcsncpy_s || !tc_iph_install()) TC_SKIP("missing"); tc_iph_count = 0;
    r = tc_p_wcsncpy_s(d, 4, pl, TRUNC); CHECK_EQ_I(r, STRUNC); CHECK_WSTR(d, L"abc"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(count_lt_len) { wchar_t d[8]; int r; if (!tc_p_wcsncpy_s || !tc_iph_install()) TC_SKIP("missing"); tc_iph_count = 0;
    r = tc_p_wcsncpy_s(d, 8, pl, 3); CHECK_EQ_I(r, 0); CHECK_WSTR(d, L"abc"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(null_dest) { int r; if (!tc_p_wcsncpy_s || !tc_iph_install()) TC_SKIP("missing"); tc_iph_count = 0;
    r = tc_p_wcsncpy_s(NULL, 8, pl, 3); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size_zero) { wchar_t d[4]; int r; if (!tc_p_wcsncpy_s || !tc_iph_install()) TC_SKIP("missing"); d[0] = 'x'; tc_iph_count = 0;
    r = tc_p_wcsncpy_s(d, 0, pl, 3); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(d[0], 'x'); }
TC_CASE(null_src_count3) { wchar_t d[4]; int r; if (!tc_p_wcsncpy_s || !tc_iph_install()) TC_SKIP("missing"); d[0] = 'x'; tc_iph_count = 0;
    r = tc_p_wcsncpy_s(d, 4, NULL, 3); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); TC_LOAD_PROC(wcsncpy_s); TC_RUN(count_ge_size); TC_RUN(truncate); TC_RUN(count_lt_len); TC_RUN(null_dest); TC_RUN(size_zero); TC_RUN(null_src_count3); return tc_end(); }
