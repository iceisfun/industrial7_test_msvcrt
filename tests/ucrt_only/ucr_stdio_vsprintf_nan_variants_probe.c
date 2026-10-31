/* TC
 * id:    ucr_stdio_vsprintf_nan_variants_probe
 * area:  ucrt_only
 * funcs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   UCRT printf NaN formatting; corecrt_stdio_config.h _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY (0x8)
 * note:  PROBE: from memory UCRT prints -nan(ind) for the x86 default (indefinite) NaN 0xFFF8..., nan(snan) for signalling NaN 0x7FF4..., -nan for other negative quiet NaN, and with option 0x8 legacy 1.#INF00 / 1.#QNAN0 / -1.#IND00 style. Each is its own case.
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

TC_CASE(indefinite) { char b[64]; vs(0, b, sizeof b, "%f", dv(0xfff8000000000000ULL)); CHECK_STR(b, "-nan(ind)"); }
TC_CASE(snan) { char b[64]; vs(0, b, sizeof b, "%f", dv(0x7ff4000000000000ULL)); CHECK_STR(b, "nan(snan)"); }
TC_CASE(neg_qnan_payload) { char b[64]; vs(0, b, sizeof b, "%f", dv(0xfff8000000000001ULL)); CHECK_STR(b, "-nan"); }
TC_CASE(legacy_inf) { char b[64]; vs(0x8, b, sizeof b, "%f", dv(0x7ff0000000000000ULL)); CHECK_STR(b, "1.#INF00"); }
TC_CASE(legacy_neg_inf) { char b[64]; vs(0x8, b, sizeof b, "%f", dv(0xfff0000000000000ULL)); CHECK_STR(b, "-1.#INF00"); }
TC_CASE(legacy_qnan) { char b[64]; vs(0x8, b, sizeof b, "%f", dv(0x7ff8000000000000ULL)); CHECK_STR(b, "1.#QNAN0"); }

int main(void)
{
    tc_begin();
    TC_RUN(indefinite);
    TC_RUN(snan);
    TC_RUN(neg_qnan_payload);
    TC_RUN(legacy_inf);
    TC_RUN(legacy_neg_inf);
    TC_RUN(legacy_qnan);
    return tc_end();
}
