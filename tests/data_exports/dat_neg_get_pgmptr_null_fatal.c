/* TC
 * id:    dat_neg_get_pgmptr_null_fatal
 * area:  data_exports
 * kind:  negative
 * funcs: _get_pgmptr
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Parameter Validation / _set_invalid_parameter_handler: default handler terminates the program
 * note:  Child process calls _get_pgmptr(NULL) with the default invalid-parameter handler. Exact status is probed in dat_neg_get_pgmptr_null_fatal_status_probe.
 */
#include "tc.h"

#include <stdlib.h>

TC_CASE(child_terminates_abnormally)
{
    DWORD code = 0; int ran = tc_spawn_self("nullptr", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { tc_iph_default(); _get_pgmptr(NULL); return 77; }
    TC_RUN(child_terminates_abnormally);
    return tc_end();
}
