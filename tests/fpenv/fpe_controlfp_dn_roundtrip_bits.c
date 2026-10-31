/* TC
 * id:    fpe_controlfp_dn_roundtrip_bits
 * area:  fpenv
 * funcs: _controlfp
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _controlfp_s (_MCW_DN, _DN_FLUSH, _DN_SAVE)
 * note:  _MCW_DN field round trip (UCRT only; old CRTs in fpe_dn_flush_win7_probe).
 */
#include "tc.h"
#include <float.h>

static unsigned int g_orig;
#define SAVE()    do { g_orig = _controlfp(0, 0); } while (0)
#define RESTORE() do { _clearfp(); _controlfp(g_orig, _MCW_RC | _MCW_EM); } while (0)

TC_CASE(flush) { unsigned int a, b; SAVE(); a = _controlfp(0, 0); b = _controlfp(_DN_FLUSH, _MCW_DN); CHECK_EQ_U(b & _MCW_DN, _DN_FLUSH); CHECK_EQ_U(_controlfp(0, 0) & _MCW_DN, _DN_FLUSH); CHECK_EQ_U(b & ~(unsigned)_MCW_DN, a & ~(unsigned)_MCW_DN); _controlfp(g_orig, _MCW_DN); RESTORE(); }
TC_CASE(save) { unsigned int a, b; SAVE(); _controlfp(_DN_FLUSH, _MCW_DN); a = _controlfp(0, 0); b = _controlfp(_DN_SAVE, _MCW_DN); CHECK_EQ_U(b & _MCW_DN, _DN_SAVE); CHECK_EQ_U(b & ~(unsigned)_MCW_DN, a & ~(unsigned)_MCW_DN); _controlfp(g_orig, _MCW_DN); RESTORE(); }
int main(void) { tc_begin(); TC_RUN(flush); TC_RUN(save); return tc_end(); }
