/* TC
 * id:    gmb_controlfp_neg_null_ptr_probe
 * area:  math
 * funcs: _controlfp_s
 * procs: _set_controlfp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _controlfp_s
 * note:  PROBE: whether a NULL currentControl pointer is accepted (return 0, no handler) or rejected (EINVAL + handler).
 *        Asserts only that the outcome is one of the two consistent possibilities and the control word is unchanged;
 *        also probes that _set_controlfp with bits like _PC_24 on x64 does not crash.
 */
#include "tc.h"
#include <float.h>
#include <errno.h>

typedef void (__cdecl *setcfp_t)(unsigned int, unsigned int);

TC_CASE(null_pointer_consistent)
{
    unsigned int before = 0, after = 0; int r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _controlfp_s(&before, 0, 0);
    tc_iph_count = 0;
    r = _controlfp_s(NULL, 0, 0);
    CHECK((r == 0 && tc_iph_count == 0) || (r == EINVAL && tc_iph_count == 1));
    _controlfp_s(&after, 0, 0);
    CHECK_EQ_U(after, before);
}
TC_CASE(set_pc_bits_no_crash)
{
    setcfp_t p = (setcfp_t)tc_crt_proc("_set_controlfp");
    unsigned int before = 0, after = 0;
    if (!p) TC_SKIP("absent");
    tc_iph_install();
    _controlfp_s(&before, 0, 0);
    tc_iph_count = 0;
    p(_PC_24, _MCW_PC);
    _controlfp_s(&after, 0, 0);
    p(before & _MCW_PC, _MCW_PC);
    _controlfp_s(&after, 0, 0);
    CHECK_EQ_U(after, before);
}

int main(void)
{
    tc_begin();
    TC_RUN(null_pointer_consistent); TC_RUN(set_pc_bits_no_crash);
    return tc_end();
}
