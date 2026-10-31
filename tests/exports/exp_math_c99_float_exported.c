/* TC
 * id:    exp_math_c99_float_exported
 * area:  exports
 * funcs: 
 * procs: sinf cosf tanf asinf acosf atanf atan2f sinhf coshf tanhf expf logf log10f powf sqrtf ceilf floorf fmodf modff roundf truncf cbrtf fmaxf fminf expm1f exp2f log2f erff tgammaf
 * crt:   msvcr120 ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs float variants (VS2013+)
 * note:  Pre-120 CRTs left out on purpose. Plain export presence.
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


TC_CASE(classic_float)
{
    HAS("sinf");
    HAS("cosf");
    HAS("tanf");
    HAS("asinf");
    HAS("acosf");
    HAS("atanf");
    HAS("atan2f");
    HAS("sinhf");
    HAS("coshf");
    HAS("tanhf");
    HAS("expf");
    HAS("logf");
    HAS("log10f");
    HAS("powf");
    HAS("sqrtf");
    HAS("ceilf");
}

TC_CASE(c99_float_rounding)
{
    HAS("roundf");
    HAS("truncf");
    HAS("ceilf");
    HAS("floorf");
}

TC_CASE(c99_float_misc)
{
    HAS("cbrtf");
    HAS("fmaxf");
    HAS("fminf");
    HAS("expm1f");
    HAS("exp2f");
    HAS("log2f");
    HAS("erff");
    HAS("tgammaf");
}

int main(void)
{
    tc_begin();
    TC_RUN(classic_float);
    TC_RUN(c99_float_rounding);
    TC_RUN(c99_float_misc);
    return tc_end();
}
