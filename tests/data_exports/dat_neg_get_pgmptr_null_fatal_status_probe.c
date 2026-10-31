/* TC
 * id:    dat_neg_get_pgmptr_null_fatal_status_probe
 * area:  data_exports
 * kind:  negative
 * funcs: _get_pgmptr
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs Parameter validation: default handler terminates the process
 * note:  Child process calls _get_pgmptr(NULL) with the default invalid-parameter handler. PROBE: exact status (expected 0xC0000409 (__fastfail, when PF_FASTFAIL_AVAILABLE) or 0xC0000417 (STATUS_INVALID_CRUNTIME_PARAMETER, older OS without fastfail); either accepted on every CRT).
 */
#include "tc.h"

#include <stdlib.h>

TC_CASE(child_status_fastfail_or_crt_param)
{
    DWORD code = 0; int ran = tc_spawn_self("nullptr", &code, 20000);
    CHECK(ran);
    CHECK(code == TC_STATUS_FASTFAIL || code == TC_STATUS_INVALID_CRT_PARAM);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { _get_pgmptr(NULL); return 77; }
    TC_RUN(child_status_fastfail_or_crt_param);
    return tc_end();
}
