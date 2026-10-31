/* TC
 * id:    cv_neg_itoa_overlong_radix2
 * area:  conv
 * funcs: _itoa _ltoa _ultoa _i64toa _ui64toa
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _itoa, _ltoa, _ultoa, _i64toa, _ui64toa (buffer must hold the worst case: 33 chars for radix 2 32-bit, 65 for 64-bit; minus sign only for radix 10)
 * note:  The guard-byte checks are an implementation-quality expectation (docs only state the maximum length), not a documented guarantee. Guard bytes after the terminator must be untouched (a converter that writes digits then reverses in place beyond the NUL would fail).
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#define CAN 0x5A
static volatile unsigned long v_ul = 0xFFFFFFFFUL;
static volatile unsigned long long v_u64 = 0xFFFFFFFFFFFFFFFFULL;
static volatile long long v_i64min = LLONG_MIN;
static volatile int v_imin = INT_MIN;
static volatile long v_lmin = LONG_MIN;

static void fill(char *b, size_t n) { size_t i; for (i = 0; i < n; i++) b[i] = CAN; }
static int guard_ok(const char *b, size_t from, size_t n) { size_t i; for (i = from; i < n; i++) if ((unsigned char)b[i] != CAN) return 0; return 1; }

TC_CASE(ultoa_radix2)
{
    char b[48]; char *r; fill(b, sizeof b);
    r = _ultoa(v_ul, b, 2);
    CHECK(r == b); CHECK_STR(b, "11111111111111111111111111111111");
    CHECK_EQ_I(b[32], 0); CHECK(guard_ok(b, 33, sizeof b));
}
TC_CASE(ui64toa_radix2)
{
    char b[80]; char *r; fill(b, sizeof b);
    r = _ui64toa(v_u64, b, 2);
    CHECK(r == b);
    CHECK_STR(b, "1111111111111111111111111111111111111111111111111111111111111111");
    CHECK_EQ_I(b[64], 0); CHECK(guard_ok(b, 65, sizeof b));
}
TC_CASE(i64toa_min)
{
    char b[40]; fill(b, sizeof b);
    _i64toa(v_i64min, b, 10);
    CHECK_STR(b, "-9223372036854775808");
    CHECK_EQ_I(b[20], 0); CHECK(guard_ok(b, 21, sizeof b));
}
TC_CASE(itoa_min)
{
    char b[24]; fill(b, sizeof b);
    _itoa(v_imin, b, 10);
    CHECK_STR(b, "-2147483648");
    CHECK_EQ_I(b[11], 0); CHECK(guard_ok(b, 12, sizeof b));
}
TC_CASE(ltoa_min_hex_unsigned)
{
    char b[24]; fill(b, sizeof b);
    _ltoa(v_lmin, b, 16);
    CHECK_STR(b, "80000000");   /* sign only emitted for radix 10 */
    CHECK_EQ_I(b[8], 0); CHECK(guard_ok(b, 9, sizeof b));
}
TC_CASE(ltoa_min_dec)
{
    char b[24]; fill(b, sizeof b);
    _ltoa(v_lmin, b, 10);
    CHECK_STR(b, "-2147483648");
    CHECK(guard_ok(b, 12, sizeof b));
}

int main(void)
{
    tc_begin();
    TC_RUN(ultoa_radix2); TC_RUN(ui64toa_radix2); TC_RUN(i64toa_min); TC_RUN(itoa_min);
    TC_RUN(ltoa_min_hex_unsigned); TC_RUN(ltoa_min_dec);
    return tc_end();
}
