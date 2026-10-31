/* TC
 * id:    err_neg_iph_direct_default_fatal_child
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: ?_invalid_parameter@@YAXPEBG00I_K@Z _set_invalid_parameter_handler
 * crt:   msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs _set_invalid_parameter_handler (no handler installed: the CRT default terminates the process); _invalid_parameter itself is an undocumented CRT export
 * note:  msvcr80-120 export _invalid_parameter C++-mangled (x64: ?_invalid_parameter@@YAXPEBG00I_K@Z); plain name is a fallback. ucrtbase: see err_neg_probe_iph_direct_ucrt_export. Exact termination status is not asserted here (see err_neg_default_handler_fatal); only that the child did not continue.
 */
#include "tc.h"

typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
TC_DECL_PROC(void, _invalid_parameter, (const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t));
TC_DECL_PROC(iph_t, _set_invalid_parameter_handler, (iph_t));
#ifdef _WIN64
#define IP_MANGLED "?_invalid_parameter@@YAXPEBG00I_K@Z"
#else
#define IP_MANGLED "?_invalid_parameter@@YAXPBG00II@Z"
#endif
static void load_ip(void)
{
    tc_p__invalid_parameter = (void (__cdecl *)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t))tc_crt_proc(IP_MANGLED);
    if (!tc_p__invalid_parameter)
        tc_p__invalid_parameter = (void (__cdecl *)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t))tc_crt_proc("_invalid_parameter");
}

TC_CASE(no_handler_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nohandler", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
}
TC_CASE(with_handler_returns)
{
    DWORD code = 0; int ran = tc_spawn_self("withhandler", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    load_ip(); TC_LOAD_PROC(_set_invalid_parameter_handler);
    if (!tc_p__invalid_parameter || !tc_p__set_invalid_parameter_handler) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'n') tc_p__set_invalid_parameter_handler(NULL);
        else tc_p__set_invalid_parameter_handler(tc_iph);
        tc_p__invalid_parameter(NULL, NULL, NULL, 0, 0);
        return 77;
    }
    TC_RUN(no_handler_terminates); TC_RUN(with_handler_returns);
    return tc_end();
}
