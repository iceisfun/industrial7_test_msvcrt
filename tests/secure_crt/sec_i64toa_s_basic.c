/* TC
 * id:    sec_i64toa_s_basic
 * area:  secure_crt
 * funcs: _i64toa_s _ui64toa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _i64toa_s, _ui64toa_s

 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
static char b[80];
TC_CASE(i64_min) { volatile __int64 v = -9223372036854775807LL - 1; CHECK_EQ_I(_i64toa_s(v, b, sizeof b, 10), 0); CHECK_STR(b, "-9223372036854775808"); }
TC_CASE(i64_max) { volatile __int64 v = 9223372036854775807LL; CHECK_EQ_I(_i64toa_s(v, b, sizeof b, 10), 0); CHECK_STR(b, "9223372036854775807"); }
TC_CASE(ui64_max_radix10) { volatile unsigned __int64 v = 0xffffffffffffffffULL; CHECK_EQ_I(_ui64toa_s(v, b, sizeof b, 10), 0); CHECK_STR(b, "18446744073709551615"); }
TC_CASE(ui64_max_radix16) { volatile unsigned __int64 v = 0xffffffffffffffffULL; CHECK_EQ_I(_ui64toa_s(v, b, sizeof b, 16), 0); CHECK_STR(b, "ffffffffffffffff"); }
TC_CASE(negative_radix16_unsigned_16_digits) { volatile __int64 v = -1; CHECK_EQ_I(_i64toa_s(v, b, sizeof b, 16), 0); CHECK_STR(b, "ffffffffffffffff"); }
TC_CASE(zero)
{
    volatile __int64 v = 0; volatile unsigned __int64 u = 0;
    CHECK_EQ_I(_i64toa_s(v, b, sizeof b, 10), 0); CHECK_STR(b, "0");
    CHECK_EQ_I(_ui64toa_s(u, b, sizeof b, 2), 0); CHECK_STR(b, "0");
}
int main(void) { tc_begin(); TC_RUN(i64_min); TC_RUN(i64_max); TC_RUN(ui64_max_radix10); TC_RUN(ui64_max_radix16);
    TC_RUN(negative_radix16_unsigned_16_digits); TC_RUN(zero); return tc_end(); }
