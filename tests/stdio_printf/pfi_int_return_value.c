/* TC
 * id:    pfi_int_return_value
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.6p3 (sprintf returns count excluding NUL)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(plain) { char b[80]; volatile int v = 12345; CHECK_EQ_I(sprintf(b, "%d", v), 5); CHECK_STR(b, "12345"); }
TC_CASE(width) { char b[80]; volatile int v = 42; CHECK_EQ_I(sprintf(b, "%10d", v), 10); CHECK_STR(b, "        42"); }
TC_CASE(empty) { char b[80]; volatile int v = 0; b[0] = 'Z'; CHECK_EQ_I(sprintf(b, "%.0d", v), 0); CHECK_EQ_I(b[0], 0); }
TC_CASE(sign) { char b[80]; volatile int v = 7; CHECK_EQ_I(sprintf(b, "%+d", v), 2); CHECK_STR(b, "+7"); }
TC_CASE(sum) { char b[80]; volatile int v = 1; CHECK_EQ_I(sprintf(b, "%d,%d;%03d", v, v + 1, v + 2), 7); CHECK_STR(b, "1,2;003"); }
TC_CASE(literal) { char b[80]; CHECK_EQ_I(sprintf(b, "hello"), 5); CHECK_STR(b, "hello"); }

int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(width); TC_RUN(empty); TC_RUN(sign); TC_RUN(sum); TC_RUN(literal); return tc_end(); }
