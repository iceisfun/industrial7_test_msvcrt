/* TC
 * id:    pff_neg_sprintf_s_e_overflow
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s __stdio_common_vsprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s: buffer too small -> invalid parameter handler, returns -1, buffer[0]=0, errno ERANGE
 * note:  Floating-point conversions (%e, %f, %g) that do not fit the destination. Output "1.500000e+00" is 12 chars on ucrt (13 on older CRTs); buffer sizes below are well under that.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>

#if TC_CRT_VER >= 140
typedef int (__cdecl *vsprintf_s_t)(unsigned __int64, char *, size_t, const char *, void *, va_list);
static vsprintf_s_t p_common;
static int xsprintf_s(char *b, size_t n, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = p_common(0, b, n, f, NULL, ap); va_end(ap); return r; }
#define HAVE_SPRINTF_S() ((p_common = (vsprintf_s_t)tc_crt_proc("__stdio_common_vsprintf_s")) != NULL)
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define xsprintf_s tc_p_sprintf_s
#define HAVE_SPRINTF_S() (TC_LOAD_PROC(sprintf_s) != NULL)
#endif

static int p_ok;
static volatile double v = 1.5;
TC_CASE(e_too_small)
{
    char b[16]; int n, i; if (!p_ok) TC_SKIP("none"); for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0; errno = 0;
    n = xsprintf_s(b, 8, "%e", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0);
}
TC_CASE(f_wide_precision_too_small)
{
    char b[16]; int n, i; if (!p_ok) TC_SKIP("none"); for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0; errno = 0;
    n = xsprintf_s(b, 10, "%.20f", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0);
}
TC_CASE(g_width_too_small)
{
    char b[16]; int n, i; if (!p_ok) TC_SKIP("none"); for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0; errno = 0;
    n = xsprintf_s(b, 4, "%10g", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0);
}
TC_CASE(fits_exactly_ok)
{
    char b[16]; int n; if (!p_ok) TC_SKIP("none");
    tc_iph_count = 0;
    n = xsprintf_s(b, 4, "%g", v);   /* "1.5" + NUL == 4 */
    CHECK_EQ_I(n, 3); CHECK_STR(b, "1.5"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(one_short_fails)
{
    char b[16]; int n; if (!p_ok) TC_SKIP("none");
    tc_iph_count = 0; errno = 0;
    n = xsprintf_s(b, 3, "%g", v);   /* needs 4 with NUL */
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); p_ok = HAVE_SPRINTF_S(); CHECK(p_ok);
  TC_RUN(e_too_small); TC_RUN(f_wide_precision_too_small); TC_RUN(g_width_too_small); TC_RUN(fits_exactly_ok); TC_RUN(one_short_fails); return tc_end(); }
