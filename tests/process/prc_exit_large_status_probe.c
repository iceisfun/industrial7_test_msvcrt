/* TC
 * id:    prc_exit_large_status_probe
 * area:  process
 * funcs: exit _exit
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs exit, _exit
 * note:  Probes that the status given to exit/_exit is passed unmodified (full DWORD) to ExitProcess, i.e. it is not
 *        truncated to 8 bits as on POSIX. Each case spawns a child 'e<n>' or 'u<n>'.
 */
#include "tc.h"
#include <stdlib.h>

static void child_main(const char *name)
{
    volatile int v = 0;
    if (tc_strequ(name, "e256")) exit(256);
    if (tc_strequ(name, "u65536")) _exit(65536);
    if (tc_strequ(name, "em1")) exit(-1);
    if (tc_strequ(name, "um2")) _exit(-2);
    (void)v;
}
static void roundtrip(const char *name, DWORD want)
{
    DWORD code = 0x55; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, want);
}
TC_CASE(exit_256)     { roundtrip("e256", 256); }
TC_CASE(_exit_65536)  { roundtrip("u65536", 65536); }
TC_CASE(exit_minus1)  { roundtrip("em1", 0xFFFFFFFFu); }
TC_CASE(_exit_minus2) { roundtrip("um2", 0xFFFFFFFEu); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { child_main(c); return 77; }
    TC_RUN(exit_256); TC_RUN(_exit_65536); TC_RUN(exit_minus1); TC_RUN(_exit_minus2);
    return tc_end();
}
