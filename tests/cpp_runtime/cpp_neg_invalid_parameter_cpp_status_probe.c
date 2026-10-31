/* TC
 * id:    cpp_neg_invalid_parameter_cpp_status_probe
 * area:  cpp_runtime
 * kind:  negative
 * procs: ?_invalid_parameter@@YAXPEBG00I_K@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Parameter Validation"
 * note:  PROBE: exact termination status of the default _invalid_parameter path in the old CRTs: one of
 *        0xC0000417 (STATUS_INVALID_CRUNTIME_PARAMETER) or 0xC0000409 (fast-fail). Documents only promise termination.
 */
#include "tc.h"

#ifdef _WIN64
#define IP_NAME "?_invalid_parameter@@YAXPEBG00I_K@Z"
#else
#define IP_NAME "?_invalid_parameter@@YAXPBG00II@Z"
#endif

TC_CASE(status_is_crt_param_or_fastfail)
{
    DWORD code = 0; int ran = tc_spawn_self("ip", &code, 20000);
    CHECK(ran);
    CHECK(code == TC_STATUS_INVALID_CRT_PARAM || code == TC_STATUS_FASTFAIL);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        void (__cdecl *ip)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t) =
            (void (__cdecl *)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t))tc_crt_proc(IP_NAME);
        if (!ip) return 78;
        ip(L"expr", L"func", L"file", 1, 0);
        return 77;
    }
    TC_RUN(status_is_crt_param_or_fastfail);
    return tc_end();
}
