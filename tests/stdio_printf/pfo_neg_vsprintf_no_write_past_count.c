/* TC
 * id:    pfo_neg_vsprintf_no_write_past_count
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _snprintf, snprintf ("If len < count ... if len == count, no null terminator; if len > count, count chars written and -1 returned"); C11 7.21.6.5
 * note:  Replaces the planned overlap test (overlap is UB, not exercised). Checks that output never spills into the adjacent bytes of the
 *        same array. Legacy options (1 = _CRT_INTERNAL_PRINTF_LEGACY_VSPRINTF_NULL_TERMINATION, what the inline _vsnprintf passes) vs standard
 *        snprintf behaviour (2). The NUL state of the last in-range byte is NOT asserted for legacy (undocumented; see pfo_probe_vsprintf_trunc_nul).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int vs(unsigned long long opt, char *b, size_t n, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = __stdio_common_vsprintf(opt, b, n, fmt, NULL, ap);
    va_end(ap);
    return r;
}
static void fill(char *a) { int i; for (i = 0; i < 16; i++) a[i] = 'Z'; a[16] = 0; }

TC_CASE(legacy_overflow)
{
    char a[17]; int r; fill(a);
    r = vs(1, a, 4, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1);
    CHECK_MEM(a, "abc", 3);
    CHECK_STR(a + 4, "ZZZZZZZZZZZZ");
}
TC_CASE(legacy_exact_fit)
{
    char a[17]; int r; fill(a);
    r = vs(1, a, 8, "%s", "abcdefgh");
    CHECK_EQ_I(r, 8);
    CHECK_MEM(a, "abcdefg", 7);
    CHECK_STR(a + 8, "ZZZZZZZZ");
}
TC_CASE(standard_overflow)
{
    char a[17]; int r; fill(a);
    r = vs(2, a, 4, "%s", "abcdefgh");
    CHECK_EQ_I(r, 8);
    CHECK_MEM(a, "abc\0", 4);
    CHECK_STR(a + 4, "ZZZZZZZZZZZZ");
}
int main(void)
{
    tc_begin();
    TC_RUN(legacy_overflow); TC_RUN(legacy_exact_fit); TC_RUN(standard_overflow);
    return tc_end();
}
