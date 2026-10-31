/* TC
 * id:    pfm_2_len_h_p_e_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.1 (h with p, e: undefined)
 * note:  PROBE: on ucrtbase the h modifier is ignored for %p and %e: output equals the unmodified conversion, no invalid-parameter call.
 *        (%hn / %hhn size semantics are covered by pfp_n_enabled_sizes and pfp_n_hhn_probe; %hZ is unsupported on ucrt.)
 */
#include "tc.h"
#include <stdio.h>

static char b[64], p[64];
TC_CASE(hp) { static int obj; void * volatile q = &obj; tc_iph_count = 0; sprintf(b, "%hp", q); sprintf(p, "%p", q); CHECK_STR(b, p); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(he) { volatile double d = 1.5; tc_iph_count = 0; sprintf(b, "%he", d); CHECK_STR(b, "1.500000e+00"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(hp); TC_RUN(he);
    return tc_end();
}
