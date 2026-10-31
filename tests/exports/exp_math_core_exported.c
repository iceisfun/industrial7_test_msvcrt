/* TC
 * id:    exp_math_core_exported
 * area:  exports
 * funcs: 
 * procs: sin cos tan asin acos atan atan2 sinh cosh tanh exp log log10 pow sqrt ceil floor fabs fmod frexp ldexp modf abs labs div ldiv
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12, 7.22.6
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


TC_CASE(trig)
{
    HAS("sin");
    HAS("cos");
    HAS("tan");
    HAS("asin");
    HAS("acos");
    HAS("atan");
    HAS("atan2");
}

TC_CASE(hyperbolic)
{
    HAS("sinh");
    HAS("cosh");
    HAS("tanh");
}

TC_CASE(exp_log)
{
    HAS("exp");
    HAS("log");
    HAS("log10");
    HAS("pow");
    HAS("sqrt");
}

TC_CASE(rounding)
{
    HAS("ceil");
    HAS("floor");
    HAS("fabs");
    HAS("fmod");
}

TC_CASE(decompose)
{
    HAS("frexp");
    HAS("ldexp");
    HAS("modf");
}

TC_CASE(integer_helpers)
{
    HAS("abs");
    HAS("labs");
    HAS("div");
    HAS("ldiv");
}

int main(void)
{
    tc_begin();
    TC_RUN(trig);
    TC_RUN(hyperbolic);
    TC_RUN(exp_log);
    TC_RUN(rounding);
    TC_RUN(decompose);
    TC_RUN(integer_helpers);
    return tc_end();
}
