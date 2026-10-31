/* TC
 * id:    cv_itow_s_basic_probe
 * area:  conv
 * funcs: 
 * procs: _itow_s _ltow_s _i64tow_s _ui64tow_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _itow_s family
 * note:  Probes that the wide secure variants exist and behave like _itoa_s (resolved at run time; mingw import libs lack some).
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, _itow_s, (int, wchar_t *, size_t, int));
TC_DECL_PROC(int, _ltow_s, (long, wchar_t *, size_t, int));
TC_DECL_PROC(int, _i64tow_s, (__int64, wchar_t *, size_t, int));
TC_DECL_PROC(int, _ui64tow_s, (unsigned __int64, wchar_t *, size_t, int));

TC_CASE(success) { wchar_t b[32]; if (!tc_p__itow_s) TC_SKIP("missing"); CHECK_EQ_I(tc_p__itow_s(123, b, 32, 10), 0); CHECK_WSTR(b, L"123"); }
TC_CASE(exact_fit) { wchar_t b[4]; if (!tc_p__itow_s) TC_SKIP("missing"); CHECK_EQ_I(tc_p__itow_s(123, b, 4, 10), 0); CHECK_WSTR(b, L"123"); }
TC_CASE(neg_hex) { wchar_t b[32]; if (!tc_p__itow_s) TC_SKIP("missing"); CHECK_EQ_I(tc_p__itow_s(-255, b, 32, 16), 0); CHECK_WSTR(b, L"ffffff01"); }
TC_CASE(ltow_s) { wchar_t b[32]; if (!tc_p__ltow_s) TC_SKIP("missing"); CHECK_EQ_I(tc_p__ltow_s(-99L, b, 32, 10), 0); CHECK_WSTR(b, L"-99"); }
TC_CASE(i64_min) { wchar_t b[32]; if (!tc_p__i64tow_s) TC_SKIP("missing"); CHECK_EQ_I(tc_p__i64tow_s(-9223372036854775807LL - 1, b, 32, 10), 0); CHECK_WSTR(b, L"-9223372036854775808"); }
TC_CASE(ui64_max) { wchar_t b[32]; if (!tc_p__ui64tow_s) TC_SKIP("missing"); CHECK_EQ_I(tc_p__ui64tow_s(0xFFFFFFFFFFFFFFFFULL, b, 32, 10), 0); CHECK_WSTR(b, L"18446744073709551615"); }

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_itow_s); TC_LOAD_PROC(_ltow_s); TC_LOAD_PROC(_i64tow_s); TC_LOAD_PROC(_ui64tow_s);
    TC_RUN(success); TC_RUN(exact_fit); TC_RUN(neg_hex); TC_RUN(ltow_s); TC_RUN(i64_min); TC_RUN(ui64_max);
    return tc_end();
}
