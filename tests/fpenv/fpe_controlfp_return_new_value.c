/* TC
 * id:    fpe_controlfp_return_new_value
 * area:  fpenv
 * funcs: _controlfp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _controlfp_s
 * note:  Return value is the new control word.
 */
#include "tc.h"
#include <float.h>

static unsigned int g_orig;
#define SAVE()    do { g_orig = _controlfp(0, 0); } while (0)
#define RESTORE() do { _clearfp(); _controlfp(g_orig, _MCW_RC | _MCW_EM); } while (0)

TC_CASE(equals_query) { unsigned int r; SAVE(); r = _controlfp(_RC_DOWN, _MCW_RC); CHECK_EQ_U(r, _controlfp(0, 0)); r = _controlfp(_RC_UP, _MCW_RC); CHECK_EQ_U(r, _controlfp(0, 0)); RESTORE(); }
TC_CASE(reflects_rc) { SAVE(); CHECK_EQ_U(_controlfp(_RC_CHOP, _MCW_RC) & _MCW_RC, _RC_CHOP); CHECK_EQ_U(_controlfp(_RC_NEAR, _MCW_RC) & _MCW_RC, _RC_NEAR); RESTORE(); }
int main(void) { tc_begin(); TC_RUN(equals_query); TC_RUN(reflects_rc); return tc_end(); }
