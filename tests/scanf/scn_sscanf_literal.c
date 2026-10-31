/* TC
 * id:    scn_sscanf_literal
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p5-p9 (ordinary characters, %%)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(prefix) { int a = 0, r; r = sscanf("x=5", "x=%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 5); }
TC_CASE(percent) { int a = 0, n = -1, r; r = sscanf("%7", "%%%d%n", &a, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 7); CHECK_EQ_I(n, 2); }
TC_CASE(pair) { int a = 0, b = 0, r; r = sscanf("(3,-4)", "(%d,%d)", &a, &b); CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 3); CHECK_EQ_I(b, -4); }
TC_CASE(case_sensitive) { int a = 99, r; r = sscanf("X=5", "x=%d", &a); CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 99); }
TC_CASE(trailing_literal) { int a = 0, r; r = sscanf("5;", "%d;", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 5); r = sscanf("5", "%d;", &a); CHECK_EQ_I(r, 1); }

int main(void) { tc_begin(); TC_RUN(prefix); TC_RUN(percent); TC_RUN(pair); TC_RUN(case_sensitive); TC_RUN(trailing_literal); return tc_end(); }
