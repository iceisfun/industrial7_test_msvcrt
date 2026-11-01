/* TC
 * id:    stl_neg_atexit_null_probe
 * area:  stdlib
 * kind:  negative
 * funcs: exit _exit
 * procs: atexit _crt_atexit
 * crt:   >=100
 * tier:  p1
 * conf:  probe
 * ref:   MS docs atexit, _onexit (NULL function -> invalid parameter handling in VS2005+ CRTs)
 * note:  PROBES the registration of a NULL function via atexit (ucrtbase: _crt_atexit). Asserted invariant only: either
 *        (failure return AND handler fired once) or (success return AND handler not fired); and a later exit(0) does not crash.
 *        Class D: the _onexit(NULL) case was removed because ucrtbase.dll does not export _onexit (it is in the static startup
 *        library; probed with GetProcAddress on the reference).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef int (__cdecl *atexit_t)(void (__cdecl *)(void));
static void __cdecl nop(void) {}

static int child_main(const char *c)
{
    atexit_t ae = (atexit_t)tc_crt_proc(TC_AT_LEAST(140) ? "_crt_atexit" : "atexit");
    void (__cdecl *volatile nullfn)(void) = NULL;
    tc_iph_install(); tc_iph_count = 0;
    if (c[0] == 'a') { int r; if (!ae) return 70; r = ae(nullfn); _exit(100 + (r != 0 ? 1 : 0) + (tc_iph_count ? 2 : 0)); }
    if (c[0] == 'e') { if (!ae) return 70; ae(nullfn); exit(0); }
    return 71;
}
static void run(const char *name, DWORD want)
{
    DWORD code = 0; int ok = tc_spawn_self(name, &code, 15000);
    CHECK(ok); CHECK_EQ_U(code, want);
}
static void run_inv(const char *name)
{
    DWORD code = 0; int ok = tc_spawn_self(name, &code, 15000);
    CHECK(ok); CHECK(code == 100 || code == 103);
}
TC_CASE(atexit_null_consistent)   { run_inv("a"); }
TC_CASE(exit_after_atexit_null_no_crash) { run("e", 0); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(atexit_null_consistent); TC_RUN(exit_after_atexit_null_no_crash);
    return tc_end();
}
