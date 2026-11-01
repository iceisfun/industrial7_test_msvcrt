/* TC
 * id:    ext_exit_onexit_lifo
 * area:  exit_signal
 * funcs: exit
 * procs: _crt_atexit _onexit
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _onexit (functions called LIFO at exit)
 * note:  Class D: ucrtbase does not export _onexit (probed); on ucrtbase handlers are registered with the exported _crt_atexit
 *        (same LIFO order, MS docs atexit/_onexit), older CRTs keep _onexit.
 */
#include "tc.h"
#include <stdlib.h>

/* Class D: ucrtbase.dll does not export _onexit (it lives in the static startup library and forwards to the exported _crt_atexit,
 * void(*)(void) -> 0 on success; probed with GetProcAddress on the Win10 22H2 reference). Older CRTs export _onexit. */
typedef int (__cdecl *tc_onx_t)(void);
#if TC_CRT_VER >= 140
typedef int (__cdecl *reg_t)(tc_onx_t);
#define REG_NAME "_crt_atexit"
#define REG_OK(f) (p_reg(f) == 0)
#else
typedef tc_onx_t (__cdecl *reg_t)(tc_onx_t);
#define REG_NAME "_onexit"
#define REG_OK(f) (p_reg(f) == (f))
#endif
static reg_t p_reg;

static int seq;
static int __cdecl ha(void) { seq = seq * 4 + 1; _exit(seq); return 0; }
static int __cdecl hb(void) { seq = seq * 4 + 2; return 0; }
static int __cdecl hc(void) { seq = seq * 4 + 3; return 0; }
static int __cdecl h2a(void) { seq = seq * 4 + 1; _exit(seq); return 0; }
static int __cdecl h2b(void) { seq = seq * 4 + 2; return 0; }

TC_CASE(three) { DWORD code = 0; int ran = tc_spawn_self("three", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 57); }
TC_CASE(two)   { DWORD code = 0; int ran = tc_spawn_self("two", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 9); } /* b runs first: 2, then a: 2*4+1 */

int main(void)
{
    const char *c;
    tc_begin(); p_reg = (reg_t)tc_crt_proc(REG_NAME); if (!p_reg) { tc_write("TC FAIL " TC_TEST_ID "::" REG_NAME " missing\n"); return 1; }
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 't' && c[1] == 'h') { p_reg(ha); p_reg(hb); p_reg(hc); }
        else { p_reg(h2a); p_reg(h2b); }
        exit(0);
        return 77;
    }
    TC_RUN(three); TC_RUN(two);
    return tc_end();
}
