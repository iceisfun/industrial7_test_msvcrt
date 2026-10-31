/* TC
 * id:    scn_common_vsscanf_default
 * area:  scanf
 * funcs: __stdio_common_vsscanf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   corecrt_stdio_config.h / stdio.h (__stdio_common_vsscanf, options bitmask _CRT_INTERNAL_SCANF_*); MS docs sscanf
 * note:  Direct calls with options = 0 (no secure flag, no legacy flags). length (size_t)-1 means NUL-terminated input,
 *        the value the inline sscanf wrapper passes. Locale NULL means the current locale.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int cs(const char *in, const char *fmt, ...)
{
    va_list ap; int r; va_start(ap, fmt);
    r = __stdio_common_vsscanf(0, in, (size_t)-1, fmt, NULL, ap);
    va_end(ap); return r;
}
static const char *volatile vin;

TC_CASE(int_str)
{
    int a = 0, n; char s[16];
    vin = "123 word";
    n = cs((const char *)vin, "%d %s", &a, s);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 123); CHECK_STR(s, "word");
}
TC_CASE(returns_count)
{
    int a = 0, b = 9, n;
    vin = "1 q";
    n = cs((const char *)vin, "%d %d", &a, &b);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 1); CHECK_EQ_I(b, 9);
}
TC_CASE(eof_empty)
{
    int a = 4, n;
    vin = "";
    n = cs((const char *)vin, "%d", &a);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(a, 4);
}
TC_CASE(no_conversions)
{
    int n;
    vin = "abc";
    n = cs((const char *)vin, "abc");
    CHECK_EQ_I(n, 0);
}
TC_CASE(hex_and_n)
{
    unsigned x = 0; int n = -1, r;
    vin = "ff!";
    r = cs((const char *)vin, "%x%n", &x, &n);
    CHECK_EQ_I(r, 1); CHECK_EQ_U(x, 255); CHECK_EQ_I(n, 2);
}
TC_CASE(double_v)
{
    double d = 0; int r;
    vin = "2.5";
    r = cs((const char *)vin, "%lf", &d);
    CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 2.5);
}
int main(void)
{
    tc_begin();
    TC_RUN(int_str); TC_RUN(returns_count); TC_RUN(eof_empty); TC_RUN(no_conversions); TC_RUN(hex_and_n); TC_RUN(double_v);
    return tc_end();
}
