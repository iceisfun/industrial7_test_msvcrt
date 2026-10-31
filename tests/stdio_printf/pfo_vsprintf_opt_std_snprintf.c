/* TC
 * id:    pfo_vsprintf_opt_std_snprintf
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   C11 7.21.6.5; MS docs snprintf, _snprintf
 * note:  options=2 (_CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR) as used by the UCRT inline snprintf in stdio.h.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

TC_CASE(trunc_returns_full_len)
{
    char b[16]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(2, b, 4, "abcdefgh");
    CHECK_EQ_I(n, 8);
}
TC_CASE(trunc_nul_terminated)
{
    char b[16];
    memset(b, 'Z', sizeof b);
    vs(2, b, 4, "abcdefgh");
    CHECK_STR(b, "abc"); CHECK_EQ_I(b[3], 0);
}
TC_CASE(sentinel_untouched)
{
    char b[16];
    memset(b, 'Z', sizeof b);
    vs(2, b, 4, "abcdefgh");
    CHECK_EQ_I(b[4], 'Z'); CHECK_EQ_I(b[15], 'Z');
}
TC_CASE(exact_fit)
{
    char b[16]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(2, b, 9, "abcdefgh");
    CHECK_EQ_I(n, 8); CHECK_STR(b, "abcdefgh"); CHECK_EQ_I(b[9], 'Z');
}
TC_CASE(size_query)
{
    int n = vs(2, NULL, 0, "%d-%s", 123, "xy");
    CHECK_EQ_I(n, 6);
}
TC_CASE(n_equals_one)
{
    char b[4]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(2, b, 1, "abc");
    CHECK_EQ_I(n, 3); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 'Z');
}
int main(void)
{
    tc_begin();
    TC_RUN(trunc_returns_full_len); TC_RUN(trunc_nul_terminated); TC_RUN(sentinel_untouched);
    TC_RUN(exact_fit); TC_RUN(size_query); TC_RUN(n_equals_one);
    return tc_end();
}
