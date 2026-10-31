/* TC
 * id:    gmb_controlfp_set_basic
 * area:  math
 * funcs: _controlfp_s _clearfp
 * procs: _set_controlfp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _controlfp_s (float.h _RC_* / _MCW_RC); _set_controlfp is not documented
 * note:  PROBE: _set_controlfp(new, mask) is assumed to behave like _controlfp (set bits of new selected by mask).
 *        Its return type is unknown, so the return value is ignored. The rounding mode is always restored.
 */
#include "tc.h"
#include <float.h>

typedef void (__cdecl *setcfp_t)(unsigned int, unsigned int);
static setcfp_t p_set;
static unsigned int rd(void) { unsigned int cw = 0; _controlfp_s(&cw, 0, 0); return cw; }

TC_CASE(exported) { CHECK(p_set != NULL); }
TC_CASE(rounding_modes)
{
    unsigned int orig, modes[4], i;
    if (!p_set) TC_SKIP("absent");
    orig = rd();
    modes[0] = _RC_CHOP; modes[1] = _RC_UP; modes[2] = _RC_DOWN; modes[3] = _RC_NEAR;
    for (i = 0; i < 4; i++) { p_set(modes[i], _MCW_RC); CHECK_EQ_U(rd() & _MCW_RC, modes[i]); }
    p_set(orig, _MCW_RC);
    CHECK_EQ_U(rd(), orig);
}
TC_CASE(zero_mask_no_change)
{
    unsigned int orig;
    if (!p_set) TC_SKIP("absent");
    orig = rd();
    p_set(_RC_CHOP, 0);
    CHECK_EQ_U(rd(), orig);
    p_set(~0u, 0);
    CHECK_EQ_U(rd(), orig);
}
TC_CASE(other_bits_preserved)
{
    unsigned int orig, now;
    if (!p_set) TC_SKIP("absent");
    orig = rd();
    p_set(_RC_UP, _MCW_RC);
    now = rd();
    CHECK_EQ_U(now & ~_MCW_RC, orig & ~_MCW_RC);
    p_set(orig, _MCW_RC);
    _clearfp();
}

int main(void)
{
    tc_begin();
    p_set = (setcfp_t)tc_crt_proc("_set_controlfp");
    TC_RUN(exported); TC_RUN(rounding_modes); TC_RUN(zero_mask_no_change); TC_RUN(other_bits_preserved);
    return tc_end();
}
