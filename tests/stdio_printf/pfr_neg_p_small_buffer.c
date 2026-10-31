/* TC
 * id:    pfr_neg_p_small_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _sprintf_p, _scprintf_p, __stdio_common_vsprintf_p
 * crt:   >=80
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _printf_p, _sprintf_p, _scprintf_p (positional parameters; parameter validation)
 * note:  Returns -1, EINVAL and calls the invalid-parameter handler for NULL buffer / NULL format.
 *        The too-small-buffer behaviour (return, handler, ERANGE, buffer state) is not clearly documented for the _p
 *        functions and is probed in pfr_probe_p_small_buffer_errno.
 *        ucrtbase has no _sprintf_p export; the test calls __stdio_common_vsprintf_p as the inline wrapper does.
 */
#include "tc.h"
#include <errno.h>

#include <stdarg.h>

typedef int (__cdecl *sprintf_p_t)(char *, size_t, const char *, ...);
typedef int (__cdecl *scprintf_p_t)(const char *, ...);
static sprintf_p_t p_sprintf_p;
static scprintf_p_t p_scprintf_p;
#ifdef TC_UCRT
/* ucrtbase does not export _sprintf_p/_scprintf_p (inline wrappers over __stdio_common_vsprintf_p): call the export. */
typedef int (__cdecl *vsp_p_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
static vsp_p_t p_vsp_p;
static int ucrt_sprintf_p(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsp_p(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int ucrt_scprintf_p(const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsp_p(2, NULL, 0, f, NULL, ap); va_end(ap); return r; }
#endif
static void p_setup(void)
{
#ifdef TC_UCRT
    p_vsp_p = (vsp_p_t)tc_crt_proc("__stdio_common_vsprintf_p");
    if (p_vsp_p) { p_sprintf_p = ucrt_sprintf_p; p_scprintf_p = ucrt_scprintf_p; }
#else
    p_sprintf_p = (sprintf_p_t)tc_crt_proc("_sprintf_p");
    p_scprintf_p = (scprintf_p_t)tc_crt_proc("_scprintf_p");
#endif
}

TC_CASE(null_buffer)
{
    char *volatile nb = NULL; int r;
    if (!p_sprintf_p) TC_SKIP("_sprintf_p not exported");
    tc_iph_count = 0; errno = 0;
    r = p_sprintf_p(nb, 8, "abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(null_format)
{
    char b[8]; const char *volatile nf = NULL; int r;
    if (!p_sprintf_p) TC_SKIP("_sprintf_p not exported");
    tc_iph_count = 0; errno = 0;
    r = p_sprintf_p(b, 8, nf);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(scprintf_null)
{
    const char *volatile nf = NULL; int r;
    if (!p_scprintf_p) TC_SKIP("_scprintf_p not exported");
    tc_iph_count = 0; errno = 0;
    r = p_scprintf_p(nf);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_setup();
    TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(scprintf_null);
    return tc_end();
}
