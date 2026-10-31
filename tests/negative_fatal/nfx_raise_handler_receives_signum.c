/* TC
 * id:    nfx_raise_handler_receives_signum
 * area:  negative_fatal
 * kind:  positive
 * funcs: raise signal

 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs raise, signal; signal.h values
 * note:  Handler invoked synchronously by raise. Values: SIGINT 2, SIGILL 4, SIGFPE 8, SIGSEGV 11, SIGTERM 15, SIGBREAK 21, SIGABRT 22.
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

static void __cdecl h(int sig) { _exit(sig); }
static void run(const char *name, int sig)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, sig);
}
TC_CASE(sigs)
{
    run("s2", 2); run("s4", 4); run("s8", 8); run("s11", 11);
    run("s15", 15); run("s21", 21); run("s22", 22);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile int sig = 0; const char *p = c + 1;
        while (*p) sig = sig * 10 + (*p++ - '0');
        signal(sig, h);
        raise(sig);
        return 77;
    }
    TC_RUN(sigs);
    return tc_end();
}
