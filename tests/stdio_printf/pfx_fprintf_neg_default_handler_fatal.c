/* TC
 * id:    pfx_fprintf_neg_default_handler_fatal
 * area:  stdio_printf
 * funcs: fprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation: default handler terminates the process
 * note:  Probes: fprintf(NULL, ...) with the default invalid-parameter handler terminates the process; exact status expected to be fast-fail 0xC0000409 on ucrtbase.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nullstream", &code, 20000);
    CHECK(ran);
    CHECK(code != 0 && code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) { volatile FILE *nf = NULL; fprintf((FILE *)nf, "x"); return 77; }
    TC_RUN(child_terminates);
    return tc_end();
}
