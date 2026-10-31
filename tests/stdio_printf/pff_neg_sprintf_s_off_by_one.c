/* TC
 * id:    pff_neg_sprintf_s_off_by_one
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s __stdio_common_vsprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs sprintf_s: buffer must hold the terminating NUL
 * note:  too-small buffer -> handler, -1, buffer[0]==0 is real behaviour (not all spelled out on the sprintf_s page). size boundary: %f of 1.5 is 8 chars
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

static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }

TC_CASE(size_8_fails)
{
    char b[16]; int n; volatile double v = 1.5; volatile size_t sz = 8; fill(b);
    tc_iph_count = 0;
    n = xsprintf_s(b, sz, "%f", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(size_9_ok)
{
    char b[16]; int n; volatile double v = 1.5; volatile size_t sz = 9; fill(b);
    tc_iph_count = 0;
    n = xsprintf_s(b, sz, "%f", v);
    CHECK_EQ_I(n, 8); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "1.500000");
}
TC_CASE(size_1_fails)
{
    char b[16]; int n; volatile double v = 1.5; volatile size_t sz = 1; fill(b);
    tc_iph_count = 0;
    n = xsprintf_s(b, sz, "%f", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(size_2_rounded)
{
    char b[16]; int n; volatile double v = 2.7; volatile size_t sz = 2; fill(b);
    tc_iph_count = 0;
    n = xsprintf_s(b, sz, "%.0f", v);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "3");
}
int main(void)
{
    tc_begin(); CHECK(LOAD_SPRINTF_S()); CHECK(tc_iph_install());
    TC_RUN(size_8_fails); TC_RUN(size_9_ok); TC_RUN(size_1_fails); TC_RUN(size_2_rounded);
    return tc_end();
}
