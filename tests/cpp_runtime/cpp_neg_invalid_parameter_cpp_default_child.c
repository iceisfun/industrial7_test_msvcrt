/* TC
 * id:    cpp_neg_invalid_parameter_cpp_default_child
 * area:  cpp_runtime
 * kind:  negative
 * procs: ?_invalid_parameter@@YAXPEBG00I_K@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  inferred
 * ref:   MS docs "Parameter Validation": with the default handler an invalid parameter terminates the process
 *        (calling the internal C++-mangled _invalid_parameter directly is not itself documented)
 * note:  Fatal path runs in a child. Only abnormal termination is asserted; the exact status is in
 *        cpp_neg_invalid_parameter_cpp_status_probe.
 */
#include "tc.h"

#ifdef _WIN64
#define IP_NAME "?_invalid_parameter@@YAXPEBG00I_K@Z"
#else
#define IP_NAME "?_invalid_parameter@@YAXPBG00II@Z"
#endif

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("ip", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
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
    TC_RUN(child_terminates);
    return tc_end();
}
