/* TC
 * id:    pfv_snprintf_c99
 * area:  stdio_printf
 * funcs: snprintf
 * crt:   win10
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.5; MS docs snprintf
 */
#include "tc.h"
#include <stdio.h>
static double D(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static const char *vabc = "abc", *vfmt = "%d";
TC_CASE(fits) { char b[16]; int n = snprintf(b, 16, vabc); CHECK_EQ_I(n, 3); CHECK_STR(b, "abc"); }
TC_CASE(exact_fit_nul) { char b[8]; int i, n; for (i = 0; i < 8; i++) b[i] = 'X'; n = snprintf(b, 4, vabc); CHECK_EQ_I(n, 3); CHECK_STR(b, "abc"); CHECK(b[3] == 0); CHECK(b[4] == 'X'); }
TC_CASE(count_only_null_zero) { int n = snprintf(NULL, 0, vfmt, 12345); CHECK_EQ_I(n, 5); }
TC_CASE(n1_empty) { char b[4]; int n; b[0] = 'X'; b[1] = 'X'; n = snprintf(b, 1, vabc); CHECK_EQ_I(n, 3); CHECK(b[0] == 0); CHECK(b[1] == 'X'); }
int main(void) { tc_begin(); TC_RUN(fits); TC_RUN(exact_fit_nul); TC_RUN(count_only_null_zero); TC_RUN(n1_empty); return tc_end(); }
