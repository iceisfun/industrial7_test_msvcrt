/* TC
 * id:    ucr_stdio_vsscanf_securecrt_s
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs scanf_s, scanf_s_l, wscanf_s, wscanf_s_l (the _s variants require a buffer size after each %s/%c/%[ argument); corecrt_stdio_config.h _CRT_INTERNAL_SCANF_SECURECRT = 0x1
 * note:  __stdio_common_vsscanf(options, buffer, buffer_count, format, locale, va_list) is the UCRT back end of sscanf/sscanf_s. Option bit 0x1 selects the secure (_s) argument convention.
 */
#include "tc.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef int (__cdecl *vsscanf_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
static vsscanf_t p_vss;
#define SECURECRT 0x1ULL

static int xs(unsigned long long opt, const char *in, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = p_vss(opt, in, (size_t)-1, fmt, NULL, ap);
    va_end(ap);
    return r;
}

TC_CASE(s_with_size)
{
    char b[16]; int r;
    memset(b, 'z', sizeof b);
    r = xs(SECURECRT, "hello world", "%s", b, 16u);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "hello");
}
TC_CASE(c_with_size)
{
    char b[4] = { 'z', 'z', 'z', 'z' }; int r;
    r = xs(SECURECRT, "xyz", "%c", b, 1u);
    CHECK_EQ_I(r, 1); CHECK(b[0] == 'x'); CHECK(b[1] == 'z');
}
TC_CASE(c_with_size_multi)
{
    char b[4] = { 'z', 'z', 'z', 'z' }; int r;
    r = xs(SECURECRT, "abcdef", "%3c", b, 3u);
    CHECK_EQ_I(r, 1); CHECK(b[0] == 'a' && b[1] == 'b' && b[2] == 'c' && b[3] == 'z');
}
TC_CASE(set_with_size)
{
    char b[16]; int r;
    memset(b, 'z', sizeof b);
    r = xs(SECURECRT, "abcXYZ", "%[a-z]", b, 16u);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "abc");
}
TC_CASE(mixed)
{
    int n = 0; char b[16]; int r;
    r = xs(SECURECRT, "42 word", "%d %s", &n, b, 16u);
    CHECK_EQ_I(r, 2); CHECK_EQ_I(n, 42); CHECK_STR(b, "word");
}
TC_CASE(exact_fit)
{
    char b[4]; int r;   /* "abc" + NUL needs exactly 4 */
    r = xs(SECURECRT, "abc", "%s", b, 4u);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "abc");
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vss = (vsscanf_t)tc_crt_proc("__stdio_common_vsscanf");
    if (!p_vss) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(s_with_size); TC_RUN(c_with_size); TC_RUN(c_with_size_multi); TC_RUN(set_with_size); TC_RUN(mixed); TC_RUN(exact_fit);
    return tc_end();
}
