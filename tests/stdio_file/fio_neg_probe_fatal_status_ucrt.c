/* TC
 * id:    fio_neg_probe_fatal_status_ucrt
 * area:  stdio_file
 * funcs: fopen
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Parameter validation"; fopen
 * note:  Probes the exact termination status of the UCRT default invalid-parameter path for fopen(NULL,"r"): expected
 *        0xC0000409 (STATUS_STACK_BUFFER_OVERRUN, FAST_FAIL_INVALID_ARG). Docs only promise termination.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(fopen_null_status)
{
    DWORD code = 0; int ran = tc_spawn_self("fopennull", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) tc_iph_default();
    if (tc_child_name()) { const char *volatile p = NULL; FILE *f = fopen(p, "r"); if (f) fclose(f); return 77; }
    TC_RUN(fopen_null_status);
    return tc_end();
}
