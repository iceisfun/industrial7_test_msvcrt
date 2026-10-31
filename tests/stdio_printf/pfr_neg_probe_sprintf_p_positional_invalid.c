/* TC
 * id:    pfr_neg_probe_sprintf_p_positional_invalid
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _sprintf_p, __stdio_common_vsprintf_p
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _printf_p, _sprintf_p (positional parameters 'n$'); printf_p Positional Parameters
 * note:  PROBE: invalid positional use is rejected like any malformed format: -1, errno EINVAL, handler called once.
 *        Cases: gap ("%2$s" with no "%1$s"), position 0 ("%0$s"), mixing positional with non-positional ("%1$s %s").
 *        Extra arguments are always supplied so a CRT that did not reject the format would not read missing varargs.
 *        A valid reordered format ("%2$s%1$s" -> "ba") must still work afterwards.
 *        ucrtbase has no _sprintf_p export; the test calls __stdio_common_vsprintf_p as the inline wrapper does.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *sprintf_p_t)(char *, size_t, const char *, ...);
static sprintf_p_t p_sprintf_p;
#if TC_CRT_VER >= 140
typedef int (__cdecl *vsp_p_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
static vsp_p_t p_vsp_p;
static int ucrt_sprintf_p(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsp_p(0, b, n, f, NULL, ap); va_end(ap); return r; }
#endif

static int bad(const char *volatile f, int *err)
{
    char b[32]; int r; b[0] = 0;
    tc_iph_count = 0; errno = 0;
    r = p_sprintf_p(b, sizeof b, f, "a", "b");
    *err = errno;
    return r;
}
TC_CASE(gap)   { int e, r; if (!p_sprintf_p) TC_SKIP("no _sprintf_p"); r = bad("%2$s", &e);    CHECK_EQ_I(r, -1); CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(zero)  { int e, r; if (!p_sprintf_p) TC_SKIP("no _sprintf_p"); r = bad("%0$s", &e);    CHECK_EQ_I(r, -1); CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(mixed) { int e, r; if (!p_sprintf_p) TC_SKIP("no _sprintf_p"); r = bad("%1$s %s", &e); CHECK_EQ_I(r, -1); CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(valid_after)
{
    char b[32]; int r;
    if (!p_sprintf_p) TC_SKIP("no _sprintf_p");
    tc_iph_count = 0;
    r = p_sprintf_p(b, sizeof b, "%2$s%1$s", "a", "b");
    CHECK_EQ_I(r, 2); CHECK_STR(b, "ba"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
#if TC_CRT_VER >= 140
    p_vsp_p = (vsp_p_t)tc_crt_proc("__stdio_common_vsprintf_p");
    if (p_vsp_p) p_sprintf_p = ucrt_sprintf_p;
#else
    p_sprintf_p = (sprintf_p_t)tc_crt_proc("_sprintf_p");
#endif
    TC_RUN(gap); TC_RUN(zero); TC_RUN(mixed); TC_RUN(valid_after);
    return tc_end();
}
