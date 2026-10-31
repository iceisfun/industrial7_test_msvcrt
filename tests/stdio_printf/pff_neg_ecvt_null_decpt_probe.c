/* TC
 * id:    pff_neg_ecvt_null_decpt_probe
 * area:  stdio_printf
 * funcs:
 * procs: _ecvt
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _ecvt
 * note:  Probes: _ecvt with a NULL decpt output returns NULL via the invalid parameter handler (count >= 1)
 *        instead of crashing. Documentation of this validation is not explicit; errno not asserted.
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(char *, _ecvt, (double, int, int *, int *));

TC_CASE(null_out)
{
    int o = 0; char *r;
    if (!tc_p__ecvt || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0;
    r = tc_p__ecvt(1.5, 3, NULL, &o);
    CHECK(r == NULL); CHECK(tc_iph_count >= 1);
}

int main(void) { tc_begin(); TC_LOAD_PROC(_ecvt); TC_RUN(null_out); return tc_end(); }
