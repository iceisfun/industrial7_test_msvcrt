/* TC
 * id:    exp_neg_probe_c99_math_absent_pre110
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: malloc
 * crt:   msvcr80 msvcr90 msvcr100 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS What's new VS2013 (C99 math added to the CRT in Visual Studio 2013)
 * note:  Probing: these C99 names were first exported by msvcr120. msvcr110 is excluded because its status is less certain. hypot is isolated in its own case: it is believed to be absent before VS2013 (only _hypot) but the x64 CRTs may differ.
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


TC_CASE(rounding_absent)
{
    HASNOT("round");
    HASNOT("trunc");
    HASNOT("rint");
    HASNOT("nearbyint");
    HASNOT("lround");
    HASNOT("llround");
    HASNOT("lrint");
}

TC_CASE(exp_log_absent)
{
    HASNOT("cbrt");
    HASNOT("exp2");
    HASNOT("log2");
    HASNOT("expm1");
    HASNOT("log1p");
}

TC_CASE(hypot_absent)
{
    /* isolated: x64 CRTs may export hypot alongside _hypot even before VS2013 */
    HASNOT("hypot");
}

TC_CASE(special_absent)
{
    HASNOT("tgamma");
    HASNOT("lgamma");
    HASNOT("erf");
    HASNOT("erfc");
    HASNOT("remainder");
    HASNOT("fma");
    HASNOT("fmax");
    HASNOT("fmin");
    HASNOT("acosh");
    HASNOT("asinh");
    HASNOT("atanh");
    HASNOT("nan");
}

int main(void)
{
    tc_begin();
    TC_RUN(rounding_absent);
    TC_RUN(exp_log_absent);
    TC_RUN(hypot_absent);
    TC_RUN(special_absent);
    return tc_end();
}
