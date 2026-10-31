/* TC
 * id:    wcs_neg_wcsncat_s
 * area:  wstring
 * funcs: 
 * procs: wcsncat_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncat_s, wcsncat_s
 * note:  STRUNCATE=80; _TRUNCATE=(size_t)-1
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
TC_DECL_PROC(int, wcsncat_s, (wchar_t *, size_t, const wchar_t *, size_t));
static const wchar_t *volatile pl = L"cdefg";
static void init(wchar_t *d, int n, const wchar_t *s) { int i = 0; for (; i < n; i++) d[i] = 0x5555; for (i = 0; s[i]; i++) d[i] = s[i]; d[i] = 0; }
TC_CASE(too_small) { wchar_t d[4]; int r; if (!tc_p_wcsncat_s || !tc_iph_install()) TC_SKIP("missing"); init(d, 4, L"ab"); tc_iph_count = 0;
    r = tc_p_wcsncat_s(d, 4, pl, 5); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(truncate) { wchar_t d[6]; int r; if (!tc_p_wcsncat_s || !tc_iph_install()) TC_SKIP("missing"); init(d, 6, L"ab"); tc_iph_count = 0;
    r = tc_p_wcsncat_s(d, 6, pl, (size_t)-1); CHECK_EQ_I(r, 80); CHECK_WSTR(d, L"abcde"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(nulls) { wchar_t d[8]; int r; if (!tc_p_wcsncat_s || !tc_iph_install()) TC_SKIP("missing"); init(d, 8, L"ab"); tc_iph_count = 0;
    r = tc_p_wcsncat_s(NULL, 8, pl, 2); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    r = tc_p_wcsncat_s(d, 8, NULL, 2); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 2); }
TC_CASE(unterminated) { wchar_t d[4] = {'a', 'b', 'c', 'd'}; int r; if (!tc_p_wcsncat_s || !tc_iph_install()) TC_SKIP("missing"); tc_iph_count = 0;
    r = tc_p_wcsncat_s(d, 4, pl, 1); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); TC_LOAD_PROC(wcsncat_s); TC_RUN(too_small); TC_RUN(truncate); TC_RUN(nulls); TC_RUN(unterminated); return tc_end(); }
