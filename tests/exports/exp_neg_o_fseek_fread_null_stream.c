/* TC
 * id:    exp_neg_o_fseek_fread_null_stream
 * area:  exports
 * funcs:
 * procs: _o_fseek _o_fread _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs fseek, _fseeki64, fread (EINVAL / invalid parameter; size or count 0 returns 0)
 * note:  Windows-observed (Win10 22H2): the _o_ exports run the plain function under ucrtbase's separate OS global state (wrapped_invoke/enter_os_call), so
 *        errno and the invalid-parameter handler are the OS-state ones (_o__errno, _o__set_invalid_parameter_handler); the app-state errno/handler stay untouched.
 *        Expectations are the documented errno/handler behaviour of the underlying function, checked against the OS state, plus app-state isolation.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>
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

typedef int (__cdecl *fseek_t)(FILE *, long, int);
typedef size_t (__cdecl *fread_t)(void *, size_t, size_t, FILE *);
static fseek_t p_fseek; static fread_t p_fread;

TC_CASE(fseek_null)
{
    int r;
    if (!p_fseek) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset(); r = p_fseek(NULL, 0, SEEK_SET);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(o_errno(), EINVAL); CHECK_EQ_I(o_iph_count, 1);    o_isolated();
}
TC_CASE(fread_null_stream)
{
    char b[8]; size_t r;
    if (!p_fread) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset(); r = p_fread(b, 1, 1, NULL);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(o_errno(), EINVAL); CHECK_EQ_I(o_iph_count, 1);    o_isolated();
}
int main(void)
{
    tc_begin();
    p_fseek = (fseek_t)tc_crt_proc("_o_fseek"); p_fread = (fread_t)tc_crt_proc("_o_fread");
    TC_RUN(fseek_null); TC_RUN(fread_null_stream);
    return tc_end();
}
