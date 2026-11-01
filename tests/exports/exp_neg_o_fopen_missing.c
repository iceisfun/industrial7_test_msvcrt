/* TC
 * id:    exp_neg_o_fopen_missing
 * area:  exports
 * funcs:
 * procs: _o_fopen _o_fopen_s _o_fclose
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs fopen, fopen_s; _o_ aliases undocumented
 * note:  Windows-observed (Win10 22H2): the _o_ exports run the plain function under ucrtbase's separate OS global state (wrapped_invoke/enter_os_call), so
 *        errno and the invalid-parameter handler are the OS-state ones (_o__errno, _o__set_invalid_parameter_handler); the app-state errno/handler stay untouched.
 *        Expectations are the documented errno/handler behaviour of the underlying function, checked against the OS state, plus app-state isolation.
 */
#include "tc.h"
#include <errno.h>
#include <stdio.h>

/* _o_ exports run the normal function under ucrtbase's separate OS global state (wrapped_invoke + enter_os_call), so errno and the
 * invalid-parameter handler are the OS-state ones: read errno via _o__errno, install the counting handler via _o__set_invalid_parameter_handler.
 * The app-state errno / handler (plain errno, _set_invalid_parameter_handler) must stay untouched (isolation check). */
static int o_iph_count;
static void __cdecl o_iph(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; o_iph_count++; }
static int *o_errp(void) { int *(__cdecl *f)(void) = (int *(__cdecl *)(void))tc_crt_proc("_o__errno"); return f ? f() : NULL; }
static int o_errno(void) { int *p = o_errp(); return p ? *p : -12345; }
static int o_install(void)   /* 1 if the OS-state handler API exists; also arms the app-state counter for the isolation check */
{
    void *(__cdecl *set)(void *) = (void *(__cdecl *)(void *))tc_crt_proc("_o__set_invalid_parameter_handler");
    if (!set || !o_errp() || !tc_iph_install()) return 0;
    set((void *)o_iph); return 1;
}
static void o_reset(void) { int *p = o_errp(); o_iph_count = 0; tc_iph_count = 0; errno = 0; if (p) *p = 0; }
static void o_isolated(void) { CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }   /* app state untouched by the _o_ call */

typedef FILE *(__cdecl *fopen_t)(const char *, const char *);
typedef int (__cdecl *fopen_s_t)(FILE **, const char *, const char *);
typedef int (__cdecl *fclose_t)(FILE *);
static fopen_t p_fopen; static fopen_s_t p_fopen_s; static fclose_t p_fclose;
static const char *const missing = "tc_exp_neg_o_fopen_missing_nonexistent.tmp";

TC_CASE(fopen_missing_null_enoent)
{
    FILE *f;
    if (!p_fopen) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset();
    f = p_fopen(missing, "r");
    CHECK(f == NULL);
    CHECK_EQ_I(o_errno(), ENOENT);
    if (f && p_fclose) p_fclose(f);
}
TC_CASE(fopen_s_null_pfile_einval)
{
    int r;
    if (!p_fopen_s) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset();
    r = p_fopen_s(NULL, missing, "r");
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(o_iph_count, 1);    o_isolated();
}
TC_CASE(fopen_s_missing_enoent)
{
    FILE *f = (FILE *)1; int r;
    if (!p_fopen_s) TC_SKIP("not exported");
    r = p_fopen_s(&f, missing, "r");
    CHECK_EQ_I(r, ENOENT);
    CHECK(f == NULL);
    if (f && f != (FILE *)1 && p_fclose) p_fclose(f);
}
int main(void)
{
    tc_begin();
    p_fopen = (fopen_t)tc_crt_proc("_o_fopen"); p_fopen_s = (fopen_s_t)tc_crt_proc("_o_fopen_s");
    p_fclose = (fclose_t)tc_crt_proc("_o_fclose");
    TC_RUN(fopen_missing_null_enoent); TC_RUN(fopen_s_null_pfile_einval); TC_RUN(fopen_s_missing_enoent);
    return tc_end();
}
