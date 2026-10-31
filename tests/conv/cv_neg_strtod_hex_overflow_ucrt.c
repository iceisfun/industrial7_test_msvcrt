/* TC
 * id:    cv_neg_strtod_hex_overflow_ucrt
 * area:  conv
 * funcs: strtod wcstod strtof
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.22.1.3 (hex floating input, ERANGE on overflow); MS docs strtod (returns +/-HUGE_VAL on overflow, 0 on underflow, errno ERANGE)
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>
#include <math.h>

static const char *volatile vs;
static const wchar_t *volatile vw;

TC_CASE(overflow_pos) { char *e; double d; errno = 0; vs = "0x1p1024"; d = strtod(vs, &e); CHECK_BITS_D(d, HUGE_VAL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 8); }
TC_CASE(overflow_neg) { char *e; double d; errno = 0; vs = "-0x1p1024"; d = strtod(vs, &e); CHECK_BITS_D(d, -HUGE_VAL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 9); }
TC_CASE(underflow)    { char *e; double d; errno = 0; vs = "0x1p-1080"; d = strtod(vs, &e); CHECK_BITS_D(d, 0.0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 9); }
TC_CASE(float_overflow) { char *e; float f; errno = 0; vs = "0x1p128"; f = strtof(vs, &e); CHECK_BITS_F(f, HUGE_VALF); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 7); }
TC_CASE(wide_overflow) { wchar_t *e; double d; errno = 0; vw = L"0x1p1024"; d = wcstod(vw, &e); CHECK_BITS_D(d, HUGE_VAL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vw + 8); }
int main(void)
{
    tc_begin();
    TC_RUN(overflow_pos); TC_RUN(overflow_neg); TC_RUN(underflow); TC_RUN(float_overflow); TC_RUN(wide_overflow);
    return tc_end();
}
