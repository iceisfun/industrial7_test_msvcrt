/* TC
 * id:    ucr_atexit_32_registrations
 * area:  ucrt_only
 * funcs:
 * procs: _crt_atexit atexit
 * crt:   all
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.22.4.2 (at least 32 functions can be registered); MS docs atexit, _onexit
 * note:  UCRT exports _crt_atexit (atexit is a static wrapper in the import library); older CRTs export atexit.
 *        Handlers are harmless no-ops that run when the test process exits.
 */
#include "tc.h"

typedef int (__cdecl *atexit_t)(void (__cdecl *)(void));
static atexit_t p_reg;

static void __cdecl nop(void) {}

TC_CASE(exported) { CHECK(p_reg != NULL); }
TC_CASE(register_32)
{
    int i;
    if (!p_reg) TC_SKIP("no atexit entry point");
    for (i = 0; i < 32; i++) {
        int r = p_reg(nop);
        if (r != 0) { CHECK_EQ_I(r, 0); CHECK_EQ_I(i, -1); break; }
    }
}

int main(void)
{
    tc_begin();
#if TC_AT_LEAST(140)
    p_reg = (atexit_t)tc_crt_proc("_crt_atexit");
#else
    p_reg = (atexit_t)tc_crt_proc("atexit");
#endif
    TC_RUN(exported); TC_RUN(register_32);
    return tc_end();
}
