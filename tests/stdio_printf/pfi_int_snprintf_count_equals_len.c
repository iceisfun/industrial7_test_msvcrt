/* TC
 * id:    pfi_int_snprintf_count_equals_len
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf: count equal to length -> no NUL appended
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(no_terminator) {
    char b[16]; volatile int v = 12345; int n, i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    n = _snprintf(b, 5, "%d", v);
    CHECK_EQ_I(n, 5);
    CHECK_MEM(b, "12345", 5);
    CHECK_EQ_I(b[5], 'Z');
}
TC_CASE(width_form) {
    char b[16]; volatile int v = 42; int n, i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    n = _snprintf(b, 6, "%06d", v);
    CHECK_EQ_I(n, 6);
    CHECK_MEM(b, "000042", 6);
    CHECK_EQ_I(b[6], 'Z');
}

int main(void) { tc_begin(); TC_RUN(no_terminator); TC_RUN(width_form); return tc_end(); }
