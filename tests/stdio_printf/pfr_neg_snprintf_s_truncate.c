/* TC
 * id:    pfr_neg_snprintf_s_truncate
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vsnprintf_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s, _vsnprintf_s: with count == _TRUNCATE the string is truncated, NUL-terminated, returns -1, no invalid-parameter call
 * note:  errno (STRUNCATE) is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

TC_DECL_PROC(int, _vsnprintf_s, (char *, size_t, size_t, const char *, va_list));

#define G 0x5A
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
static void fill(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = G; }


/* ucrtbase: mingw inlines the *_s printf family onto __stdio_common_vsnprintf_s; older CRTs: call the DLL export. */
static int w_snprintf_s(char *b, size_t n, size_t c, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f);
#if TC_CRT_VER >= 140
    r = _vsnprintf_s(b, n, c, f, ap);
#else
    r = tc_p__vsnprintf_s(b, n, c, f, ap);
#endif
    va_end(ap); return r;
}
static int w_vsnprintf_s(char *b, size_t n, size_t c, const char *f, va_list ap)
{
#if TC_CRT_VER >= 140
    return _vsnprintf_s(b, n, c, f, ap);
#else
    return tc_p__vsnprintf_s(b, n, c, f, ap);
#endif
}

static int cv(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = w_vsnprintf_s(b, n, c, f, ap); va_end(ap); return r; }

TC_CASE(snprintf_s_trunc)
{
    char b[16]; volatile size_t n = 4; int r;
    fill(b, 16); tc_iph_count = 0;
    r = w_snprintf_s(b, n, _TRUNCATE, "%s", "abcdef");
    CHECK_EQ_I(r, -1);
    CHECK_STR(b, "abc");
    CHECK_EQ_I((unsigned char)b[4], G);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(vsnprintf_s_trunc)
{
    char b[16]; volatile size_t n = 4; int r;
    fill(b, 16); tc_iph_count = 0;
    r = cv(b, n, _TRUNCATE, "%s", "abcdef");
    CHECK_EQ_I(r, -1);
    CHECK_STR(b, "abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(size1_truncate)
{
    char b[4]; volatile size_t n = 1; int r;
    fill(b, 4); tc_iph_count = 0;
    r = w_snprintf_s(b, n, _TRUNCATE, "%s", "abc");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(b[0], 0);
    CHECK_EQ_I((unsigned char)b[1], G);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(truncate_fits)
{
    char b[16]; volatile size_t n = 16; int r;
    fill(b, 16); tc_iph_count = 0;
    r = w_snprintf_s(b, n, _TRUNCATE, "%s", "abc");
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_vsnprintf_s);
#if TC_CRT_VER < 140
    if (!tc_p__vsnprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(snprintf_s_trunc); TC_RUN(vsnprintf_s_trunc); TC_RUN(size1_truncate); TC_RUN(truncate_fits);
    return tc_end();
}
