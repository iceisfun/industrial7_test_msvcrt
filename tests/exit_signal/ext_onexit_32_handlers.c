/* TC
 * id:    ext_onexit_32_handlers
 * area:  exit_signal
 * funcs: exit
 * procs: _onexit
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.22.4.2 by analogy; MS docs _onexit (at least 32 handlers)
 * note:  one handler registered 32 times
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *tc_onx_t)(void);
typedef tc_onx_t (__cdecl *tc_pt__onexit)(tc_onx_t); static tc_pt__onexit tc_p__onexit;

static int count, in_child;
static int __cdecl h(void) { if (in_child && ++count == 32) _exit(count); return 0; }

TC_CASE(register_all) { int i, bad = 0; for (i = 0; i < 32; i++) if (tc_p__onexit(h) == NULL) bad++; CHECK_EQ_I(bad, 0); }
TC_CASE(child_runs_all) { DWORD code = 0; int ran = tc_spawn_self("many", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 32); }

int main(void)
{
    int i;
    tc_begin(); TC_LOAD_PROC(_onexit); if (!tc_p__onexit) { tc_write("TC FAIL " TC_TEST_ID "::__onexit missing\n"); return 1; }
    if (tc_child_name() != NULL) {
        in_child = 1;
        for (i = 0; i < 32; i++) if (tc_p__onexit(h) == NULL) return 78;
        exit(0);
        return 77;
    }
    TC_RUN(register_all); TC_RUN(child_runs_all);
    return tc_end();
}
