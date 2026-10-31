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
 * note:  PROBES the registration of a NULL function (atexit; ucrtbase: _crt_atexit; and _onexit). It is NOT known whether the CRT
 *        validates NULL (failure return + invalid-parameter handler) or silently accepts it. Asserted invariant only: either
 *        (failure return AND handler fired once) or (success return AND handler not fired); and a later exit(0) does not crash.
 *        All in child processes; child exit code 100 = accepted, 103 = rejected with handler.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef int (__cdecl *atexit_t)(void (__cdecl *)(void));
typedef void *(__cdecl *oe_t)(void *);
static void __cdecl nop(void) {}

static int child_main(const char *c)
{
    atexit_t ae = (atexit_t)tc_crt_proc(TC_AT_LEAST(140) ? "_crt_atexit" : "atexit");
    oe_t oe = (oe_t)tc_crt_proc("_onexit");
    void (__cdecl *volatile nullfn)(void) = NULL;
    tc_iph_install(); tc_iph_count = 0;
    if (c[0] == 'a') { int r; if (!ae) return 70; r = ae(nullfn); _exit(100 + (r != 0 ? 1 : 0) + (tc_iph_count ? 2 : 0)); }
    if (c[0] == 'o') { void *r; if (!oe) return 70; r = oe((void *)nullfn); _exit(100 + (r == NULL ? 1 : 0) + (tc_iph_count ? 2 : 0)); }
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
TC_CASE(onexit_null_consistent) { run_inv("o"); }
TC_CASE(exit_after_atexit_null_no_crash) { run("e", 0); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(atexit_null_consistent); TC_RUN(onexit_null_consistent); TC_RUN(exit_after_atexit_null_no_crash);
    return tc_end();
}
