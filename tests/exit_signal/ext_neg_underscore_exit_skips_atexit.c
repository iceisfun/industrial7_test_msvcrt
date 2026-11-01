/* TC
 * id:    ext_neg_underscore_exit_skips_atexit
 * area:  exit_signal
 * funcs: _exit
 * procs: _crt_atexit _onexit
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _exit: terminates without processing atexit/_onexit handlers
 * note:  handler would ExitProcess(99); seeing 99 means handlers wrongly ran. Class D: registration goes through the exported
 *        _crt_atexit on ucrtbase (ucrtbase does not export _onexit), _onexit on older CRTs.
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

static int __cdecl h(void) { ExitProcess(99); return 0; }
TC_CASE(exit7)  { DWORD code = 0; int ran = tc_spawn_self("x7", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 7); }
TC_CASE(exit0)  { DWORD code = 5; int ran = tc_spawn_self("x0", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 0); }

int main(void)
{
    const char *c; volatile int v;
    tc_begin(); p_reg = (reg_t)tc_crt_proc(REG_NAME); if (!p_reg) { tc_write("TC FAIL " TC_TEST_ID "::" REG_NAME " missing\n"); return 1; }
    if ((c = tc_child_name()) != NULL) {
        v = (c[1] == '7') ? 7 : 0;
        p_reg(h);
        _exit(v);
        return 77;
    }
    TC_RUN(exit7); TC_RUN(exit0);
    return tc_end();
}
