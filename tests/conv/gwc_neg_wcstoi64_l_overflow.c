/* TC
 * id:    gwc_neg_wcstoi64_l_overflow
 * area:  conv
 * funcs: _wcstoi64_l _wcstoui64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strtoi64, _wcstoi64 (_I64_MAX/_I64_MIN/_UI64_MAX and ERANGE)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <limits.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static __int64 S(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstoi64_l(vp, e, b, L); }
static unsigned __int64 U(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstoui64_l(vp, e, b, L); }

TC_CASE(pos_over) { wchar_t *e; errno = 0; CHECK_EQ_I(S(L"9223372036854775808", &e, 10), _I64_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg_under) { wchar_t *e; errno = 0; CHECK_EQ_I(S(L"-9223372036854775809", &e, 10), _I64_MIN); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(u_over) { wchar_t *e; errno = 0; CHECK_EQ_U(U(L"18446744073709551616", &e, 10), _UI64_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(hex_over) { wchar_t *e; errno = 0; CHECK_EQ_I(S(L"0x1FFFFFFFFFFFFFFFF", &e, 16), _I64_MAX); CHECK_EQ_I(errno, ERANGE); errno = 0; CHECK_EQ_U(U(L"0x1FFFFFFFFFFFFFFFF", &e, 16), _UI64_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(endptr_after_run)
{
    const wchar_t *s = L"99999999999999999999999z"; wchar_t *e; errno = 0;
    CHECK_EQ_I(S(s, &e, 10), _I64_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 23);
}
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(pos_over); TC_RUN(neg_under); TC_RUN(u_over); TC_RUN(hex_over); TC_RUN(endptr_after_run);
    _free_locale(L);
    return tc_end();
}
