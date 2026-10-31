/* TC
 * id:    ext_neg_c_exit_skips_handlers
 * area:  exit_signal
 * funcs: _c_exit _exit
 * procs: _onexit
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _c_exit: like _cexit but does not call atexit/_onexit handlers; returns to caller
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *tc_onx_t)(void);
typedef tc_onx_t (__cdecl *tc_pt__onexit)(tc_onx_t); static tc_pt__onexit tc_p__onexit;

static volatile int flag;
static int __cdecl h(void) { flag = 1; return 0; }
TC_CASE(child) { DWORD code = 0; int ran = tc_spawn_self("c", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 20); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(_onexit); if (!tc_p__onexit) { tc_write("TC FAIL " TC_TEST_ID "::__onexit missing\n"); return 1; }
    if (tc_child_name() != NULL) {
        tc_p__onexit(h);
        _c_exit();
        _exit(flag ? 21 : 20);
    }
    TC_RUN(child);
    return tc_end();
}
