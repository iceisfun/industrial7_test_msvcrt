/* TC
 * id:    pfv_probe_rounding_half_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   UCRT printf (exactly-representable halves)
 * note:  exact halves. CLASS F (caller options decide). OBSERVED Win10 22H2 (19045) ucrtbase, run twice: __stdio_common_vsprintf with options 0x1 (what the mingw-w64 11 sprintf passes; its corecrt_stdio_config.h has no _CRT_INTERNAL_PRINTF_STANDARD_ROUNDING bit) gives legacy half-away rounding: %.0f 0.5 -> 1; 2.5 -> 3; %.1f 0.25 -> 0.3; %.2f 0.125 -> 0.13. With _CRT_INTERNAL_PRINTF_STANDARD_ROUNDING (0x20, passed as 0x21) the same ucrtbase rounds exact halves to even: 0, 2, 0.2, 0.12, 0.375 -> 0.38. This matches the MS docs printf format specification (2004+ IEEE round-half-even; legacy behaviour is opt-in via legacy_stdio_float_rounding.obj). The mingw-sprintf cases (legacy_*) therefore pin the caller-option behaviour; the std_* cases call the worker with the standard-rounding bit and pin the IEEE behaviour of the CRT itself.
 *        Raw evidence (baseline): f0_half b == "0" got="1" ; f0_two_half got="3" ; f1_quarter got="0.3" ; f2_eighth got="0.13".
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static volatile double d;
TC_CASE(legacy_f0) { char b[128]; d = 0.5; sprintf(b, "%.0f", d); CHECK_STR(b, "1"); d = 2.5; sprintf(b, "%.0f", d); CHECK_STR(b, "3"); }
TC_CASE(legacy_f1) { char b[128]; d = 0.25; sprintf(b, "%.1f", d); CHECK_STR(b, "0.3"); }
TC_CASE(legacy_f2) { char b[128]; d = 0.125; sprintf(b, "%.2f", d); CHECK_STR(b, "0.13"); d = 0.375; sprintf(b, "%.2f", d); CHECK_STR(b, "0.38"); }
#if TC_CRT_VER >= 140
/* options 0x21 = _CRT_INTERNAL_PRINTF_STANDARD_ROUNDING (0x20) | the bit mingw sprintf passes (0x1) */
static int std_sprintf(char *b, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(0x21, b, 128, f, NULL, ap); va_end(ap); return r; }
TC_CASE(std_f0) { char b[128]; d = 0.5; std_sprintf(b, "%.0f", d); CHECK_STR(b, "0"); d = 2.5; std_sprintf(b, "%.0f", d); CHECK_STR(b, "2"); }
TC_CASE(std_f1) { char b[128]; d = 0.25; std_sprintf(b, "%.1f", d); CHECK_STR(b, "0.2"); }
TC_CASE(std_f2) { char b[128]; d = 0.125; std_sprintf(b, "%.2f", d); CHECK_STR(b, "0.12"); d = 0.375; std_sprintf(b, "%.2f", d); CHECK_STR(b, "0.38"); }
#define STD_RUN TC_RUN(std_f0); TC_RUN(std_f1); TC_RUN(std_f2);
#else
#define STD_RUN
#endif

int main(void) { tc_begin(); TC_RUN(legacy_f0); TC_RUN(legacy_f1); TC_RUN(legacy_f2); STD_RUN return tc_end(); }
