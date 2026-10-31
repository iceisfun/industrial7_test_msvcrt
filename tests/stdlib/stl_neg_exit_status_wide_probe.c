/* TC
 * id:    stl_neg_exit_status_wide_probe
 * area:  stdlib
 * kind:  negative
 * funcs: exit _exit
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs exit, _exit (status passed to the process exit code); Win32 GetExitCodeProcess returns a 32-bit DWORD
 * note:  PROBES that exit/_exit pass the full 32-bit int unchanged to ExitProcess (no truncation to 8 bits as on POSIX).
 */
#include "tc.h"
#include <stdlib.h>

static int child_main(const char *c)
{
    volatile int v;
    if (c[0] == 'a') { v = 256; exit(v); }
    if (c[0] == 'b') { v = -1; _exit(v); }
    if (c[0] == 'c') { v = 0x12345678; exit(v); }
    return 71;
}
static void run(const char *n, DWORD want) { DWORD code = 0; CHECK(tc_spawn_self(n, &code, 15000)); CHECK_EQ_U(code, want); }
TC_CASE(exit_256) { run("a", 256); }
TC_CASE(underscore_exit_minus1) { run("b", 0xFFFFFFFFu); }
TC_CASE(exit_big) { run("c", 0x12345678u); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(exit_256); TC_RUN(underscore_exit_minus1); TC_RUN(exit_big);
    return tc_end();
}
