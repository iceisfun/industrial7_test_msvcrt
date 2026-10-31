/* TC
 * id:    pfl_neg_thread_local_iph_l_variants
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsprintf_l _sprintf_s_l _create_locale _free_locale
 * procs: _set_invalid_parameter_handler _set_thread_local_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_thread_local_invalid_parameter_handler, _set_invalid_parameter_handler (thread-local handler takes precedence over the global one; NULL resets it)
 * note:  Cases skip if the thread-local setter is not exported. Handlers are restored at the end.
 */
#include "tc.h"
#include "pfl_shim.h"
#include <errno.h>

typedef void (__cdecl *iph_fn)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
TC_DECL_PROC(iph_fn, _set_invalid_parameter_handler, (iph_fn));
TC_DECL_PROC(iph_fn, _set_thread_local_invalid_parameter_handler, (iph_fn));

static int cnt_a, cnt_b;
static void __cdecl hA(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e; (void)f; (void)fl; (void)l; (void)r; cnt_a++; }
static void __cdecl hB(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e; (void)f; (void)fl; (void)l; (void)r; cnt_b++; }

static _locale_t loc;
static const char * volatile v_nof;

TC_CASE(tl_wins_for_null_format)
{
    char b[8]; int r;
    if (!tc_p__set_thread_local_invalid_parameter_handler) TC_SKIP("no thread-local setter");
    tc_p__set_invalid_parameter_handler(hA); tc_p__set_thread_local_invalid_parameter_handler(hB);
    cnt_a = cnt_b = 0; errno = 0;
    r = pl_sprintf_l(b, v_nof, loc, 1);
    CHECK(r < 0);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(cnt_b, 1); CHECK_EQ_I(cnt_a, 0);
}
TC_CASE(tl_wins_for_s_l_overflow)
{
    char b[4]; int r;
    if (!tc_p__set_thread_local_invalid_parameter_handler) TC_SKIP("no thread-local setter");
    tc_p__set_invalid_parameter_handler(hA); tc_p__set_thread_local_invalid_parameter_handler(hB);
    cnt_a = cnt_b = 0; errno = 0;
    r = _sprintf_s_l(b, sizeof b, "%s", loc, "toolong");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(cnt_b, 1); CHECK_EQ_I(cnt_a, 0);
}
TC_CASE(reset_falls_back_to_global)
{
    char b[8]; int r;
    if (!tc_p__set_thread_local_invalid_parameter_handler) TC_SKIP("no thread-local setter");
    tc_p__set_invalid_parameter_handler(hA); tc_p__set_thread_local_invalid_parameter_handler(hB);
    tc_p__set_thread_local_invalid_parameter_handler(NULL);
    cnt_a = cnt_b = 0; errno = 0;
    r = pl_sprintf_l(b, v_nof, loc, 1);
    CHECK(r < 0);
    CHECK_EQ_I(cnt_a, 1); CHECK_EQ_I(cnt_b, 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_set_invalid_parameter_handler); TC_LOAD_PROC(_set_thread_local_invalid_parameter_handler);
    CHECK(tc_p__set_invalid_parameter_handler != NULL);
    loc = _create_locale(LC_ALL, "C");
    if (!loc || !tc_p__set_invalid_parameter_handler) { tc_write("TC FAIL " TC_TEST_ID "::main setup failed\n"); return 1; }
    TC_RUN(tl_wins_for_null_format); TC_RUN(tl_wins_for_s_l_overflow); TC_RUN(reset_falls_back_to_global);
    if (tc_p__set_thread_local_invalid_parameter_handler) tc_p__set_thread_local_invalid_parameter_handler(NULL);
    tc_p__set_invalid_parameter_handler(tc_iph);
    _free_locale(loc);
    return tc_end();
}
