/* TC
 * id:    pfi_int_width_basic
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

TC_CASE(w1_3_10) { char b[64]; int n; n = sprintf(b, "%1d|%3d|%10d", v42, v42, v42); CHECK_STR(b, "42| 42|        42"); CHECK_EQ_I(n, 17); }
TC_CASE(exact) { char b[64]; int n; n = sprintf(b, "%2d", v42); CHECK_STR(b, "42"); CHECK_EQ_I(n, 2); }
TC_CASE(negative) { char b[64]; int n; n = sprintf(b, "%4d", vm42); CHECK_STR(b, " -42"); CHECK_EQ_I(n, 4); n = sprintf(b, "%3d", vm42); CHECK_STR(b, "-42"); CHECK_EQ_I(n, 3); }
TC_CASE(digits) { char b[64]; int n; n = sprintf(b, "%2d", (int)7); CHECK_STR(b, " 7"); CHECK_EQ_I(n, 2); n = sprintf(b, "%2d", (int)12); CHECK_STR(b, "12"); CHECK_EQ_I(n, 2); n = sprintf(b, "%1d", (int)7); CHECK_STR(b, "7"); CHECK_EQ_I(n, 1); }

int main(void) { tc_begin(); TC_RUN(w1_3_10); TC_RUN(exact); TC_RUN(negative); TC_RUN(digits); return tc_end(); }
