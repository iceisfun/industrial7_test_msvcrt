/* TC
 * id:    err_neg_probe_iph_direct_errno
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: ?_invalid_parameter@@YAXPEBG00I_K@Z _set_invalid_parameter_handler _errno
 * crt:   msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _invalid_parameter (silent about errno)
 * note:  msvcr80-120 export _invalid_parameter C++-mangled (x64: ?_invalid_parameter@@YAXPEBG00I_K@Z); plain name is a fallback. ucrtbase: see err_neg_probe_iph_direct_ucrt_export. PROBE: expects a direct call to _invalid_parameter (handler returns) leaves errno unchanged. Callers such as strcpy_s set errno themselves, _invalid_parameter itself is not documented to.
 */
#include "tc.h"
#include <errno.h>

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
TC_DECL_PROC(int *, _errno, (void));

TC_CASE(errno_zero)   { *tc_p__errno() = 0;      tc_iph_count = 0; tc_p__invalid_parameter(NULL, NULL, NULL, 0, 0); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(*tc_p__errno(), 0); }
TC_CASE(errno_erange) { *tc_p__errno() = ERANGE; tc_iph_count = 0; tc_p__invalid_parameter(NULL, NULL, NULL, 0, 0); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(*tc_p__errno(), ERANGE); }

int main(void)
{
    tc_begin();
    load_ip(); TC_LOAD_PROC(_set_invalid_parameter_handler); TC_LOAD_PROC(_errno);
    if (!tc_p__invalid_parameter || !tc_p__set_invalid_parameter_handler || !tc_p__errno) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    tc_p__set_invalid_parameter_handler(tc_iph);
    TC_RUN(errno_zero); TC_RUN(errno_erange);
    return tc_end();
}
