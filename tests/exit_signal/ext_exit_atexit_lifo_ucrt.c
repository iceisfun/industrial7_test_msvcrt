/* TC
 * id:    ext_exit_atexit_lifo_ucrt
 * area:  exit_signal
 * funcs: exit _crt_atexit
 * crt:   >=140
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.22.4.4 (atexit: reverse order of registration); ucrt atexit is inline over _crt_atexit
 * note:  _crt_atexit is an undocumented ucrt export (the one atexit inlines to); LIFO is the C-standard semantics. order encoded in exit code: handlers run C,B,A so seq = ((3)*4+2)*4+1 = 57
 */
#include "tc.h"
#include <stdlib.h>

typedef int (__cdecl *tc_ae_t)(void (__cdecl *)(void));
int __cdecl _crt_atexit(void (__cdecl *)(void));

static int seq;
static void __cdecl ha(void) { seq = seq * 4 + 1; _exit(seq); }
static void __cdecl hb(void) { seq = seq * 4 + 2; }
static void __cdecl hc(void) { seq = seq * 4 + 3; }
static void __cdecl nop(void) { }

TC_CASE(returns_zero) { volatile int r = _crt_atexit(nop); CHECK_EQ_I(r, 0); }
TC_CASE(child_lifo)
{
    DWORD code = 0; int ran = tc_spawn_self("lifo", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 57);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        _crt_atexit(ha); _crt_atexit(hb); _crt_atexit(hc);
        exit(0);
        return 77;
    }
    TC_RUN(returns_zero); TC_RUN(child_lifo);
    return tc_end();
}
