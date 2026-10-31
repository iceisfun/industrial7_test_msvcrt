/* TC
 * id:    prc_neg_abort_exit_code
 * area:  process
 * funcs: abort
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs abort (exit code 3 when message/Watson are suppressed)
 * note:  raise(SIGABRT) and _exit full-DWORD status are covered elsewhere (exit_signal/, prc_exit_large_status_probe).
 *        Fatal paths run in child processes. tc_begin() in the child clears the abort message box / Watson flags.
 */
#include "tc.h"
#include <process.h>
#include <stdlib.h>

static int child_main(const char *n)
{
    if (n[0] == 'a') abort();
    return 77;
}
static DWORD run(const char *n) { DWORD c = 0; if (!tc_spawn_self(n, &c, 20000)) return 0xdead0002; return c; }

TC_CASE(abort_exit_3) { CHECK_EQ_U(run("abort"), 3); }

int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(abort_exit_3);
    return tc_end();
}
