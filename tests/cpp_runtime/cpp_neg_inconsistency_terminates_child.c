/* TC
 * id:    cpp_neg_inconsistency_terminates_child
 * area:  cpp_runtime
 * kind:  negative
 * procs: ?_inconsistency@@YAXXZ ?set_terminate@@YAP6AXXZP6AXXZ@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MSVC EH runtime: _inconsistency() reports corrupt EH data by calling terminate()
 * note:  PROBE: the internal _inconsistency hook is documented nowhere; the expectation is that it calls terminate(), so a
 *        handler set with set_terminate runs (child exits 48) and, without one, the process aborts (exit code 3).
 *        x64 decoration only for the plain void(void) names (x86 is identical for these two).
 */
#include "tc.h"

typedef void (__cdecl *term_t)(void);
static void __cdecl my_term(void) { ExitProcess(48); }

static DWORD run(const char *name) { DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000); return ran ? code : 0xdead0003; }

TC_CASE(terminate_handler_runs) { CHECK_EQ_U(run("handler"), 48); }
TC_CASE(default_aborts) { CHECK_EQ_U(run("default"), 3); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        void (__cdecl *inc)(void) = (void (__cdecl *)(void))tc_crt_proc("?_inconsistency@@YAXXZ");
        term_t (__cdecl *st)(term_t) = (term_t (__cdecl *)(term_t))tc_crt_proc("?set_terminate@@YAP6AXXZP6AXXZ@Z");
        if (!inc || !st) return 78;
        if (c[0] == 'h') st(my_term);
        inc();
        return 77;
    }
    TC_RUN(terminate_handler_runs); TC_RUN(default_aborts);
    return tc_end();
}
