/* TC
 * id:    gwc_neg_wcstol_l_invalid_base
 * area:  conv
 * funcs: _wcstol_l _wcstoul_l _wcstoi64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtol, wcstol, _strtol_l, _wcstol_l (base out of range or NULL string: EINVAL + invalid parameter handler, return 0); Parameter validation
 * note:  endptr is not asserted. msvcr90 assumed to validate like later CRTs (docs list the behaviour since VS2005).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;

#define BAD(s, base) do { wchar_t *e; \
    tc_iph_count = 0; errno = 0; vp = (s); CHECK_EQ_I(_wcstol_l(vp, &e, base, L), 0);   CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); \
    tc_iph_count = 0; errno = 0; vp = (s); CHECK_EQ_U(_wcstoul_l(vp, &e, base, L), 0);  CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); \
    tc_iph_count = 0; errno = 0; vp = (s); CHECK_EQ_I(_wcstoi64_l(vp, &e, base, L), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(base1)   { BAD(L"10", 1); }
TC_CASE(base37)  { BAD(L"10", 37); }
TC_CASE(base_m1) { BAD(L"10", -1); }
TC_CASE(null_string) { BAD(NULL, 10); }
TC_CASE(null_string_endptr_null)
{
    tc_iph_count = 0; errno = 0; vp = NULL; CHECK_EQ_I(_wcstol_l(vp, NULL, 10, L), 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_bounds_no_handler)
{
    wchar_t *e; tc_iph_count = 0;
    vp = L"z"; CHECK_EQ_I(_wcstol_l(vp, &e, 36, L), 35);
    vp = L"11"; CHECK_EQ_I(_wcstol_l(vp, &e, 2, L), 3);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid parameter handler API\n"); return tc_end(); }
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(base1); TC_RUN(base37); TC_RUN(base_m1); TC_RUN(null_string); TC_RUN(null_string_endptr_null); TC_RUN(valid_bounds_no_handler);
    _free_locale(L);
    return tc_end();
}
