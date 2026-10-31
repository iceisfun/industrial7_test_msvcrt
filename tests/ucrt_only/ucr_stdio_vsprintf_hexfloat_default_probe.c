/* TC
 * id:    ucr_stdio_vsprintf_hexfloat_default_probe
 * area:  ucrt_only
 * funcs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.1 (%a default: exact digits) vs MS docs (default precision 13 for double)
 * note:  PROBE: C says %a without precision prints exactly enough digits (0x1p+0); Microsoft CRT is believed to pad to 13 hex digits (0x1.0000000000000p+0).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static int vs(unsigned long long opts, char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf(opts, b, n, f, NULL, ap); va_end(ap);
    return r;
}
static double mk(unsigned long long bits) { union { double d; unsigned long long u; } x; x.u = bits; return x.d; }
static volatile unsigned long long v_bits;
static double dv(unsigned long long bits) { v_bits = bits; return mk(v_bits); }

TC_CASE(one) { char b[64]; vs(0, b, sizeof b, "%a", dv(0x3ff0000000000000ULL)); CHECK_STR(b, "0x1.0000000000000p+0"); }
TC_CASE(half) { char b[64]; vs(0, b, sizeof b, "%a", dv(0x3fe0000000000000ULL)); CHECK_STR(b, "0x1.0000000000000p-1"); }
TC_CASE(denorm_min) { char b[64]; vs(0, b, sizeof b, "%a", dv(1)); CHECK_STR(b, "0x0.0000000000001p-1022"); }

int main(void)
{
    tc_begin();
    TC_RUN(one);
    TC_RUN(half);
    TC_RUN(denorm_min);
    return tc_end();
}
