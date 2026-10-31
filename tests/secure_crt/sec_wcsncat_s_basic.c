/* TC
 * id:    sec_wcsncat_s_basic
 * area:  secure_crt
 * funcs: wcsncat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strncat_s, wcsncat_s, _mbsncat_s

 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#define TRUNC ((size_t)-1)

TC_CASE(count_less)
{
    wchar_t d[16] = L"ab"; volatile size_t n = 2;
    CHECK_EQ_I(wcsncat_s(d, 16, L"cdef", n), 0);
    CHECK_WSTR(d, L"abcd");
}
TC_CASE(count_greater)
{
    wchar_t d[16] = L"ab"; volatile size_t n = 10;
    CHECK_EQ_I(wcsncat_s(d, 16, L"cd", n), 0);
    CHECK_WSTR(d, L"abcd");
}
TC_CASE(count_zero)
{
    wchar_t d[16] = L"ab"; volatile size_t n = 0;
    CHECK_EQ_I(wcsncat_s(d, 16, L"cd", n), 0);
    CHECK_WSTR(d, L"ab");
}
TC_CASE(truncate_exceeds_returns_80)
{
    wchar_t d[6] = L"ab"; volatile size_t n = TRUNC;
    CHECK_EQ_I(wcsncat_s(d, 6, L"cdefgh", n), 80);
    CHECK_WSTR(d, L"abcde");
}
int main(void) { tc_begin(); TC_RUN(count_less); TC_RUN(count_greater); TC_RUN(count_zero); TC_RUN(truncate_exceeds_returns_80); return tc_end(); }
