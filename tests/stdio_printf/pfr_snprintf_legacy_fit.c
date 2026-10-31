/* TC
 * id:    pfr_snprintf_legacy_fit
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _snprintf_l, _snwprintf
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(room) { char b[32]; volatile size_t c = 20; int n = _snprintf(b, c, "%s%d", "ab", 12); CHECK_EQ_I(n, 4); CHECK_EQ_I(b[4], 0); CHECK_STR(b, "ab12"); }
TC_CASE(len_plus_1) { char b[16]; volatile size_t c = 6; int n = _snprintf(b, c, "%s", "hello"); CHECK_EQ_I(n, 5); CHECK_EQ_I(b[5], 0); CHECK_STR(b, "hello"); }
TC_CASE(empty) { char b[4]; volatile size_t c = 1; volatile const char *f = ""; int n; b[0] = 'Q'; n = _snprintf(b, c, (const char *)f); CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0); }

int main(void) { tc_begin(); TC_RUN(room); TC_RUN(len_plus_1); TC_RUN(empty); return tc_end(); }
