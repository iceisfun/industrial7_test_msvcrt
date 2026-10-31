/* TC
 * id:    pff_neg_set_output_format_absent_ucrt_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _set_output_format _get_output_format
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_output_format / _get_output_format (UCRT: two-digit exponent is permanent)
 * note:  PROBE: ucrtbase does not export the _set_output_format setter (must-not-be-exported claim), while the getter stays exported and returns 0.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(setter_absent) { CHECK(tc_crt_proc("_set_output_format") == NULL); }
TC_CASE(getter_present) { CHECK(tc_crt_proc("_get_output_format") != NULL); }
TC_CASE(getter_zero)
{
    unsigned int (__cdecl *g)(void) = (unsigned int (__cdecl *)(void))tc_crt_proc("_get_output_format");
    if (!g) TC_SKIP("not exported");
    CHECK_EQ_U(g(), 0);
}

int main(void) { tc_begin(); TC_RUN(setter_absent); TC_RUN(getter_present); TC_RUN(getter_zero); return tc_end(); }
