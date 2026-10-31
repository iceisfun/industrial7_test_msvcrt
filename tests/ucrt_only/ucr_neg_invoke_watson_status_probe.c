/* TC
 * id:    ucr_neg_invoke_watson_status_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _invoke_watson
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _invoke_watson
 * note:  PROBE: exit status after _invoke_watson(NULL,NULL,NULL,0,0) with a handler installed; expected 0xC0000409 (__fastfail).
 */
#include "tc.h"

TC_CASE(status)
{
    DWORD code = 0;
    CHECK(tc_spawn_self("watson", &code, 20000));
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
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
    TC_RUN(status);
    return tc_end();
}
