/* TC
 * id:    ext_neg_c_exit_skips_handlers
 * area:  exit_signal
 * funcs: _c_exit _exit
 * procs: _onexit
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _c_exit: like _cexit but does not call atexit/_onexit handlers; returns to caller
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

static volatile int flag;
static void __cdecl h(void) { flag = 1; }
TC_CASE(child) { DWORD code = 0; int ran = tc_spawn_self("c", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 20); }

int main(void)
{
    tc_begin(); TC_REG_LOAD(); if (!tc_p_reg) { tc_write("TC FAIL " TC_TEST_ID "::__" TC_REG_NAME " missing\n"); return 1; }
    if (tc_child_name() != NULL) {
        TC_REG_FAILS(h);
        _c_exit();
        _exit(flag ? 21 : 20);
    }
    TC_RUN(child);
    return tc_end();
}
