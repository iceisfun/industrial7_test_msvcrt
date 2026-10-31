/* TC
 * id:    fpe_controlfp_dn_flush_ucrt
 * area:  fpenv
 * funcs: _controlfp
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _controlfp_s (_MCW_DN, _DN_FLUSH, _DN_SAVE)
 * note:  _MCW_DN field readback and _DN_SAVE behaviour only (UCRT; old x64 CRTs are covered by fpe_dn_flush_win7_probe).
 *        The _DN_FLUSH result-flushing effect is in fpe_controlfp_dn_flush_effect_probe (docs do not say FTZ vs DAZ).
 */
#include "tc.h"
#include <float.h>

static unsigned int g_orig;
#define SAVE()    do { g_orig = _controlfp(0, 0); } while (0)
#define RESTORE() do { _clearfp(); _controlfp(g_orig, _MCW_RC | _MCW_EM); } while (0)

#define RESTORE_DN() do { _controlfp(g_orig, _MCW_DN); RESTORE(); } while (0)
static volatile double v_min = DBL_MIN, v_two = 2.0;
static double half(void) { volatile double r = v_min / v_two; return r; }
TC_CASE(query_flush) { SAVE(); _controlfp(_DN_FLUSH, _MCW_DN); CHECK_EQ_U(_controlfp(0, 0) & _MCW_DN, _DN_FLUSH); RESTORE_DN(); }
TC_CASE(restore_save) { SAVE(); _controlfp(_DN_FLUSH, _MCW_DN); _controlfp(_DN_SAVE, _MCW_DN); CHECK(half() != 0.0); CHECK_EQ_U(_controlfp(0, 0) & _MCW_DN, _DN_SAVE); RESTORE_DN(); }
int main(void) { tc_begin(); TC_RUN(query_flush); TC_RUN(restore_save); return tc_end(); }
