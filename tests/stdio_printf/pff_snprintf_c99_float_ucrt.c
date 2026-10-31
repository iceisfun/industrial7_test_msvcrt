/* TC
 * id:    pff_snprintf_c99_float_ucrt
 * area:  stdio_printf
 * funcs: snprintf vsnprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.5 snprintf; MS docs snprintf
 * note:  UCRT snprintf is C99 conformant (VS2015+); msvcr120 not claimed
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];

static int vsn(char *s, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = vsnprintf(s, n, f, ap); va_end(ap); return r; }
TC_CASE(fits) { volatile double d = 2.5; CHECK_EQ_I(snprintf(b, 32, "%.2f", d), 4); CHECK_STR(b, "2.50"); }
TC_CASE(null_count) { volatile double d = 1.5; CHECK_EQ_I(snprintf(NULL, 0, "%f", d), 8); }
TC_CASE(vsn_null) { volatile double d = 1.5; CHECK_EQ_I(vsn(NULL, 0, "%f", d), 8); CHECK_EQ_I(vsn(b, 32, "%.1f", d), 3); CHECK_STR(b, "1.5"); }
TC_CASE(exact_fit) { volatile double d = 1.5; memset(b, 'Z', 16); CHECK_EQ_I(snprintf(b, 9, "%f", d), 8); CHECK_STR(b, "1.500000"); }
TC_CASE(truncate) { volatile double d = 1.5; memset(b, 'Z', 16); CHECK_EQ_I(snprintf(b, 8, "%f", d), 8); CHECK_STR(b, "1.50000"); }
int main(void) { tc_begin(); TC_RUN(fits); TC_RUN(null_count); TC_RUN(vsn_null); TC_RUN(exact_fit); TC_RUN(truncate); return tc_end(); }
