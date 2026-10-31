/* TC
 * id:    fpe_controlfp_dn_flush_effect_probe
 * area:  fpenv
 * funcs: _controlfp
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _control87, _controlfp, _controlfp_s (_DN_FLUSH)
 * note:  PROBE: whether _DN_FLUSH maps to MXCSR FTZ (denormal RESULTS flushed to zero) on x64 UCRT.
 *        DBL_MIN / 2 is a denormal result from normal operands; expected 0.0 if _DN_FLUSH = FTZ. Docs only say
 *        "flush denormal values to zero", so this is kept apart from the documented DN tests.
 */
#include "tc.h"
#include <float.h>

static volatile double v_min = DBL_MIN, v_two = 2.0;

TC_CASE(flush_result_zero)
{
    unsigned int old = _controlfp(0, 0);
    volatile double r;
    _controlfp(_DN_FLUSH, _MCW_DN);
    r = v_min / v_two;
    _controlfp(old & _MCW_DN, _MCW_DN);
    _clearfp();
    CHECK(r == 0.0);
}
int main(void) { tc_begin(); TC_RUN(flush_result_zero); return tc_end(); }
