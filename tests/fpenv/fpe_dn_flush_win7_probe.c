/* TC
 * id:    fpe_dn_flush_win7_probe
 * area:  fpenv
 * funcs: _controlfp
 * crt:   msvcr120 msvcr110 msvcr100
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _control87, _controlfp, _clearfp
 * note:  PROBE: whether old x64 CRTs implement _MCW_DN/_DN_FLUSH (MXCSR FZ) via _controlfp; also whether the readback reports _DN_FLUSH.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>

static volatile double v_min = DBL_MIN, v_two = 2.0;

TC_CASE(flush_effect)
{
    unsigned int old = _controlfp(0, 0);
    volatile double r;
    _controlfp(_DN_FLUSH, _MCW_DN);
    r = v_min / v_two;
    _controlfp(old & _MCW_DN, _MCW_DN);
    CHECK_BITS_D(r, 0.0);
    _clearfp();
}
TC_CASE(flush_readback)
{
    unsigned int old = _controlfp(0, 0), now;
    _controlfp(_DN_FLUSH, _MCW_DN);
    now = _controlfp(0, 0);
    _controlfp(old & _MCW_DN, _MCW_DN);
    CHECK_EQ_U(now & _MCW_DN, _DN_FLUSH);
}
int main(void) { tc_begin(); TC_RUN(flush_effect); TC_RUN(flush_readback); return tc_end(); }
