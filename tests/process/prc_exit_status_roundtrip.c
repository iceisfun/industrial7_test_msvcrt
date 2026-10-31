/* TC
 * id:    prc_exit_status_roundtrip
 * area:  process
 * funcs: exit
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs exit (status returned to the OS); C11 7.22.4.4 only defines 0/EXIT_SUCCESS/EXIT_FAILURE
 * note:  Each case spawns this exe as child 'exit_<n>' which calls exit(n).
 */
#include "tc.h"
#include <stdlib.h>

static void child_main(const char *name)
{
    volatile int n = 0; const char *p = name + 5;
    while (*p >= '0' && *p <= '9') n = n * 10 + (*p++ - '0');
    exit(n);
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
TC_CASE(exit_failure)
{
    DWORD code = 0xffff; int ran = tc_spawn_self("exit_1", &code, 20000);
    volatile int ef = EXIT_FAILURE;
    CHECK(ran);
    CHECK_EQ_U(code, (DWORD)ef);
    CHECK_EQ_I(ef, 1);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { child_main(c); return 77; }
    TC_RUN(exit0); TC_RUN(exit1); TC_RUN(exit42); TC_RUN(exit255); TC_RUN(exit_failure);
    return tc_end();
}
