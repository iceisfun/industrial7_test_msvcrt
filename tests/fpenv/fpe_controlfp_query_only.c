/* TC
 * id:    fpe_controlfp_query_only
 * area:  fpenv
 * funcs: _controlfp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _controlfp_s
 * note:  mask 0 queries without modifying.
 */
#include "tc.h"
#include <float.h>

static unsigned int g_orig;
#define SAVE()    do { g_orig = _controlfp(0, 0); } while (0)
#define RESTORE() do { _clearfp(); _controlfp(g_orig, _MCW_RC | _MCW_EM); } while (0)

TC_CASE(query_twice) { unsigned int a = _controlfp(0, 0), b = _controlfp(0, 0); CHECK_EQ_U(a, b); }
TC_CASE(all_ones_new_mask0) { unsigned int a, b; SAVE(); a = _controlfp(0, 0); b = _controlfp(0xFFFFFFFFu, 0); CHECK_EQ_U(a, b); CHECK_EQ_U(_controlfp(0, 0), a); RESTORE(); }
TC_CASE(rc_up_mask0) { unsigned int a, b; SAVE(); a = _controlfp(0, 0); b = _controlfp(_RC_UP, 0); CHECK_EQ_U(b & _MCW_RC, a & _MCW_RC); CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, a & _MCW_RC); RESTORE(); }
int main(void) { tc_begin(); TC_RUN(query_twice); TC_RUN(all_ones_new_mask0); TC_RUN(rc_up_mask0); return tc_end(); }
