/* TC
 * id:    fpe_statusfp_invalid
 * area:  fpenv
 * funcs: _statusfp _clearfp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _statusfp; C11 F.9.1; IEEE 754 7.2

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

static volatile double v_inf, v_zero = 0.0, v_one = 1.0, sink;

TC_CASE(inf_minus_inf)
{
    v_inf = v_one / v_zero; _clearfp();
    sink = v_inf - v_inf;
    CHECK_EQ_U(_statusfp() & _SW_INVALID, _SW_INVALID);
    CHECK_IS_NAN(sink);
    _clearfp();
}
TC_CASE(zero_over_zero)
{
    _clearfp(); sink = v_zero / v_zero;
    CHECK_EQ_U(_statusfp() & _SW_INVALID, _SW_INVALID);
    CHECK_IS_NAN(sink);
    _clearfp();
}
TC_CASE(zero_times_inf)
{
    v_inf = v_one / v_zero; _clearfp();
    sink = v_zero * v_inf;
    CHECK_EQ_U(_statusfp() & _SW_INVALID, _SW_INVALID);
    CHECK_IS_NAN(sink);
    _clearfp();
}
int main(void) { tc_begin(); TC_RUN(inf_minus_inf); TC_RUN(zero_over_zero); TC_RUN(zero_times_inf); return tc_end(); }
