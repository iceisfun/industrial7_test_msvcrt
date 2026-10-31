/* TC
 * id:    err_neg_iph_handler_receives_null_after_clear_thread
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: strcpy_s _set_thread_local_invalid_parameter_handler _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_thread_local_invalid_parameter_handler: a thread-local handler takes precedence; NULL clears it so the global handler is used
 * note:  Skips (not fails) on a CRT that does not export _set_thread_local_invalid_parameter_handler.
 */
#include "tc.h"
#include <errno.h>

typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
TC_DECL_PROC(iph_t, _set_thread_local_invalid_parameter_handler, (iph_t));
TC_DECL_PROC(iph_t, _set_invalid_parameter_handler, (iph_t));
TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));

static int local_count;
static void __cdecl local_iph(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; local_count++; }

static void bad(void) { char d[4]; volatile size_t n = 0; tc_p_strcpy_s(d, n, "x"); }

TC_CASE(clear_falls_back_to_global)
{
    if (!tc_p__set_thread_local_invalid_parameter_handler) TC_SKIP("thread-local handler API missing");
    tc_p__set_invalid_parameter_handler(tc_iph);
    tc_p__set_thread_local_invalid_parameter_handler(local_iph);
    tc_iph_count = 0; local_count = 0;
    bad();
    CHECK_EQ_I(local_count, 1); CHECK_EQ_I(tc_iph_count, 0);
    tc_p__set_thread_local_invalid_parameter_handler(NULL);
    bad();
    CHECK_EQ_I(local_count, 1); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_thread_local_invalid_parameter_handler); TC_LOAD_PROC(_set_invalid_parameter_handler); TC_LOAD_PROC(strcpy_s);
    if (!tc_p__set_invalid_parameter_handler || !tc_p_strcpy_s) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(clear_falls_back_to_global);
    errno = 0;
    return tc_end();
}
