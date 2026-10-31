/* TC
 * id:    pfs_str_c_nul
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.21.6.1p8 (c); MS docs _snprintf
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile int zero = 0;
TC_CASE(embedded) {
    char b[8]; memset(b, 0x7e, sizeof b);
    int n = _snprintf(b, 8, "a%cb", zero);
    CHECK_EQ_I(n, 3); CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], 'b');
}
TC_CASE(width) {
    char b[8]; memset(b, 0x7e, sizeof b);
    int n = _snprintf(b, 8, "%3c", zero);
    CHECK_EQ_I(n, 3); CHECK_EQ_I(b[0], ' '); CHECK_EQ_I(b[1], ' '); CHECK_EQ_I(b[2], 0);
}
TC_CASE(guard) {
    char b[8]; memset(b, 0x7e, sizeof b);
    _snprintf(b, 8, "a%cb", zero);
    CHECK_EQ_I(b[3], 0); /* len 3 < count 8: terminator appended (MS docs _snprintf) */
    CHECK_EQ_I(b[4], 0x7e); CHECK_EQ_I(b[7], 0x7e);
}
int main(void) { tc_begin(); TC_RUN(embedded); TC_RUN(width); TC_RUN(guard); return tc_end(); }
