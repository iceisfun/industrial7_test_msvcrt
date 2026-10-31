/* TC
 * id:    ext_exit_atexit_lifo_win7
 * area:  exit_signal
 * funcs: exit
 * procs: atexit
 * crt:   win7
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.22.4.4; MS docs atexit
 * note:  handlers registered A,B,C run C,B,A: seq = 57
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(int, atexit, (void (__cdecl *)(void)));

static int seq;
static void __cdecl ha(void) { seq = seq * 4 + 1; _exit(seq); }
static void __cdecl hb(void) { seq = seq * 4 + 2; }
static void __cdecl hc(void) { seq = seq * 4 + 3; }
static void __cdecl nop(void) { }

TC_CASE(returns_zero) { int r; if (!tc_p_atexit) TC_SKIP("atexit not exported"); r = tc_p_atexit(nop); CHECK_EQ_I(r, 0); }
TC_CASE(child_lifo)
{
    DWORD code = 0; int ran;
    if (!tc_p_atexit) TC_SKIP("atexit not exported");
    ran = tc_spawn_self("lifo", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 57);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(atexit);
    if (tc_child_name() != NULL) {
        if (!tc_p_atexit) return 78;
        tc_p_atexit(ha); tc_p_atexit(hb); tc_p_atexit(hc);
        exit(0);
        return 77;
    }
    TC_RUN(returns_zero); TC_RUN(child_lifo);
    return tc_end();
}
