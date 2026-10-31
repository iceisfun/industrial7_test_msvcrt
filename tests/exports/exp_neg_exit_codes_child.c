/* TC
 * id:    exp_neg_exit_codes_child
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: exit _exit abort
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs exit, _exit (status becomes the process exit code); MS docs abort: returns exit code 3
 * note:  abort path: tc_begin disables the abort message box and Watson (_set_abort_behavior) where available.
 */
#include "tc.h"

typedef void (__cdecl *ex_t)(int);
typedef void (__cdecl *ab_t)(void);

static int child_main(const char *c)
{
    if (c[0] == 'e') { ex_t f = (ex_t)tc_crt_proc("exit"); if (f) f(42); }
    if (c[0] == 'u') { ex_t f = (ex_t)tc_crt_proc("_exit"); if (f) f(7); }
    if (c[0] == 'a') { ab_t f = (ab_t)tc_crt_proc("abort"); if (f) f(); }
    return 77;   /* not reached when the CRT behaves */
}
TC_CASE(exit_42) { DWORD code = 0; CHECK(tc_spawn_self("exit42", &code, 20000)); CHECK_EQ_U(code, 42); }
TC_CASE(underscore_exit_7) { DWORD code = 0; CHECK(tc_spawn_self("uexit7", &code, 20000)); CHECK_EQ_U(code, 7); }
TC_CASE(abort_is_3) { DWORD code = 0; CHECK(tc_spawn_self("abort", &code, 20000)); CHECK_EQ_U(code, 3); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(exit_42); TC_RUN(underscore_exit_7); TC_RUN(abort_is_3);
    return tc_end();
}
