/* TC
 * id:    err_neg_default_handler_fatal
 * area:  errno_param
 * kind:  negative
 * funcs: strcpy_s
 * crt:   >=140
 * tier:  p0
 * conf:  observed
 * ref:   MS docs "Parameter validation": without a handler an invalid parameter terminates the process
 * note:  Observed on Win10 22H2 19045 ucrtbase: with the CRT's default invalid-parameter handler restored in the child
 *        (tc_iph_default(); the mingw startup installs a no-op handler that made the child just return 77) strcpy_s(d, 0, "x")
 *        ends the process with STATUS_STACK_BUFFER_OVERRUN 0xC0000409 (fast-fail). Stable over repeated runs. Fatal path runs
 *        in a child process.
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
    if ((c = tc_child_name()) != NULL) { char d[2]; volatile int one = 1; (void)one; tc_iph_default(); strcpy_s(d, 0, "x"); return 77; /* reached only if the CRT did not terminate */ }
    TC_RUN(child_terminates);
    return tc_end();
}
