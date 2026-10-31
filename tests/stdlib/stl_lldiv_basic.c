/* TC
 * id:    stl_lldiv_basic
 * area:  stdlib
 * funcs: lldiv
 * crt:   >=120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.6.2; MS docs div, ldiv, lldiv
 */
#include "tc.h"
#include <stdlib.h>

static volatile long long n, d;

TC_CASE(large)
{
    lldiv_t r;
    n = 1000000000000000LL; d = 7; r = lldiv(n, d);
    CHECK_EQ_I(r.quot, 142857142857142LL); CHECK_EQ_I(r.rem, 6);
}
TC_CASE(signs)
{
    lldiv_t r;
    n = -7; d = 2; r = lldiv(n, d); CHECK_EQ_I(r.quot, -3); CHECK_EQ_I(r.rem, -1);
    n = 7; d = -2; r = lldiv(n, d); CHECK_EQ_I(r.quot, -3); CHECK_EQ_I(r.rem, 1);
    n = -7; d = -2; r = lldiv(n, d); CHECK_EQ_I(r.quot, 3); CHECK_EQ_I(r.rem, -1);
    n = -5000000000LL; d = 3; r = lldiv(n, d); CHECK_EQ_I(r.quot, -1666666666LL); CHECK_EQ_I(r.rem, -2);
}
TC_CASE(zero_num) { lldiv_t r; n = 0; d = 123456789012LL; r = lldiv(n, d); CHECK_EQ_I(r.quot, 0); CHECK_EQ_I(r.rem, 0); }
TC_CASE(max)
{
    lldiv_t r;
    n = 9223372036854775807LL; d = n; r = lldiv(n, d); CHECK_EQ_I(r.quot, 1); CHECK_EQ_I(r.rem, 0);
    d = 1; r = lldiv(n, d); CHECK_EQ_I(r.quot, 9223372036854775807LL); CHECK_EQ_I(r.rem, 0);
}

int main(void) { tc_begin(); TC_RUN(large); TC_RUN(signs); TC_RUN(zero_num); TC_RUN(max); return tc_end(); }
