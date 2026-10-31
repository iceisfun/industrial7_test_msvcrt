/* TC
 * id:    exp_math_c99_double_exported
 * area:  exports
 * funcs: 
 * procs: acosh asinh atanh cbrt copysign erf erfc exp2 expm1 fdim fma fmax fmin ilogb lgamma llrint llround log1p log2 logb lrint lround nan nearbyint nextafter nexttoward remainder remquo rint round scalbln scalbn tgamma trunc
 * crt:   msvcr120 ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS What's new VS2013 (C99 math); function Requirements Visual Studio 2013+
 * note:  Plain export presence.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#define HAS(n) do { if (!tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "not exported: " n); } while (0)
#define HASNOT(n) do { if (tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "unexpectedly exported: " n); } while (0)


TC_CASE(inverse_hyperbolic)
{
    HAS("acosh");
    HAS("asinh");
    HAS("atanh");
}

TC_CASE(gamma_erf)
{
    HAS("erf");
    HAS("erfc");
    HAS("lgamma");
    HAS("tgamma");
}

TC_CASE(rounding_family)
{
    HAS("llrint");
    HAS("llround");
    HAS("lrint");
    HAS("lround");
    HAS("nearbyint");
    HAS("rint");
    HAS("round");
    HAS("trunc");
}

TC_CASE(exp_log_family)
{
    HAS("cbrt");
    HAS("exp2");
    HAS("expm1");
    HAS("log1p");
    HAS("log2");
    HAS("logb");
    HAS("ilogb");
}

TC_CASE(fma_minmax)
{
    HAS("fdim");
    HAS("fma");
    HAS("fmax");
    HAS("fmin");
    HAS("copysign");
}

TC_CASE(nan_and_next)
{
    HAS("nan");
    HAS("nextafter");
    HAS("nexttoward");
    HAS("remainder");
    HAS("remquo");
    HAS("scalbln");
    HAS("scalbn");
}

int main(void)
{
    tc_begin();
    TC_RUN(inverse_hyperbolic);
    TC_RUN(gamma_erf);
    TC_RUN(rounding_family);
    TC_RUN(exp_log_family);
    TC_RUN(fma_minmax);
    TC_RUN(nan_and_next);
    return tc_end();
}
