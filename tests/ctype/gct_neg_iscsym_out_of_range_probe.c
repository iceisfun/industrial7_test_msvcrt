/* TC
 * id:    gct_neg_iscsym_out_of_range_probe
 * area:  ctype
 * funcs:
 * procs: __iscsym __iscsymf
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs __iscsym, __iscsymf
 * note:  PROBE: out-of-range arguments (256, -2, 0x1000) are believed to return 0 without calling the invalid
 *        parameter handler (cases iscsym_oor/iscsymf_oor); errno untouched is a separate guess (errno_untouched). Docs call
 *        out-of-range input undefined, so all are guesses.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *ic_t)(int);
static ic_t p_sym, p_symf;
static volatile int v256 = 256, vm2 = -2, v1000 = 0x1000;

TC_CASE(iscsym_oor)
{
    int b; errno = 0; b = tc_iph_count;
    CHECK_EQ_I(p_sym(v256), 0); CHECK_EQ_I(p_sym(vm2), 0); CHECK_EQ_I(p_sym(v1000), 0);
    CHECK_EQ_I(tc_iph_count, b);
}
TC_CASE(iscsymf_oor)
{
    int b; errno = 0; b = tc_iph_count;
    CHECK_EQ_I(p_symf(v256), 0); CHECK_EQ_I(p_symf(vm2), 0); CHECK_EQ_I(p_symf(v1000), 0);
    CHECK_EQ_I(tc_iph_count, b);
}
TC_CASE(errno_untouched)
{
    errno = 0; (void)p_sym(v256); (void)p_sym(vm2); (void)p_symf(v1000); (void)p_symf(vm2);
    CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin();
    p_sym = (ic_t)tc_crt_proc("__iscsym"); p_symf = (ic_t)tc_crt_proc("__iscsymf");
    if (!tc_iph_install()) return 2;
    if (!p_sym || !p_symf) { tc_write("TC FAIL missing export\n"); return 1; }
    TC_RUN(iscsym_oor); TC_RUN(iscsymf_oor); TC_RUN(errno_untouched);
    return tc_end();
}
