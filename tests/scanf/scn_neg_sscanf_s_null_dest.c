/* TC
 * id:    scn_neg_sscanf_s_null_dest
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs scanf_s / Parameter validation: a NULL destination pointer is an invalid parameter
 * note:  Probes: sscanf_s %s with NULL destination and size 8 runs the invalid-parameter handler and does not return 1.
 */
#include "tc.h"
#include <stdio.h>

static volatile const char *v;
static char *volatile np;
TC_CASE(null_dest) { int n; tc_iph_install(); tc_iph_count = 0; v = "abc"; n = sscanf_s((const char *)v, "%s", np, (unsigned)8); CHECK(n != 1); CHECK(tc_iph_count >= 1); }
int main(void) { tc_begin(); TC_RUN(null_dest); return tc_end(); }
