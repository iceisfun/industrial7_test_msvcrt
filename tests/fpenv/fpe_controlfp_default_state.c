/* TC
 * id:    fpe_controlfp_default_state
 * area:  fpenv
 * funcs: _controlfp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _controlfp_s
 * note:  Default FP control word at process start; only EM and RC checked (PC/DN not meaningful on x64).
 */
#include "tc.h"
#include <float.h>

static unsigned int g_orig;
#define SAVE()    do { g_orig = _controlfp(0, 0); } while (0)
#define RESTORE() do { _clearfp(); _controlfp(g_orig, _MCW_RC | _MCW_EM); } while (0)

TC_CASE(em_all_masked) { unsigned int cw = _controlfp(0, 0); CHECK_EQ_U(cw & 0x1F, 0x1F); CHECK_EQ_U(cw & _MCW_EM & 0x1F, _EM_INVALID | _EM_ZERODIVIDE | _EM_OVERFLOW | _EM_UNDERFLOW | _EM_INEXACT); }
TC_CASE(rc_near) { unsigned int cw = _controlfp(0, 0); CHECK_EQ_U(cw & _MCW_RC, _RC_NEAR); }
TC_CASE(query_twice) { unsigned int a = _controlfp(0, 0), b = _controlfp(0, 0); CHECK_EQ_U(a, b); }
int main(void) { tc_begin(); TC_RUN(em_all_masked); TC_RUN(rc_near); TC_RUN(query_twice); return tc_end(); }
