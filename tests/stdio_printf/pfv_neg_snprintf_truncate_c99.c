/* TC
 * id:    pfv_neg_snprintf_truncate_c99
 * area:  stdio_printf
 * funcs: snprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.5; MS docs snprintf
 */
#include "tc.h"
#include <stdio.h>
static double D(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static const char *vs = "abcdefgh";
TC_CASE(trunc_ret) { char b[16]; int n = snprintf(b, 5, "%s", vs); CHECK_EQ_I(n, 8); }
TC_CASE(trunc_nul) { char b[16]; int i; for (i = 0; i < 16; i++) b[i] = 'X'; snprintf(b, 5, "%s", vs); CHECK_STR(b, "abcd"); CHECK(b[4] == 0); }
TC_CASE(no_write_past_n) { char b[16]; int i; for (i = 0; i < 16; i++) b[i] = 'X'; snprintf(b, 5, "%s", vs); CHECK(b[5] == 'X'); CHECK(b[6] == 'X'); CHECK(b[15] == 'X'); }
TC_CASE(n0_untouched) { char b[16]; int i, n; for (i = 0; i < 16; i++) b[i] = 'X'; n = snprintf(b, 0, "%s", vs); CHECK_EQ_I(n, 8); for (i = 0; i < 16; i++) CHECK(b[i] == 'X'); }
int main(void) { tc_begin(); TC_RUN(trunc_ret); TC_RUN(trunc_nul); TC_RUN(no_write_past_n); TC_RUN(n0_untouched); return tc_end(); }
