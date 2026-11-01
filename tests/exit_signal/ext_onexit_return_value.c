/* TC
 * id:    ext_onexit_return_value
 * area:  exit_signal
 * funcs: 
 * procs: _crt_atexit _onexit
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _onexit: returns a pointer to the function if successful, NULL otherwise
 * note:  no-op handlers; they run harmlessly when the parent exits. Class D: ucrtbase does not export _onexit (probed), so on
 *        ucrtbase the exported _crt_atexit is used and its success value 0 is asserted (_crt_atexit is undocumented; 0 is OBSERVED on
 *        Win10 22H2 19045 via the export probe); older CRTs still assert that _onexit returns the registered pointer.
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

static int __cdecl f1(void) { return 0; }
static int __cdecl f2(void) { return 0; }

/* UCRT: _crt_atexit returns 0 on success (MS docs atexit); older CRTs: _onexit returns the registered function pointer. */
TC_CASE(result_equals_fn) { CHECK(REG_OK(f1)); }
TC_CASE(two_functions) { CHECK(REG_OK(f1)); CHECK(REG_OK(f2)); }
TC_CASE(same_twice) { CHECK(REG_OK(f2)); CHECK(REG_OK(f2)); }
int main(void)
{
    tc_begin(); p_reg = (reg_t)tc_crt_proc(REG_NAME);
    if (!p_reg) { tc_write("TC FAIL " TC_TEST_ID "::" REG_NAME " missing\n"); return 1; }
    TC_RUN(result_equals_fn); TC_RUN(two_functions); TC_RUN(same_twice);
    return tc_end();
}
