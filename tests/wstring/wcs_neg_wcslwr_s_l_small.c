/* TC
 * id:    wcs_neg_wcslwr_s_l_small
 * area:  wstring
 * funcs:
 * procs: _wcslwr_s_l _wcsupr_s_l _create_locale _free_locale
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strlwr_s, _strlwr_s_l, _wcslwr_s_l, _wcsupr_s_l
 * note:  EINVAL if str is NULL or not NUL-terminated within sizeInWords (too small / size 0); handler invoked once.
 *        Run with locale NULL and with a C-locale object from _create_locale. Buffer state after failure not asserted.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, _wcslwr_s_l, (wchar_t *, size_t, void *));
TC_DECL_PROC(int, _wcsupr_s_l, (wchar_t *, size_t, void *));
TC_DECL_PROC(void *, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (void *));
static void *volatile nloc = NULL;
static void *cloc;

#define PRE() do { if (!tc_p__wcslwr_s_l || !tc_p__wcsupr_s_l) TC_SKIP("not exported"); tc_iph_count = 0; errno = 0; } while (0)
#define BAD() do { CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

static void lwr_small(void *loc) { wchar_t b[8] = { L'A', L'B', L'C', 0 }; int r; PRE(); r = tc_p__wcslwr_s_l(b, 3, loc); BAD(); }
static void upr_small(void *loc) { wchar_t b[8] = { L'a', L'b', L'c', 0 }; int r; PRE(); r = tc_p__wcsupr_s_l(b, 3, loc); BAD(); }
TC_CASE(lwr_too_small) { lwr_small(nloc); if (cloc) lwr_small(cloc); }
TC_CASE(upr_too_small) { upr_small(nloc); if (cloc) upr_small(cloc); }
TC_CASE(lwr_size_zero) { wchar_t b[4] = { L'A', 0 }; int r; PRE(); r = tc_p__wcslwr_s_l(b, 0, nloc); BAD();
    tc_iph_count = 0; errno = 0; if (cloc) { r = tc_p__wcslwr_s_l(b, 0, cloc); BAD(); } }
TC_CASE(upr_null_str) { int r; PRE(); r = tc_p__wcsupr_s_l(NULL, 10, nloc); BAD();
    tc_iph_count = 0; errno = 0; if (cloc) { r = tc_p__wcsupr_s_l(NULL, 10, cloc); BAD(); } }
TC_CASE(lwr_exact_fit_ok)
{
    int pass; PRE();
    for (pass = 0; pass < 2; pass++) {
        wchar_t b[8] = { L'A', L'B', L'C', 0, L'Z', L'Z', L'Z', 0 }; int r;
        void *loc = pass ? cloc : nloc;
        if (pass && !loc) break;
        r = tc_p__wcslwr_s_l(b, 4, loc);
        CHECK_EQ_I(r, 0); CHECK(b[0] == L'a' && b[1] == L'b' && b[2] == L'c' && b[3] == 0 && b[4] == L'Z');
    }
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(upr_exact_fit_ok)
{
    int pass; PRE();
    for (pass = 0; pass < 2; pass++) {
        wchar_t b[8] = { L'a', L'b', L'c', 0, L'z', L'z', L'z', 0 }; int r;
        void *loc = pass ? cloc : nloc;
        if (pass && !loc) break;
        r = tc_p__wcsupr_s_l(b, 4, loc);
        CHECK_EQ_I(r, 0); CHECK(b[0] == L'A' && b[1] == L'B' && b[2] == L'C' && b[3] == 0 && b[4] == L'z');
    }
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wcslwr_s_l); TC_LOAD_PROC(_wcsupr_s_l); TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    if (tc_p__create_locale) cloc = tc_p__create_locale(0 /* LC_ALL */, "C");
    TC_RUN(lwr_too_small); TC_RUN(upr_too_small); TC_RUN(lwr_size_zero); TC_RUN(upr_null_str);
    TC_RUN(lwr_exact_fit_ok); TC_RUN(upr_exact_fit_ok);
    if (cloc && tc_p__free_locale) tc_p__free_locale(cloc);
    return tc_end();
}
