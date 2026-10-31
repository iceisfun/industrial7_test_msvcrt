/* TC
 * id:    fpe_clearfp_returns_and_clears
 * area:  fpenv
 * funcs: _clearfp _statusfp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _clearfp, _statusfp

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

static volatile double v_one = 1.0, v_three = 3.0, sink;

TC_CASE(returns_prior_status)
{
    unsigned int r;
    _clearfp();
    sink = v_one / v_three;
    r = _clearfp();
    CHECK_EQ_U(r & _SW_INEXACT, _SW_INEXACT);
}
TC_CASE(zero_afterwards)
{
    _clearfp();
    sink = v_one / v_three;
    _clearfp();
    CHECK_EQ_U(_statusfp(), 0);
}
TC_CASE(second_clear_zero)
{
    _clearfp();
    sink = v_one / v_three;
    _clearfp();
    CHECK_EQ_U(_clearfp(), 0);
}
int main(void) { tc_begin(); TC_RUN(returns_prior_status); TC_RUN(zero_afterwards); TC_RUN(second_clear_zero); return tc_end(); }
