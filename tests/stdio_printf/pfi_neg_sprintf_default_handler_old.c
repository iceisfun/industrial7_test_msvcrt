/* TC
 * id:    pfi_neg_sprintf_default_handler_old
 * area:  stdio_printf
 * funcs: sprintf
 * procs: 
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation (VS2005+): invalid parameter terminates the process
 * note:  PROBE: exact exit code (0xC0000417 vs 0xC0000409 vs 3) unknown; only asserts the child did not return 77 and the spawn ran.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(null_buffer_abnormal_exit)
{
    DWORD code = 0; int ran = tc_spawn_self("nullbuf", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char *volatile p = NULL; volatile int v = 1;
        sprintf(p, "%d", v);
        return 77;
    }
    TC_RUN(null_buffer_abnormal_exit);
    return tc_end();
}
