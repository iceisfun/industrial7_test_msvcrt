/* TC
 * id:    ucr_strtod_special_values
 * area:  ucrt_only
 * funcs: strtod
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C99/C11 7.22.1.3 (INF, INFINITY, NAN, hex floating); MS docs strtod
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile v_s;

TC_CASE(inf_spellings)
{
    char *e; double d;
    v_s = "inf"; d = strtod(v_s, &e); CHECK(d > 1e308 && d == d + 1e300 + d); CHECK_EQ_P(e, v_s + 3);
    v_s = "INF"; d = strtod(v_s, &e); CHECK(d > 1e308 && d == d * 2); CHECK_EQ_P(e, v_s + 3);
    v_s = "infinity"; d = strtod(v_s, &e); CHECK(d > 1e308 && d == d * 2); CHECK_EQ_P(e, v_s + 8);
}
TC_CASE(neg_inf)
{
    char *e; double d; v_s = "-inf"; d = strtod(v_s, &e);
    CHECK(d < -1e308 && d == d * 2); CHECK_EQ_P(e, v_s + 4);
}
TC_CASE(nan_sign)
{
    char *e; double d;
    v_s = "nan"; d = strtod(v_s, &e); CHECK_IS_NAN(d); CHECK_EQ_P(e, v_s + 3);
    v_s = "-nan"; d = strtod(v_s, &e); CHECK_IS_NAN(d); CHECK_EQ_P(e, v_s + 4);
    CHECK((tc_dbits(d) >> 63) == 1);
}
TC_CASE(hex_float)
{
    char *e; double d; v_s = "0x1.8p1"; d = strtod(v_s, &e);
    CHECK_BITS_D(d, 3.0); CHECK_EQ_P(e, v_s + 7);
}
TC_CASE(neg_zero)
{
    char *e; double d; v_s = "-0"; d = strtod(v_s, &e);
    CHECK_EQ_U(tc_dbits(d), 0x8000000000000000ULL); CHECK_EQ_P(e, v_s + 2);
}
int main(void)
{
    tc_begin();
    TC_RUN(inf_spellings); TC_RUN(neg_inf); TC_RUN(nan_sign); TC_RUN(hex_float); TC_RUN(neg_zero);
    return tc_end();
}
