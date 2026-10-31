/* TC
 * id:    pfr_sprintf_count_basic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.6; MS docs sprintf
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(mix) { char b[64]; volatile const char *s = "xyz"; int n = sprintf(b, "%d/%s", 123, (const char *)s); CHECK_EQ_I(n, 7); CHECK_STR(b, "123/xyz"); }
TC_CASE(empty) { char b[8]; volatile const char *f = ""; int n; b[0] = 'Q'; n = sprintf(b, (const char *)f); CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0); }
TC_CASE(percent) { char b[8]; volatile const char *f = "%%"; int n = sprintf(b, (const char *)f); CHECK_EQ_I(n, 1); CHECK_STR(b, "%"); }
TC_CASE(chr) { char b[8]; volatile int c = 'A'; int n = sprintf(b, "%c", c); CHECK_EQ_I(n, 1); CHECK_STR(b, "A"); }
TC_CASE(empty_str) { char b[8]; volatile const char *s = ""; int n; b[0] = 'Q'; n = sprintf(b, "%s", (const char *)s); CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0); }
TC_CASE(width) { char b[32]; volatile const char *s = "ab"; int n = sprintf(b, "%10s", (const char *)s); CHECK_EQ_I(n, 10); CHECK_STR(b, "        ab"); }

int main(void) { tc_begin(); TC_RUN(mix); TC_RUN(empty); TC_RUN(percent); TC_RUN(chr); TC_RUN(empty_str); TC_RUN(width); return tc_end(); }
