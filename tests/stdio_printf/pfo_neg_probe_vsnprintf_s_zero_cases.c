/* TC
 * id:    pfo_neg_probe_vsnprintf_s_zero_cases
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsnprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf_s
 * note:  PROBES: size 0 / count 0 behaviour. Size 0 with non-NULL buffer and count 4: negative return and invalid parameter handler. Expected: size 0 count 0 NULL buffer returns 0 or -1 without crashing;
 *        size 8 count 0 with a 3-char string writes an empty string and returns -1 (truncated at count 0; 0 also tolerated).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int cn(char *b, size_t n, size_t m, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsnprintf_s(0, b, n, m, f, NULL, ap); va_end(ap); return r; }

TC_CASE(size0_count0_null)
{
    char *volatile b = NULL; int r; tc_iph_count = 0;
    r = cn(b, 0, 0, "%s", "abc");
    CHECK(r == 0 || r == -1);
}
TC_CASE(size0_nonnull_buf)
{
    char b[8] = "zzzzzzz"; int r; tc_iph_count = 0;
    r = cn(b, 0, 4, "a");
    CHECK(r < 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size8_count0)
{
    char b[8] = "zzzzzzz"; int r; tc_iph_count = 0;
    r = cn(b, 8, 0, "%s", "abc");
    CHECK(r == -1 || r == 0); CHECK_STR(b, "");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(size0_count0_null); TC_RUN(size0_nonnull_buf); TC_RUN(size8_count0);
    return tc_end();
}
