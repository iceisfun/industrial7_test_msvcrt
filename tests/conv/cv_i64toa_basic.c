/* TC
 * id:    cv_i64toa_basic
 * area:  conv
 * funcs: _i64toa _ui64toa
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _i64toa, _ui64toa
 */
#include "tc.h"
#include <stdlib.h>

static volatile __int64 vq;
static volatile unsigned __int64 vu;
static volatile int rad;

TC_CASE(max) { char b[80]; vq = 9223372036854775807LL; rad = 10; _i64toa(vq, b, rad); CHECK_STR(b, "9223372036854775807"); }
TC_CASE(min) { char b[80]; vq = -9223372036854775807LL - 1; rad = 10; _i64toa(vq, b, rad); CHECK_STR(b, "-9223372036854775808"); }
TC_CASE(umax10) { char b[80]; vu = 0xFFFFFFFFFFFFFFFFULL; rad = 10; _ui64toa(vu, b, rad); CHECK_STR(b, "18446744073709551615"); }
TC_CASE(umax16) { char b[80]; vu = 0xFFFFFFFFFFFFFFFFULL; rad = 16; _ui64toa(vu, b, rad); CHECK_STR(b, "ffffffffffffffff"); }
TC_CASE(umax2)
{
    char b[80]; vu = 0xFFFFFFFFFFFFFFFFULL; rad = 2; _ui64toa(vu, b, rad);
    CHECK_STR(b, "1111111111111111111111111111111111111111111111111111111111111111");
}
TC_CASE(zero) { char b[80]; vq = 0; rad = 10; _i64toa(vq, b, rad); CHECK_STR(b, "0"); vu = 0; _ui64toa(vu, b, rad); CHECK_STR(b, "0"); }
TC_CASE(b36) { char b[80]; vu = 35; rad = 36; _ui64toa(vu, b, rad); CHECK_STR(b, "z"); vu = 36ULL * 36 * 36; _ui64toa(vu, b, rad); CHECK_STR(b, "1000"); }

int main(void) { tc_begin(); TC_RUN(max); TC_RUN(min); TC_RUN(umax10); TC_RUN(umax16); TC_RUN(umax2); TC_RUN(zero); TC_RUN(b36); return tc_end(); }
