/* TC
 * id:    fpe_statusfp_inexact
 * area:  fpenv
 * funcs: _statusfp _clearfp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _statusfp; IEEE 754 5.5

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

TC_CASE(inexact_set) { _clearfp(); sink = v_one / v_three; CHECK_EQ_U(_statusfp() & _SW_INEXACT, _SW_INEXACT); _clearfp(); }
TC_CASE(exact_not_set) { _clearfp(); sink = v_one + v_one; CHECK_EQ_U(_statusfp() & _SW_INEXACT, 0); CHECK_BITS_D(sink, 2.0); _clearfp(); }
int main(void) { tc_begin(); TC_RUN(inexact_set); TC_RUN(exact_not_set); return tc_end(); }
