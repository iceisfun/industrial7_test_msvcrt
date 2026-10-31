/* TC
 * id:    cv_neg_wcstol_overflow
 * area:  conv
 * funcs: wcstol wcstoul _wcstoi64 _wcstoui64
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.4.1.2p8 (LONG_MAX/LONG_MIN/ULONG_MAX + ERANGE); MS docs wcstol, _wcstoi64
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static const wchar_t *volatile vs;

TC_CASE(wcstol_max) { wchar_t *e; long v; vs = L"2147483648"; errno = 0; v = wcstol(vs, &e, 10); CHECK_EQ_I(v, LONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 10); }
TC_CASE(wcstol_min) { long v; vs = L"-2147483649"; errno = 0; v = wcstol(vs, NULL, 10); CHECK_EQ_I(v, LONG_MIN); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(wcstol_boundaries_ok)
{
    vs = L"2147483647"; errno = 0; CHECK_EQ_I(wcstol(vs, NULL, 10), LONG_MAX); CHECK_EQ_I(errno, 0);
    vs = L"-2147483648"; errno = 0; CHECK_EQ_I(wcstol(vs, NULL, 10), LONG_MIN); CHECK_EQ_I(errno, 0);
}
TC_CASE(wcstoul_max) { unsigned long v; vs = L"4294967296"; errno = 0; v = wcstoul(vs, NULL, 10); CHECK_EQ_U(v, ULONG_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(wcstoul_ok) { vs = L"4294967295"; errno = 0; CHECK_EQ_U(wcstoul(vs, NULL, 10), ULONG_MAX); CHECK_EQ_I(errno, 0); }
TC_CASE(i64_max) { __int64 v; vs = L"9223372036854775808"; errno = 0; v = _wcstoi64(vs, NULL, 10); CHECK_EQ_I(v, _I64_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(i64_min) { __int64 v; vs = L"-9223372036854775809"; errno = 0; v = _wcstoi64(vs, NULL, 10); CHECK_EQ_I(v, _I64_MIN); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(ui64_max) { unsigned __int64 v; vs = L"18446744073709551616"; errno = 0; v = _wcstoui64(vs, NULL, 10); CHECK_EQ_U(v, _UI64_MAX); CHECK_EQ_I(errno, ERANGE); }

int main(void)
{
    tc_begin();
    TC_RUN(wcstol_max); TC_RUN(wcstol_min); TC_RUN(wcstol_boundaries_ok); TC_RUN(wcstoul_max); TC_RUN(wcstoul_ok);
    TC_RUN(i64_max); TC_RUN(i64_min); TC_RUN(ui64_max);
    return tc_end();
}
