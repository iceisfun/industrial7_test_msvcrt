/* TC
 * id:    scn_neg_sscanf_sign_space
 * area:  scanf
 * kind:  negative
 * funcs: sscanf swscanf
 * note:  Floating-point forms live in scn_neg_sscanf_sign_space_float_probe (return value 0 vs EOF is less certain there).
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2 p9-10 (a numeric conversion matches an optionally signed sequence with no intervening white space; mismatch = matching failure, return count so far)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char *volatile vs; static const wchar_t *volatile vw;
#define NOMATCH(fmt, input, T, init, conv) do { T v = init; int k = -7, n; vs = input; \
    n = sscanf((const char *)vs, fmt "%n", &v, &k); CHECK_EQ_I(n, 0); CHECK(v == (init)); CHECK_EQ_I(k, -7); (void)conv; } while (0)
TC_CASE(d_minus_space) { NOMATCH("%d", "- 5", int, 99, 0); }
TC_CASE(d_plus_space)  { NOMATCH("%d", "+ 5", int, 99, 0); }
TC_CASE(i_minus_space) { NOMATCH("%i", "- 5", int, 99, 0); }
TC_CASE(x_minus_space) { NOMATCH("%x", "- 5", unsigned, 99u, 0); }
TC_CASE(d_double_sign)
{
    NOMATCH("%d", "+-5", int, 99, 0);
    NOMATCH("%d", "--5", int, 99, 0);
    NOMATCH("%u", "+-5", unsigned, 99u, 0);
}
TC_CASE(wide_d_minus_space)
{
    int v = 99, k = -7, n; vw = L"- 5";
    n = swscanf((const wchar_t *)vw, L"%d%n", &v, &k); CHECK_EQ_I(n, 0); CHECK_EQ_I(v, 99); CHECK_EQ_I(k, -7);
    vw = L"+-5"; n = swscanf((const wchar_t *)vw, L"%d%n", &v, &k); CHECK_EQ_I(n, 0); CHECK_EQ_I(v, 99); CHECK_EQ_I(k, -7);
}
int main(void)
{
    tc_begin(); TC_RUN(d_minus_space); TC_RUN(d_plus_space); TC_RUN(i_minus_space); TC_RUN(x_minus_space);
    TC_RUN(d_double_sign); TC_RUN(wide_d_minus_space);
    return tc_end();
}
