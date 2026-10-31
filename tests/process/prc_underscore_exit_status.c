/* TC
 * id:    prc_underscore_exit_status
 * area:  process
 * funcs: _exit
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _exit (terminates without atexit handlers; status passed to the process)
 * note:  Only 0..255 asserted here (see prc_exit_large_status_probe). Each case spawns this exe as child 'exit_<n>' which calls _exit(n).
 */
#include "tc.h"
#include <stdlib.h>

static void child_main(const char *name)
{
    volatile int n = 0; const char *p = name + 5;
    while (*p >= '0' && *p <= '9') n = n * 10 + (*p++ - '0');
    _exit(n);
}
static void roundtrip(const char *name, DWORD want)
{
    DWORD code = 0xffff; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, want);
}
TC_CASE(exit0)   { roundtrip("exit_0", 0); }
TC_CASE(exit1)   { roundtrip("exit_1", 1); }
TC_CASE(exit42)  { roundtrip("exit_42", 42); }
TC_CASE(exit255) { roundtrip("exit_255", 255); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { child_main(c); return 77; }
    TC_RUN(exit0); TC_RUN(exit1); TC_RUN(exit42); TC_RUN(exit255);
    return tc_end();
}
