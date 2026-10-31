/* TC
 * id:    fpe_controlfp_rc_roundtrip
 * area:  fpenv
 * funcs: _controlfp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _controlfp_s
 * note:  set each rounding mode via _MCW_RC.
 */
#include "tc.h"
#include <float.h>

static unsigned int g_orig;
#define SAVE()    do { g_orig = _controlfp(0, 0); } while (0)
#define RESTORE() do { _clearfp(); _controlfp(g_orig, _MCW_RC | _MCW_EM); } while (0)

static void one(unsigned int rc)
{
    unsigned int r = _controlfp(rc, _MCW_RC);
    CHECK_EQ_U(r & _MCW_RC, rc);
    CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, rc);
}
TC_CASE(down) { SAVE(); one(_RC_DOWN); RESTORE(); }
TC_CASE(up)   { SAVE(); one(_RC_UP); RESTORE(); }
TC_CASE(chop) { SAVE(); one(_RC_CHOP); RESTORE(); }
TC_CASE(near_) { SAVE(); one(_RC_DOWN); one(_RC_NEAR); RESTORE(); }
TC_CASE(restored) { unsigned int o; SAVE(); o = g_orig; one(_RC_UP); RESTORE(); CHECK_EQ_U(_controlfp(0, 0) & (_MCW_RC | 0x1F), o & (_MCW_RC | 0x1F)); }
int main(void) { tc_begin(); TC_RUN(down); TC_RUN(up); TC_RUN(chop); TC_RUN(near_); TC_RUN(restored); return tc_end(); }
