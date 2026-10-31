/* TC
 * id:    exp_error_handling_exported
 * area:  exports
 * funcs:
 * procs: _get_errno _set_errno _get_doserrno _set_doserrno _set_invalid_parameter_handler _invalid_parameter_noinfo _set_purecall_handler _set_abort_behavior _set_printf_count_output _get_printf_count_output _set_error_mode _errno
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Parameter Validation; _set_invalid_parameter_handler; _get_errno; _set_error_mode; _set_printf_count_output
 * note:  _get_invalid_parameter_handler and _get_purecall_handler are in exp_probe_error_getters (first CRT generation unsure).
 *        _invalid_parameter (full-argument form) is not a public ucrtbase export (only _invalid_parameter_noinfo[_noreturn]); it is
 *        resolved only for the pre-UCRT DLLs and exercised there if present (see iph_accessors).
 */
#include "tc.h"
static void need(const char *const *n)
{
    for (; *n; n++) if (!tc_crt_proc(*n)) { tc_failhdr(__FILE__, __LINE__); tc_write("missing export: "); tc_write(*n); tc_write("\n"); }
}
static void absent(const char *const *n)
{
    for (; *n; n++) if (tc_crt_proc(*n)) { tc_failhdr(__FILE__, __LINE__); tc_write("unexpected export: "); tc_write(*n); tc_write("\n"); }
}

typedef void (__cdecl *pure_t)(void);
static void __cdecl my_pure(void) {}

TC_CASE(exported_all)
{
    static const char *const n[] = { "_get_errno", "_set_errno", "_get_doserrno", "_set_doserrno", "_set_invalid_parameter_handler",
        "_invalid_parameter_noinfo", "_set_purecall_handler", "_set_abort_behavior", "_set_printf_count_output",
        "_get_printf_count_output", "_set_error_mode", "_errno", 0 };
    need(n);
}
TC_CASE(errno_accessors)
{
    int (__cdecl *ge)(int *) = (int (__cdecl *)(int *))tc_crt_proc("_get_errno");
    int (__cdecl *se)(int) = (int (__cdecl *)(int))tc_crt_proc("_set_errno");
    int (__cdecl *gd)(unsigned long *) = (int (__cdecl *)(unsigned long *))tc_crt_proc("_get_doserrno");
    int (__cdecl *sd)(unsigned long) = (int (__cdecl *)(unsigned long))tc_crt_proc("_set_doserrno");
    int *(__cdecl *pe)(void) = (int *(__cdecl *)(void))tc_crt_proc("_errno");
    int e = 0; unsigned long d = 0;
    if (!ge || !se || !gd || !sd || !pe) TC_SKIP("not exported");
    CHECK_EQ_I(se(34), 0); CHECK_EQ_I(ge(&e), 0); CHECK_EQ_I(e, 34); CHECK_EQ_I(*pe(), 34);
    *pe() = 2; CHECK_EQ_I(ge(&e), 0); CHECK_EQ_I(e, 2);
    CHECK_EQ_I(sd(5), 0); CHECK_EQ_I(gd(&d), 0); CHECK_EQ_U(d, 5);
    se(0); sd(0);
}
TC_CASE(iph_accessors)
{
    typedef void (__cdecl *h_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
    h_t (__cdecl *set)(h_t) = (h_t (__cdecl *)(h_t))tc_crt_proc("_set_invalid_parameter_handler");
    void (__cdecl *noinfo)(void) = (void (__cdecl *)(void))tc_crt_proc("_invalid_parameter_noinfo");
    void (__cdecl *full)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t) = NULL;
    h_t old;
#if TC_BELOW(140)
    full = (void (__cdecl *)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t))tc_crt_proc("_invalid_parameter");
    CHECK(full != NULL);
#endif
    if (!set || !noinfo) TC_SKIP("not exported");
    old = set(tc_iph);
    tc_iph_count = 0;
    noinfo(); CHECK_EQ_I(tc_iph_count, 1);
    if (full) { full(L"e", L"f", L"file", 1, 0); CHECK_EQ_I(tc_iph_count, 2); }
    CHECK(set(old) == tc_iph);   /* returns the previously installed handler */
}
TC_CASE(purecall_accessors)
{
    pure_t (__cdecl *set)(pure_t) = (pure_t (__cdecl *)(pure_t))tc_crt_proc("_set_purecall_handler");
    pure_t old, back;
    if (!set) TC_SKIP("not exported");
    old = set(my_pure);
    back = set(old);
    CHECK(back == my_pure);
}
TC_CASE(abort_error_mode)
{
    unsigned int (__cdecl *ab)(unsigned int, unsigned int) = (unsigned int (__cdecl *)(unsigned int, unsigned int))tc_crt_proc("_set_abort_behavior");
    int (__cdecl *em)(int) = (int (__cdecl *)(int))tc_crt_proc("_set_error_mode");
    unsigned int f1;
    int m0;
    if (!ab || !em) TC_SKIP("not exported");
    f1 = ab(0, 0);   /* returns the previous flags; tc_begin cleared _WRITE_ABORT_MSG|_CALL_REPORTFAULT (0x1|0x2) via mask 0x3 */
    CHECK_EQ_U(f1 & 0x3, 0);
    m0 = em(3);      /* _REPORT_ERRMODE: query only */
    CHECK(m0 >= 0 && m0 <= 2);
}
TC_CASE(printf_count_output)
{
    unsigned int (__cdecl *set)(int) = (unsigned int (__cdecl *)(int))tc_crt_proc("_set_printf_count_output");
    int (__cdecl *get)(void) = (int (__cdecl *)(void))tc_crt_proc("_get_printf_count_output");
    if (!set || !get) TC_SKIP("not exported");
    CHECK_EQ_I(set(0), 0);   /* default is disabled */
    CHECK_EQ_I(get(), 0);
    CHECK_EQ_I(set(1), 0); CHECK_EQ_I(get(), 1);
    CHECK_EQ_I(set(0), 1); CHECK_EQ_I(get(), 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(exported_all); TC_RUN(errno_accessors); TC_RUN(iph_accessors); TC_RUN(purecall_accessors); TC_RUN(abort_error_mode); TC_RUN(printf_count_output);
    return tc_end();
}
