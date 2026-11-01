/* TC
 * id:    exp_neg_o_itoa_s_errors
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: _o__itoa_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs _itoa_s, _ltoa_s, _ultoa_s (EINVAL for NULL buffer/zero size/bad radix, buffer[0] cleared, invalid parameter handler)
 * note:  (The old test compared later calls against a stale 'before' count: fixed to re-baseline per call.)
 *        Windows-observed (Win10 22H2): the _o_ exports run the plain function under ucrtbase's separate OS global state (wrapped_invoke/enter_os_call), so
 *        errno and the invalid-parameter handler are the OS-state ones (_o__errno, _o__set_invalid_parameter_handler); the app-state errno/handler stay untouched.
 *        Expectations are the documented errno/handler behaviour of the underlying function, checked against the OS state, plus app-state isolation.
 */
#include "tc.h"
#include <errno.h>

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

typedef int (__cdecl *itoa_s_t)(int, char *, size_t, int);
static itoa_s_t p_itoa_s;

TC_CASE(bad_radix)
{
    char b[40]; int before;
    if (!p_itoa_s) { CHECK(p_itoa_s != NULL); return; }
    before = o_iph_count;
    CHECK_EQ_I(p_itoa_s(5, b, sizeof b, 1), EINVAL);
    CHECK_EQ_I(o_iph_count, before + 1);
    before = o_iph_count;
    before = o_iph_count;   /* each call must raise exactly one more handler call */
    CHECK_EQ_I(p_itoa_s(5, b, sizeof b, 37), EINVAL);
    CHECK_EQ_I(o_iph_count, before + 1);
    o_isolated();
}
TC_CASE(small_buffer)
{
    char b[3]; int before = o_iph_count;
    if (!p_itoa_s) { CHECK(p_itoa_s != NULL); return; }
    b[0] = 'x';
    CHECK_EQ_I(p_itoa_s(12345, b, 3, 10), ERANGE);   /* observed ERANGE (34) in the OS state */
    CHECK_EQ_I(b[0], 0);
    CHECK_EQ_I(o_iph_count, before + 1);
    o_isolated();
}
TC_CASE(null_and_zero)
{
    char b[8]; int before = o_iph_count;
    if (!p_itoa_s) { CHECK(p_itoa_s != NULL); return; }
    CHECK_EQ_I(p_itoa_s(5, NULL, 8, 10), EINVAL);
    CHECK_EQ_I(o_iph_count, before + 1);
    before = o_iph_count;
    before = o_iph_count;
    CHECK_EQ_I(p_itoa_s(5, b, 0, 10), EINVAL);
    CHECK_EQ_I(o_iph_count, before + 1);
    o_isolated();
}
TC_CASE(valid_radix_control)
{
    char b[40]; int before = o_iph_count;
    if (!p_itoa_s) { CHECK(p_itoa_s != NULL); return; }
    CHECK_EQ_I(p_itoa_s(5, b, sizeof b, 2), 0);
    CHECK_STR(b, "101");
    CHECK_EQ_I(p_itoa_s(35, b, sizeof b, 36), 0);
    CHECK_STR(b, "z");
    CHECK_EQ_I(o_iph_count, before);
    o_isolated();
}
int main(void)
{
    tc_begin();
    CHECK(o_install());
    p_itoa_s = (itoa_s_t)tc_crt_proc("_o__itoa_s");
    TC_RUN(bad_radix); TC_RUN(small_buffer); TC_RUN(null_and_zero); TC_RUN(valid_radix_control);
    return tc_end();
}
