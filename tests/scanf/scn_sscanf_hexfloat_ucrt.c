/* TC
 * id:    scn_sscanf_hexfloat_ucrt
 * area:  scanf
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   C99 7.21.6.2 fscanf (a,A,e,E,f,F,g,G accept hex floats)
 * note:  UCRT scanf accepts C99 hexadecimal floating input.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;

TC_CASE(p3) { double d = 0; int r; v = "0x1p3"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 8.0); }
TC_CASE(frac) { double d = 0; int r; v = "0x1.8p1"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 3.0); }
TC_CASE(neg_exp) { double d = 0; int r; v = "-0x1p-1"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, -0.5); }
TC_CASE(no_int_digits) { double d = 0; int r; v = "0X.8P1"; r = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 1.0); }
TC_CASE(conversions) { double a = 0, b = 0, c = 0; int r, n = -1; v = "0x10p0 0xAp0 0x.4p0";
    r = sscanf((const char *)v, "%la %lA %le%n", &a, &b, &c, &n); CHECK_EQ_I(r, 3);
    CHECK_BITS_D(a, 16.0); CHECK_BITS_D(b, 10.0); CHECK_BITS_D(c, 0.25); CHECK_EQ_I(n, 19); }
TC_CASE(consumed) { double d = 0; int r, n = -1; v = "0x1.8p1z"; r = sscanf((const char *)v, "%lf%n", &d, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(n, 7); }
TC_CASE(float_dest) { float f = 0; int r; v = "0x1.8p0"; r = sscanf((const char *)v, "%f", &f); CHECK_EQ_I(r, 1); CHECK_BITS_F(f, 1.5f); }
int main(void) { tc_begin(); TC_RUN(p3); TC_RUN(frac); TC_RUN(neg_exp); TC_RUN(no_int_digits); TC_RUN(conversions); TC_RUN(consumed); TC_RUN(float_dest); return tc_end(); }
