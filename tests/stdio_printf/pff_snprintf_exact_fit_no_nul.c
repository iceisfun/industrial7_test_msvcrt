/* TC
 * id:    pff_snprintf_exact_fit_no_nul
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _snprintf
 * note:  MS docs _snprintf: if len == count, no NUL is appended and len is returned; len > count returns negative
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];

TC_CASE(exact) { volatile double d = 1.5; int n; memset(b, 'Z', 16); n = _snprintf(b, 8, "%f", d);
    CHECK_EQ_I(n, 8); CHECK(b[8] == 'Z'); CHECK_MEM(b, "1.500000", 8); }
TC_CASE(one_more) { volatile double d = 1.5; int n; memset(b, 'Z', 16); n = _snprintf(b, 9, "%f", d);
    CHECK_EQ_I(n, 8); CHECK_STR(b, "1.500000"); }
TC_CASE(too_small) { volatile double d = 1.5; int n; memset(b, 'Z', 16); n = _snprintf(b, 7, "%f", d);
    CHECK(n < 0); CHECK_MEM(b, "1.50000", 7); CHECK(b[7] == 'Z'); }
int main(void) { tc_begin(); TC_RUN(exact); TC_RUN(one_more); TC_RUN(too_small); return tc_end(); }
