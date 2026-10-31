/* TC
 * id:    ext_quick_exit_basic
 * area:  exit_signal
 * funcs: quick_exit _crt_at_quick_exit
 * crt:   >=140
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.22.4.7 quick_exit, 7.22.4.3 at_quick_exit (LIFO)
 * note:  _crt_at_quick_exit is an undocumented ucrt export; quick_exit itself is C11. handlers A,B,C registered, run C,B,A: 57
 */
#include "tc.h"
#include <stdlib.h>

int __cdecl _crt_at_quick_exit(void (__cdecl *)(void));
static int seq;
static void __cdecl ha(void) { seq = seq * 4 + 1; _exit(seq); }
static void __cdecl hb(void) { seq = seq * 4 + 2; }
static void __cdecl hc(void) { seq = seq * 4 + 3; }
static void __cdecl nop(void) { }

TC_CASE(returns_zero) { volatile int r = _crt_at_quick_exit(nop); CHECK_EQ_I(r, 0); }
TC_CASE(lifo) { DWORD code = 0; int ran = tc_spawn_self("lifo", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 57); }
TC_CASE(plain) { DWORD code = 0; int ran = tc_spawn_self("plain", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 5); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'l') { _crt_at_quick_exit(ha); _crt_at_quick_exit(hb); _crt_at_quick_exit(hc); quick_exit(0); }
        else quick_exit(5);
        return 77;
    }
    TC_RUN(returns_zero); TC_RUN(lifo); TC_RUN(plain);
    return tc_end();
}
