/* TC
 * id:    ucr_neg_invoke_watson_child
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _invoke_watson _set_invalid_parameter_handler
 * crt:   win10
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _invoke_watson: terminates the process
 * note:  Documented part: process terminates (does not return, nonzero). Exact status is in ucr_neg_invoke_watson_status_probe.
 */
#include "tc.h"

TC_CASE(terminates)
{
    DWORD code = 0;
    CHECK(tc_spawn_self("watson", &code, 20000));
    CHECK(code != 77);
    CHECK(code != 0);
    CHECK(code != 90);   /* 90 = export missing in child */
    CHECK(code != 0xdead0001u);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        void (__cdecl *f)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t) =
            (void (__cdecl *)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t))tc_crt_proc("_invoke_watson");
        if (!f || !tc_iph_install()) ExitProcess(90);
        f(NULL, NULL, NULL, 0, 0);
        return 77;
    }
    TC_RUN(terminates);
    return tc_end();
}
