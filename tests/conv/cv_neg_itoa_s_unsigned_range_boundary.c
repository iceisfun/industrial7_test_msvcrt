/* TC
 * id:    cv_neg_itoa_s_unsigned_range_boundary
 * area:  conv
 * funcs: _ultoa_s _ui64toa_s _itoa_s _i64toa_s
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itoa_s, _i64toa_s, _ui64toa_s (buffer must hold digits plus NUL; too small -> ERANGE)
 * note:  Exact-fit vs one-short boundaries. Buffer state after failure is in cv_neg_itoa_s_toosmall_state_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

static volatile int v_min32 = (-2147483647 - 1);
static volatile long long v_min64 = (-9223372036854775807LL - 1);
static volatile unsigned long long v_max64 = 18446744073709551615ULL;
static volatile unsigned long v_max32 = 4294967295UL;

TC_CASE(itoa_s_min)
{
    char b[32];
    tc_iph_install();
    CHECK_EQ_I(_itoa_s(v_min32, b, 11, 10), ERANGE);
    CHECK_EQ_I(_itoa_s(v_min32, b, 12, 10), 0); CHECK_STR(b, "-2147483648");
}
TC_CASE(i64toa_s_min)
{
    char b[32];
    tc_iph_install();
    CHECK_EQ_I(_i64toa_s(v_min64, b, 20, 10), ERANGE);
    CHECK_EQ_I(_i64toa_s(v_min64, b, 21, 10), 0); CHECK_STR(b, "-9223372036854775808");
}
TC_CASE(ui64toa_s_max)
{
    char b[32];
    tc_iph_install();
    CHECK_EQ_I(_ui64toa_s(v_max64, b, 20, 10), ERANGE);
    CHECK_EQ_I(_ui64toa_s(v_max64, b, 21, 10), 0); CHECK_STR(b, "18446744073709551615");
}
TC_CASE(ultoa_s_max)
{
    char b[32];
    tc_iph_install();
    CHECK_EQ_I(_ultoa_s(v_max32, b, 10, 10), ERANGE);
    CHECK_EQ_I(_ultoa_s(v_max32, b, 11, 10), 0); CHECK_STR(b, "4294967295");
}
TC_CASE(ultoa_s_hex_boundary)
{
    char b[32];
    tc_iph_install();
    CHECK_EQ_I(_ultoa_s(v_max32, b, 8, 16), ERANGE);
    CHECK_EQ_I(_ultoa_s(v_max32, b, 9, 16), 0); CHECK_STR(b, "ffffffff");
}
int main(void)
{
    tc_begin();
    TC_RUN(itoa_s_min); TC_RUN(i64toa_s_min); TC_RUN(ui64toa_s_max); TC_RUN(ultoa_s_max); TC_RUN(ultoa_s_hex_boundary);
    return tc_end();
}
