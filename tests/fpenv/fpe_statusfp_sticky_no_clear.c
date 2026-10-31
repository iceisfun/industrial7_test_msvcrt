/* TC
 * id:    fpe_statusfp_sticky_no_clear
 * area:  fpenv
 * funcs: _statusfp _clearfp
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _statusfp, _clearfp
 * note:  Flags are sticky status bits; only _clearfp (or _fpreset) resets them.
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

static volatile double v_one = 1.0, v_three = 3.0, v_zero = 0.0, v_big = 1e308;

TC_CASE(two_reads_equal)
{
    volatile double r; unsigned a, b;
    _clearfp();
    r = v_one / v_three; (void)r;
    a = _statusfp(); b = _statusfp();
    CHECK((a & _SW_INEXACT) != 0);
    CHECK_EQ_U(a, b);
    _clearfp();
}
TC_CASE(accumulate)
{
    volatile double r; unsigned s;
    _clearfp();
    r = v_one / v_three; (void)r;
    s = _statusfp();
    CHECK((s & _SW_INEXACT) != 0);
    CHECK((s & _SW_OVERFLOW) == 0);
    r = v_big * v_big; (void)r;
    s = _statusfp();
    CHECK((s & _SW_INEXACT) != 0);   /* still set */
    CHECK((s & _SW_OVERFLOW) != 0);  /* newly set */
    r = v_one / v_zero; (void)r;
    s = _statusfp();
    CHECK((s & (_SW_INEXACT | _SW_OVERFLOW | _SW_ZERODIVIDE)) == (_SW_INEXACT | _SW_OVERFLOW | _SW_ZERODIVIDE));
    _clearfp();
}
TC_CASE(only_clearfp_resets)
{
    volatile double r; unsigned prev, s;
    _clearfp();
    r = v_one / v_three; (void)r;
    r = v_big * v_big; (void)r;
    prev = _clearfp();               /* returns the status before clearing */
    CHECK((prev & _SW_INEXACT) != 0);
    CHECK((prev & _SW_OVERFLOW) != 0);
    s = _statusfp();
    CHECK_EQ_U(s, 0);
    CHECK_EQ_U(_clearfp(), 0);
}
int main(void) { tc_begin(); TC_RUN(two_reads_equal); TC_RUN(accumulate); TC_RUN(only_clearfp_resets); return tc_end(); }
