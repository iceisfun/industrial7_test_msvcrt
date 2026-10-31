/* TC
 * id:    gwc_neg_wcstoul_l_negative_wrap
 * area:  conv
 * funcs: _wcstoul_l _wcstoui64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4 (a leading '-' negates the value in unsigned arithmetic; no error)
 * note:  must-not-fail cases: unsigned conversion of negative text wraps and leaves errno untouched
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <limits.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static unsigned long U(const wchar_t *s) { wchar_t *e; vp = s; return _wcstoul_l(vp, &e, 10, L); }
static unsigned __int64 U64(const wchar_t *s) { wchar_t *e; vp = s; return _wcstoui64_l(vp, &e, 10, L); }

TC_CASE(minus_one) { errno = 0; CHECK_EQ_U(U(L"-1"), ULONG_MAX); CHECK_EQ_I(errno, 0); }
TC_CASE(minus_zero) { errno = 0; CHECK_EQ_U(U(L"-0"), 0); CHECK_EQ_I(errno, 0); }
TC_CASE(minus_one_64) { errno = 0; CHECK_EQ_U(U64(L"-1"), _UI64_MAX); CHECK_EQ_I(errno, 0); }
TC_CASE(minus_two_64) { errno = 0; CHECK_EQ_U(U64(L"-2"), _UI64_MAX - 1); CHECK_EQ_I(errno, 0); }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(minus_one); TC_RUN(minus_zero); TC_RUN(minus_one_64); TC_RUN(minus_two_64);
    _free_locale(L);
    return tc_end();
}
