/* TC
 * id:    sec_itoa_s_radix
 * area:  secure_crt
 * funcs: _itoa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _itoa_s, _itow_s
 * note:  only radix 10 yields a leading '-'; other radixes treat value as unsigned
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
static char b[40];
static volatile int v;
#define T(val, rad, want) do { v = (val); CHECK_EQ_I(_itoa_s(v, b, sizeof b, rad), 0); CHECK_STR(b, want); } while (0)

TC_CASE(radix10_pos_neg_zero) { T(123, 10, "123"); T(-456, 10, "-456"); T(0, 10, "0"); }
TC_CASE(radix2) { T(5, 2, "101"); T(0, 2, "0"); }
TC_CASE(radix16_lowercase) { T(255, 16, "ff"); T(0xabc, 16, "abc"); }
TC_CASE(radix8) { T(64, 8, "100"); T(8, 8, "10"); }
TC_CASE(radix36) { T(35, 36, "z"); T(36, 36, "10"); T(1295, 36, "zz"); }
TC_CASE(negative_radix16_is_unsigned_ffffffff) { T(-1, 16, "ffffffff"); }
TC_CASE(int_min_radix10) { T(-2147483647 - 1, 10, "-2147483648"); }
TC_CASE(exact_fit)
{
    char e[4]; v = 123;
    CHECK_EQ_I(_itoa_s(v, e, 4, 10), 0); CHECK_STR(e, "123");
    v = -12;
    CHECK_EQ_I(_itoa_s(v, e, 4, 10), 0); CHECK_STR(e, "-12");
}
int main(void) { tc_begin(); TC_RUN(radix10_pos_neg_zero); TC_RUN(radix2); TC_RUN(radix16_lowercase); TC_RUN(radix8); TC_RUN(radix36);
    TC_RUN(negative_radix16_is_unsigned_ffffffff); TC_RUN(int_min_radix10); TC_RUN(exact_fit); return tc_end(); }
