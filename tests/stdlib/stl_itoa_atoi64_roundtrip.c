/* TC
 * id:    stl_itoa_atoi64_roundtrip
 * area:  stdlib
 * funcs: _itoa _atoi64
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _itoa, _ltoa, _ultoa (negative values only get a sign for radix 10), _atoi64
 * note:  Overflow of _atoi64 is covered by stl_neg_atoi64_overflow_probe.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(radices)
{
    char b[40];
    CHECK_STR(_itoa(255, b, 16), "ff");
    CHECK_STR(_itoa(255, b, 2), "11111111");
    CHECK_STR(_itoa(8, b, 8), "10");
    CHECK_STR(_itoa(35, b, 36), "z");
    CHECK_STR(_itoa(36, b, 36), "10");
    CHECK_STR(_itoa(2147483647, b, 10), "2147483647");
}
TC_CASE(negatives)
{
    char b[40];
    CHECK_STR(_itoa(-255, b, 10), "-255");
    CHECK_STR(_itoa(-255, b, 16), "ffffff01");
    CHECK_STR(_itoa(-1, b, 2), "11111111111111111111111111111111");
}
TC_CASE(zero) { char b[40]; CHECK_STR(_itoa(0, b, 2), "0"); CHECK_STR(_itoa(0, b, 10), "0"); CHECK_STR(_itoa(0, b, 36), "0"); }
TC_CASE(returns_buffer) { char b[40]; CHECK_EQ_P(_itoa(5, b, 10), b); }
TC_CASE(atoi64_basic)
{
    CHECK_EQ_I(_atoi64("9223372036854775807"), 9223372036854775807LL);
    CHECK_EQ_I(_atoi64("-9223372036854775808"), (-9223372036854775807LL - 1));
    CHECK_EQ_I(_atoi64("0"), 0);
    CHECK_EQ_I(_atoi64("123abc"), 123);
    CHECK_EQ_I(_atoi64("abc"), 0);
}
TC_CASE(atoi64_ws_sign)
{
    CHECK_EQ_I(_atoi64("   42"), 42);
    CHECK_EQ_I(_atoi64(" \t\n-17"), -17);
    CHECK_EQ_I(_atoi64("+99"), 99);
    CHECK_EQ_I(_atoi64("  -4000000000"), -4000000000LL);
}
TC_CASE(roundtrip)
{
    char b[40]; int v[5] = {0, 1, -1, 2147483647, -2147483647 - 1}; int i;
    for (i = 0; i < 5; i++) { _itoa(v[i], b, 10); CHECK_EQ_I(_atoi64(b), v[i]); }
}
int main(void)
{
    tc_begin();
    TC_RUN(radices); TC_RUN(negatives); TC_RUN(zero); TC_RUN(returns_buffer);
    TC_RUN(atoi64_basic); TC_RUN(atoi64_ws_sign); TC_RUN(roundtrip);
    return tc_end();
}
