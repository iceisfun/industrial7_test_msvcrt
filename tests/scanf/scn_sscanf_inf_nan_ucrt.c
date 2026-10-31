/* TC
 * id:    scn_sscanf_inf_nan_ucrt
 * area:  scanf
 * funcs: sscanf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   C99 7.21.6.2 fscanf (inf, infinity, nan)
 * note:  UCRT scanf parses inf/infinity/nan (case-insensitive) since VS2015.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
#define INFB 0x7FF0000000000000ULL
#define ISNAN_BITS(u) ((((u) >> 52) & 0x7FF) == 0x7FF && ((u) & 0xFFFFFFFFFFFFFULL) != 0)

TC_CASE(inf_forms) { double d; int n;
    d = 0; v = "inf"; n = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(n, 1); CHECK_EQ_U(tc_dbits(d), INFB);
    d = 0; v = "INF"; n = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(n, 1); CHECK_EQ_U(tc_dbits(d), INFB);
    d = 0; v = "-inf"; n = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(n, 1); CHECK_EQ_U(tc_dbits(d), INFB | 0x8000000000000000ULL);
    d = 0; v = "+Inf"; n = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(n, 1); CHECK_EQ_U(tc_dbits(d), INFB);
}
TC_CASE(infinity_consumed) { double d = 0; int n, c = -1, r; v = "infinity"; r = sscanf((const char *)v, "%lf%n", &d, &c);
    CHECK_EQ_I(r, 1); CHECK_EQ_U(tc_dbits(d), INFB); CHECK_EQ_I(c, 8);
    d = 0; c = -1; v = "-INFINITY"; r = sscanf((const char *)v, "%lf%n", &d, &c); (void)n;
    CHECK_EQ_I(r, 1); CHECK_EQ_U(tc_dbits(d), INFB | 0x8000000000000000ULL); CHECK_EQ_I(c, 9);
}
TC_CASE(nan_forms) { double d; int n;
    d = 0; v = "nan"; n = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(n, 1); CHECK(ISNAN_BITS(tc_dbits(d)));
    d = 0; v = "NAN"; n = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(n, 1); CHECK(ISNAN_BITS(tc_dbits(d)));
    d = 0; v = "-nan"; n = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(n, 1); CHECK(ISNAN_BITS(tc_dbits(d)));
    d = 0; v = "NaN"; n = sscanf((const char *)v, "%lf", &d); CHECK_EQ_I(n, 1); CHECK_IS_NAN(d);
}
TC_CASE(float_variants) { float f = 0; int n;
    v = "inf"; n = sscanf((const char *)v, "%f", &f); CHECK_EQ_I(n, 1); CHECK_EQ_U(tc_fbits(f), 0x7F800000u);
    f = 0; v = "-infinity"; n = sscanf((const char *)v, "%f", &f); CHECK_EQ_I(n, 1); CHECK_EQ_U(tc_fbits(f), 0xFF800000u);
    f = 0; v = "nan"; n = sscanf((const char *)v, "%f", &f); CHECK_EQ_I(n, 1); CHECK(f != f);
}
TC_CASE(e_g_conversions) { double a = 0, b = 0; int n; v = "inf nan";
    n = sscanf((const char *)v, "%le %lg", &a, &b); CHECK_EQ_I(n, 2); CHECK_EQ_U(tc_dbits(a), INFB); CHECK_IS_NAN(b);
}
int main(void) { tc_begin(); TC_RUN(inf_forms); TC_RUN(infinity_consumed); TC_RUN(nan_forms); TC_RUN(float_variants); TC_RUN(e_g_conversions); return tc_end(); }
