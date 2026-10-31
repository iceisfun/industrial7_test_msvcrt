/* TC
 * id:    nfx_neg_iph_invalid_parameter_direct_status_ucrt_probe
 * area:  negative_fatal
 * funcs: _set_invalid_parameter_handler
 * procs: _invalid_parameter
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _invalid_parameter
 * note:  PROBE: exact exit status when _invalid_parameter (no handler) terminates the process. Expected
 *        0xC0000409 (__fastfail, STATUS_STACK_BUFFER_OVERRUN) on ucrtbase.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(status)
{
    DWORD code = 0; int ran = tc_spawn_self("direct", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        void (__cdecl *ip)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t) =
            (void (__cdecl *)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t))tc_crt_proc("_invalid_parameter");
        if (!ip) return 78;
        _set_invalid_parameter_handler(NULL);
        ip(L"e", L"f", L"g", 1, 0);
        return 77;
    }
    TC_RUN(status);
    return tc_end();
}
