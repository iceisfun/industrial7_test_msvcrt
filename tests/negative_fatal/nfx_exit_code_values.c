/* TC
 * id:    nfx_exit_code_values
 * area:  negative_fatal
 * kind:  positive
 * funcs: exit _exit
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.22.4.4; MS docs exit, _exit
 * note:  Each case runs a child that terminates with the given status; parent reads GetExitCodeProcess.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>


static volatile int v_code;

static void run(const char *n, DWORD want)
{
    DWORD code = 0xdeadbeef; int ran = tc_spawn_self(n, &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, want);
}
TC_CASE(exit0)   { run("exit0", 0); }
TC_CASE(exit1)   { run("exit1", 1); }
TC_CASE(exit42)  { run("exit42", 42); }
TC_CASE(exit255) { run("exit255", 255); }
TC_CASE(_exit7)  { run("u7", 7); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'u') _exit(7);
        if (c[4] == '0') exit(0);
        if (c[4] == '1' && !c[5]) exit(1);
        if (c[4] == '4') exit(42);
        v_code = 255; exit(v_code);
    }
    TC_RUN(exit0); TC_RUN(exit1); TC_RUN(exit42); TC_RUN(exit255); TC_RUN(_exit7);
    return tc_end();
}
