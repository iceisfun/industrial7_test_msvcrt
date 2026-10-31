/* TC
 * id:    fpe_controlfp_dn_save_default
 * area:  fpenv
 * funcs: _controlfp
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _controlfp_s (_MCW_DN, _DN_FLUSH, _DN_SAVE)
 * note:  Default is denormals saved.
 */
#include "tc.h"
#include <float.h>

static unsigned int g_orig;
#define SAVE()    do { g_orig = _controlfp(0, 0); } while (0)
#define RESTORE() do { _clearfp(); _controlfp(g_orig, _MCW_RC | _MCW_EM); } while (0)

static volatile double v_min = DBL_MIN, v_two = 2.0;
TC_CASE(field) { CHECK_EQ_U(_controlfp(0, 0) & _MCW_DN, _DN_SAVE); }
TC_CASE(denormal_result) { volatile double r = v_min / v_two; CHECK(r != 0.0); CHECK_EQ_U(tc_dbits(r), 0x0008000000000000ULL); }
int main(void) { tc_begin(); TC_RUN(field); TC_RUN(denormal_result); return tc_end(); }
