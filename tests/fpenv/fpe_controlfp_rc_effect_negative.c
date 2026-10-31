/* TC
 * id:    fpe_controlfp_rc_effect_negative
 * area:  fpenv
 * funcs: _controlfp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _controlfp_s
 * note:  Directed rounding on negative operands.
 */
#include "tc.h"
#include <float.h>

static unsigned int g_orig;
#define SAVE()    do { g_orig = _controlfp(0, 0); } while (0)
#define RESTORE() do { _clearfp(); _controlfp(g_orig, _MCW_RC | _MCW_EM); } while (0)

static volatile double v_m1 = -1.0, v_tiny = 1e-20;
static double sub(void) { volatile double r = v_m1 - v_tiny; return r; }
TC_CASE(up) { SAVE(); _controlfp(_RC_UP, _MCW_RC); CHECK_BITS_D(sub(), -1.0); RESTORE(); }
TC_CASE(down) { SAVE(); _controlfp(_RC_DOWN, _MCW_RC); CHECK(sub() < -1.0); RESTORE(); }
TC_CASE(chop) { SAVE(); _controlfp(_RC_CHOP, _MCW_RC); CHECK_BITS_D(sub(), -1.0); RESTORE(); }
int main(void) { tc_begin(); TC_RUN(up); TC_RUN(down); TC_RUN(chop); return tc_end(); }
