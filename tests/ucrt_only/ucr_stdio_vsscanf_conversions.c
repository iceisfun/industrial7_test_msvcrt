/* TC
 * id:    ucr_stdio_vsscanf_conversions
 * area:  ucrt_only
 * funcs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2; MS docs scanf Width Specification / Type Field Characters
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <errno.h>

static int vsc(unsigned long long opts, const char *in, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsscanf(opts, in, (size_t)-1, f, NULL, ap); va_end(ap);
    return r;
}

TC_CASE(suppress) { int v = 0; int r = vsc(0, "5 6", "%*d %d", &v); CHECK_EQ_I(r, 1); CHECK_EQ_I(v, 6); }
TC_CASE(n_not_counted) { int a = 0, n = -1; int r = vsc(0, "12 34", "%d%n", &a, &n); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(n, 2); }
TC_CASE(scanset) { char a[16] = "", b[16] = ""; int r = vsc(0, "abcd", "%[a-c]", a); CHECK_EQ_I(r, 1); CHECK_STR(a, "abc");
    r = vsc(0, "hello,w", "%[^,]", b); CHECK_EQ_I(r, 1); CHECK_STR(b, "hello"); }
TC_CASE(width_s) { char s[16] = ""; int r = vsc(0, "abcdefgh", "%5s", s); CHECK_EQ_I(r, 1); CHECK_STR(s, "abcde"); }
TC_CASE(bases) { unsigned x = 0, o = 0; int i1 = 0, i2 = 0, i3 = 0;
    CHECK_EQ_I(vsc(0, "0x1f", "%x", &x), 1); CHECK_EQ_U(x, 31);
    CHECK_EQ_I(vsc(0, "017", "%o", &o), 1); CHECK_EQ_U(o, 15);
    CHECK_EQ_I(vsc(0, "0x1f", "%i", &i1), 1); CHECK_EQ_I(i1, 31);
    CHECK_EQ_I(vsc(0, "017", "%i", &i2), 1); CHECK_EQ_I(i2, 15);
    CHECK_EQ_I(vsc(0, "17", "%i", &i3), 1); CHECK_EQ_I(i3, 17); }
TC_CASE(size_prefix) { signed char c = 0; short h = 0; long long ll = 0, i64 = 0;
    CHECK_EQ_I(vsc(0, "-5", "%hhd", &c), 1); CHECK_EQ_I(c, -5);
    CHECK_EQ_I(vsc(0, "-1234", "%hd", &h), 1); CHECK_EQ_I(h, -1234);
    CHECK_EQ_I(vsc(0, "-9000000000000", "%lld", &ll), 1); CHECK_EQ_I(ll, -9000000000000LL);
    CHECK_EQ_I(vsc(0, "9000000000000", "%I64d", &i64), 1); CHECK_EQ_I(i64, 9000000000000LL); }
TC_CASE(percent_literal) { int v = 0; CHECK_EQ_I(vsc(0, "50%", "%d%%", &v), 1); CHECK_EQ_I(v, 50); }

int main(void)
{
    tc_begin();
    TC_RUN(suppress);
    TC_RUN(n_not_counted);
    TC_RUN(scanset);
    TC_RUN(width_s);
    TC_RUN(bases);
    TC_RUN(size_prefix);
    TC_RUN(percent_literal);
    return tc_end();
}
