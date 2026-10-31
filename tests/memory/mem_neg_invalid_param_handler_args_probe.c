/* TC
 * id:    mem_neg_invalid_param_handler_args_probe
 * area:  memory
 * funcs:
 * procs: memcpy_s _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler (release builds pass NULL/0 arguments)
 * note:  PROBE: the arguments ucrtbase's memcpy_s passes to the invalid-parameter handler. Release CRTs call _invalid_parameter_noinfo so expression/function/file are NULL, line 0, reserved 0. Asserted only: handler called once with a consistent triple (all NULL or all non-NULL). Actual values are logged as TC INFO lines.
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

TC_DECL_PROC(int, memcpy_s, (void *, size_t, const void *, size_t));
typedef void (__cdecl *ih_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
static ih_t (__cdecl *p_set)(ih_t);

static int calls; static const wchar_t *ge, *gf, *gfl; static unsigned int gl; static uintptr_t gr;
static void __cdecl h(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ calls++; ge = e; gf = f; gfl = fl; gl = l; gr = r; }

static void logv(const char *name, unsigned long long v)
{ char b[32]; tc_write("TC INFO " TC_TEST_ID " "); tc_write(name); tc_write("="); tc_hex(v, b); tc_write(b); tc_write("\n"); }

TC_CASE(args_consistent)
{
    volatile size_t n = 4; int r;
    if (!tc_p_memcpy_s || !p_set) TC_SKIP("missing");
    calls = 0;
    r = tc_p_memcpy_s(NULL, 8, "abcd", n);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(calls, 1);
    logv("expression", (unsigned long long)(ULONG_PTR)ge); logv("function", (unsigned long long)(ULONG_PTR)gf);
    logv("file", (unsigned long long)(ULONG_PTR)gfl); logv("line", gl); logv("reserved", gr);
    CHECK((ge == NULL && gf == NULL && gfl == NULL) || (ge != NULL && gf != NULL && gfl != NULL));
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(memcpy_s);
    p_set = (ih_t (__cdecl *)(ih_t))tc_crt_proc("_set_invalid_parameter_handler");
    if (p_set) p_set(h);
    TC_RUN(args_consistent);
    return tc_end();
}
