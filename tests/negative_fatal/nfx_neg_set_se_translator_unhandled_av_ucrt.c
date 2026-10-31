/* TC
 * id:    nfx_neg_set_se_translator_unhandled_av_ucrt
 * area:  negative_fatal
 * kind:  negative
 * funcs: _set_se_translator signal
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs signal (SIGSEGV), _set_se_translator (only used by C++ /EHa exception handling)
 * note:  Probes: with a C-only program a NULL write reaches the signal(SIGSEGV) handler (which _exit(9)s) and the
 *        SE translator installed with _set_se_translator is NOT consulted (a translator that would _exit(8) must not run).
 *        Toolchain note: exes are built with mingw-w64, whose startup does the exception-to-signal translation.
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>
typedef void (__cdecl *se_fn)(unsigned int, struct _EXCEPTION_POINTERS *);
se_fn __cdecl _set_se_translator(se_fn);

static volatile char *volatile p;
static void __cdecl h(int s) { (void)s; _exit(9); }
static void __cdecl tr(unsigned int c, struct _EXCEPTION_POINTERS *e) { (void)c; (void)e; _exit(8); }

TC_CASE(handler_exit9)
{
    DWORD code = 0; int ran = tc_spawn_self("av", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 9);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        _set_se_translator(tr);
        signal(SIGSEGV, h);
        p = NULL; *p = 1;
        return 77;
    }
    TC_RUN(handler_exit9);
    return tc_end();
}
