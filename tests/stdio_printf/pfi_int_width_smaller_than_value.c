/* TC
 * id:    pfi_int_width_smaller_than_value
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf format specification
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile int v42 = 42, vm42 = -42;
static volatile unsigned vu = 42;

TC_CASE(d) { char b[64]; int n; n = sprintf(b, "%2d", (int)12345); CHECK_STR(b, "12345"); CHECK_EQ_I(n, 5); }
TC_CASE(u) { char b[64]; int n; n = sprintf(b, "%1u", (unsigned)4294967295u); CHECK_STR(b, "4294967295"); CHECK_EQ_I(n, 10); }
TC_CASE(x) { char b[64]; int n; n = sprintf(b, "%3x", (unsigned)0xdeadbeef); CHECK_STR(b, "deadbeef"); CHECK_EQ_I(n, 8); }

int main(void) { tc_begin(); TC_RUN(d); TC_RUN(u); TC_RUN(x); return tc_end(); }
