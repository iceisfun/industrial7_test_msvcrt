/* TC
 * id:    fpe_neg_controlfp_s_bad_mask_win7_probe
 * area:  fpenv
 * funcs: 
 * procs: _controlfp_s
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _controlfp_s (returns EINVAL for an invalid mask, calls the invalid parameter handler)
 * note:  Probe: documented for the UCRT; assumed identical on Windows 7 era CRTs. _controlfp_s resolved dynamically (import libs lack it for old CRTs). Return value and handler count are in separate cases.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>

TC_DECL_PROC(int, _controlfp_s, (unsigned *, unsigned, unsigned));

TC_CASE(return_value)
{
    unsigned c = 0x12345678, w0 = 0, w1 = 0;
    if (!tc_p__controlfp_s) TC_SKIP("_controlfp_s not exported");
    if (!tc_iph_install()) TC_SKIP("no iph api (default handler may be fatal)");
    tc_p__controlfp_s(&w0, 0, 0);
    CHECK_EQ_I(tc_p__controlfp_s(&c, 0, 0x00004000), EINVAL);
    tc_p__controlfp_s(&w1, 0, 0);
    CHECK_EQ_U(w1, w0);
}
TC_CASE(iph_count)
{
    unsigned c = 0;
    if (!tc_p__controlfp_s) TC_SKIP("_controlfp_s not exported");
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0;
    tc_p__controlfp_s(&c, 0, 0x00004000);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_controlfp_s); TC_RUN(return_value); TC_RUN(iph_count); return tc_end(); }
