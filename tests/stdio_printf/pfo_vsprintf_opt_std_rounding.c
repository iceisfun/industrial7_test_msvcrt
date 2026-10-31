/* TC
 * id:    pfo_vsprintf_opt_std_rounding
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   corecrt_stdio_config.h _CRT_INTERNAL_PRINTF_STANDARD_ROUNDING (0x20); MS docs printf rounding note (Win10 2004+)
 * note:  PROBE (flag added with the Windows 10 2004 / SDK 19041 rounding change; older ucrtbase builds may ignore it): options=0x20 requests IEEE round-half-even on exactly representable ties. Behaviour of options=0 is left to pfo_probe_vsprintf_opt_std_rounding. Inexact values are rounded correctly regardless.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int vw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

static char b[64];
static volatile double v05 = 0.5, v15 = 1.5, v25 = 2.5, v025 = 0.25, v1005 = 1.005;

TC_CASE(ties_to_even) { vs(0x20, b, sizeof b, "%.0f|%.0f|%.0f", v05, v15, v25); CHECK_STR(b, "0|2|2"); }
TC_CASE(tie_one_digit) { vs(0x20, b, sizeof b, "%.1f", v025); CHECK_STR(b, "0.2"); }
TC_CASE(inexact_below_tie) { vs(0x20, b, sizeof b, "%.2f", v1005); CHECK_STR(b, "1.00"); }
int main(void)
{
    tc_begin();
    TC_RUN(ties_to_even); TC_RUN(tie_one_digit); TC_RUN(inexact_below_tie);
    return tc_end();
}
