/* TC
 * id:    ext_onexit_32_handlers
 * area:  exit_signal
 * funcs: exit
 * procs: _onexit
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.22.4.2 by analogy; MS docs _onexit (at least 32 handlers)
 * note:  one handler registered 32 times
 *        Win10 22H2 19045 (observed, GetProcAddress on ucrtbase): _onexit and atexit are NOT exported; the registration entry is _crt_atexit
 *        (void handler, returns 0 on success). Under TC_AT_LEAST(140) (ucrtbase) the test registers through _crt_atexit (class D); other CRTs keep _onexit.
 */
#include "tc.h"
#include <stdlib.h>

/* ucrtbase.dll exports _crt_atexit (void handler, 0 = ok) and no _onexit; the msvcr* DLLs export _onexit (int handler, NULL = failure). */
#if TC_AT_LEAST(140)   /* ucrtbase; NB tools/tc.py never defines TC_UCRT */
typedef int (__cdecl *tc_pt_reg)(void (__cdecl *)(void)); static tc_pt_reg tc_p_reg;
#define TC_REG_LOAD() (tc_p_reg = (tc_pt_reg)tc_crt_proc("_crt_atexit"))
#define TC_REG_FAILS(f) (tc_p_reg(f) != 0)
#define TC_REG_NAME "_crt_atexit"
#else
typedef int (__cdecl *tc_onx_t)(void);
typedef tc_onx_t (__cdecl *tc_pt_reg)(tc_onx_t); static tc_pt_reg tc_p_reg;
#define TC_REG_LOAD() (tc_p_reg = (tc_pt_reg)tc_crt_proc("_onexit"))
#define TC_REG_FAILS(f) (tc_p_reg((tc_onx_t)(f)) == NULL)
#define TC_REG_NAME "_onexit"
#endif

static int count, in_child;
static void __cdecl h(void) { if (in_child && ++count == 32) _exit(count); }

TC_CASE(register_all) { int i, bad = 0; for (i = 0; i < 32; i++) if (TC_REG_FAILS(h)) bad++; CHECK_EQ_I(bad, 0); }
TC_CASE(child_runs_all) { DWORD code = 0; int ran = tc_spawn_self("many", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 32); }

int main(void)
{
    int i;
    tc_begin(); TC_REG_LOAD(); if (!tc_p_reg) { tc_write("TC FAIL " TC_TEST_ID "::__" TC_REG_NAME " missing\n"); return 1; }
    if (tc_child_name() != NULL) {
        in_child = 1;
        for (i = 0; i < 32; i++) if (TC_REG_FAILS(h)) return 78;
        exit(0);
        return 77;
    }
    TC_RUN(register_all); TC_RUN(child_runs_all);
    return tc_end();
}
