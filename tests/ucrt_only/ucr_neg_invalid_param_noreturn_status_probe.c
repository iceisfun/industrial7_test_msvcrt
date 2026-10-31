/* TC
 * id:    ucr_neg_invalid_param_noreturn_status_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _invalid_parameter_noinfo_noreturn
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _invalid_parameter_noinfo_noreturn
 * note:  PROBE: exit status of the process after _invalid_parameter_noinfo_noreturn with a returning handler installed.
 *        Expected 0xC0000409 (fast fail via _invoke_watson).
 */
#include "tc.h"

TC_CASE(status)
{
    DWORD code = 0;
    CHECK(tc_spawn_self("noret", &code, 20000));
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        void (__cdecl *f)(void) = (void (__cdecl *)(void))tc_crt_proc("_invalid_parameter_noinfo_noreturn");
        if (!f || !tc_iph_install()) ExitProcess(90);
        f();
        return 77;
    }
    TC_RUN(status);
    return tc_end();
}
