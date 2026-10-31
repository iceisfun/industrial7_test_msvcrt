/* TC
 * id:    cv_ltoa_ultoa_basic
 * area:  conv
 * funcs: _ltoa _ultoa
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _itoa, _ltoa, _ultoa (long is 32-bit on Windows)
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static volatile long vl;
static volatile unsigned long vu;
static volatile int rad = 10;

TC_CASE(min) { char b[40]; vl = LONG_MIN; _ltoa(vl, b, rad); CHECK_STR(b, "-2147483648"); }
TC_CASE(max) { char b[40]; vl = LONG_MAX; _ltoa(vl, b, rad); CHECK_STR(b, "2147483647"); }
TC_CASE(umax) { char b[40]; vu = ULONG_MAX; _ultoa(vu, b, rad); CHECK_STR(b, "4294967295"); }
TC_CASE(zero) { char b[40]; vl = 0; _ltoa(vl, b, rad); CHECK_STR(b, "0"); vu = 0; _ultoa(vu, b, rad); CHECK_STR(b, "0"); }
TC_CASE(million) { char b[40]; vl = 1000000; _ltoa(vl, b, rad); CHECK_STR(b, "1000000"); vu = 1000000; _ultoa(vu, b, rad); CHECK_STR(b, "1000000"); }

int main(void) { tc_begin(); TC_RUN(min); TC_RUN(max); TC_RUN(umax); TC_RUN(zero); TC_RUN(million); return tc_end(); }
