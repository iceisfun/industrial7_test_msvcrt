/* TC
 * id:    wcs_neg_wcscoll_l_null
 * area:  wstring
 * funcs:
 * procs: _wcscoll_l _wcsicoll_l _wcsncoll_l _wcsnicoll_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcoll, wcscoll, _wcscoll_l, _wcsicoll_l, _wcsncoll_l, _wcsnicoll_l (return _NLSCMPERROR on error)
 * note:  NULL string -> invalid parameter handler, errno EINVAL, return INT_MAX. Valid strings with NULL locale (C) order a < b.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>

TC_DECL_PROC(int, _wcscoll_l, (const wchar_t *, const wchar_t *, void *));
TC_DECL_PROC(int, _wcsicoll_l, (const wchar_t *, const wchar_t *, void *));
TC_DECL_PROC(int, _wcsncoll_l, (const wchar_t *, const wchar_t *, size_t, void *));
TC_DECL_PROC(int, _wcsnicoll_l, (const wchar_t *, const wchar_t *, size_t, void *));
static const wchar_t *volatile pa = L"a";
static const wchar_t *volatile pb = L"b";
static const wchar_t *volatile pn = NULL;
static void *volatile ploc = NULL;

#define PRE() do { if (!tc_p__wcscoll_l || !tc_p__wcsicoll_l || !tc_p__wcsncoll_l || !tc_p__wcsnicoll_l) TC_SKIP("not exported"); \
    tc_iph_count = 0; errno = 0; } while (0)
#define BAD() do { CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)
TC_CASE(coll_l_first_null) { int r; PRE(); r = tc_p__wcscoll_l(pn, pa, ploc); BAD(); }
TC_CASE(coll_l_second_null) { int r; PRE(); r = tc_p__wcscoll_l(pa, pn, ploc); BAD(); }
TC_CASE(icoll_l_first_null) { int r; PRE(); r = tc_p__wcsicoll_l(pn, pa, ploc); BAD(); }
TC_CASE(icoll_l_second_null) { int r; PRE(); r = tc_p__wcsicoll_l(pa, pn, ploc); BAD(); }
TC_CASE(ncoll_l_null)
{
    int r; PRE();
    r = tc_p__wcsncoll_l(pn, pa, 1, ploc); BAD();
    tc_iph_count = 0; errno = 0;
    r = tc_p__wcsncoll_l(pa, pn, 1, ploc); BAD();
}
TC_CASE(nicoll_l_null)
{
    int r; PRE();
    r = tc_p__wcsnicoll_l(pn, pa, 1, ploc); BAD();
    tc_iph_count = 0; errno = 0;
    r = tc_p__wcsnicoll_l(pa, pn, 1, ploc); BAD();
}
TC_CASE(valid_control_no_iph)
{
    PRE();
    CHECK(tc_p__wcscoll_l(pa, pb, ploc) < 0);
    CHECK(tc_p__wcsicoll_l(pa, pb, ploc) < 0);
    CHECK(tc_p__wcsncoll_l(pa, pb, 1, ploc) < 0);
    CHECK(tc_p__wcsnicoll_l(pa, pb, 1, ploc) < 0);
    CHECK_EQ_I(tc_p__wcscoll_l(pa, pa, ploc), 0);
    CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wcscoll_l); TC_LOAD_PROC(_wcsicoll_l); TC_LOAD_PROC(_wcsncoll_l); TC_LOAD_PROC(_wcsnicoll_l);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(coll_l_first_null); TC_RUN(coll_l_second_null); TC_RUN(icoll_l_first_null); TC_RUN(icoll_l_second_null);
    TC_RUN(ncoll_l_null); TC_RUN(nicoll_l_null); TC_RUN(valid_control_no_iph);
    return tc_end();
}
