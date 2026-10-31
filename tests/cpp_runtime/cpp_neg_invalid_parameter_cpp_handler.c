/* TC
 * id:    cpp_neg_invalid_parameter_cpp_handler
 * area:  cpp_runtime
 * kind:  negative
 * procs: ?_invalid_parameter@@YAXPEBG00I_K@Z _set_invalid_parameter_handler
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Parameter Validation", _set_invalid_parameter_handler
 * note:  PROBE: the exported C++-mangled internal _invalid_parameter(expr, func, file, line, reserved) dispatches to the
 *        handler installed with _set_invalid_parameter_handler and forwards the argument pointers/line unchanged.
 *        Uses a custom capturing handler (not tc_iph). x64 decoration; x86 uses ?_invalid_parameter@@YAXPBG00II@Z.
 */
#include "tc.h"

#ifdef _WIN64
#define IP_NAME "?_invalid_parameter@@YAXPEBG00I_K@Z"
#else
#define IP_NAME "?_invalid_parameter@@YAXPBG00II@Z"
#endif
typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
typedef void (__cdecl *ip_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
static ip_t p_ip; static iph_t (__cdecl *p_set)(iph_t);
static int calls; static const wchar_t *g_e, *g_f, *g_fl; static unsigned g_line;

static void __cdecl cap(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned l, uintptr_t r)
{ (void)r; calls++; g_e = e; g_f = f; g_fl = fl; g_line = l; }

static const wchar_t w_e[] = L"expr", w_f[] = L"func", w_fl[] = L"file";

TC_CASE(handler_called)
{
    if (!p_ip || !p_set) TC_SKIP("missing proc");
    calls = 0; p_set(cap); p_ip(w_e, w_f, w_fl, 42, 0); p_set(NULL);
    CHECK_EQ_I(calls, 1);
}
TC_CASE(args_forwarded)
{
    if (!p_ip || !p_set) TC_SKIP("missing proc");
    calls = 0; g_e = g_f = g_fl = NULL; g_line = 0;
    p_set(cap); p_ip(w_e, w_f, w_fl, 42, 0); p_set(NULL);
    CHECK_EQ_P(g_e, w_e); CHECK_EQ_P(g_f, w_f); CHECK_EQ_P(g_fl, w_fl); CHECK_EQ_U(g_line, 42);
}
TC_CASE(null_strings)
{
    if (!p_ip || !p_set) TC_SKIP("missing proc");
    calls = 0; p_set(cap); p_ip(NULL, NULL, NULL, 0, 0); p_set(NULL);
    CHECK_EQ_I(calls, 1);
}

int main(void)
{
    tc_begin();
    p_ip = (ip_t)tc_crt_proc(IP_NAME); p_set = (iph_t (__cdecl *)(iph_t))tc_crt_proc("_set_invalid_parameter_handler");
    CHECK_EQ_P(p_ip != NULL, 1);
    TC_RUN(handler_called); TC_RUN(args_forwarded); TC_RUN(null_strings);
    return tc_end();
}
