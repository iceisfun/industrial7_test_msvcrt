/* TC
 * id:    fpe_statusfp_underflow
 * area:  fpenv
 * funcs: _statusfp _clearfp
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _statusfp/_clearfp; float.h _SW_*
 * note:  Exact denormal results are deliberately not asserted.
 */
#include "tc.h"
#include <float.h>
#ifndef _SW_INEXACT /* mingw float.h lacks the _SW_* aliases of the MS header; values per MS float.h */
#define _SW_INEXACT 0x1
#define _SW_UNDERFLOW 0x2
#define _SW_OVERFLOW 0x4
#define _SW_ZERODIVIDE 0x8
#define _SW_INVALID 0x10
#define _SW_DENORMAL 0x80000
#endif

static volatile double v_min = DBL_MIN, v_half = 0.5, v_tiny = 1e-200;

TC_CASE(mul_underflows)
{
    volatile double r; unsigned s;
    _clearfp();
    r = v_tiny * v_tiny;            /* 1e-400 underflows to 0 (inexact) */
    s = _statusfp();
    CHECK(r == 0.0);
    CHECK((s & _SW_UNDERFLOW) != 0);
    _clearfp();
}
TC_CASE(tiny_is_inexact)
{
    volatile double r; unsigned s;
    _clearfp();
    r = v_min * v_tiny;             /* far below denormal range: rounds to 0 */
    s = _statusfp();
    CHECK(r == 0.0);
    CHECK((s & _SW_INEXACT) != 0);
    CHECK((s & _SW_UNDERFLOW) != 0);
    _clearfp();
}
TC_CASE(inexact_denormal_result)
{
    volatile double r; unsigned s;
    _clearfp();
    r = v_min * v_half * 0.3;       /* denormal, not exactly representable: tiny and inexact */
    s = _statusfp();
    CHECK(r > 0.0 && r < DBL_MIN);
    CHECK((s & _SW_UNDERFLOW) != 0);
    CHECK((s & _SW_INEXACT) != 0);
    _clearfp();
}
TC_CASE(no_underflow_normal)
{
    volatile double r; unsigned s;
    _clearfp();
    r = v_half * v_half;
    s = _statusfp();
    CHECK(r == 0.25);
    CHECK((s & _SW_UNDERFLOW) == 0);
    CHECK((s & _SW_OVERFLOW) == 0);
    _clearfp();
}
int main(void) { tc_begin(); TC_RUN(mul_underflows); TC_RUN(tiny_is_inexact); TC_RUN(inexact_denormal_result); TC_RUN(no_underflow_normal); return tc_end(); }
