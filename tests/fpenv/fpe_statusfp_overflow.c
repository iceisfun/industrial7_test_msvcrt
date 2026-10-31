/* TC
 * id:    fpe_statusfp_overflow
 * area:  fpenv
 * funcs: _statusfp _clearfp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _statusfp; C11 F.9.1; IEEE 754 7.4

 */
#include "tc.h"
#include <float.h>
#include <errno.h>
#ifndef _SW_INEXACT
#define _SW_INEXACT 0x1
#endif
#ifndef _SW_ZERODIVIDE
#define _SW_ZERODIVIDE 0x8
#endif
#ifndef _SW_OVERFLOW
#define _SW_OVERFLOW 0x4
#endif
#ifndef _SW_INVALID
#define _SW_INVALID 0x10
#endif

static volatile double v_max = DBL_MAX, v_two = 2.0, sink;
static volatile float f_max = FLT_MAX, f_two = 2.0f, fsink;

TC_CASE(double_overflow)
{
    _clearfp(); sink = v_max * v_two;
    CHECK_EQ_U(_statusfp() & (_SW_OVERFLOW | _SW_INEXACT), _SW_OVERFLOW | _SW_INEXACT);
    CHECK(sink > DBL_MAX);
    _clearfp();
}
TC_CASE(float_overflow)
{
    _clearfp(); fsink = f_max * f_two;
    CHECK_EQ_U(_statusfp() & (_SW_OVERFLOW | _SW_INEXACT), _SW_OVERFLOW | _SW_INEXACT);
    CHECK(fsink > FLT_MAX);
    _clearfp();
}
TC_CASE(no_overflow)
{
    _clearfp(); sink = v_max / v_two;
    CHECK_EQ_U(_statusfp() & _SW_OVERFLOW, 0);
    _clearfp();
}
int main(void) { tc_begin(); TC_RUN(double_overflow); TC_RUN(float_overflow); TC_RUN(no_overflow); return tc_end(); }
