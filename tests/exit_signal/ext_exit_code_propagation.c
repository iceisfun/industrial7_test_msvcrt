/* TC
 * id:    ext_exit_code_propagation
 * area:  exit_signal
 * funcs: exit
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.22.4.4; MS docs exit (status returned to host)
 * note:  one child name per value
 */
#include "tc.h"
#include <stdlib.h>

static void run(const char *n, DWORD want) { DWORD code = 0xabcd; int ran = tc_spawn_self(n, &code, 20000); CHECK(ran); CHECK_EQ_U(code, want); }
TC_CASE(e0)   { run("e0", 0); }
TC_CASE(e1)   { run("e1", 1); }
TC_CASE(e42)  { run("e42", 42); }
TC_CASE(e255) { run("e255", 255); }
TC_CASE(efail){ run("efail", 1); }

int main(void)
{
    const char *c; volatile int v = 0;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[1] == '0') v = 0; else if (c[1] == '1') v = 1; else if (c[1] == '4') v = 42;
        else if (c[1] == '2') v = 255; else v = EXIT_FAILURE;
        exit(v);
        return 77;
    }
    TC_RUN(e0); TC_RUN(e1); TC_RUN(e42); TC_RUN(e255); TC_RUN(efail);
    return tc_end();
}
