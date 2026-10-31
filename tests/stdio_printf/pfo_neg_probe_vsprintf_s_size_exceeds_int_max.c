/* TC
 * id:    pfo_neg_probe_vsprintf_s_size_exceeds_int_max
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_s __stdio_common_vsnprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs sprintf_s (sizeOfBuffer is size_t), _snprintf_s
 * note:  PROBES: whether buffer sizes above INT_MAX / SIZE_MAX are accepted. The real buffer is small; only a short string is written.
 *        Expected: accepted, returns the length. Failure with -1 is tolerated for SIZE_MAX only.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

static int cs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cn(char *b, size_t n, size_t m, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsnprintf_s(0, b, n, m, f, NULL, ap); va_end(ap); return r; }

TC_CASE(size_int_max_plus_1)
{
    char b[16]; volatile size_t n = (size_t)INT_MAX + 1; int r;
    if (sizeof(size_t) < 8) TC_SKIP("32-bit size_t");
    tc_iph_count = 0;
    r = cs(b, n, "%s", "abcd");
    CHECK_EQ_I(r, 4); CHECK_STR(b, "abcd");
}
TC_CASE(size_max)
{
    char b[16]; volatile size_t n = SIZE_MAX; int r;
    tc_iph_count = 0;
    r = cs(b, n, "%s", "abcd");
    CHECK(r == 4 || r == -1);
    if (r == 4) CHECK_STR(b, "abcd");
}
TC_CASE(snprintf_s_size_max_count3)
{
    char b[16] = ""; volatile size_t n = SIZE_MAX; int r;
    tc_iph_count = 0;
    r = cn(b, n, 3, "%s", "abcdef");
    CHECK(r == -1 || r == 3);
    if (tc_iph_count == 0) CHECK_STR(b, "abc");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(size_int_max_plus_1); TC_RUN(size_max); TC_RUN(snprintf_s_size_max_count3);
    return tc_end();
}
