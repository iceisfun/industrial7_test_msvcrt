/* TC
 * id:    fpe_fpreset_clears_status
 * area:  fpenv
 * funcs: _statusfp _clearfp
 * procs: _fpreset
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _fpreset
 * note:  PROBE: _fpreset also clears the sticky exception status flags (FNINIT clears x87 flags; whether the SSE MXCSR flags are cleared on x64 is what is probed).
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

static volatile double v_one = 1.0, v_three = 3.0;
typedef void (__cdecl *fpreset_t)(void);

TC_CASE(clears_inexact)
{
    fpreset_t p = (fpreset_t)tc_crt_proc("_fpreset");
    volatile double r;
    if (!p) TC_SKIP("no _fpreset");
    _clearfp();
    r = v_one / v_three; (void)r;
    CHECK((_statusfp() & _SW_INEXACT) != 0);
    p();
    CHECK_EQ_U(_statusfp(), 0);
    _clearfp();
}
int main(void) { tc_begin(); TC_RUN(clears_inexact); return tc_end(); }
