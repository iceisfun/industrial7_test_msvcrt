/* TC
 * id:    ct_neg_probe_towctrans_bad_desc_probe
 * area:  ctype
 * funcs:
 * procs: towctrans
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.30.3.2.1; MS docs towctrans
 * note:  PROBE (isolated): towctrans with unknown descriptors 0 and 77 returns the argument and never calls the IPH.
 */
#include "tc.h"
#include <wchar.h>

typedef wint_t (__cdecl *tt_t)(wint_t, wint_t);
static tt_t p_tt;

TC_CASE(unknown_descs)
{
    volatile wint_t a = L'a', z = L'z';
    CHECK_EQ_U(p_tt(a, 0), L'a');
    CHECK_EQ_U(p_tt(z, 77), L'z');
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_tt = (tt_t)tc_crt_proc("towctrans");
    if (!p_tt) { tc_write("TC FAIL " TC_TEST_ID "::export missing\n"); return 1; }
    TC_RUN(unknown_descs);
    return tc_end();
}
