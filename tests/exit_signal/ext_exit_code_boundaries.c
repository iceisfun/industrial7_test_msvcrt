/* TC
 * id:    ext_exit_code_boundaries
 * area:  exit_signal
 * funcs: exit
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   Windows process exit codes are 32-bit DWORD (GetExitCodeProcess)
 * note:  exit status is passed to ExitProcess without 8-bit truncation
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static void run(const char *n, DWORD want) { DWORD code = 0xabcd; int ran = tc_spawn_self(n, &code, 20000); CHECK(ran); CHECK_EQ_U(code, want); }
TC_CASE(e256)   { run("a256", 256); }
TC_CASE(e12345) { run("b12345", 0x12345); }
TC_CASE(eminus1){ run("cminus1", 0xFFFFFFFFu); }
TC_CASE(eintmin){ run("dintmin", 0x80000000u); }

int main(void)
{
    const char *c; volatile int v = 0;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        switch (c[0]) { case 'a': v = 256; break; case 'b': v = 0x12345; break; case 'c': v = -1; break; default: v = INT_MIN; }
        exit(v);
        return 77;
    }
    TC_RUN(e256); TC_RUN(e12345); TC_RUN(eminus1); TC_RUN(eintmin);
    return tc_end();
}
