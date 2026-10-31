/* TC
 * id:    ext_probe_raise_sigfpe_subcode
 * area:  exit_signal
 * funcs: signal raise
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs signal: SIGFPE handler receives a second int (_FPE_* code); float.h _FPE_EXPLICITGEN
 * note:  Probes that raise(SIGFPE) passes _FPE_EXPLICITGEN (0x8c) as the handler's second argument.
 */
#include "tc.h"
#include <signal.h>

static volatile int seen, sub;
static void __cdecl rh(int s, int code) { seen = s; sub = code; }
TC_CASE(subcode)
{
    void *(__cdecl *sp)(int, void *) = (void *(__cdecl *)(int, void *))signal;
    seen = 0; sub = 0;
    sp(SIGFPE, (void *)rh);
    CHECK_EQ_I(raise(SIGFPE), 0);
    CHECK_EQ_I(seen, SIGFPE);
    CHECK_EQ_I(sub, 0x8c);
    sp(SIGFPE, (void *)SIG_DFL);
}
int main(void) { tc_begin(); TC_RUN(subcode); return tc_end(); }
