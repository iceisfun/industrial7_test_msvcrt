/* TC
 * id:    pfo_vsprintf_basic
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs __stdio_common_vsprintf (corecrt_stdio_config.h); C11 7.21.6.1
 * note:  options=0, buffer large enough; return count excludes NUL.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

TC_CASE(int_str_char)
{
    char b[64]; int n;
    n = vs(0, b, sizeof b, "%d|%s|%c", (int)vi, "xyz", 'Q');
    CHECK_STR(b, "42|xyz|Q"); CHECK_EQ_I(n, 8);
}
TC_CASE(nul_excluded)
{
    char b[16]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(0, b, sizeof b, "abc");
    CHECK_EQ_I(n, 3); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 'Z');
}
TC_CASE(exact_buffer)
{
    char b[6]; int n;
    n = vs(0, b, 6, "hello");
    CHECK_EQ_I(n, 5); CHECK_STR(b, "hello"); CHECK_EQ_I(b[5], 0);
}
TC_CASE(width_prec_star)
{
    char b[64];
    vs(0, b, sizeof b, "[%5d][%-5d][%.3s][%*d][%.*s]", 42, 42, "abcdef", 4, 7, 2, "hello");
    CHECK_STR(b, "[   42][42   ][abc][   7][he]");
}
TC_CASE(percent_literal)
{
    char b[16]; int n = vs(0, b, sizeof b, "100%%");
    CHECK_STR(b, "100%"); CHECK_EQ_I(n, 4);
}
TC_CASE(empty_format)
{
    char b[8]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(0, b, sizeof b, "");
    CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(int_str_char); TC_RUN(nul_excluded); TC_RUN(exact_buffer); TC_RUN(width_prec_star);
    TC_RUN(percent_literal); TC_RUN(empty_format);
    return tc_end();
}
