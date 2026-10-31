/* TC
 * id:    pfo_neg_probe_vsprintf_p_malformed_positional
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_p
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf_p positional parameters
 * note:  PROBES: malformed positional syntax ("%1$" trailing, "%$d", position overflow) is expected to be rejected with -1 (EINVAL);
 *        Guard bytes after the permitted size must stay intact. (One position used with two different types is
 *        undefined and deliberately not tested.)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

static int cp(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_p(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int guard_ok(const char *b, int from, int to)
{ int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != 0xA5) return 0; return 1; }

TC_CASE(trailing_position)
{
    char b[16]; int r; memset(b, 0xA5, sizeof b);
    tc_iph_count = 0; errno = 0;
    r = cp(b, 8, "%1$", 1);
    CHECK_EQ_I(r, -1); CHECK(guard_ok(b, 8, 16));
}
TC_CASE(dollar_without_digits)
{
    char b[16]; int r; memset(b, 0xA5, sizeof b);
    tc_iph_count = 0; errno = 0;
    r = cp(b, 8, "%$d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(guard_ok(b, 8, 16));
}
TC_CASE(position_overflow)
{
    char b[16]; int r; memset(b, 0xA5, sizeof b);
    tc_iph_count = 0; errno = 0;
    r = cp(b, 8, "%99999999999$d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(guard_ok(b, 8, 16));
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(trailing_position); TC_RUN(dollar_without_digits); TC_RUN(position_overflow);
    return tc_end();
}
