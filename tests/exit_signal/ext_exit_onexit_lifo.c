/* TC
 * id:    ext_exit_onexit_lifo
 * area:  exit_signal
 * funcs: exit
 * procs: _onexit
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _onexit (functions called LIFO at exit)
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *tc_onx_t)(void);
typedef tc_onx_t (__cdecl *tc_pt__onexit)(tc_onx_t); static tc_pt__onexit tc_p__onexit;

static int seq;
static int __cdecl ha(void) { seq = seq * 4 + 1; _exit(seq); return 0; }
static int __cdecl hb(void) { seq = seq * 4 + 2; return 0; }
static int __cdecl hc(void) { seq = seq * 4 + 3; return 0; }
static int __cdecl h2a(void) { seq = seq * 4 + 1; _exit(seq); return 0; }
static int __cdecl h2b(void) { seq = seq * 4 + 2; return 0; }

TC_CASE(three) { DWORD code = 0; int ran = tc_spawn_self("three", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 57); }
TC_CASE(two)   { DWORD code = 0; int ran = tc_spawn_self("two", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 9); } /* b runs first: 2, then a: 2*4+1 */

int main(void)
{
    const char *c;
    tc_begin(); TC_LOAD_PROC(_onexit); if (!tc_p__onexit) { tc_write("TC FAIL " TC_TEST_ID "::__onexit missing\n"); return 1; }
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 't' && c[1] == 'h') { tc_p__onexit(ha); tc_p__onexit(hb); tc_p__onexit(hc); }
        else { tc_p__onexit(h2a); tc_p__onexit(h2b); }
        exit(0);
        return 77;
    }
    TC_RUN(three); TC_RUN(two);
    return tc_end();
}
