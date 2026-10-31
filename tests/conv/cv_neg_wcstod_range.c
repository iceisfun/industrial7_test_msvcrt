/* TC
 * id:    cv_neg_wcstod_range
 * area:  conv
 * funcs: wcstod
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.3p8 (HUGE_VAL + ERANGE on overflow, 0 or denormal + ERANGE on underflow); MS docs strtod, wcstod
 * note:  _wcstod_l is not tested here (NULL/valid locale behaviour is covered elsewhere).
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

static const wchar_t *volatile v_s;
static volatile double v_zero = 0.0, v_one = 1.0;

TC_CASE(overflow_pos)
{
    wchar_t *e = (wchar_t *)1; double r; double inf = v_one / v_zero;
    static const wchar_t s[] = L"1e999";
    v_s = s; errno = 0; r = wcstod(v_s, &e);
    CHECK_BITS_D(r, inf);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_P(e, s + 5);
}
TC_CASE(overflow_neg)
{
    wchar_t *e = (wchar_t *)1; double r; double inf = v_one / v_zero;
    static const wchar_t s[] = L"-1e999";
    v_s = s; errno = 0; r = wcstod(v_s, &e);
    CHECK_BITS_D(r, -inf);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_P(e, s + 6);
}
TC_CASE(underflow)
{
    wchar_t *e = (wchar_t *)1; double r;
    static const wchar_t s[] = L"1e-999";
    v_s = s; errno = 0; r = wcstod(v_s, &e);
    CHECK_BITS_D(r, 0.0);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_P(e, s + 6);
}
TC_CASE(in_range_keeps_errno)
{
    wchar_t *e = (wchar_t *)1; double r;
    static const wchar_t s[] = L"1.5e3";
    v_s = s; errno = 0; r = wcstod(v_s, &e);
    CHECK_BITS_D(r, 1500.0);
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_P(e, s + 5);
}
TC_CASE(overflow_trailing)
{
    wchar_t *e = (wchar_t *)1; double r; double inf = v_one / v_zero;
    static const wchar_t s[] = L"1e999xyz";
    v_s = s; errno = 0; r = wcstod(v_s, &e);
    CHECK_BITS_D(r, inf);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_P(e, s + 5);
    CHECK(*e == L'x');
}

int main(void)
{
    tc_begin();
    TC_RUN(overflow_pos); TC_RUN(overflow_neg); TC_RUN(underflow); TC_RUN(in_range_keeps_errno); TC_RUN(overflow_trailing);
    return tc_end();
}
