/* TC
 * id:    fio_neg_probe_remove_null_fatal
 * area:  stdio_file
 * funcs: remove
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Parameter validation"; remove
 * note:  Probes that remove(NULL) with the default invalid-parameter handler terminates the process abnormally on UCRT
 *        (the remove docs do not state a NULL check explicitly; older CRTs may just return -1 with errno set, so >=140 only).
 *        Runs in a child; the child returns 77 only if remove() returned.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(remove_null_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("rmnull", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) { const char *volatile p = NULL; remove(p); return 77; }
    TC_RUN(remove_null_terminates);
    return tc_end();
}
