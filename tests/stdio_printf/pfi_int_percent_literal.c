/* TC
 * id:    pfi_int_percent_literal
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p8 (%% conversion)
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(after) { char b[80]; volatile int v = 50; CHECK_EQ_I(sprintf(b, "%d%%", v), 3); CHECK_STR(b, "50%"); }
TC_CASE(before) { char b[80]; volatile int v = 50; sprintf(b, "%%%d", v); CHECK_STR(b, "%50"); }
TC_CASE(escaped_d) { char b[80]; CHECK_EQ_I(sprintf(b, "%%d"), 2); CHECK_STR(b, "%d"); }
TC_CASE(both) { char b[80]; volatile int v = 5; sprintf(b, "%%%d%%%%", v); CHECK_STR(b, "%5%%"); }

int main(void) { tc_begin(); TC_RUN(after); TC_RUN(before); TC_RUN(escaped_d); TC_RUN(both); return tc_end(); }
