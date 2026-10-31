/* TC
 * id:    pff_neg_sprintf_s_f_overflow
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s __stdio_common_vsprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s: buffer too small -> invalid parameter handler, returns -1, buffer[0]=0, errno ERANGE
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
TC_CASE(too_small)
{   /* "1.500000" needs 9 bytes with NUL */
    char b[16]; int n, i; volatile double v = 1.5; if (!p_ok) TC_SKIP("none"); for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0; errno = 0;
    n = xsprintf_s(b, 8, "%f", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0);
}
TC_CASE(exact_fit)
{
    char b[16]; int n; volatile double v = 1.5; if (!p_ok) TC_SKIP("none");
    tc_iph_count = 0; errno = 0;
    n = xsprintf_s(b, 9, "%f", v);
    CHECK_EQ_I(n, 8); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "1.500000");
}
TC_CASE(huge_value)
{
    char b[16]; int n; volatile double v = 1e300; if (!p_ok) TC_SKIP("none");
    tc_iph_count = 0; errno = 0;
    n = xsprintf_s(b, 16, "%f", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); p_ok = HAVE_SPRINTF_S(); CHECK(p_ok);
  TC_RUN(too_small); TC_RUN(exact_fit); TC_RUN(huge_value); return tc_end(); }
