/* TC
 * id:    pfs_str_large_output
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1; C11 5.2.4.1 (4095 chars per call)

 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static char big[3001];
static char out[4200];
static const char *volatile x = "x";

TC_CASE(width500) {
    int n = sprintf(out, "%500s", x), i, ok = 1;
    CHECK_EQ_I(n, 500);
    for (i = 0; i < 499; i++) if (out[i] != ' ') ok = 0;
    CHECK(ok); CHECK_EQ_I(out[499], 'x'); CHECK_EQ_I(out[500], 0);
}
TC_CASE(left1000) {
    int n = sprintf(out, "%-1000s", x), i, ok = 1;
    CHECK_EQ_I(n, 1000); CHECK_EQ_I(out[0], 'x');
    for (i = 1; i < 1000; i++) if (out[i] != ' ') ok = 0;
    CHECK(ok); CHECK_EQ_I(out[1000], 0);
}
TC_CASE(long_copy) {
    int n, i, ok = 1; const char *p = big;
    n = sprintf(out, "%s", p);
    CHECK_EQ_I(n, 3000);
    for (i = 0; i < 3000; i++) if (out[i] != (char)('a' + i % 26)) ok = 0;
    CHECK(ok); CHECK_EQ_I(out[3000], 0);
}
TC_CASE(prec2000) {
    int n = sprintf(out, "%.2000s", (const char *)big);
    CHECK_EQ_I(n, 2000); CHECK_EQ_I(out[1999], (char)('a' + 1999 % 26)); CHECK_EQ_I(out[2000], 0);
}

int main(void) {
    int i; tc_begin();
    for (i = 0; i < 3000; i++) big[i] = (char)('a' + i % 26);
    TC_RUN(width500); TC_RUN(left1000); TC_RUN(long_copy); TC_RUN(prec2000); return tc_end();
}
