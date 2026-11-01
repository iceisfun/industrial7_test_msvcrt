/* TC
 * id:    pfo_probe_vsprintf_opt_std_rounding
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   MS docs printf (rounding note: exact ties round up before Windows 10 2004)
 * note:  PROBE (ties_opt20/digits_*_opt20 need a Win10 2004+ ucrtbase; older builds round ties up and zero-pad after 17 digits): behaviour of default options=0 vs 0x20 on exact ties and long digit strings. ties_opt0 expects the legacy round-half-away result "1|2|3|4" (observed on Win10 22H2 19045: options==0 keeps the legacy tie rounding; only 0x20 selects IEEE half-even), the rest expect and exact (not 17-digit zero padded) digits.
 *        B/A: observed legacy tie rounding with options 0 on build 19045; ties_opt20 (half-even) passes. Stable (deterministic).
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

static char b[128];
static volatile double v05 = 0.5, v15 = 1.5, v25 = 2.5, v35 = 3.5, v01 = 0.1, vd = 5e-324;

TC_CASE(ties_opt0)   { vs(0, b, sizeof b, "%.0f|%.0f|%.0f|%.0f", v05, v15, v25, v35); CHECK_STR(b, "1|2|3|4"); }
TC_CASE(ties_opt20)  { vs(0x20, b, sizeof b, "%.0f|%.0f|%.0f|%.0f", v05, v15, v25, v35); CHECK_STR(b, "0|2|2|4"); }
TC_CASE(digits_01_opt0)  { vs(0, b, sizeof b, "%.30f", v01); CHECK_STR(b, "0.100000000000000005551115123126"); }
TC_CASE(digits_01_opt20) { vs(0x20, b, sizeof b, "%.30f", v01); CHECK_STR(b, "0.100000000000000005551115123126"); }
TC_CASE(denorm_opt0)  { vs(0, b, sizeof b, "%.20e", vd); CHECK_STR(b, "4.94065645841246544177e-324"); }
TC_CASE(denorm_opt20) { vs(0x20, b, sizeof b, "%.20e", vd); CHECK_STR(b, "4.94065645841246544177e-324"); }
int main(void)
{
    tc_begin();
    TC_RUN(ties_opt0); TC_RUN(ties_opt20); TC_RUN(digits_01_opt0); TC_RUN(digits_01_opt20);
    TC_RUN(denorm_opt0); TC_RUN(denorm_opt20);
    return tc_end();
}
