/* TC
 * id:    pfo_probe_vsprintf_opt_3digit_hexfloat
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   corecrt_stdio_config.h _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS
 * note:  PROBE: the three-digit-exponent option (0x10) is expected not to change %a output; the C99 form for 1.5 is "0x1.8p+0".
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile double d15 = 1.5;

TC_CASE(hexfloat_same)
{
    char a[64], c[64];
    vs(0, a, sizeof a, "%a", d15); vs(0x10, c, sizeof c, "%a", d15);
    CHECK_STR(a, c);
}
int main(void)
{
    tc_begin();
    TC_RUN(hexfloat_same);
    return tc_end();
}
