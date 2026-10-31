/* TC
 * id:    pff_neg_sprintf_s_errno_probe
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s __stdio_common_vsprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf_s: errno values
 * note:  probes errno: ERANGE for too-small buffer, EINVAL for NULL buffer / NULL format (documented return -1 is asserted in other tests)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#if TC_CRT_VER >= 140
typedef int (__cdecl *vsprintf_s_t)(unsigned __int64, char *, size_t, const char *, void *, va_list);
static vsprintf_s_t p_common;
static int xsprintf_s(char *b, size_t n, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = p_common(0, b, n, f, NULL, ap); va_end(ap); return r; }
#define LOAD_SPRINTF_S() ((p_common = (vsprintf_s_t)tc_crt_proc("__stdio_common_vsprintf_s")) != NULL)
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define xsprintf_s tc_p_sprintf_s
#define LOAD_SPRINTF_S() (TC_LOAD_PROC(sprintf_s) != NULL)
#endif
#include <errno.h>

TC_CASE(too_small_erange)
{
    char b[16]; int n; volatile double v = 1.5; volatile size_t sz = 4;
    tc_iph_count = 0; errno = 0;
    n = xsprintf_s(b, sz, "%f", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(null_buffer_einval)
{
    int n; volatile double v = 1.5; char *volatile p = NULL;
    tc_iph_count = 0; errno = 0;
    n = xsprintf_s(p, 10, "%f", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(null_format_einval)
{
    char b[16]; int n; const char *volatile f = NULL;
    tc_iph_count = 0; errno = 0;
    n = xsprintf_s(b, sizeof b, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin(); CHECK(LOAD_SPRINTF_S()); CHECK(tc_iph_install());
    TC_RUN(too_small_erange); TC_RUN(null_buffer_einval); TC_RUN(null_format_einval);
    return tc_end();
}
