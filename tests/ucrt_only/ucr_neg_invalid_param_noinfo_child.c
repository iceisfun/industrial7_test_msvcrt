/* TC
 * id:    ucr_neg_invalid_param_noinfo_child
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _invalid_parameter_noinfo _invalid_parameter_noinfo_noreturn _set_invalid_parameter_handler
 * crt:   win10
 * tier:  p1
 * conf:  documented
 * ref:   MS docs "_invalid_parameter, _invalid_parameter_noinfo, _invalid_parameter_noinfo_noreturn, _invoke_watson"
 * note:  _noinfo calls the installed handler with NULL args and returns; _noreturn never returns (documented). Exact termination status is in ucr_neg_invalid_param_noreturn_status_probe.
 */
#include "tc.h"

typedef void (__cdecl *noinfo_t)(void);
static noinfo_t p_noinfo, p_noret;

TC_CASE(noinfo_calls_handler_and_returns)
{
    if (!p_noinfo || !tc_iph_install()) TC_SKIP("not exported");
    tc_iph_count = 0;
    p_noinfo();
    CHECK_EQ_I(tc_iph_count, 1);
    p_noinfo();
    CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(noreturn_terminates_even_with_handler)
{
    DWORD code = 0;
    CHECK(tc_spawn_self("noret", &code, 20000));
    CHECK(code != 77);   /* 77 = call returned */
    CHECK(code != 0);
    CHECK(code != 90);   /* 90 = export missing in child */
    CHECK(code != 0xdead0001u);
}
int main(void)
{
    const char *c;
    tc_begin();
    p_noinfo = (noinfo_t)tc_crt_proc("_invalid_parameter_noinfo");
    p_noret = (noinfo_t)tc_crt_proc("_invalid_parameter_noinfo_noreturn");
    if ((c = tc_child_name()) != NULL) {
        if (!p_noret || !tc_iph_install()) ExitProcess(90);
        p_noret();
        return 77;
    }
    TC_RUN(noinfo_calls_handler_and_returns); TC_RUN(noreturn_terminates_even_with_handler);
    return tc_end();
}
