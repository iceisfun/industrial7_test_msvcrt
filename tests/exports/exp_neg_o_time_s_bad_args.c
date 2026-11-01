/* TC
 * id:    exp_neg_o_time_s_bad_args
 * area:  exports
 * funcs:
 * procs: _o__gmtime64_s _o__localtime64_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _gmtime64_s, _localtime64_s (EINVAL on NULL / beyond 3000-12-31; fields set to -1)
 * note:  Windows-observed (Win10 22H2): the _o_ exports run the plain function under ucrtbase's separate OS global state (wrapped_invoke/enter_os_call), so
 *        errno and the invalid-parameter handler are the OS-state ones (_o__errno, _o__set_invalid_parameter_handler); the app-state errno/handler stay untouched.
 *        Expectations are the documented errno/handler behaviour of the underlying function, checked against the OS state, plus app-state isolation.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

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

typedef int (__cdecl *tm_s_t)(struct tm *, const long long *);
static tm_s_t p_gm, p_loc;
static const long long tbig = 0x100000000000LL;

static int all_minus1(const struct tm *t)
{ return t->tm_sec == -1 && t->tm_min == -1 && t->tm_hour == -1 && t->tm_mday == -1 && t->tm_mon == -1 &&
         t->tm_year == -1 && t->tm_wday == -1 && t->tm_yday == -1 && t->tm_isdst == -1; }

static void null_checks(tm_s_t f)
{
    struct tm t; long long v = 0; int r; volatile long long *pv = &v; const long long *nullt = NULL;
    if (!o_install()) return;
    o_reset();
    r = f(NULL, (const long long *)pv);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(o_iph_count, 1);
    memset(&t, 0, sizeof t);
    r = f(&t, nullt);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(o_iph_count, 2); CHECK(all_minus1(&t));
    o_isolated();
}
static void range_check(tm_s_t f)
{
    struct tm t; long long v = tbig; int r;
    memset(&t, 0, sizeof t);
    r = f(&t, &v);
    CHECK_EQ_I(r, EINVAL); CHECK(all_minus1(&t));
}
TC_CASE(gm_null) { if (!p_gm) TC_SKIP("not exported"); null_checks(p_gm); }
TC_CASE(gm_range) { if (!p_gm) TC_SKIP("not exported"); if (!o_install()) TC_SKIP("no handler api"); range_check(p_gm); }
TC_CASE(loc_null) { if (!p_loc) TC_SKIP("not exported"); null_checks(p_loc); }
TC_CASE(loc_range) { if (!p_loc) TC_SKIP("not exported"); if (!o_install()) TC_SKIP("no handler api"); range_check(p_loc); }
int main(void)
{
    tc_begin();
    p_gm = (tm_s_t)tc_crt_proc("_o__gmtime64_s"); p_loc = (tm_s_t)tc_crt_proc("_o__localtime64_s");
    TC_RUN(gm_null); TC_RUN(gm_range); TC_RUN(loc_null); TC_RUN(loc_range);
    return tc_end();
}
