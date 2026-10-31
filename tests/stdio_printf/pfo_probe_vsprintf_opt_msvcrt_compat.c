/* TC
 * id:    pfo_probe_vsprintf_opt_msvcrt_compat
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   corecrt_stdio_config.h _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY
 * note:  PROBE: what options=0x8 changes: inf/nan formatting becomes old msvcrt style (1.#INF00, 1.#QNAN0, -1.#IND00) and %p. Opts=0 expectations are the C99 style (inf, nan, -nan(ind) for the indefinite NaN).
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

static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static volatile unsigned long long pinf = 0x7FF0000000000000ULL, pqnan = 0x7FF8000000000000ULL, pind = 0xFFF8000000000000ULL;
static char b[64];

TC_CASE(inf_opt0)   { vs(0, b, sizeof b, "%f", mk(pinf)); CHECK_STR(b, "inf"); }
TC_CASE(inf_opt8)   { vs(8, b, sizeof b, "%f", mk(pinf)); CHECK_STR(b, "1.#INF00"); }
TC_CASE(qnan_opt0)  { vs(0, b, sizeof b, "%f", mk(pqnan)); CHECK_STR(b, "nan"); }
TC_CASE(qnan_opt8)  { vs(8, b, sizeof b, "%f", mk(pqnan)); CHECK_STR(b, "1.#QNAN0"); }
TC_CASE(ind_opt0)   { vs(0, b, sizeof b, "%f", mk(pind)); CHECK_STR(b, "-nan(ind)"); }
TC_CASE(ind_opt8)   { vs(8, b, sizeof b, "%f", mk(pind)); CHECK_STR(b, "-1.#IND00"); }
TC_CASE(ptr_opt0)   { vs(0, b, sizeof b, "%p", (void *)(size_t)0xAB); CHECK_STR(b, sizeof(void *) == 8 ? "00000000000000AB" : "000000AB"); }
TC_CASE(ptr_opt8)   { vs(8, b, sizeof b, "%p", (void *)(size_t)0xAB); CHECK_STR(b, sizeof(void *) == 8 ? "00000000000000AB" : "000000AB"); }
int main(void)
{
    tc_begin();
    TC_RUN(inf_opt0); TC_RUN(inf_opt8); TC_RUN(qnan_opt0); TC_RUN(qnan_opt8);
    TC_RUN(ind_opt0); TC_RUN(ind_opt8); TC_RUN(ptr_opt0); TC_RUN(ptr_opt8);
    return tc_end();
}
