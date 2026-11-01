/* TC
 * id:    exp_neg_o_sopen_s_errors
 * area:  exports
 * funcs:
 * procs: _o__sopen_s _o__wfopen_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _sopen_s, _wfopen_s (ENOENT, EINVAL, fd -1 / fp NULL on failure)
 * note:  Windows-observed (Win10 22H2): the _o_ exports run the plain function under ucrtbase's separate OS global state (wrapped_invoke/enter_os_call), so
 *        errno and the invalid-parameter handler are the OS-state ones (_o__errno, _o__set_invalid_parameter_handler); the app-state errno/handler stay untouched.
 *        Expectations are the documented errno/handler behaviour of the underlying function, checked against the OS state, plus app-state isolation.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <fcntl.h>
#include <share.h>
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

typedef int (__cdecl *sopen_s_t)(int *, const char *, int, int, int);
typedef int (__cdecl *wfopen_s_t)(FILE **, const wchar_t *, const wchar_t *);
static sopen_s_t p_sopen_s; static wfopen_s_t p_wfopen_s;
static volatile int bad_sh = 0x7777;
static const char *volatile miss = "missing\\x_tc_exp_neg";
static const wchar_t *volatile wmiss = L"missing\\x_tc_exp_neg", *volatile wx = L"tc_exp_neg_x.tmp", *volatile wzz = L"zz";

TC_CASE(null_fd_out)
{
    int r;
    if (!p_sopen_s) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset(); r = p_sopen_s(NULL, miss, _O_RDONLY, _SH_DENYNO, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(o_iph_count, 1);    o_isolated();
}
TC_CASE(missing_file)
{
    int fd = 123, r;
    if (!p_sopen_s) TC_SKIP("not exported");
    r = p_sopen_s(&fd, miss, _O_RDONLY, _SH_DENYNO, 0);
    CHECK_EQ_I(r, ENOENT); CHECK_EQ_I(fd, -1);
}
TC_CASE(bad_shflag)
{
    int fd = 123, r;
    if (!p_sopen_s) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset(); r = p_sopen_s(&fd, miss, _O_RDONLY, bad_sh, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(fd, -1); CHECK_EQ_I(o_iph_count, 1);    o_isolated();
}
TC_CASE(wfopen_missing)
{
    FILE *fp = (FILE *)(ULONG_PTR)1; int r;
    if (!p_wfopen_s) TC_SKIP("not exported");
    r = p_wfopen_s(&fp, wmiss, L"r");
    CHECK_EQ_I(r, ENOENT); CHECK(fp == NULL);
}
TC_CASE(wfopen_bad_mode)
{
    FILE *fp = (FILE *)(ULONG_PTR)1; int r;
    if (!p_wfopen_s) TC_SKIP("not exported");
    if (!o_install()) TC_SKIP("no handler api");
    o_reset(); r = p_wfopen_s(&fp, wx, wzz);
    CHECK_EQ_I(r, EINVAL); CHECK(fp == NULL); CHECK_EQ_I(o_iph_count, 1);    o_isolated();
}
int main(void)
{
    tc_begin();
    p_sopen_s = (sopen_s_t)tc_crt_proc("_o__sopen_s"); p_wfopen_s = (wfopen_s_t)tc_crt_proc("_o__wfopen_s");
    TC_RUN(null_fd_out); TC_RUN(missing_file); TC_RUN(bad_shflag); TC_RUN(wfopen_missing); TC_RUN(wfopen_bad_mode);
    return tc_end();
}
