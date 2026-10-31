/* TC
 * id:    nfx_neg_iph_invalid_parameter_default_fatal_ucrt
 * area:  negative_fatal
 * funcs: _set_invalid_parameter_handler
 * procs: _invalid_parameter
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs "Parameter validation", _set_invalid_parameter_handler: with no handler the CRT invokes Watson / terminates
 * note:  The child calls _invalid_parameter directly with no handler and must never return.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("direct", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);   /* 77 == returned from _invalid_parameter */
    CHECK(code != 0);
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
    TC_RUN(child_terminates);
    return tc_end();
}
