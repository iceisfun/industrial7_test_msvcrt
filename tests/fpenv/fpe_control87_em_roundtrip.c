/* TC
 * id:    fpe_control87_em_roundtrip
 * area:  fpenv
 * funcs: _control87
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _clearfp

 */
#include "tc.h"
#include <float.h>
#include <errno.h>

#define TC_EM5 0x1f /* the five IEEE exception mask bits; excludes x64-only _EM_DENORMAL */
static const unsigned int bits[5] = { _EM_INVALID, _EM_ZERODIVIDE, _EM_OVERFLOW, _EM_UNDERFLOW, _EM_INEXACT };

TC_CASE(unmask_remask_each)
{
    unsigned int orig = _control87(0, 0), r; int i;
    for (i = 0; i < 5; i++) {
        _clearfp();
        _control87(TC_EM5, TC_EM5);                 /* all masked */
        r = _control87(0, bits[i]);                 /* unmask just this */
        CHECK_EQ_U(r & bits[i], 0);
        CHECK_EQ_U(_control87(0, 0) & TC_EM5, TC_EM5 & ~bits[i]);
        r = _control87(bits[i], bits[i]);           /* re-mask */
        CHECK_EQ_U(r & bits[i], bits[i]);
        CHECK_EQ_U(_control87(0, 0) & TC_EM5, TC_EM5);
    }
    _control87(orig, TC_EM5);
}
TC_CASE(query_only)
{
    unsigned int a = _control87(0, 0), b = _control87(0, 0);
    CHECK_EQ_U(a, b);
    CHECK_EQ_U(a & TC_EM5, TC_EM5);   /* default: all exceptions masked */
}
int main(void) { tc_begin(); TC_RUN(unmask_remask_each); TC_RUN(query_only); return tc_end(); }
