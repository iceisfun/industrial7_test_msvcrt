/* TC
 * id:    err_neg_default_handler_fatal
 * area:  errno_param
 * kind:  negative
 * funcs: strcpy_s
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation": without a handler an invalid parameter terminates the process
 * note:  Probes the exact termination status of the UCRT default invalid-parameter path. Expected to be a fast-fail
 *        (0xC0000409) on ucrtbase; the docs only promise termination, so the exact code is what we want measured on real Windows.
 *        Fatal path runs in a child process (see tc_spawn_self).
 */
#include "tc.h"
#include <string.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("badcopy", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);                       /* documented: abnormal termination */
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);   /* probe: exact status */
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { char d[2]; volatile int one = 1; (void)one; strcpy_s(d, 0, "x"); return 77; /* reached only if the CRT did not terminate */ }
    TC_RUN(child_terminates);
    return tc_end();
}
