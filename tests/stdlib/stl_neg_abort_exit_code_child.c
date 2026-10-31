/* TC
 * id:    stl_neg_abort_exit_code_child
 * area:  stdlib
 * kind:  negative
 * funcs: abort _set_abort_behavior raise signal _exit
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs abort ("returns exit code 3" when the abort message / Watson report is suppressed); signal/raise (SIGABRT default terminates with exit code 3)
 * note:  Fatal paths run in child processes. Children clear _WRITE_ABORT_MSG and _CALL_REPORTFAULT first
 *        (tc_begin does it too) so that no dialog or WER report is produced.
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>
#include <process.h>

static void __cdecl h55(int sig) { (void)sig; _exit(55); }

static int child_main(const char *c)
{
    _set_abort_behavior(0, 3);
    if (c[0] == 'a' && c[1] == 'b') abort();                       /* "abort" */
    if (c[0] == 'r') { raise(SIGABRT); return 77; }                /* "raise": default action terminates */
    if (c[0] == 'h') { signal(SIGABRT, h55); abort(); return 77; } /* "handler" */
    return 78;
}
static DWORD run(const char *n) { DWORD code = 0xffff; CHECK(tc_spawn_self(n, &code, 20000)); return code; }

TC_CASE(abort_exit3) { CHECK_EQ_U(run("abort"), 3); }
TC_CASE(raise_sigabrt_exit3) { CHECK_EQ_U(run("raise"), 3); }
TC_CASE(handler_exit55) { CHECK_EQ_U(run("handler"), 55); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(abort_exit3); TC_RUN(raise_sigabrt_exit3); TC_RUN(handler_exit55);
    return tc_end();
}
