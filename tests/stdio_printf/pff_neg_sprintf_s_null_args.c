/* TC
 * id:    pff_neg_sprintf_s_null_args
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s __stdio_common_vsprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s: NULL buffer or format invokes the invalid parameter handler, returns -1
 * note:  buffer content beyond [0] must not be modified
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

TC_CASE(null_buffer)
{
    int n; volatile double v = 1.5; char *volatile p = NULL;
    tc_iph_count = 0;
    n = xsprintf_s(p, 10, "%f", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[16]; int n, i; const char *volatile f = NULL;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_count = 0;
    n = xsprintf_s(b, 10, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
    for (i = 1; i < 16; i++) CHECK_EQ_I(b[i], 'Z');
}
int main(void)
{
    tc_begin(); CHECK(LOAD_SPRINTF_S()); CHECK(tc_iph_install());
    TC_RUN(null_buffer); TC_RUN(null_format);
    return tc_end();
}
