/* TC
 * id:    nfx_exit_onexit_many_handlers_probe
 * area:  negative_fatal
 * kind:  positive
 * funcs:  exit _exit
 * procs: _onexit
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.22.4.2; MS docs _onexit
 * note:  Probes that the handler table grows past 32 entries (33, 64, 129, 1024, 5000), each count in its own child. C only guarantees 32.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(_onexit_t, _onexit, (_onexit_t));


static volatile int count, want_n;
static int __cdecl hc(void) { count++; return 0; }
/* registered first => runs last */
static int __cdecl hfinal(void) { _exit(count == want_n - 1 ? 0 : 1); return 0; }

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
    TC_LOAD_PROC(_onexit);
    if ((c = tc_child_name()) != NULL) {
        int i, n = 0;
        for (c++; *c; c++) n = n * 10 + (*c - '0');
        want_n = n;
        if (!tc_p__onexit(hfinal)) _exit(60);
        for (i = 1; i < n; i++) if (!tc_p__onexit(hc)) _exit(61);
        exit(2);
    }
    TC_RUN(n33); TC_RUN(n64); TC_RUN(n129); TC_RUN(n1024); TC_RUN(n5000);
    return tc_end();
}
