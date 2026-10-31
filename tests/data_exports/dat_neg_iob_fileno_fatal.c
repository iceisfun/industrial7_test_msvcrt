/* TC
 * id:    dat_neg_iob_fileno_fatal
 * area:  data_exports
 * kind:  negative
 * funcs: _fileno
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation": default handler terminates the process
 * note:  Probes the exact termination status (expected fast-fail 0xC0000409) of _fileno(NULL) with no handler installed.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(child_terminates_fastfail)
{
    DWORD code = 0; int ran = tc_spawn_self("filenonull", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { FILE *volatile p = NULL; _fileno((FILE *)p); return 77; }
    TC_RUN(child_terminates_fastfail);
    return tc_end();
}
