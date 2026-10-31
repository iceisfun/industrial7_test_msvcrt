/* TC
 * id:    scn_neg_sscanf_float_no_digits_old_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   win7
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.21.6.2p12 (e,f,g match a floating-point number per strtod), 7.22.1.3
 * note:  PROBES pre-UCRT runtimes for the strict-C result on digit-less float text (ret 0 / dest untouched). Strings with no digits in the mantissa are not valid floating-point numbers: return 0, destination untouched.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile in;

static void nofloat(const char *str)
{
    double d = 7.0; float f = 7.0f; int r, r2; in = str;
    r = sscanf(in, "%lf", &d); r2 = sscanf(in, "%f", &f);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(r2, 0); CHECK_BITS_D(d, 7.0); CHECK_BITS_F(f, 7.0f);
}
static void noflt_end(const char *str)  /* input ends right after the lone sign/dot: 0 or EOF (C is ambiguous), dest untouched */
{
    double d = 7.0; int r; in = str; r = sscanf(in, "%lf", &d); CHECK(r == 0 || r == EOF); CHECK_BITS_D(d, 7.0);
}

TC_CASE(dot)        { nofloat(".z"); }
TC_CASE(minus_dot)  { nofloat("-.z"); }
TC_CASE(e5)         { nofloat("e5"); }
TC_CASE(plus_e1)    { nofloat("+e1"); }
TC_CASE(minus)      { nofloat("-z"); }
TC_CASE(dot_end)    { noflt_end("."); }
TC_CASE(minus_end)  { noflt_end("-"); }
TC_CASE(minus_dot_end) { noflt_end("-."); }

int main(void) { tc_begin(); TC_RUN(dot); TC_RUN(minus_dot); TC_RUN(e5); TC_RUN(plus_e1); TC_RUN(minus); TC_RUN(dot_end); TC_RUN(minus_end); TC_RUN(minus_dot_end); return tc_end(); }
