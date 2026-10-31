/* TC
 * id:    gst_wcstold_l_neg_range_null
 * area:  conv
 * funcs: _create_locale _free_locale
 * procs: _wcstold_l
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstold (HUGE_VAL / 0 with ERANGE), Parameter validation (NULL: EINVAL, handler; default handler terminates)
 * note:  NULL-nptr cases run on ucrtbase only (msvcr120 probed in gst_strtof_family_neg_null_probe).
 *        long double == double on MSVC; called through a double-typed proc.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <errno.h>

TC_DECL_PROC(double, _wcstold_l, (const wchar_t *, wchar_t **, _locale_t));
static _locale_t loc;
static const wchar_t *volatile v;

TC_CASE(overflow)
{
    errno = 0; CHECK_EQ_U(tc_dbits(tc_p__wcstold_l(v = L"1e309", NULL, loc)), 0x7ff0000000000000ull); CHECK_EQ_I(errno, ERANGE);
    errno = 0; CHECK_EQ_U(tc_dbits(tc_p__wcstold_l(v = L"-1e309", NULL, loc)), 0xfff0000000000000ull); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(underflow) { errno = 0; CHECK_EQ_U(tc_dbits(tc_p__wcstold_l(v = L"1e-400", NULL, loc)), 0u); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(no_conversion) { wchar_t *e = (wchar_t *)1; v = L"xyz"; CHECK_EQ_U(tc_dbits(tc_p__wcstold_l(v, &e, loc)), 0u); CHECK_EQ_P(e, v); }
TC_CASE(null_nptr)
{
    double d;
    if (!TC_AT_LEAST(140)) TC_SKIP("msvcr120 NULL handling is in gst_strtof_family_neg_null_probe");
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler");
    tc_iph_count = 0; errno = 0; v = NULL; d = tc_p__wcstold_l(v, NULL, loc);
    CHECK_EQ_U(tc_dbits(d), 0u); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(sanity) { DWORD code = 0; CHECK(tc_spawn_self("sanity", &code, 20000)); CHECK_EQ_U(code, 77); }
TC_CASE(fatal_default) { DWORD code = 0; int ran; if (!TC_AT_LEAST(140)) TC_SKIP("ucrtbase only");
    ran = tc_spawn_self("null", &code, 20000); CHECK(ran); CHECK(code != 0); CHECK(code != 77); }
int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_wcstold_l);
    if (!tc_p__wcstold_l) { tc_write("TC FAIL " TC_TEST_ID "::__wcstold_l not exported\n"); return 1; }
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 's') return 77;
        (void)tc_p__wcstold_l(NULL, NULL, NULL);
        return 77;
    }
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(overflow); TC_RUN(underflow); TC_RUN(no_conversion); TC_RUN(null_nptr); TC_RUN(sanity); TC_RUN(fatal_default);
    _free_locale(loc);
    return tc_end();
}
