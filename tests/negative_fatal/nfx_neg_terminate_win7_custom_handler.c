/* TC
 * id:    nfx_neg_terminate_win7_custom_handler
 * area:  negative_fatal
 * kind:  negative
 * funcs:
 * procs: ?set_terminate@@YAP6AXXZP6AXXZ@Z ?terminate@@YAXXZ
 * crt:   win7
 * tier:  p2
 * conf:  documented
 * ref:   MS docs set_terminate, terminate (terminate calls the handler installed by set_terminate; set_terminate returns the previous one)
 * note:  Old CRTs export the C++ functions only under decorated names, so they are resolved with tc_crt_proc.
 */
#include "tc.h"

typedef void (__cdecl *th_t)(void);
static void __cdecl h(void) { ExitProcess(17); }
static void __cdecl h2(void) { ExitProcess(18); }

TC_CASE(handler_called) { DWORD c = 0; CHECK(tc_spawn_self("call", &c, 20000)); CHECK_EQ_U(c, 17); }
TC_CASE(returns_previous) { DWORD c = 99; CHECK(tc_spawn_self("prev", &c, 20000)); CHECK_EQ_U(c, 0); }
int main(void)
{
    const char *n; tc_begin();
    if ((n = tc_child_name()) != NULL) {
        th_t (__cdecl *set)(th_t) = (th_t (__cdecl *)(th_t))tc_crt_proc("?set_terminate@@YAP6AXXZP6AXXZ@Z");
        void (__cdecl *term)(void) = (void (__cdecl *)(void))tc_crt_proc("?terminate@@YAXXZ");
        if (!set || !term) return 78;
        set(h);
        if (n[0] == 'c') { term(); return 77; }
        return set(h2) == h ? 0 : 1;
    }
    TC_RUN(handler_called); TC_RUN(returns_previous);
    return tc_end();
}
