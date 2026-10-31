/* TC
 * id:    sec_ultoa_s_basic
 * area:  secure_crt
 * funcs: _ultoa_s _ltoa_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _itoa_s, _ltoa_s, _ultoa_s (long is 32-bit on Windows)

 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
static char b[40];
TC_CASE(ulong_max_radix10) { volatile unsigned long v = 0xffffffffUL; CHECK_EQ_I(_ultoa_s(v, b, sizeof b, 10), 0); CHECK_STR(b, "4294967295"); }
TC_CASE(ulong_max_radix16) { volatile unsigned long v = 0xffffffffUL; CHECK_EQ_I(_ultoa_s(v, b, sizeof b, 16), 0); CHECK_STR(b, "ffffffff"); }
TC_CASE(radix2_zero) { volatile unsigned long v = 0; CHECK_EQ_I(_ultoa_s(v, b, sizeof b, 2), 0); CHECK_STR(b, "0"); }
TC_CASE(ltoa_long_min) { volatile long v = -2147483647L - 1; CHECK_EQ_I(_ltoa_s(v, b, sizeof b, 10), 0); CHECK_STR(b, "-2147483648"); }
TC_CASE(ltoa_negative_radix16_unsigned) { volatile long v = -1; CHECK_EQ_I(_ltoa_s(v, b, sizeof b, 16), 0); CHECK_STR(b, "ffffffff"); }
TC_CASE(exact_fit)
{
    char e[11]; volatile unsigned long v = 4294967295UL;
    CHECK_EQ_I(_ultoa_s(v, e, 11, 10), 0); CHECK_STR(e, "4294967295");
    { char f[12]; volatile long w = -2147483647L - 1; CHECK_EQ_I(_ltoa_s(w, f, 12, 10), 0); CHECK_STR(f, "-2147483648"); }
}
int main(void) { tc_begin(); TC_RUN(ulong_max_radix10); TC_RUN(ulong_max_radix16); TC_RUN(radix2_zero); TC_RUN(ltoa_long_min);
    TC_RUN(ltoa_negative_radix16_unsigned); TC_RUN(exact_fit); return tc_end(); }
