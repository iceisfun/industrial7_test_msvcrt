/* TC
 * id:    pff_neg_sprintf_s_zero_size
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s __stdio_common_vsprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs sprintf_s: sizeOfBuffer 0 is an invalid parameter
 * note:  buffer must be untouched. Zero sizeOfBuffer -> EINVAL + handler is real behaviour but not spelled out on the sprintf_s page.
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

TC_CASE(zero_size)
{
    char b[8]; int n, i; volatile double v = 1.5; volatile size_t sz = 0;
    for (i = 0; i < 8; i++) b[i] = 'Z';
    tc_iph_count = 0;
    n = xsprintf_s(b, sz, "%f", v);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 'Z');
}
int main(void)
{
    tc_begin(); CHECK(LOAD_SPRINTF_S()); CHECK(tc_iph_install());
    TC_RUN(zero_size);
    return tc_end();
}
