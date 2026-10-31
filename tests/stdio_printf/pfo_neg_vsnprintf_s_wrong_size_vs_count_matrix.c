/* TC
 * id:    pfo_neg_vsnprintf_s_wrong_size_vs_count_matrix
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsnprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s (count >= size and string too long: buffer[0]=0, invalid parameter, ERANGE; count < size: truncated at count, returns -1)
 * note:  Zero-count / zero-size edge cases are in pfo_neg_probe_vsnprintf_s_zero_cases.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int cn(char *b, size_t n, size_t m, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsnprintf_s(0, b, n, m, f, NULL, ap); va_end(ap); return r; }

TC_CASE(count_equals_size_overflow)
{
    char b[8] = "zzzzzzz"; int r; tc_iph_count = 0; errno = 0;
    r = cn(b, 4, 4, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(count_less_than_size_truncates)
{
    char b[8]; int r; tc_iph_count = 0;
    r = cn(b, 4, 3, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_exceeds_size_overflow)
{
    char b[8] = "zzzzzzz"; int r; tc_iph_count = 0; errno = 0;
    r = cn(b, 4, 100, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(count_equals_size_overflow); TC_RUN(count_less_than_size_truncates); TC_RUN(count_exceeds_size_overflow);
    return tc_end();
}
