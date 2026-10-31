/* TC
 * id:    pff_neg_fcvt_null_sign_probe
 * area:  stdio_printf
 * funcs:
 * procs: _fcvt
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fcvt
 * note:  Probes: _fcvt with a NULL sign output returns NULL via the invalid parameter handler (count >= 1)
 *        instead of crashing. Documentation of this validation is not explicit; errno not asserted.
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(char *, _fcvt, (double, int, int *, int *));

TC_CASE(null_out)
{
    int o = 0; char *r;
    if (!tc_p__fcvt || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0;
    r = tc_p__fcvt(1.5, 3, &o, NULL);
    CHECK(r == NULL); CHECK(tc_iph_count >= 1);
}

int main(void) { tc_begin(); TC_LOAD_PROC(_fcvt); TC_RUN(null_out); return tc_end(); }
