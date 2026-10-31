/* TC
 * id:    wcs_neg_wcsicmp_l_null
 * area:  wstring
 * funcs:
 * procs: _wcsicmp_l _wcsnicmp_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _stricmp, _wcsicmp, _stricmp_l, _wcsicmp_l; _strnicmp, _wcsnicmp, _wcsnicmp_l
 * note:  Docs: a NULL string makes the invalid parameter handler run, errno=EINVAL, return _NLSCMPERROR (INT_MAX).
 *        Locale argument NULL = current locale (C here).
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>

TC_DECL_PROC(int, _wcsicmp_l, (const wchar_t *, const wchar_t *, void *));
TC_DECL_PROC(int, _wcsnicmp_l, (const wchar_t *, const wchar_t *, size_t, void *));
static const wchar_t *volatile pa = L"abc";
static const wchar_t *volatile pb = L"ABC";
static const wchar_t *volatile pn = NULL;
static void *volatile ploc = NULL;

#define PRE() do { if (!tc_p__wcsicmp_l || !tc_p__wcsnicmp_l) TC_SKIP("not exported"); tc_iph_count = 0; errno = 0; } while (0)
#define BAD() do { CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)
TC_CASE(first_null_nulllocale) { int r; PRE(); r = tc_p__wcsicmp_l(pn, pa, ploc); BAD(); }
TC_CASE(second_null_nulllocale) { int r; PRE(); r = tc_p__wcsicmp_l(pa, pn, ploc); BAD(); }
TC_CASE(both_null) { int r; PRE(); r = tc_p__wcsicmp_l(pn, pn, ploc); BAD(); }
TC_CASE(nicmp_first_null_n3) { int r; PRE(); r = tc_p__wcsnicmp_l(pn, pa, 3, ploc); BAD(); }
TC_CASE(nicmp_second_null_n3) { int r; PRE(); r = tc_p__wcsnicmp_l(pa, pn, 3, ploc); BAD(); }
TC_CASE(count_accumulates_4)
{
    PRE();
    tc_p__wcsicmp_l(pn, pa, ploc); tc_p__wcsicmp_l(pa, pn, ploc);
    tc_p__wcsnicmp_l(pn, pa, 2, ploc); tc_p__wcsnicmp_l(pa, pn, 2, ploc);
    CHECK_EQ_I(tc_iph_count, 4); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(valid_nulllocale_no_iph)
{
    PRE();
    CHECK_EQ_I(tc_p__wcsicmp_l(pa, pb, ploc), 0);
    CHECK_EQ_I(tc_p__wcsnicmp_l(pa, pb, 3, ploc), 0);
    CHECK_EQ_I(tc_p__wcsnicmp_l(pa, pb, 0, ploc), 0);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wcsicmp_l); TC_LOAD_PROC(_wcsnicmp_l);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(first_null_nulllocale); TC_RUN(second_null_nulllocale); TC_RUN(both_null);
    TC_RUN(nicmp_first_null_n3); TC_RUN(nicmp_second_null_n3); TC_RUN(count_accumulates_4);
    TC_RUN(valid_nulllocale_no_iph);
    return tc_end();
}
