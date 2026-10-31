/* TC
 * id:    pfv_underscore_snprintf_fit
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _snprintf (len < count: NUL appended; len == count: len chars stored, returns len)
 * note:  exact-fit NUL-absence is NOT asserted here (UCRT legacy flag unclear); see pfv_probe_snprintf_exact_fit_no_nul.
 */
#include "tc.h"
#include <stdio.h>
static const char *vabc = "abc";
static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'X'; }
TC_CASE(fits_nul) { char b[16]; int n; fill(b); n = _snprintf(b, 8, "%s", vabc); CHECK_EQ_I(n, 3); CHECK_STR(b, "abc"); CHECK(b[3] == 0); }
TC_CASE(exact_fit_returns_count) { char b[16]; int n; fill(b); n = _snprintf(b, 3, "%s", vabc); CHECK_EQ_I(n, 3); CHECK(b[0] == 'a' && b[1] == 'b' && b[2] == 'c'); }
TC_CASE(return_len) { char b[16]; int n; fill(b); n = _snprintf(b, 16, "%d-%s", 42, vabc); CHECK_EQ_I(n, 6); CHECK_STR(b, "42-abc"); }
int main(void) { tc_begin(); TC_RUN(fits_nul); TC_RUN(exact_fit_returns_count); TC_RUN(return_len); return tc_end(); }
