/* TC
 * id:    scn_neg_sscanf_int_nomatch
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.2p12-16; MS docs sscanf return value
 * note:  Integer conversions: matching failures and %n/suppression do not count; EOF only for input failure before first conversion.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(letters) { int v = 55; int r = sscanf("xyz", "%d", &v); CHECK_EQ_I(r, 0); CHECK_EQ_I(v, 55); }
TC_CASE(hex_digit_in_dec) { int v = 55; char c = 0; int r = sscanf("a", "%d%c", &v, &c); CHECK_EQ_I(r, 0); CHECK_EQ_I(v, 55); CHECK_EQ_I(c, 0); }
TC_CASE(empty) { int v = 55; int r = sscanf("", "%d", &v); CHECK_EQ_I(r, EOF); CHECK_EQ_I(v, 55); }
TC_CASE(sign_only) { int v = 55; int r = sscanf("+", "%d", &v); CHECK(r == 0 || r == EOF); CHECK_EQ_I(v, 55); }
TC_CASE(octal_bad_digit) { unsigned v = 55; int r = sscanf("9", "%o", &v); CHECK_EQ_I(r, 0); CHECK_EQ_U(v, 55); }
TC_CASE(hex_bad) { unsigned v = 55; int r = sscanf("g", "%x", &v); CHECK_EQ_I(r, 0); CHECK_EQ_U(v, 55); }
TC_CASE(literal_mismatch) { int a = 1, b = 66; int r = sscanf("5,6", "%d;%d", &a, &b); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 5); CHECK_EQ_I(b, 66); }
TC_CASE(suppressed_not_counted) { int v = 55; int r = sscanf("1 2", "%*d %d", &v); CHECK_EQ_I(r, 1); CHECK_EQ_I(v, 2); }

int main(void) { tc_begin(); TC_RUN(letters); TC_RUN(hex_digit_in_dec); TC_RUN(empty); TC_RUN(sign_only); TC_RUN(octal_bad_digit); TC_RUN(hex_bad); TC_RUN(literal_mismatch); TC_RUN(suppressed_not_counted); return tc_end(); }
