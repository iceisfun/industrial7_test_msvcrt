/* TC
 * id:    cv_itoa_basic
 * area:  conv
 * funcs: _itoa
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _itoa, _ltoa, _ultoa, _i64toa
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static volatile int v;
static int vr = 10;

TC_CASE(zero) { char b[40]; v = 0; _itoa(v, b, vr); CHECK_STR(b, "0"); }
TC_CASE(pos) { char b[40]; v = 123; _itoa(v, b, vr); CHECK_STR(b, "123"); }
TC_CASE(neg) { char b[40]; v = -123; _itoa(v, b, vr); CHECK_STR(b, "-123"); }
TC_CASE(max) { char b[40]; v = INT_MAX; _itoa(v, b, vr); CHECK_STR(b, "2147483647"); }
TC_CASE(min) { char b[40]; v = INT_MIN; _itoa(v, b, vr); CHECK_STR(b, "-2147483648"); }
TC_CASE(retval) { char b[40]; char *r; v = 99; r = _itoa(v, b, vr); CHECK_EQ_P(r, b); CHECK_STR(r, "99"); }

int main(void) { tc_begin(); TC_RUN(zero); TC_RUN(pos); TC_RUN(neg); TC_RUN(max); TC_RUN(min); TC_RUN(retval); return tc_end(); }
