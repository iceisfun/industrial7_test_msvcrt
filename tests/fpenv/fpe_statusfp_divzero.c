/* TC
 * id:    fpe_statusfp_divzero
 * area:  fpenv
 * funcs: _statusfp _clearfp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _statusfp; C11 F.9.1; IEEE 754 7.3

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

static volatile double v_one = 1.0, v_neg = -1.0, v_zero = 0.0, sink;

TC_CASE(pos)
{
    _clearfp(); sink = v_one / v_zero;
    CHECK_EQ_U(_statusfp() & _SW_ZERODIVIDE, _SW_ZERODIVIDE);
    CHECK(sink > 1e308 && sink == sink + sink);
    _clearfp();
}
TC_CASE(neg)
{
    _clearfp(); sink = v_neg / v_zero;
    CHECK_EQ_U(_statusfp() & _SW_ZERODIVIDE, _SW_ZERODIVIDE);
    CHECK(sink < -1e308 && sink == sink + sink);
    _clearfp();
}
TC_CASE(zero_over_one_no_flag)
{
    _clearfp(); sink = v_zero / v_one;
    CHECK_EQ_U(_statusfp() & _SW_ZERODIVIDE, 0);
    CHECK_BITS_D(sink, 0.0);
    _clearfp();
}
int main(void) { tc_begin(); TC_RUN(pos); TC_RUN(neg); TC_RUN(zero_over_one_no_flag); return tc_end(); }
