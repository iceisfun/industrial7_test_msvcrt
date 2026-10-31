/* TC
 * id:    cv_strtoul_limits_hex_oct
 * area:  conv
 * funcs: strtoul _strtoui64
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.1.4 (ULONG_MAX and ERANGE on overflow); MS docs strtoul, _strtoui64 (long is 32 bits on Windows)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile vs;

TC_CASE(base2_32_ones)  { char *e; vs = "11111111111111111111111111111111"; errno = 0; CHECK_EQ_U(strtoul((const char *)vs, &e, 2), 0xFFFFFFFFul); CHECK_EQ_I(errno, 0); CHECK(e == vs + 32); }
TC_CASE(base8)          { char *e; vs = "37777777777"; errno = 0; CHECK_EQ_U(strtoul((const char *)vs, &e, 8), 0xFFFFFFFFul); CHECK_EQ_I(errno, 0); CHECK(e == vs + 11); }
TC_CASE(base8_overflow) { char *e; vs = "40000000000"; errno = 0; CHECK_EQ_U(strtoul((const char *)vs, &e, 8), 0xFFFFFFFFul); CHECK_EQ_I(errno, 34); CHECK(e == vs + 11); }
TC_CASE(base36_max)     { char *e; vs = "1z141z3"; errno = 0; CHECK_EQ_U(strtoul((const char *)vs, &e, 36), 4294967295ul); CHECK_EQ_I(errno, 0); CHECK(e == vs + 7); }
TC_CASE(base36_overflow){ char *e; vs = "1z141z4"; errno = 0; CHECK_EQ_U(strtoul((const char *)vs, &e, 36), 0xFFFFFFFFul); CHECK_EQ_I(errno, 34); CHECK(e == vs + 7); }
TC_CASE(u64_base36_no_overflow) { char *e; vs = "1z141z4"; errno = 0; CHECK_EQ_U(_strtoui64((const char *)vs, &e, 36), 4294967296ull); CHECK_EQ_I(errno, 0); }
TC_CASE(u64_hex)
{
    char *e; vs = "FFFFFFFFFFFFFFFF"; errno = 0;
    CHECK_EQ_U(_strtoui64((const char *)vs, &e, 16), 0xFFFFFFFFFFFFFFFFull); CHECK_EQ_I(errno, 0); CHECK(e == vs + 16);
    vs = "10000000000000000"; errno = 0;
    CHECK_EQ_U(_strtoui64((const char *)vs, &e, 16), 0xFFFFFFFFFFFFFFFFull); CHECK_EQ_I(errno, 34); CHECK(e == vs + 17);
}
int main(void)
{
    tc_begin();
    TC_RUN(base2_32_ones); TC_RUN(base8); TC_RUN(base8_overflow); TC_RUN(base36_max); TC_RUN(base36_overflow); TC_RUN(u64_base36_no_overflow); TC_RUN(u64_hex);
    return tc_end();
}
