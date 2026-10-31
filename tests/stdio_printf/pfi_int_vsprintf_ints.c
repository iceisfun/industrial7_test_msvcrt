/* TC
 * id:    pfi_int_vsprintf_ints
 * area:  stdio_printf
 * funcs: vsprintf _vsnprintf _vscprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.12; MS docs vsprintf, _vsnprintf, _vscprintf
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int do_vsprintf(char *b, const char *f, ...) { int n; va_list ap; va_start(ap, f); n = vsprintf(b, f, ap); va_end(ap); return n; }
static int do_vsnprintf(char *b, size_t c, const char *f, ...) { int n; va_list ap; va_start(ap, f); n = _vsnprintf(b, c, f, ap); va_end(ap); return n; }
static int do_vscprintf(const char *f, ...) { int n; va_list ap; va_start(ap, f); n = _vscprintf(f, ap); va_end(ap); return n; }

TC_CASE(vsprintf_basic) { char b[64]; int n = do_vsprintf(b, "%d %u %x", -3, 4000000000u, 0xfeu); CHECK_STR(b, "-3 4000000000 fe"); CHECK_EQ_I(n, 16); }
TC_CASE(vsnprintf_fits) { char b[64]; int n = do_vsnprintf(b, 64, "%05d|%-4d|", 42, 7); CHECK_STR(b, "00042|7   |"); CHECK_EQ_I(n, 11); }
TC_CASE(vscprintf_len) { CHECK_EQ_I(do_vscprintf("%d|%08x", -12345, 0xabcu), 6 + 1 + 8); }
TC_CASE(star) { char b[64]; do_vsprintf(b, "[%*d][%-*d][%.*d]", 5, 1, 5, 2, 4, 3); CHECK_STR(b, "[    1][2    ][0003]"); }
TC_CASE(i64) { char b[64]; do_vsprintf(b, "%I64d|%I64x", -4294967296LL, 0x100000000ULL); CHECK_STR(b, "-4294967296|100000000"); }
int main(void) { tc_begin(); TC_RUN(vsprintf_basic); TC_RUN(vsnprintf_fits); TC_RUN(vscprintf_len); TC_RUN(star); TC_RUN(i64); return tc_end(); }
