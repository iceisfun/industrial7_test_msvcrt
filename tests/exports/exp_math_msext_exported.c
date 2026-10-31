/* TC
 * id:    exp_math_msext_exported
 * area:  exports
 * funcs: 
 * procs: _chgsign _copysign _finite _fpclass _isnan _logb _nextafter _scalb _hypot _j0 _j1 _jn _y0 _y1 _yn _cabs _clearfp _controlfp _statusfp _fpreset _control87
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs float.h / math.h underscore functions
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


TC_CASE(classification)
{
    HAS("_finite");
    HAS("_fpclass");
    HAS("_isnan");
    HAS("_logb");
}

TC_CASE(sign_ops)
{
    HAS("_chgsign");
    HAS("_copysign");
    HAS("_nextafter");
    HAS("_scalb");
}

TC_CASE(bessel)
{
    HAS("_j0");
    HAS("_j1");
    HAS("_jn");
    HAS("_y0");
    HAS("_y1");
    HAS("_yn");
}

TC_CASE(fp_control)
{
    HAS("_clearfp");
    HAS("_controlfp");
    HAS("_statusfp");
    HAS("_fpreset");
    HAS("_control87");
}

TC_CASE(cabs)
{
    HAS("_cabs");
    HAS("_hypot");
}

int main(void)
{
    tc_begin();
    TC_RUN(classification);
    TC_RUN(sign_ops);
    TC_RUN(bessel);
    TC_RUN(fp_control);
    TC_RUN(cabs);
    return tc_end();
}
