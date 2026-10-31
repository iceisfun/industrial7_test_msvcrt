/* TC
 * id:    gwc_neg_wcstol_l_overflow_errno
 * area:  conv
 * funcs: _wcstol_l _wcstoul_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstol, wcstoul (LONG_MAX/LONG_MIN/ULONG_MAX and ERANGE); C11 7.22.1.4
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <limits.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static long  S(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstol_l(vp, e, b, L); }
static unsigned long U(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstoul_l(vp, e, b, L); }

TC_CASE(pos_over) { wchar_t *e; errno = 0; CHECK_EQ_I(S(L"2147483648", &e, 10), LONG_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg_under) { wchar_t *e; errno = 0; CHECK_EQ_I(S(L"-2147483649", &e, 10), LONG_MIN); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(huge) { const wchar_t *s = L"99999999999999999999x"; wchar_t *e; errno = 0; CHECK_EQ_I(S(s, &e, 10), LONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 20); }
TC_CASE(ulong_over) { wchar_t *e; errno = 0; CHECK_EQ_U(U(L"4294967296", &e, 10), ULONG_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(in_range_no_errno) { wchar_t *e; errno = 0; CHECK_EQ_I(S(L"2147483647", &e, 10), LONG_MAX); CHECK_EQ_I(errno, 0); CHECK_EQ_U(U(L"4294967295", &e, 10), ULONG_MAX); CHECK_EQ_I(errno, 0); }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(pos_over); TC_RUN(neg_under); TC_RUN(huge); TC_RUN(ulong_over); TC_RUN(in_range_no_errno);
    _free_locale(L);
    return tc_end();
}
