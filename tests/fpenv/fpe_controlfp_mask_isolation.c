/* TC
 * id:    fpe_controlfp_mask_isolation
 * area:  fpenv
 * funcs: _controlfp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _controlfp_s
 * note:  Only bits in mask change.
 */
#include "tc.h"
#include <float.h>

static unsigned int g_orig;
#define SAVE()    do { g_orig = _controlfp(0, 0); } while (0)
#define RESTORE() do { _clearfp(); _controlfp(g_orig, _MCW_RC | _MCW_EM); } while (0)

TC_CASE(rc_leaves_em) { unsigned int a, b; SAVE(); a = _controlfp(0, 0); b = _controlfp(_RC_UP, _MCW_RC); CHECK_EQ_U(b & 0x1F, a & 0x1F); RESTORE(); }
TC_CASE(em_leaves_rc) { unsigned int a, b; SAVE(); _controlfp(_RC_DOWN, _MCW_RC); a = _controlfp(0, 0); _clearfp(); b = _controlfp(0, _EM_INEXACT); CHECK_EQ_U(b & _MCW_RC, a & _MCW_RC); CHECK_EQ_U(b & _MCW_RC, _RC_DOWN); _controlfp(_EM_INEXACT, _EM_INEXACT); RESTORE(); }
TC_CASE(outside_mask_ignored) { unsigned int a, b; SAVE(); a = _controlfp(0, 0); b = _controlfp(0xFFFFFFFFu, _MCW_RC); CHECK_EQ_U(b & _MCW_RC, _RC_CHOP); CHECK_EQ_U(b & 0x1F, a & 0x1F); RESTORE(); }
int main(void) { tc_begin(); TC_RUN(rc_leaves_em); TC_RUN(em_leaves_rc); TC_RUN(outside_mask_ignored); return tc_end(); }
