/* TC
 * id:    gsj_neg_free_base_null_noop
 * area:  exit_signal
 * kind:  negative
 * funcs:
 * procs: _free_base
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs free: "If memblock is NULL, the pointer is ignored and free immediately returns"
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(void, _free_base, (void *));
static void *volatile v_null;

TC_CASE(single_null)
{
    if (!tc_p__free_base) TC_SKIP("no _free_base");
    tc_iph_count = 0; errno = 0;
    tc_p__free_base(v_null);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(errno, 0);
}
TC_CASE(repeated_null)
{
    int i;
    if (!tc_p__free_base) TC_SKIP("no _free_base");
    tc_iph_count = 0; errno = 0;
    for (i = 0; i < 3; i++) tc_p__free_base(v_null);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_free_base);
    TC_RUN(single_null); TC_RUN(repeated_null);
    return tc_end();
}
