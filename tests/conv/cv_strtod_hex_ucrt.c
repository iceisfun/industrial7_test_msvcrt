/* TC
 * id:    cv_strtod_hex_ucrt
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.3 (hexadecimal floating constants, binary-exponent p); MS docs strtod (UCRT supports hex)
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static const char *volatile vs;
static double FB(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static double P(const char *s, long *off) { char *e = NULL; double d; vs = s; d = strtod(vs, &e); *off = (long)(e - s); return d; }

TC_CASE(basic) { long o; CHECK_BITS_D(P("0x1p0", &o), 1.0); CHECK_EQ_I(o, 5); CHECK_BITS_D(P("0x1.8p1", &o), 3.0); CHECK_EQ_I(o, 7); }
TC_CASE(upper_no_int_digits) { long o; CHECK_BITS_D(P("0X.8P0", &o), 0.5); CHECK_EQ_I(o, 6); }
TC_CASE(no_exponent) { long o; CHECK_BITS_D(P("0x10", &o), 16.0); CHECK_EQ_I(o, 4); CHECK_BITS_D(P("0xAp0", &o), 10.0); }
TC_CASE(denorm_min) { long o; CHECK_BITS_D(P("0x1p-1074", &o), FB(1)); CHECK_EQ_I(o, 9); }
TC_CASE(negative) { long o; CHECK_BITS_D(P("-0x1p3", &o), -8.0); CHECK_EQ_I(o, 6); }
TC_CASE(bare_0x) { long o; CHECK_BITS_D(P("0x", &o), 0.0); CHECK_EQ_I(o, 1); }
TC_CASE(partial_exponent) { long o; CHECK_BITS_D(P("0x1p", &o), 1.0); CHECK_EQ_I(o, 3); CHECK_BITS_D(P("0x1p+", &o), 1.0); CHECK_EQ_I(o, 3); }

int main(void)
{
    tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod"); 
    TC_RUN(basic); TC_RUN(upper_no_int_digits); TC_RUN(no_exponent); TC_RUN(denorm_min);
    TC_RUN(negative); TC_RUN(bare_0x); TC_RUN(partial_exponent);
    return tc_end();
}
