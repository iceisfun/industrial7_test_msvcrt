/* TC
 * id:    fpe_controlfp_rc_effect_div_order
 * area:  fpenv
 * funcs: _controlfp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _controlfp_s
 * note:  1.0/3.0 and -1.0/3.0 under each rounding mode.
 */
#include "tc.h"
#include <float.h>

static unsigned int g_orig;
#define SAVE()    do { g_orig = _controlfp(0, 0); } while (0)
#define RESTORE() do { _clearfp(); _controlfp(g_orig, _MCW_RC | _MCW_EM); } while (0)

static volatile double v_one = 1.0, v_neg = -1.0, v_three = 3.0;
static double q(volatile double *n, unsigned int rc) { volatile double r; _controlfp(rc, _MCW_RC); r = *n / v_three; return r; }
TC_CASE(ordering) { double d, n, u, c; SAVE(); d = q(&v_one, _RC_DOWN); n = q(&v_one, _RC_NEAR); u = q(&v_one, _RC_UP); c = q(&v_one, _RC_CHOP); RESTORE();
    CHECK(d <= n); CHECK(n <= u); CHECK(d < u); CHECK_BITS_D(c, d); }
TC_CASE(negative_chop) { double d, u, c; SAVE(); d = q(&v_neg, _RC_DOWN); u = q(&v_neg, _RC_UP); c = q(&v_neg, _RC_CHOP); RESTORE();
    CHECK(d < u); CHECK_BITS_D(c, u); }
int main(void) { tc_begin(); TC_RUN(ordering); TC_RUN(negative_chop); return tc_end(); }
