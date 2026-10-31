/* TC
 * id:    fpe_control87_equiv_controlfp
 * area:  fpenv
 * funcs: _control87 _controlfp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _control87, _controlfp, _clearfp
 * note:  controlfp is the portable variant; differences exist only on x86 (denormal/EM semantics). On x64 both are expected to agree for RC and EM.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>

#define TC_EM5 0x1f /* excludes x64-only _EM_DENORMAL, which _controlfp/_control87 may treat differently */
TC_CASE(same_query) { CHECK_EQ_U(_control87(0, 0) & (_MCW_RC | TC_EM5), _controlfp(0, 0) & (_MCW_RC | TC_EM5)); }
TC_CASE(rc_cross)
{
    unsigned int orig = _controlfp(0, 0);
    _controlfp(_RC_UP, _MCW_RC);
    CHECK_EQ_U(_control87(0, 0) & _MCW_RC, _RC_UP);
    _control87(_RC_DOWN, _MCW_RC);
    CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, _RC_DOWN);
    _controlfp(orig, _MCW_RC);
}
TC_CASE(em_cross)
{
    unsigned int orig = _controlfp(0, 0);
    _clearfp();
    _controlfp(0, _EM_INEXACT);
    CHECK_EQ_U(_control87(0, 0) & _EM_INEXACT, 0);
    _control87(_EM_INEXACT, _EM_INEXACT);
    CHECK_EQ_U(_controlfp(0, 0) & _EM_INEXACT, _EM_INEXACT);
    _controlfp(orig, TC_EM5);
}
int main(void) { tc_begin(); TC_RUN(same_query); TC_RUN(rc_cross); TC_RUN(em_cross); return tc_end(); }
