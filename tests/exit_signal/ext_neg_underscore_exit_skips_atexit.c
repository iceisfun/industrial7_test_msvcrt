/* TC
 * id:    ext_neg_underscore_exit_skips_atexit
 * area:  exit_signal
 * funcs: _exit
 * procs: _onexit
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _exit: terminates without processing atexit/_onexit handlers
 * note:  handler would ExitProcess(99); seeing 99 means handlers wrongly ran
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *tc_onx_t)(void);
typedef tc_onx_t (__cdecl *tc_pt__onexit)(tc_onx_t); static tc_pt__onexit tc_p__onexit;

static int __cdecl h(void) { ExitProcess(99); return 0; }
TC_CASE(exit7)  { DWORD code = 0; int ran = tc_spawn_self("x7", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 7); }
TC_CASE(exit0)  { DWORD code = 5; int ran = tc_spawn_self("x0", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 0); }

int main(void)
{
    const char *c; volatile int v;
    tc_begin(); TC_LOAD_PROC(_onexit); if (!tc_p__onexit) { tc_write("TC FAIL " TC_TEST_ID "::__onexit missing\n"); return 1; }
    if ((c = tc_child_name()) != NULL) {
        v = (c[1] == '7') ? 7 : 0;
        tc_p__onexit(h);
        _exit(v);
        return 77;
    }
    TC_RUN(exit7); TC_RUN(exit0);
    return tc_end();
}
