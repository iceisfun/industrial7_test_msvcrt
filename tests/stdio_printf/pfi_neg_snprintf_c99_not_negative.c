/* TC
 * id:    pfi_neg_snprintf_c99_not_negative
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * note:  snprintf is an inline wrapper in ucrt headers calling __stdio_common_vsprintf with
 *        _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR; the wrapper below does the same.
 * ref:   C11 7.21.6.5; MS docs snprintf, _snprintf (snprintf returns required length, always NUL-terminates)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int snprintf_c99(char *b, size_t n, const char *f, ...)
{
    int r; va_list ap; va_start(ap, f);
    r = __stdio_common_vsprintf(0x2ULL /* STANDARD_SNPRINTF_BEHAVIOR */, b, n, f, NULL, ap);
    va_end(ap); return r;
}
#define snprintf snprintf_c99

TC_CASE(truncation_returns_needed)
{
    char b[16]; int n, i; for (i = 0; i < 16; i++) b[i] = 'Z';
    tc_iph_install(); tc_iph_count = 0;
    n = snprintf(b, 3, "%d", 12345);
    CHECK_EQ_I(n, 5);
    CHECK_STR(b, "12");
    CHECK(b[3] == 'Z');
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_zero_null_buffer)
{
    int n;
    tc_iph_install(); tc_iph_count = 0;
    n = snprintf(NULL, 0, "%d", 12345);
    CHECK_EQ_I(n, 5);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(truncation_returns_needed); TC_RUN(count_zero_null_buffer); return tc_end(); }
