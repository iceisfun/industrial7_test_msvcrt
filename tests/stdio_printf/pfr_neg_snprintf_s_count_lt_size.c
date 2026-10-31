/* TC
 * id:    pfr_neg_snprintf_s_count_lt_size
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vsnprintf_s _set_invalid_parameter_handler
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s: count < size and output longer than count -> first count chars written, NUL-terminated, returns -1, no handler; count == len returns len
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

TC_DECL_PROC(int, _vsnprintf_s, (char *, size_t, size_t, const char *, va_list));

#define G 0x5A
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

TC_CASE(count3_of_6)
{
    char b[16]; volatile size_t n = 10, c = 3; int r;
    fill(b, 16); tc_iph_count = 0;
    r = w_snprintf_s(b, n, c, "%s", "abcdef");
    CHECK_EQ_I(r, -1);
    CHECK_STR(b, "abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_equals_len)
{
    char b[16]; volatile size_t n = 10, c = 6; int r;
    fill(b, 16); tc_iph_count = 0;
    r = w_snprintf_s(b, n, c, "%s", "abcdef");
    CHECK_EQ_I(r, 6);
    CHECK_STR(b, "abcdef");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_one_past_len)
{
    char b[16]; volatile size_t n = 10, c = 5; int r;
    fill(b, 16); tc_iph_count = 0;
    r = w_snprintf_s(b, n, c, "%s", "abcdef");
    CHECK_EQ_I(r, -1);
    CHECK_STR(b, "abcde");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_zero)
{
    char b[16]; volatile size_t n = 10, c = 0; int r;
    fill(b, 16); tc_iph_count = 0;
    r = w_snprintf_s(b, n, c, "%s", "abc");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(b[0], 0);
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
    TC_RUN(count3_of_6); TC_RUN(count_equals_len); TC_RUN(count_one_past_len); TC_RUN(count_zero);
    return tc_end();
}
