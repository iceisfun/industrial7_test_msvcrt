/* TC
 * id:    pfr_snprintf_c99_fit
 * area:  stdio_printf
 * funcs: snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.5; MS docs snprintf (UCRT: conforming)
 * note:  msvcr<=110 do not export a conforming snprintf; msvcr120 excluded here.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(exact_room) { char b[16]; volatile size_t c = 6; int n = snprintf(b, c, "%s", "hello"); CHECK_EQ_I(n, 5); CHECK_EQ_I(b[5], 0); CHECK_STR(b, "hello"); }
TC_CASE(large) { char b[64]; volatile size_t c = 60; int n = snprintf(b, c, "%d-%d", 12, 34); CHECK_EQ_I(n, 5); CHECK_STR(b, "12-34"); }
TC_CASE(empty) { char b[4]; volatile size_t c = 1; volatile const char *f = ""; int n; b[0] = 'Q'; n = snprintf(b, c, (const char *)f); CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0); }

int main(void) { tc_begin(); TC_RUN(exact_room); TC_RUN(large); TC_RUN(empty); return tc_end(); }
