/* TC
 * id:    fpe_neg_fe_null_ptr_probe
 * area:  fpenv
 * kind:  negative
 * funcs: _clearfp
 * procs: fegetenv fesetenv fegetexceptflag
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.6.4 (no defined behaviour for NULL); MS docs fegetenv/fesetenv/fegetexceptflag silent on NULL
 * note:  PROBES what happens when NULL is passed to the fenv state functions (AV 0xC0000005, invalid-parameter fast-fail,
 *        or silent success). Exit codes are logged as TC INFO lines; only 'the child could be spawned and exited' is asserted.
 */
#include "tc.h"
#include <float.h>

typedef int (__cdecl *fn1_t)(void *);
typedef int (__cdecl *fn2_t)(void *, int);

static void info(const char *name, DWORD code)
{
    char b[24]; tc_write("TC INFO " TC_TEST_ID "::"); tc_write(name); tc_write(" exit="); tc_hex(code, b); tc_write(b); tc_write("\n");
}
static void probe(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    info(name, code);
}
TC_CASE(fegetenv_null) { probe("fegetenv"); }
TC_CASE(fesetenv_null) { probe("fesetenv"); }
TC_CASE(fegetexceptflag_null) { probe("fegetexceptflag"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        FARPROC p = tc_crt_proc(c);
        if (!p) return 78;
        _clearfp();
        if (c[2] == 'g' && c[3] == 'e' && c[6] == 'x') ((fn2_t)p)(NULL, 0x3D);
        else ((fn1_t)p)(NULL);
        return 77;
    }
    TC_RUN(fegetenv_null); TC_RUN(fesetenv_null); TC_RUN(fegetexceptflag_null);
    return tc_end();
}
