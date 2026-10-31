/* TC
 * id:    fpe_controlfp_em_bits_roundtrip
 * area:  fpenv
 * funcs: _controlfp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _controlfp_s
 * note:  Toggle each exception mask bit; no FP op is executed while any exception is unmasked.
 */
#include "tc.h"
#include <float.h>

static unsigned int g_orig;
#define SAVE()    do { g_orig = _controlfp(0, 0); } while (0)
#define RESTORE() do { _clearfp(); _controlfp(g_orig, _MCW_RC | _MCW_EM); } while (0)

static void tog(unsigned int bit)
{
    unsigned int r;
    _clearfp();
    r = _controlfp(0, bit);                 /* clear bit = unmask */
    CHECK_EQ_U(r & bit, 0);
    CHECK_EQ_U(_controlfp(0, 0) & bit, 0);
    r = _controlfp(bit, bit);               /* set bit = mask */
    CHECK_EQ_U(r & bit, bit);
    CHECK_EQ_U(_controlfp(0, 0) & bit, bit);
    _clearfp();
}
TC_CASE(invalid) { SAVE(); tog(_EM_INVALID); RESTORE(); }
TC_CASE(zerodivide) { SAVE(); tog(_EM_ZERODIVIDE); RESTORE(); }
TC_CASE(overflow) { SAVE(); tog(_EM_OVERFLOW); RESTORE(); }
TC_CASE(underflow) { SAVE(); tog(_EM_UNDERFLOW); RESTORE(); }
TC_CASE(inexact) { SAVE(); tog(_EM_INEXACT); RESTORE(); }
TC_CASE(others_untouched) { unsigned int a, b; SAVE(); a = _controlfp(0, 0); _controlfp(0, _EM_OVERFLOW); b = _controlfp(0, 0); CHECK_EQ_U(b & ~(unsigned)_EM_OVERFLOW, a & ~(unsigned)_EM_OVERFLOW); RESTORE(); CHECK_EQ_U(_controlfp(0, 0) & 0x1F, g_orig & 0x1F); }
int main(void) { tc_begin(); TC_RUN(invalid); TC_RUN(zerodivide); TC_RUN(overflow); TC_RUN(underflow); TC_RUN(inexact); TC_RUN(others_untouched); return tc_end(); }
