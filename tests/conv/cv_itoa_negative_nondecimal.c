/* TC
 * id:    cv_itoa_negative_nondecimal
 * area:  conv
 * funcs: _itoa _ltoa _i64toa
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _itoa: if radix equals 10 and value is negative the first character is '-'; otherwise the value is treated as unsigned
 */
#include "tc.h"
#include <stdlib.h>

static volatile int vi;
static volatile long vl;
static volatile __int64 vq;
static volatile int rad;

TC_CASE(itoa_hex) { char b[40]; vi = -255; rad = 16; _itoa(vi, b, rad); CHECK_STR(b, "ffffff01"); }
TC_CASE(ltoa_oct) { char b[40]; vl = -1; rad = 8; _ltoa(vl, b, rad); CHECK_STR(b, "37777777777"); }
TC_CASE(i64_hex) { char b[40]; vq = -1; rad = 16; _i64toa(vq, b, rad); CHECK_STR(b, "ffffffffffffffff"); }
TC_CASE(i64_dec) { char b[40]; vq = -1; rad = 10; _i64toa(vq, b, rad); CHECK_STR(b, "-1"); }

int main(void) { tc_begin(); TC_RUN(itoa_hex); TC_RUN(ltoa_oct); TC_RUN(i64_hex); TC_RUN(i64_dec); return tc_end(); }
