/* TC
 * id:    stl_div_basic
 * area:  stdlib
 * funcs: div ldiv
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.6.2; MS docs div, ldiv
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static volatile int a, b; static volatile long la, lb;

TC_CASE(signs)
{
    div_t d; ldiv_t l;
    a = 7; b = 2; d = div(a, b); CHECK_EQ_I(d.quot, 3); CHECK_EQ_I(d.rem, 1);
    a = -7; d = div(a, b); CHECK_EQ_I(d.quot, -3); CHECK_EQ_I(d.rem, -1);
    a = 7; b = -2; d = div(a, b); CHECK_EQ_I(d.quot, -3); CHECK_EQ_I(d.rem, 1);
    a = -7; b = -2; d = div(a, b); CHECK_EQ_I(d.quot, 3); CHECK_EQ_I(d.rem, -1);
    la = 7; lb = 2; l = ldiv(la, lb); CHECK_EQ_I(l.quot, 3); CHECK_EQ_I(l.rem, 1);
    la = -7; l = ldiv(la, lb); CHECK_EQ_I(l.quot, -3); CHECK_EQ_I(l.rem, -1);
    la = 7; lb = -2; l = ldiv(la, lb); CHECK_EQ_I(l.quot, -3); CHECK_EQ_I(l.rem, 1);
    la = -7; lb = -2; l = ldiv(la, lb); CHECK_EQ_I(l.quot, 3); CHECK_EQ_I(l.rem, -1);
}
TC_CASE(zero_exact)
{
    div_t d; ldiv_t l;
    a = 0; b = 5; d = div(a, b); CHECK_EQ_I(d.quot, 0); CHECK_EQ_I(d.rem, 0);
    a = 100; b = 10; d = div(a, b); CHECK_EQ_I(d.quot, 10); CHECK_EQ_I(d.rem, 0);
    la = 0; lb = 5; l = ldiv(la, lb); CHECK_EQ_I(l.quot, 0); CHECK_EQ_I(l.rem, 0);
    la = -100; lb = 10; l = ldiv(la, lb); CHECK_EQ_I(l.quot, -10); CHECK_EQ_I(l.rem, 0);
}
TC_CASE(sweep)
{
    int n, m, bad = 0;
    for (n = -50; n <= 50; n++) for (m = -9; m <= 9; m++) {
        div_t d; ldiv_t l;
        if (m == 0) continue;
        a = n; b = m; la = n; lb = m;
        d = div(a, b); l = ldiv(la, lb);
        if (d.quot * m + d.rem != n || l.quot * m + l.rem != n) bad++;
        if (d.quot != n / m || d.rem != n % m || l.quot != n / m || l.rem != n % m) bad++;
    }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(int_max)
{
    div_t d; ldiv_t l;
    a = INT_MAX; b = 1; d = div(a, b); CHECK_EQ_I(d.quot, INT_MAX); CHECK_EQ_I(d.rem, 0);
    b = INT_MAX; d = div(a, b); CHECK_EQ_I(d.quot, 1); CHECK_EQ_I(d.rem, 0);
    a = INT_MIN; b = INT_MAX; d = div(a, b); CHECK_EQ_I(d.quot, -1); CHECK_EQ_I(d.rem, -1);
    la = LONG_MAX; lb = 1; l = ldiv(la, lb); CHECK_EQ_I(l.quot, LONG_MAX); CHECK_EQ_I(l.rem, 0);
    lb = LONG_MAX; l = ldiv(la, lb); CHECK_EQ_I(l.quot, 1); CHECK_EQ_I(l.rem, 0);
}

int main(void) { tc_begin(); TC_RUN(signs); TC_RUN(zero_exact); TC_RUN(sweep); TC_RUN(int_max); return tc_end(); }
