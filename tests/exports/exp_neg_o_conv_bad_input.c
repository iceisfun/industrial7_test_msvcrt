/* TC
 * id:    exp_neg_o_conv_bad_input
 * area:  exports
 * funcs:
 * procs: _o_strtol _o_strtoul _o_atoi _o__wtoi _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs strtol/strtoul (EINVAL for bad base, ERANGE), atoi (invalid parameter on NULL)
 * note:  Windows-observed (Win10 22H2): the _o_ exports run the plain function under ucrtbase's separate OS global state (wrapped_invoke/enter_os_call), so
 *        errno and the invalid-parameter handler are the OS-state ones (_o__errno, _o__set_invalid_parameter_handler); the app-state errno/handler stay untouched.
 *        Expectations are the documented errno/handler behaviour of the underlying function, checked against the OS state, plus app-state isolation.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>

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

typedef long (__cdecl *strtol_t)(const char *, char **, int);
typedef unsigned long (__cdecl *strtoul_t)(const char *, char **, int);
typedef int (__cdecl *atoi_t)(const char *);
typedef int (__cdecl *wtoi_t)(const wchar_t *);
static strtol_t p_strtol; static strtoul_t p_strtoul; static atoi_t p_atoi; static wtoi_t p_wtoi;
static const char *volatile s12 = "12", *volatile sbig = "99999999999999999999", *volatile sneg = "-99999999999999999999";
static const char *volatile snull = NULL; static const wchar_t *volatile wnull = NULL;

TC_CASE(bad_base)
{
    char *e = NULL; long r;
    if (!p_strtol) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset(); r = p_strtol(s12, &e, 1);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(o_errno(), EINVAL); CHECK_EQ_I(o_iph_count, 1);
    o_reset(); r = p_strtol(s12, &e, 37);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(o_errno(), EINVAL); CHECK_EQ_I(o_iph_count, 1);    o_isolated();
}
TC_CASE(null_string)
{
    long r;
    if (!p_strtol) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset(); r = p_strtol(snull, NULL, 10);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(o_errno(), EINVAL); CHECK_EQ_I(o_iph_count, 1);    o_isolated();
}
TC_CASE(strtoul_overflow)
{
    unsigned long r;
    if (!p_strtoul) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset(); r = p_strtoul(sbig, NULL, 10);
    CHECK_EQ_U(r, ULONG_MAX); CHECK_EQ_I(o_errno(), ERANGE); CHECK_EQ_I(o_iph_count, 0);    o_isolated();
}
TC_CASE(strtol_overflow)
{
    long r;
    if (!p_strtol) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset(); r = p_strtol(sbig, NULL, 10);
    CHECK_EQ_I(r, LONG_MAX); CHECK_EQ_I(o_errno(), ERANGE);
    errno = 0; r = p_strtol(sneg, NULL, 10);
    CHECK_EQ_I(r, LONG_MIN); CHECK_EQ_I(o_errno(), ERANGE);
    CHECK_EQ_I(o_iph_count, 0);    o_isolated();
}
TC_CASE(atoi_null)
{
    int r;
    if (!p_atoi) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset(); r = p_atoi(snull);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(o_errno(), EINVAL); CHECK_EQ_I(o_iph_count, 1);    o_isolated();
}
TC_CASE(wtoi_null)
{
    int r;
    if (!p_wtoi) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset(); r = p_wtoi(wnull);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(o_errno(), EINVAL); CHECK_EQ_I(o_iph_count, 1);    o_isolated();
}
int main(void)
{
    tc_begin();
    p_strtol = (strtol_t)tc_crt_proc("_o_strtol"); p_strtoul = (strtoul_t)tc_crt_proc("_o_strtoul");
    p_atoi = (atoi_t)tc_crt_proc("_o_atoi"); p_wtoi = (wtoi_t)tc_crt_proc("_o__wtoi");
    TC_RUN(bad_base); TC_RUN(null_string); TC_RUN(strtoul_overflow); TC_RUN(strtol_overflow); TC_RUN(atoi_null); TC_RUN(wtoi_null);
    return tc_end();
}
