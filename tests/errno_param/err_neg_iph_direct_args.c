/* TC
 * id:    err_neg_iph_direct_args
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: ?_invalid_parameter@@YAXPEBG00I_K@Z _set_invalid_parameter_handler
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  inferred
 * note:  The C++-mangled internal _invalid_parameter is what msvcr80-120 export (plain name tried as fallback). msvcr80 is left out because its export of this internal name is unverified. ucrtbase is covered by err_neg_probe_iph_direct_ucrt_export.
 * ref:   MS docs _set_invalid_parameter_handler (handler signature: expression, function, file, line, reserved); _invalid_parameter itself is an undocumented CRT export
 */
#include "tc.h"
#include <stddef.h>

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

static int hits;
static const wchar_t *g_e, *g_f, *g_fl; static unsigned int g_l;
static void __cdecl rec(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)r; hits++; g_e = e; g_f = f; g_fl = fl; g_l = l; }

static const wchar_t w_expr[] = L"p != NULL", w_fn[] = L"my_func", w_file[] = L"my_file.c";

TC_CASE(verbatim)
{
    volatile unsigned int ln = 4242;
    hits = 0; g_e = g_f = g_fl = NULL; g_l = 0;
    tc_p__invalid_parameter(w_expr, w_fn, w_file, ln, 0);
    CHECK_EQ_I(hits, 1);
    CHECK_EQ_U(g_l, 4242);
    CHECK_WSTR(g_e, L"p != NULL"); CHECK_WSTR(g_f, L"my_func"); CHECK_WSTR(g_fl, L"my_file.c");
}
TC_CASE(returns_to_caller)
{
    int after = 0;
    hits = 0;
    tc_p__invalid_parameter(w_expr, w_fn, w_file, 1, 0);
    after = 1;
    CHECK_EQ_I(after, 1); CHECK_EQ_I(hits, 1);
}
TC_CASE(null_args)
{
    hits = 0; g_e = g_f = g_fl = (const wchar_t *)1; g_l = 99;
    tc_p__invalid_parameter(NULL, NULL, NULL, 0, 0);
    CHECK_EQ_I(hits, 1);
    CHECK_EQ_P(g_e, NULL); CHECK_EQ_P(g_f, NULL); CHECK_EQ_P(g_fl, NULL); CHECK_EQ_U(g_l, 0);
}
TC_CASE(each_call_once)
{
    hits = 0;
    tc_p__invalid_parameter(w_expr, w_fn, w_file, 1, 0);
    tc_p__invalid_parameter(w_expr, w_fn, w_file, 2, 0);
    tc_p__invalid_parameter(NULL, NULL, NULL, 3, 0);
    CHECK_EQ_I(hits, 3); CHECK_EQ_U(g_l, 3);
}
int main(void)
{
    tc_begin();
    load_ip(); TC_LOAD_PROC(_set_invalid_parameter_handler);
    if (!tc_p__invalid_parameter || !tc_p__set_invalid_parameter_handler) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    tc_p__set_invalid_parameter_handler(rec);
    TC_RUN(verbatim); TC_RUN(returns_to_caller); TC_RUN(null_args); TC_RUN(each_call_once);
    return tc_end();
}
