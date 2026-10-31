/* TC
 * id:    cv_neg_wcstoull_overflow
 * area:  conv
 * funcs: wcstoull _wcstoui64 wcstoul
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4p8 (ULLONG_MAX + ERANGE on overflow; negation of in-range value wraps); MS docs strtoul, wcstoul, strtoull
 * note:  unsigned long is 32-bit on Windows.
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static const wchar_t *volatile v_s;

TC_CASE(ull_dec_overflow)
{
    wchar_t *e = (wchar_t *)1; unsigned long long r; static const wchar_t s[] = L"18446744073709551616";
    v_s = s; errno = 0; r = wcstoull(v_s, &e, 10);
    CHECK_EQ_U(r, ULLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 20);
}
TC_CASE(ull_max_exact)
{
    wchar_t *e = (wchar_t *)1; unsigned long long r; static const wchar_t s[] = L"18446744073709551615";
    v_s = s; errno = 0; r = wcstoull(v_s, &e, 10);
    CHECK_EQ_U(r, ULLONG_MAX); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, s + 20);
}
TC_CASE(ui64_hex_overflow)
{
    wchar_t *e = (wchar_t *)1; unsigned long long r; static const wchar_t s[] = L"0x1ffffffffffffffff";
    v_s = s; errno = 0; r = _wcstoui64(v_s, &e, 0);
    CHECK_EQ_U(r, ULLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 19);
    v_s = s; errno = 0; r = wcstoull(v_s, &e, 0);
    CHECK_EQ_U(r, ULLONG_MAX); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(ul_32bit_clamp)
{
    wchar_t *e = (wchar_t *)1; unsigned long r; static const wchar_t s[] = L"4294967296";
    v_s = s; errno = 0; r = wcstoul(v_s, &e, 10);
    CHECK_EQ_U(r, 0xFFFFFFFFu); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 10);
}
TC_CASE(ul_minus_one_wraps)
{
    wchar_t *e = (wchar_t *)1; unsigned long r; static const wchar_t s[] = L"-1";
    v_s = s; errno = 0; r = wcstoul(v_s, &e, 10);
    CHECK_EQ_U(r, 0xFFFFFFFFu); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, s + 2);
}
TC_CASE(ull_negative_overflow)
{
    wchar_t *e = (wchar_t *)1; unsigned long long r; static const wchar_t s[] = L"-18446744073709551616";
    v_s = s; errno = 0; r = wcstoull(v_s, &e, 10);
    CHECK_EQ_U(r, ULLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s + 21);
}

int main(void)
{
    tc_begin();
    TC_RUN(ull_dec_overflow); TC_RUN(ull_max_exact); TC_RUN(ui64_hex_overflow); TC_RUN(ul_32bit_clamp);
    TC_RUN(ul_minus_one_wraps); TC_RUN(ull_negative_overflow);
    return tc_end();
}
