/* TC
 * id:    scn_common_vsscanf_msvcrt_compat_probe
 * area:  scanf
 * funcs: __stdio_common_vsscanf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   stdio.h _CRT_INTERNAL_SCANF_LEGACY_MSVCRT_COMPATIBILITY (4)
 * note:  PROBE: option bit 4 is named "legacy msvcrt compatibility". Guess: it restores pre-UCRT scanf behaviour, where the
 *        text "inf" / "nan" is not accepted by %lf (return 0) and hex-float "0x1p3" parses only the leading "0". Baseline
 *        (options 0) accepting "inf" with 1 and "0x1p3" as 8.0 follows C99 7.21.6.2 and is also probed.
 *        The %d case with flag 4 must be unaffected.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int cs(unsigned long long opt, const char *in, const char *fmt, ...)
{
    va_list ap; int r; va_start(ap, fmt);
    r = __stdio_common_vsscanf(opt, in, (size_t)-1, fmt, NULL, ap);
    va_end(ap); return r;
}
static const char *volatile vin;

TC_CASE(int_unchanged) { int a = 0, n; vin = "77"; n = cs(4, (const char *)vin, "%d", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 77); }
TC_CASE(inf_default)   { double d = 0; int n; vin = "inf"; n = cs(0, (const char *)vin, "%lf", &d); CHECK_EQ_I(n, 1); CHECK(d > 1e308); }
TC_CASE(inf_flag4)     { double d = 0; int n; vin = "inf"; n = cs(4, (const char *)vin, "%lf", &d); CHECK_EQ_I(n, 0); }
TC_CASE(hexfloat_default) { double d = 0; int n; vin = "0x1p3"; n = cs(0, (const char *)vin, "%lf", &d); CHECK_EQ_I(n, 1); CHECK_BITS_D(d, 8.0); }
TC_CASE(hexfloat_flag4)   { double d = 5; int n; vin = "0x1p3"; n = cs(4, (const char *)vin, "%lf", &d); CHECK_EQ_I(n, 1); CHECK_BITS_D(d, 0.0); }
int main(void)
{
    tc_begin();
    TC_RUN(int_unchanged); TC_RUN(inf_default); TC_RUN(inf_flag4); TC_RUN(hexfloat_default); TC_RUN(hexfloat_flag4);
    return tc_end();
}
