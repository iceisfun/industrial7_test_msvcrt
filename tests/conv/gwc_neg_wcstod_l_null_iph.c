/* TC
 * id:    gwc_neg_wcstod_l_null_iph
 * area:  conv
 * funcs: _wcstod_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs Parameter validation; strtod/wcstod pages do not state the NULL-string behaviour explicitly
 * note:  Assumes the standard secure-CRT validation (EINVAL + handler + 0.0) applies to _wcstod_l NULL string; reviewer could not cite a docs sentence.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;

TC_CASE(null_with_endptr)
{
    wchar_t *e = NULL; double r; tc_iph_count = 0; errno = 0; vp = NULL;
    r = _wcstod_l(vp, &e, L);
    CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_without_endptr)
{
    double r; tc_iph_count = 0; errno = 0; vp = NULL;
    r = _wcstod_l(vp, NULL, L);
    CHECK_BITS_D(r, 0.0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid parameter handler API\n"); return tc_end(); }
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(null_with_endptr); TC_RUN(null_without_endptr);
    _free_locale(L);
    return tc_end();
}
