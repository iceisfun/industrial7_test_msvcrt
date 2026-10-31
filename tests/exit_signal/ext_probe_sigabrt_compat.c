/* TC
 * id:    ext_probe_sigabrt_compat
 * area:  exit_signal
 * funcs: signal
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   signal.h SIGABRT_COMPAT 6
 * note:  Probes that signal number 6 (SIGABRT_COMPAT) is accepted by ucrtbase.
 */
#include "tc.h"
#include <signal.h>
static void __cdecl h(int s) { (void)s; }
typedef void (__cdecl *sh_t)(int);
static sh_t sg(int n, sh_t f) { return (sh_t)signal(n, f); }

TC_CASE(compat)
{
    sh_t r = sg(6, h);
    CHECK(r != SIG_ERR);
    sg(6, SIG_DFL);
}
int main(void) { tc_begin(); TC_RUN(compat); return tc_end(); }
