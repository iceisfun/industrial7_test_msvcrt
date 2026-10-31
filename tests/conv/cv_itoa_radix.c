/* TC
 * id:    cv_itoa_radix
 * area:  conv
 * funcs: _itoa _ltoa _ultoa
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _itoa: radix 2..36, lowercase letters; sign only when radix is 10 and value negative
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static volatile int vi;
static volatile long vl;
static volatile unsigned long vu;
static volatile int rad;

TC_CASE(hex) { char b[40]; vi = 255; rad = 16; _itoa(vi, b, rad); CHECK_STR(b, "ff"); }
TC_CASE(bin) { char b[40]; vi = 255; rad = 2; _itoa(vi, b, rad); CHECK_STR(b, "11111111"); }
TC_CASE(oct) { char b[40]; vi = 255; rad = 8; _itoa(vi, b, rad); CHECK_STR(b, "377"); }
TC_CASE(b36) { char b[40]; vi = 35; rad = 36; _itoa(vi, b, rad); CHECK_STR(b, "z"); vi = 36; _itoa(vi, b, rad); CHECK_STR(b, "10"); }
TC_CASE(neg_hex) { char b[40]; vi = -1; rad = 16; _itoa(vi, b, rad); CHECK_STR(b, "ffffffff"); }
TC_CASE(ltoa_bin) { char b[40]; vl = -1; rad = 2; _ltoa(vl, b, rad); CHECK_STR(b, "11111111111111111111111111111111"); }
TC_CASE(ultoa_max) { char b[40]; vu = ULONG_MAX; rad = 10; _ultoa(vu, b, rad); CHECK_STR(b, "4294967295"); }
TC_CASE(ultoa_hex) { char b[40]; vu = 0xDEADBEEFUL; rad = 16; _ultoa(vu, b, rad); CHECK_STR(b, "deadbeef"); }

int main(void) { tc_begin(); TC_RUN(hex); TC_RUN(bin); TC_RUN(oct); TC_RUN(b36); TC_RUN(neg_hex); TC_RUN(ltoa_bin); TC_RUN(ultoa_max); TC_RUN(ultoa_hex); return tc_end(); }
