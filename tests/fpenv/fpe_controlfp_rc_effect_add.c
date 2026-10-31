/* TC
 * id:    fpe_controlfp_rc_effect_add
 * area:  fpenv
 * funcs: _controlfp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _controlfp_s
 * note:  1.0 + tiny under each rounding mode (volatile operands; SSE2 honours MXCSR, x87 honours its CW).
 */
#include "tc.h"
#include <float.h>

static unsigned int g_orig;
#define SAVE()    do { g_orig = _controlfp(0, 0); } while (0)
#define RESTORE() do { _clearfp(); _controlfp(g_orig, _MCW_RC | _MCW_EM); } while (0)

static volatile double v_one = 1.0, v_tiny = 1e-20;
static double add(void) { volatile double r = v_one + v_tiny; return r; }
static const double next1 = 1.0000000000000002220446049250313;
TC_CASE(nearest) { SAVE(); _controlfp(_RC_NEAR, _MCW_RC); CHECK_BITS_D(add(), 1.0); RESTORE(); }
TC_CASE(up) { SAVE(); _controlfp(_RC_UP, _MCW_RC); CHECK(add() > 1.0); CHECK_BITS_D(add(), next1); RESTORE(); }
TC_CASE(down) { SAVE(); _controlfp(_RC_DOWN, _MCW_RC); CHECK_BITS_D(add(), 1.0); RESTORE(); }
TC_CASE(chop) { SAVE(); _controlfp(_RC_CHOP, _MCW_RC); CHECK_BITS_D(add(), 1.0); RESTORE(); }
TC_CASE(restore) { SAVE(); _controlfp(_RC_UP, _MCW_RC); RESTORE(); CHECK_BITS_D(add(), 1.0); }
int main(void) { tc_begin(); TC_RUN(nearest); TC_RUN(up); TC_RUN(down); TC_RUN(chop); TC_RUN(restore); return tc_end(); }
