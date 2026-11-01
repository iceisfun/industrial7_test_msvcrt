/* TC
 * id:    nfx_exit_onexit_many_handlers_probe
 * area:  negative_fatal
 * kind:  positive
 * funcs:  exit _exit
 * procs: _crt_atexit _onexit
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   C11 7.22.4.2; MS docs _onexit
 * note:  D: ucrtbase.dll does not export _onexit (NULL from GetProcAddress, old code crashed 0xc0000005), so registration goes through tc_reg_atexit() (tc.h): the real export _crt_atexit where present, else _onexit (legacy CRTs).
 *        Observed on ucrtbase Win10 22H2: the table grows past 32 (33, 64, 129, 1024, 5000 handlers all run). Stable.
 *        Legacy CRTs keep the original _onexit coverage [unverified on the reference].
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>



static volatile int count, want_n;
static void __cdecl hc(void) { count++; }
/* registered first => runs last */
static void __cdecl hfinal(void) { _exit(count == want_n - 1 ? 0 : 1); }

static void run(const char *name)
{
    DWORD code = 0xdead; int ran = tc_spawn_self(name, &code, 30000);
    CHECK(ran); CHECK_EQ_U(code, 0);
}
TC_CASE(n33)   { run("n33"); }
TC_CASE(n64)   { run("n64"); }
TC_CASE(n129)  { run("n129"); }
TC_CASE(n1024) { run("n1024"); }
TC_CASE(n5000) { run("n5000"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        int i, n = 0;
        for (c++; *c; c++) n = n * 10 + (*c - '0');
        want_n = n;
        if (tc_reg_atexit(hfinal)) _exit(60);
        for (i = 1; i < n; i++) if (tc_reg_atexit(hc)) _exit(61);
        exit(2);
    }
    TC_RUN(n33); TC_RUN(n64); TC_RUN(n129); TC_RUN(n1024); TC_RUN(n5000);
    return tc_end();
}
